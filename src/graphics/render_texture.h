//
//  render_texture.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_GRAPHICS_RENDER_TEXTURE_H__
#define __KGE_GRAPHICS_RENDER_TEXTURE_H__

#include "texture.h"

#include "depth_buffer.h"
#include "render_texture_format.h"
#include "filter_mode.h"

namespace kge
{
	class RenderTexture : public Texture
	{
	public:
		static bool Init();
		static void Fini();

		static Ref<RenderTexture> Create(uint32 width, uint32 height,
			RenderTextureFormat format,	DepthBuffer depth, FilterMode filter_mode);
		static Ref<RenderTexture> GetTemporary(uint32 width, uint32 height,
			RenderTextureFormat format, DepthBuffer depth, FilterMode filter_mode);
		static void ReleaseTemporary(Ref<RenderTexture> texture);

		RenderTextureFormat GetFormat()const { return _format; }
		DepthBuffer GetDepth() const { return _depth; }

	protected:
		void SetFormat(RenderTextureFormat format) { _format = format; }
		void SetDepth(DepthBuffer depth) { _depth = depth; }

	private:
		RenderTexture();

		struct Temporary
		{
			Ref<RenderTexture> texture;
			bool in_use;
			float used_time;
		};

	private:
		static std::map<long long, std::list<Temporary>> _temporaries;

		DepthBuffer _depth;
		RenderTextureFormat _format;
	};
}

#endif // __KGE_GRAPHICS_RENDER_TEXTURE_H__