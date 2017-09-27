//
//  shader_GLES.h
//  libkge
//
//  Created by kevin on 17-9-27.
//
//

#ifndef __KGE_GRAPHICS_SHADER_GLES_H__
#define __KGE_GRAPHICS_SHADER_GLES_H__

#include "core/object.h"
#include "common.h"
#include "math/matrix.hpp"

namespace kge
{
	class UniformBuffer;
	struct UniformBufferNode;
	struct Sampler;
	struct VertexShader;


	struct GLRenderState
	{
		bool offset_enable;			//	GL_POLYGON_OFFSET_FILL
		GLfloat offset_factor;		//	glPolygonOffset
		GLfloat offset_units;
		bool cull_enable;			//	GL_CULL_FACE
		GLenum cull_face;			//	glCullFace
		GLboolean color_mask_r;		//	glColorMask
		GLboolean color_mask_g;
		GLboolean color_mask_b;
		GLboolean color_mask_a;
		bool blend_enable;			//	GL_BLEND
		GLenum blend_src_c;			//	glBlendFuncSeparate
		GLenum blend_dst_c;
		GLenum blend_src_a;
		GLenum blend_dst_a;
		GLboolean depth_mask;		//	glDepthMask
		GLenum depth_func;			//	glDepthFunc	
		bool stencil_enable;		//	GL_STENCIL_TEST
		GLenum stencil_func;		//	glStencilFunc
		GLint stencil_ref;
		GLuint stencil_read_mask;
		GLuint stencil_write_mask;	//	glStencilMask
		GLenum stencil_op_fail;		//	glStencilOp
		GLenum stencil_op_zfail;
		GLenum stencil_op_pass;
	};

	struct ShaderPass
	{
		std::string name;
		GLuint program;
		std::vector<UniformBufferNode*> uniform_buffer_infos;
		std::vector<const Sampler*> sampler_infos;
		std::vector<GLint> sampler_locations;
		const VertexShader* vs;
		GLRenderState render_state;
		unsigned int buf_obj_index;
		unsigned int lightmap_location;
	};

	class ShaderGLES : public Object
	{
	public:
		virtual ~ShaderGLES();

		uint32 GetPassCount() const { return 1; }
		void ClearPipelines();
		void PreparePass(uint32 index) {}
		void UpdateRendererDescriptorSet(Ref<UniformBuffer>& descriptor_set_buffer, const void* data, uint32 size);
		void BeginPass(uint32 index);
		void BindRendererDescriptorSet(uint32 index, Ref<UniformBuffer>& descriptor_set_buffer);
		void EndPass(uint32 index) {}

		Ref<UniformBuffer> CreateUniformBuffer(uint32 index);
		const std::vector<const Sampler*>& GetSamplerInfos(uint32 index)const { return _passes[index].sampler_infos; }
		const std::vector<GLint>& GetSamplerLocations(uint32 index)const { return _passes[index].sampler_locations; }
		const std::vector<UniformBufferNode*> GetUniformBufferInfos(uint32 index)const { return _passes[index].uniform_buffer_infos; }
		const VertexShader* GetVertexShaderInfo(uint32 index)const { return _passes[index].vs; }

	protected:
		ShaderGLES();
		void Compile();

	private:
		void CreateShaders();
		void CreatePasses();

	private:
		std::vector<ShaderPass> _passes;
		std::map<std::string, GLuint> _vertex_shaders;
		std::map<std::string, GLuint> _pixel_shaders;
	};
}

#endif // __KGE_GRAPHICS_SHADER_GLES_H__