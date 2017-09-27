//
//  shader.h
//  libkge
//
//  Created by kevin on 17-9-27.
//
//

#ifndef __KGE_GRAPHICS_SHADER_H__
#define __KGE_GRAPHICS_SHADER_H__

#if KGE_GLES
#include "shader_GLES.h"
#endif

#include "shader_xml.h"

namespace kge
{
	class Texture2D;

#if KGE_GLES
	class Shader : public ShaderGLES
	{
		friend class ShaderGLES;
#endif

	public:
		static bool Init();
		static void Fini();
		static void ClearAllPipelines();
		static Ref<Shader> Find(const std::string& name);
		static const Ref<Texture2D>& GetDefaultTexture(const std::string& name);

		uint32 GetQueue() const;

	private:
		Shader(const std::string& name);

		static std::map<std::string, Ref<Shader>> _shaders;
		static std::map<std::string, Ref<Texture2D>> _default_textures;
		ShaderXML _xml;
	};
}

#endif // __KGE_GRAPHICS_SHADER_H__