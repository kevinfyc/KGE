//
//  shader_GLES.cpp
//  libkge
//
//  Created by kevin on 17-9-27.
//
//

#include "shader_GLES.h"
#include "util/log.h"
#include "util/byte_buffer.h"
#include "render_state.h"
#include "shader_xml.h"
#include "graphics.h"
#include "display_GLES.h"
#include "io/file_tool.h"
#include "uniform_buffer.h"
#include "render_pass.h"
#include "shader.h"

namespace kge
{
	static const int UNIFORM_BUFFER_OBJ_BINDING = 0;

	static GLuint create_shader(GLenum type, const std::string& src)
	{
		KGE_LOG_GL_ERROR();

		auto shader = glCreateShader(type);

		auto source = src.c_str();
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLint len;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
			if (len > 0)
			{
				ByteBuffer buffer(len);
				glGetShaderInfoLog(shader, len, NULL, (GLchar*)buffer.Bytes());
				KGE_LOG_ERROR("shader compile error:\n%s", buffer.Bytes());
			}

			glDeleteShader(shader);
			shader = 0;
		}

		KGE_LOG_GL_ERROR();

		return shader;
	}

	static GLuint create_program(GLuint vs, GLuint ps)
	{
		KGE_LOG_GL_ERROR();

		auto program = glCreateProgram();

		glAttachShader(program, vs);
		glAttachShader(program, ps);

		glLinkProgram(program);

		GLint success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLint len;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
			if (len > 0)
			{
				ByteBuffer buffer(len);
				glGetProgramInfoLog(program, len, NULL, (GLchar*)buffer.Bytes());
				KGE_LOG_ERROR("shader link error:\n%s", buffer.Bytes());
			}

			glDeleteProgram(program);
			program = 0;
		}

		KGE_LOG_GL_ERROR();

		return program;
	}

	static void prepare_pipeline( const Pass& pass, ShaderXML& xml, ShaderPass& shader_pass)
	{
		KGE_LOG_GL_ERROR();

		DisplayGLES* display = (DisplayGLES*)Graphics::GetDisplay();
		GLuint program = shader_pass.program;

		shader_pass.buf_obj_index = glGetUniformBlockIndex(program, "buf_vs_obj");
		if (shader_pass.buf_obj_index != 0xffffffff)
			glUniformBlockBinding(program, shader_pass.buf_obj_index, UNIFORM_BUFFER_OBJ_BINDING);

		auto& uniform_buffer_infos = shader_pass.uniform_buffer_infos;
		for (auto& i : xml.vss)
		{
			if (i.name == pass.vs)
			{
				if (i.uniform_buffer.binding >= 0)
				{
					uniform_buffer_infos.push_back(&i.uniform_buffer);
				}

				shader_pass.vs = &i;
				break;
			}
		}
		for (auto& i : xml.pss)
		{
			if (i.name == pass.ps)
			{
				if (i.uniform_buffer.binding >= 0)
				{
					uniform_buffer_infos.push_back(&i.uniform_buffer);
				}
				break;
			}
		}

		auto& sampler_infos = shader_pass.sampler_infos;
		for (const auto& i : xml.pss)
		{
			if (i.name == pass.ps)
			{
				for (auto& j : i.samplers)
				{
					sampler_infos.push_back(&j);
				}
				break;
			}
		}

		int buffer_size = 0;
		for (auto& i : uniform_buffer_infos)
		{
			auto index = glGetUniformBlockIndex(program, i->name.c_str());
			glUniformBlockBinding(program, index, i->binding);

			GLint size;
			glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
			assert(size == i->size);

			int offset = buffer_size;
			int offset_alignment = display->GetMinUniformBufferOffsetAlignment();
			if (offset % offset_alignment != 0)
				offset += offset_alignment - (offset % offset_alignment);
			i->offset = offset;
			buffer_size += i->size;

			for (auto& j : i->uniforms)
			{
				auto name = i->name + "." + j.name;
				const char* names[] = { name.c_str() };

				GLuint index;
				glGetUniformIndices(program, 1, names, &index);
				GLint offset;
				glGetActiveUniformsiv(program, 1, &index, GL_UNIFORM_OFFSET, &offset);

				//Log("%s uniform:%s index:%d offset:%d", xml.name.CString(), name.CString(), index, offset);
			}
		}

		for (auto& i : sampler_infos)
		{
			auto location = glGetUniformLocation(program, i->name.c_str());

			shader_pass.sampler_locations.push_back(location);
		}

		const RenderState *prs = NULL;
		for (const auto& i : xml.rss)
		{
			if (i.name == pass.rs)
			{
				prs = &i;
				break;
			}
		}

		assert(prs != NULL);

		GLRenderState& state = shader_pass.render_state;

		state.offset_enable = prs->offset.enable;
		if (prs->offset.enable)
		{
			state.offset_factor = prs->offset.factor;
			state.offset_units = prs->offset.units;
		}

		if (prs->Cull == "Off")
		{
			state.cull_enable = false;
		}
		else
		{
			state.cull_enable = true;

			if (prs->Cull == "Back")
			{
				state.cull_face = GL_BACK;
			}
			else if (prs->Cull == "Front")
			{
				state.cull_face = GL_FRONT;
			}
		}

		{
			state.color_mask_r = GL_FALSE;
			state.color_mask_g = GL_FALSE;
			state.color_mask_b = GL_FALSE;
			state.color_mask_a = GL_FALSE;

			if (prs->ColorMask.find("R") >= 0)
				state.color_mask_r = GL_TRUE;
			if (prs->ColorMask.find("G") >= 0)
				state.color_mask_g = GL_TRUE;
			if (prs->ColorMask.find("B") >= 0)
				state.color_mask_b = GL_TRUE;
			if (prs->ColorMask.find("A") >= 0)
				state.color_mask_a = GL_TRUE;
		}

		state.blend_enable = prs->Blend.enable;
		if (prs->Blend.enable)
		{
			const char* strs[] = {
				"One",
				"Zero",
				"SrcColor",
				"SrcAlpha",
				"DstColor",
				"DstAlpha",
				"OneMinusSrcColor",
				"OneMinusSrcAlpha",
				"OneMinusDstColor",
				"OneMinusDstAlpha"
			};
			const int values[] = {
				GL_ONE,
				GL_ZERO,
				GL_SRC_COLOR,
				GL_SRC_ALPHA,
				GL_DST_COLOR,
				GL_DST_ALPHA,
				GL_ONE_MINUS_SRC_COLOR,
				GL_ONE_MINUS_SRC_ALPHA,
				GL_ONE_MINUS_DST_COLOR,
				GL_ONE_MINUS_DST_ALPHA
			};

			const int count = sizeof(values) / sizeof(values[0]);
			for (int i = 0; i < count; i++)
			{
				if (prs->Blend.src == strs[i])
				{
					state.blend_src_c = values[i];
				}

				if (prs->Blend.dst == strs[i])
				{
					state.blend_dst_c = values[i];
				}

				if (prs->Blend.src_a == strs[i])
				{
					state.blend_src_a = values[i];
				}

				if (prs->Blend.dst_a == strs[i])
				{
					state.blend_dst_a = values[i];
				}
			}
		}

		state.depth_mask = prs->ZWrite == "On" ? GL_TRUE : GL_FALSE;

		{
			const char* strs[] = {
				"Less",
				"Greater",
				"LEqual",
				"GEqual",
				"Equal",
				"NotEqual",
				"Always"
			};
			const int values[] = {
				GL_LESS,
				GL_GREATER,
				GL_LEQUAL,
				GL_GEQUAL,
				GL_EQUAL,
				GL_NOTEQUAL,
				GL_ALWAYS
			};

			const int count = sizeof(values) / sizeof(values[0]);
			for (int i = 0; i < count; i++)
			{
				if (prs->ZTest == strs[i])
				{
					state.depth_func = values[i];
					break;
				}
			}
		}

		state.stencil_enable = prs->stencil.enable;
		if (prs->stencil.enable)
		{
			state.stencil_ref = prs->stencil.RefValue;
			state.stencil_read_mask = prs->stencil.ReadMask;
			state.stencil_write_mask = prs->stencil.WriteMask;

			{
				const char* strs[] = {
					"Less",
					"Greater",
					"LEqual",
					"GEqual",
					"Equal",
					"NotEqual",
					"Always",
					"Never"
				};
				const int values[] = {
					GL_LESS,
					GL_GREATER,
					GL_LEQUAL,
					GL_GEQUAL,
					GL_EQUAL,
					GL_NOTEQUAL,
					GL_ALWAYS,
					GL_NEVER
				};

				const int count = sizeof(values) / sizeof(values[0]);
				for (int i = 0; i < count; i++)
				{
					if (prs->stencil.Comp == strs[i])
					{
						state.stencil_func = values[i];
						break;
					}
				}
			}

			{
				const char* strs[] = {
					"Keep",
					"Zero",
					"Replace",
					"IncrSat",
					"DecrSat",
					"Invert",
					"IncrWrap",
					"DecrWrap"
				};
				const int values[] = {
					GL_KEEP,
					GL_ZERO,
					GL_REPLACE,
					GL_INCR,
					GL_DECR,
					GL_INVERT,
					GL_INCR_WRAP,
					GL_DECR_WRAP,
				};

				const int count = sizeof(values) / sizeof(values[0]);
				for (int i = 0; i < count; i++)
				{
					if (prs->stencil.Pass == strs[i])
					{
						state.stencil_op_pass = values[i];
					}

					if (prs->stencil.Fail == strs[i])
					{
						state.stencil_op_fail = values[i];
					}

					if (prs->stencil.ZFail == strs[i])
					{
						state.stencil_op_zfail = values[i];
					}
				}
			}
		}

		KGE_LOG_GL_ERROR();
	}


	ShaderGLES::ShaderGLES()
	{
	}

	ShaderGLES::~ShaderGLES()
	{
		for (auto& i : _passes)
			glDeleteProgram(i.program);
		_passes.clear();

		for (auto& i : _vertex_shaders)
			glDeleteShader(i.second);
		_vertex_shaders.clear();

		for (auto& i : _pixel_shaders)
			glDeleteShader(i.second);
		_pixel_shaders.clear();
	}

	void ShaderGLES::Compile()
	{
		CreateShaders();
		CreatePasses();
	}

	const std::string g_shader_header =
		"#version 300 es\n"
		"#define KGE_GLES 1\n"
		"#define UniformBuffer(set_index, binding_index) layout(std140)\n"
		"#define UniformTexture(set_index, binding_index)\n"
		"#define Varying(location_index)\n";

	static std::string combine_shader_src(const std::vector<std::string>& includes, const std::string& src)
	{
		std::string source = g_shader_header;
		for (const auto& i : includes)
		{
			auto include_path = "shader/Include/" + i;
			std::string content;
			if (ReadFile(content, include_path, true))
				source += content + "\n";
		}
		source += src;

		return source;
	}

	void ShaderGLES::CreateShaders()
	{
		const auto& xml = ((Shader*) this)->_xml;

		for (const auto& i : xml.vss)
		{
			auto source = combine_shader_src(i.includes, i.src);
			KGE_LOG_ERROR("vs : %s", source.c_str());

			auto shader = create_shader(GL_VERTEX_SHADER, source);

			_vertex_shaders[i.name] = shader;
		}

		for (const auto& i : xml.pss)
		{
			auto source = combine_shader_src(i.includes, i.src);

			auto shader = create_shader(GL_FRAGMENT_SHADER, source);

			_pixel_shaders[i.name] = shader;
		}
	}

	void ShaderGLES::CreatePasses()
	{
		auto& xml = ((Shader*) this)->_xml;

		_passes.resize(xml.passes.size());
		for (uint32 i = 0; i < xml.passes.size(); i++)
		{
			auto& xml_pass = xml.passes[i];
			auto& pass = _passes[i];

			pass.name = xml_pass.name;
			pass.program = create_program(_vertex_shaders[xml_pass.vs], _pixel_shaders[xml_pass.ps]);

			prepare_pipeline(xml_pass, xml, pass);
		}
	}

	Ref<UniformBuffer> ShaderGLES::CreateUniformBuffer(uint32 index)
	{
		Ref<UniformBuffer> uniform_buffer;

		auto& pass = _passes[index];
		auto& uniform_buffer_infos = pass.uniform_buffer_infos;

		int buffer_size = 0;
		for (int i = uniform_buffer_infos.size() - 1; i >= 0; i--)
		{
			buffer_size = uniform_buffer_infos[i]->offset + uniform_buffer_infos[i]->size;
			break;
		}

		if (buffer_size > 0)
		{
			uniform_buffer = UniformBuffer::Create(buffer_size);
		}

		return uniform_buffer;
	}

	void ShaderGLES::BeginPass(uint32 index)
	{
		KGE_LOG_GL_ERROR();

		auto& rs = _passes[index].render_state;
		if (rs.offset_enable)
		{
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(rs.offset_factor, rs.offset_units);
		}
		else
		{
			glDisable(GL_POLYGON_OFFSET_FILL);
		}

		if (rs.cull_enable)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(rs.cull_face);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		glColorMask(rs.color_mask_r, rs.color_mask_g, rs.color_mask_b, rs.color_mask_a);

		if (rs.blend_enable)
		{
			glEnable(GL_BLEND);
			glBlendFuncSeparate(rs.blend_src_c, rs.blend_dst_c, rs.blend_src_a, rs.blend_dst_a);
		}
		else
		{
			glDisable(GL_BLEND);
		}

		glDepthMask(rs.depth_mask);
		glDepthFunc(rs.depth_func);

		if (rs.stencil_enable)
		{
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(rs.stencil_func, rs.stencil_ref, rs.stencil_read_mask);
			glStencilMask(rs.stencil_write_mask);
			glStencilOp(rs.stencil_op_fail, rs.stencil_op_zfail, rs.stencil_op_pass);
		}
		else
		{
			glDisable(GL_STENCIL_TEST);
		}

		int width = RenderPass::GetRenderPassBinding()->GetFrameBufferWidth();
		int height = RenderPass::GetRenderPassBinding()->GetFrameBufferHeight();
		auto rect = RenderPass::GetRenderPassBinding()->GetRect();

		int viewport_x = (int)(rect.x * width);
		int viewport_y = (int)(rect.y * height);
		int viewport_width = (int)(rect.width * width);
		int viewport_height = (int)(rect.height * height);

		glViewport(viewport_x, viewport_y, viewport_width, viewport_height);

		glUseProgram(_passes[index].program);

		KGE_LOG_GL_ERROR();
	}

	void ShaderGLES::UpdateRendererDescriptorSet(Ref<UniformBuffer>& descriptor_set_buffer, const void* data, uint32 size)
	{
		if (!descriptor_set_buffer)
		{
			descriptor_set_buffer = UniformBuffer::Create(size);
		}

		// update buffer
		descriptor_set_buffer->UpdateRange(0, size, data);
	}

	void ShaderGLES::BindRendererDescriptorSet(uint32 index, Ref<UniformBuffer>& descriptor_set_buffer)
	{
		KGE_LOG_GL_ERROR();

		auto& pass = _passes[index];
		if (pass.buf_obj_index != 0xffffffff)
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, UNIFORM_BUFFER_OBJ_BINDING, descriptor_set_buffer->GetBuffer());
		}

		KGE_LOG_GL_ERROR();
	}

}

