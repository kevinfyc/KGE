//
//  metrial_GLES.h
//  libkge
//
//  Created by kevin on 17-9-27.
//
//

#ifndef __KGE_GRAPHICS_MATERIAL_GLES_H__
#define __KGE_GRAPHICS_MATERIAL_GLES_H__

#include "core/object.h"

namespace kge
{
	class Texture;
	class UniformBuffer;

	class MaterialGLES: public Object
	{
	public:
		void Apply(uint32 pass_index);

	protected:
		void UpdateUniformsBegin(uint32 pass_index);
		void UpdateUniformsEnd(uint32 pass_index) {}
		void* SetUniformBegin(uint32 pass_index);
		void SetUniformEnd(uint32 pass_index);
		void SetUniform(uint32 pass_index, void* uniform_buffer, const std::string& name, void* data, uint32 size);
		void SetUniformTexture(uint32 pass_index, const std::string & name, const Texture* texture) {}

	private:
		std::vector<Ref<UniformBuffer>> _uniform_buffers;
	};
}

#endif // __KGE_GRAPHICS_MATERIAL_GLES_H__