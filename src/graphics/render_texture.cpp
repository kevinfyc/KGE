//
//  render_texture.cpp
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#include "render_texture.h"

#include "util/time.h"

namespace kge
{
	std::map<long long, std::list<RenderTexture::Temporary>> RenderTexture::_temporaries;

	bool RenderTexture::Init()
	{
		return true;
	}

	void RenderTexture::Fini()
	{

	}

	Ref<RenderTexture> RenderTexture::GetTemporary(uint32 width, uint32 height,
		RenderTextureFormat format, DepthBuffer depth, FilterMode filter_mode)
	{
		Ref<RenderTexture> texture;

		long long w = width;
		long long h = height;
		long long f = (long long)format;
		long long d = (long long)depth;
		long long key = (w << 0) | (h << 16) | (f << 32) | (d << 48);

		std::list<Temporary>* list;
		auto iter = _temporaries.find(key);
		if (iter != _temporaries.end())
		{
			list = &iter->second;
			for (auto& i : *list)
			{
				if (!i.in_use)
				{
					texture = i.texture;
					i.in_use = true;

					if (texture->GetFilterMode() != filter_mode)
					{
						texture->SetFilterMode(filter_mode);
						texture->UpdateSampler();
					}
					break;
				}
			}
		}

		if (!texture)
		{
			texture = Create(width, height, format, depth, filter_mode);

			Temporary t;
			t.texture = texture;
			t.in_use = true;

			if (list != nullptr)
			{
				list->push_back(t);
			}
			else
			{
				std::list<Temporary> list_neo;
				list_neo.push_back(t);
				_temporaries[key] = list_neo;
			}
		}

		return texture;
	}

	void RenderTexture::ReleaseTemporary(Ref<RenderTexture> texture)
	{
		long long w = texture->GetWidth();
		long long h = texture->GetHeight();
		long long f = (long long)texture->GetFormat();
		long long d = (long long)texture->GetDepth();
		long long key = (w << 0) | (h << 16) | (f << 32) | (d << 48);

		std::list<Temporary>* list;
		auto iter = _temporaries.find(key);
		if (iter != _temporaries.end())
		{
			list = &iter->second;

			for (auto& i : *list)
			{
				if (i.texture == texture)
				{
					i.in_use = false;
					i.used_time = Time::GetTime();
					break;
				}
			}
		}
	}

	Ref<RenderTexture> RenderTexture::Create(uint32 width, uint32 height,
		RenderTextureFormat format, DepthBuffer depth, FilterMode filter_mode)
	{
		Ref<RenderTexture> texture = Ref<RenderTexture>(new RenderTexture());
		texture->SetWidth(width);
		texture->SetHeight(height);
		texture->SetFormat(format);
		texture->SetDepth(depth);
		texture->SetWrapMode(TextureWrapMode::Clamp);
		texture->SetFilterMode(filter_mode);

		if (format == RenderTextureFormat::Depth)
			texture->CreateDepthRenderTexture();
		else
			texture->CreateColorRenderTexture();

		return texture;
	}

	RenderTexture::RenderTexture()
	{
		SetName("RenderTexture");
	}
}

