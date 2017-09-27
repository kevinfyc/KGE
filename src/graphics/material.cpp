//
//  material.cpp
//  libkge
//
//  Created by kevin on 17-9-27.
//
//

#include "material.h"

#include "camera.h"

#include "util/log.h"

namespace kge
{
	
	static const std::string MAIN_TEX_NAME = "_MainTex";
	static const std::string MAIN_COLOR_NAME = "_Color";

	Ref<Material> Material::Create(const std::string& shader_name)
	{
		Ref<Material> mat = Ref<Material>(new Material());
		mat->SetName(shader_name);

		Ref<Shader> shader = Shader::Find(shader_name);
		if (shader)
		{
			mat->_shader = shader;
		}
		else
		{
			KGE_LOG_ERROR("can not find shader %s", shader_name.c_str());

			mat->_shader = Shader::Find("Error");
		}

		return mat;
	}

	Material::Material()
	{
		SetMainColor(Color(1, 1, 1, 1));
	}

	void Material::SetMatrix(const std::string & name, const Matrix& v)
	{
		_matrices[name] = v;
	}

	const Matrix& Material::GetMatrix(const std::string& name)
	{
		return _matrices[name];
	}


	void Material::SetVector(const std::string & name, const Vector4& v)
	{
		_vectors[name] = v;
	}

	const Vector4& Material::GetVector(const std::string& name)
	{
		return _vectors[name];
	}

	bool Material::HasVector(const std::string& name)const
	{
		auto iter = _vectors.find(name);
		return iter != _vectors.end();
	}

	void Material::SetColor(const std::string& name, const Color& v)
	{
		_colors[name] = v;
	}

	const Color& Material::GetColor(const std::string& name)
	{
		return _colors[name];
	}

	void Material::SetMainColor(const Color& v)
	{
		SetColor(MAIN_COLOR_NAME, v);
	}

	const Color& Material::GetMainColor()
	{
		return GetColor(MAIN_COLOR_NAME);
	}

	void Material::SetVectorArray(const std::string& name, const std::vector<Vector4>& v)
	{
		_vector_arrays[name] = v;
	}

	const std::vector<Vector4>& Material::GetVectorArray(const std::string& name)
	{
		return _vector_arrays[name];
	}

	void Material::SetMainTexture(const Ref<Texture>& v)
	{
		SetTexture(MAIN_TEX_NAME, v);
	}

	bool Material::HasMainTexture()
	{
		auto iter = _textures.find(MAIN_TEX_NAME);
		return iter != _textures.end();
	}

	const Ref<Texture>& Material::GetMainTexture()
	{
		return _textures[MAIN_TEX_NAME];
	}

	void Material::SetTexture(const std::string& name, const Ref<Texture>& v)
	{
		_textures[name] = v;
	}

	void Material::SetZBufferParams(const Ref<Camera>& cam)
	{
		float cam_far = cam->GetClipFar();
		float cam_near = cam->GetClipNear();


		float zx = (1.0f - cam_far / cam_near);
		float zy = (cam_far / cam_near);

		SetVector("_ZBufferParams", Vector4(zx, zy, zx / cam_far, zy / cam_near));
	}

	void Material::SetProjectionParams(const Ref<Camera>& cam)
	{
		float cam_far = cam->GetClipFar();
		float cam_near = cam->GetClipNear();

		// x = 1 or -1 (-1 if projection is flipped)
		// y = near plane
		// z = far plane
		// w = 1/far plane
		SetVector("_ProjectionParams", Vector4(1, cam_near, cam_far, 1 / cam_far));
	}

	void Material::SetMainTexTexelSize(const Ref<Texture>& tex)
	{
		SetVector("_MainTex_TexelSize", Vector4(1.0f / tex->GetWidth(), 1.0f / tex->GetHeight(), (float)tex->GetWidth(), (float)tex->GetHeight()));
	}

	void Material::UpdateUniforms(uint32 pass_index)
	{
		UpdateUniformsBegin(pass_index);

		auto buffer = SetUniformBegin(pass_index);
		for (auto& i : _matrices)
		{
			this->SetUniform(pass_index, buffer, i.first, (void*)&i.second, sizeof(Matrix));
		}
		for (auto& i : _colors)
		{
			this->SetUniform(pass_index, buffer, i.first, (void*)&i.second, sizeof(Color));
		}
		for (auto& i : _vectors)
		{
			this->SetUniform(pass_index, buffer, i.first, (void*)&i.second, sizeof(Vector4));
		}
		for (auto& i : _vector_arrays)
		{
			this->SetUniform(pass_index, buffer, i.first, (void*)&i.second[0], sizeof(Vector4) * i.second.size());
		}
		this->SetUniformEnd(pass_index);

		for (auto& i : _textures)
		{
			this->SetUniformTexture(pass_index, i.first, i.second.get());
		}

		this->UpdateUniformsEnd(pass_index);
	}
}

