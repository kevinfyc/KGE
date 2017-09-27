//
//  render_state.h
//  libkge
//
//  Created by kevin on 17-9-27.
//
//

#ifndef __KGE_GRAPHICS_RENDERSTATE_H__
#define __KGE_GRAPHICS_RENDERSTATE_H__

#include <string>
#include <vector>
#include "vertex_attribute.h"

namespace kge
{
	struct Pass
	{
		std::string name;
		std::string vs;
		std::string ps;
		std::string rs;
	};

	struct Uniform
	{
		std::string name;
		uint32 offset;
		uint32 size;

		Uniform() :offset(0)
			, size(0)
		{

		}
	};

	struct UniformBufferNode
	{
		std::string name;
		int32 binding;
		std::vector<Uniform> uniforms;
		uint32 size;
		uint32 offset;

		UniformBufferNode() :binding(-1)
			, size(0)
			, offset(0)
		{

		}
	};

	struct Sampler
	{
		std::string name;
		int32 binding;
		std::string default_tex;

		Sampler() :binding(-1)
			, default_tex("white")
		{

		}
	};

	struct VertexAttribute
	{
		std::string name;
		VertexAttributeType type;
		int32 location;

		uint32 size;
		uint32 offset;

		VertexAttribute() :type(VertexAttributeType::None)
			, location(-1)
			, size(0)
			, offset(0)
		{

		}
	};

	struct VertexShader
	{
		std::string name;
		std::vector<std::string> includes;
		std::string src;
		UniformBufferNode uniform_buffer;
		std::vector<VertexAttribute> attrs;
		uint32 stride;

		VertexShader() :stride(0)
		{

		}
	};

	struct PixelShader
	{
		std::string name;
		std::vector<std::string> includes;
		std::string src;
		UniformBufferNode uniform_buffer;
		std::vector<Sampler> samplers;
	};

	struct Blend
	{
		bool enable;
		std::string src;
		std::string dst;
		std::string src_a;
		std::string dst_a;

		Blend() :
			enable(false)
		{
		}
	};

	struct Offset
	{
		bool enable;
		float factor;
		float units;

		Offset() :enable(false)
			, factor(0)
			, units(0)
		{

		}
	};

	struct Stencil
	{
		bool enable;
		uint32 RefValue;
		uint32 ReadMask;
		uint32 WriteMask;
		std::string Comp;
		std::string Pass;
		std::string Fail;
		std::string ZFail;

		Stencil() :enable(false)
			, RefValue(0)
			, ReadMask(255)
			, WriteMask(255)
			, Comp("Always")
			, Pass("Keep")
			, Fail("Keep")
			, ZFail("Keep")
		{

		}
	};

	struct RenderState
	{
		std::string name;
		std::string Cull;
		std::string ZTest;
		std::string ZWrite;
		std::string AlphaTest;
		Blend Blend;
		std::string ColorMask;
		Offset offset;
		Stencil stencil;

		RenderState() :Cull("Back")
			, ZTest("LEqual")
			, ZWrite("On")
			, AlphaTest("Off")
			, ColorMask("RGBA")
		{

		}

	};
}

#endif // __KGE_GRAPHICS_RENDERSTATE_H__