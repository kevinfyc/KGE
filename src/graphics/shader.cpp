//
//  shader.cpp
//  libkge
//
//  Created by kevin on 17-9-27.
//
//

#include "shader.h"
#include "util\byte_buffer.h"
#include "texture_format.h"
#include "texture2D.h"
#include "util\log.h"
#include "io\file_tool.h"

namespace kge
{
	std::map<std::string, Ref<Shader>> Shader::_shaders;
	std::map<std::string, Ref<Texture2D>> Shader::_default_textures;

	bool Shader::Init()
	{
		return true;
	}

	void Shader::Fini()
	{
		_shaders.clear();
		_default_textures.clear();
	}

	void Shader::ClearAllPipelines()
	{
		for (auto iter : _shaders)
			iter.second->ClearPipelines();
	}

	const Ref<Texture2D>& Shader::GetDefaultTexture(const std::string& name)
	{
		if (_default_textures.count(name) <= 0)
		{
			Ref<Texture2D> texture;
			if (name == "white")
			{
				ByteBuffer colors(4);
				uint32 i = 0;
				colors[i++] = 255;
				colors[i++] = 255;
				colors[i++] = 255;
				colors[i++] = 255;

				texture = Texture2D::Create(1, 1, TextureFormat::RGBA32, TextureWrapMode::Clamp, FilterMode::Point, false, colors);
			}
			else if (name == "black")
			{
				ByteBuffer colors(4);
				int i = 0;
				colors[i++] = 0;
				colors[i++] = 0;
				colors[i++] = 0;
				colors[i++] = 255;

				texture = Texture2D::Create(1, 1, TextureFormat::RGBA32, TextureWrapMode::Clamp, FilterMode::Point, false, colors);
			}
			else if (name == "bump")
			{
				ByteBuffer colors(4);
				int i = 0;
				colors[i++] = 128;
				colors[i++] = 128;
				colors[i++] = 255;
				colors[i++] = 255;

				texture = Texture2D::Create(1, 1, TextureFormat::RGBA32, TextureWrapMode::Clamp, FilterMode::Point, false, colors);
			}
			else
			{
				KGE_LOG_ERROR("invalid default texture name!");
			}

			_default_textures[name] = texture;
		}

		return _default_textures[name];
	}

	Shader::Shader(const std::string& name)
	{
		SetName(name);
	}

	Ref<Shader> Shader::Find(const std::string& name)
	{
		Ref<Shader> shader;

		auto iter = _shaders.find(name);
		if (iter != _shaders.end())
		{
			shader = iter->second;
		}
		else
		{
			std::string path = "shader/" + name + ".shader.xml";
			if (FileSystemMgr::GetInstance()->getFileSystem()->isFileExist(path))
			{
				shader = Ref<Shader>(new Shader(name));
				shader->_xml.Load(path);
				shader->Compile();

				_shaders[name] = shader;
			}
			else
			{
				KGE_LOG_ERROR("can not find shader %s", name.c_str());
			}
		}

		return shader;
	}

	uint32 Shader::GetQueue() const
	{
		return _xml.queue;
	}
}

