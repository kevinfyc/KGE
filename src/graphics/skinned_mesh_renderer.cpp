//
//  skinned_mesh_renderer.cpp
//  libkge
//
//  Created by kevin on 17-10-16.
//
//

#include "skinned_mesh_renderer.h"
#include "material.h"
#include "core/transform.h"
#include "math/matrix.hpp"

namespace kge
{
	DEFINE_COM_CLASS(SkinnedMeshRenderer);

	SkinnedMeshRenderer::SkinnedMeshRenderer()
	{
	}

	void SkinnedMeshRenderer::DeepCopy(const Ref<Object>& source)
	{
		Renderer::DeepCopy(source);

		auto src = RefCast<SkinnedMeshRenderer>(source);
		auto mats = src->GetSharedMaterials();
		for (auto& mat : mats)
		{
			auto mat_instance = Material::Create(mat->GetShader()->GetName());
			mat_instance->DeepCopy(mat);
			mat = mat_instance;
		}
		SetSharedMaterials(mats);
		SetSharedMesh(src->GetSharedMesh());
		SetBones(src->GetBones());
	}

	const VertexBuffer* SkinnedMeshRenderer::GetVertexBuffer() const
	{
		return GetSharedMesh()->GetVertexBuffer().get();
	}

	const IndexBuffer* SkinnedMeshRenderer::GetIndexBuffer() const
	{
		return GetSharedMesh()->GetIndexBuffer().get();
	}

	void SkinnedMeshRenderer::GetIndexRange(uint32 material_index, uint32& start, uint32& count) const
	{
		GetSharedMesh()->GetIndexRange(material_index, start, count);
	}

	bool SkinnedMeshRenderer::IsValidPass(uint32 material_index) const
	{
		if (m_mesh)
		{
			uint32 submesh_count = m_mesh->submeshes.size();

			if (material_index == 0)
				return true;
			else if (material_index > 0 && submesh_count > 0 && material_index < submesh_count)
				return true;
		}

		return false;
	}

	void SkinnedMeshRenderer::PreRenderByRenderer(uint32 material_index)
	{
		const auto& bindposes = this->GetSharedMesh()->bind_poses;
		const auto& bones = this->GetBones();
		std::vector<Vector4> bone_matrix;
		const void* buffer;
		int size;

		if (bones.size() > 0)
		{
			bone_matrix.resize(bones.size() * 3);
			for (uint32 i = 0; i < bones.size(); i++)
			{
				Matrix m;
				m.multiply(bones[i].lock()->GetLocal2WorldMatrix(), bindposes[i]);

				bone_matrix[i * 3 + 0] = m.row(0);
				bone_matrix[i * 3 + 1] = m.row(1);
				bone_matrix[i * 3 + 2] = m.row(2);
			}
			buffer = &bone_matrix[0];
			size = sizeof(Vector4) * bones.size() * 3;
		}
		else
		{
			buffer = &this->GetTransform()->GetLocal2WorldMatrix();
			size = sizeof(Matrix);
		}

		auto& shader = this->GetSharedMaterials()[material_index]->GetShader();
		shader->UpdateRendererDescriptorSet(m_descriptor_set_buffer, &buffer, size);
	}
}

