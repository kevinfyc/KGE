//
//  material_GLES.cpp
//  libkge
//
//  Created by kevin on 17-9-27.
//
//

#include "material_GLES.h"

#include "common.h"

#include "util/log.h"

#include "uniform_buffer.h"

#include "material.h"
#include "shader.h"
#include "texture2D.h"


namespace kge
{
	void MaterialGLES::UpdateUniformsBegin(uint32 pass_index)
	{
		Material* mat = dynamic_cast<Material*>(this);
		Ref<Shader> shader = mat->GetShader();

		if (_uniform_buffers.size() < pass_index + 1)
			_uniform_buffers.resize(pass_index + 1);

		if (!_uniform_buffers[pass_index])
			_uniform_buffers[pass_index] = shader->CreateUniformBuffer(pass_index);
	}

	void* MaterialGLES::SetUniformBegin(uint32 pass_index)
	{
		void* mapped = nullptr;

		Ref<UniformBuffer> uniform_buffer = _uniform_buffers[pass_index];
		if (uniform_buffer)
		{
			GL_ASSERT( glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer->GetBuffer()) );
			mapped = uniform_buffer->GetLocalBuffer()->Bytes();
		}

		return mapped;
	}

	void MaterialGLES::SetUniformEnd(uint32 pass_index)
	{
		Ref<UniformBuffer> uniform_buffer = _uniform_buffers[pass_index];
		if (uniform_buffer)
		{
			GL_ASSERT( glBufferSubData(GL_UNIFORM_BUFFER, 0, uniform_buffer->GetSize(), uniform_buffer->GetLocalBuffer()->Bytes()) );
			GL_ASSERT( glBindBuffer(GL_UNIFORM_BUFFER, 0) );
		}
	}

	void MaterialGLES::SetUniform(uint32 pass_index, void* uniform_buffer, const std::string& name, void* data, uint32 size)
	{
		char* buffer = static_cast<char*>(uniform_buffer);
		Material* mat = dynamic_cast<Material*>(this);
		Ref<Shader> shader = mat->GetShader();
		std::vector<UniformBufferNode*> uniform_buffer_infos = shader->GetUniformBufferInfos(pass_index);

		for (UniformBufferNode* node : uniform_buffer_infos)
		{
			for (Uniform uniform : node->uniforms)
			{
				if (uniform.name == name)
				{
					assert(uniform.size >= size);

					::memcpy(&buffer[node->offset + uniform.offset], data, size);
				}
			}
		}
	}

	void MaterialGLES::Apply(uint32 pass_index)
	{
		Material* mat = dynamic_cast<Material*>(this);
		Ref<Shader> shader = mat->GetShader();
		std::vector<const Sampler*> sampler_infos = shader->GetSamplerInfos(pass_index);
		std::vector<GLint> sampler_locations = shader->GetSamplerLocations(pass_index);
		std::map<std::string, Ref<Texture>> textures = mat->GetTextures();

		for (uint32 i = 0; i < sampler_locations.size(); ++i)
		{
			GLint location = sampler_locations[i];

			GL_ASSERT( glActiveTexture(GL_TEXTURE0 + i + 1) );

			std::string name = sampler_infos[i]->name;
			const Ref<Texture>* tex = nullptr;
			auto iter = textures.find(name);
			if (iter != textures.end())
			{
				tex = &iter->second;
				GLuint texture = (*tex)->GetTexture();
				glBindTexture(GL_TEXTURE_2D, texture);
			}
			else
			{
				GLuint default_texture = Shader::GetDefaultTexture(sampler_infos[i]->default_tex)->GetTexture();
				glBindTexture(GL_TEXTURE_2D, default_texture);
			}

			GL_ASSERT( glUniform1i(location, i + 1) );
		}

		std::vector<UniformBufferNode*> uniform_buffer_infos = shader->GetUniformBufferInfos(pass_index);
		Ref<UniformBuffer> uniform_buffer = _uniform_buffers[pass_index];

		for (UniformBufferNode* node : uniform_buffer_infos)
		{
			GL_ASSERT( glBindBufferRange(GL_UNIFORM_BUFFER, node->binding, uniform_buffer->GetBuffer(), node->offset, node->size) );
		}
	}
}

