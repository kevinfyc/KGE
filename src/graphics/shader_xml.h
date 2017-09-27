//
//  shader_xml.h
//  libkge
//
//  Created by kevin on 17-9-26.
//
//

#ifndef __KGE_GRAPHICS_SHADER_XML_H__
#define __KGE_GRAPHICS_SHADER_XML_H__

#include "render_state.h"

namespace kge
{
	struct ShaderXML
	{
		std::string name;
		uint32 queue;

		std::vector<Pass> passes;
		std::vector<VertexShader> vss;
		std::vector<PixelShader> pss;
		std::vector<RenderState> rss;

		void Load(const std::string& path);
		void Clear();
	};
}

#endif // __KGE_GRAPHICS_SHADER_XML_H__