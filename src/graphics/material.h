//
//  metrial.h
//  libkge
//
//  Created by kevin on 17-9-27.
//
//

#ifndef __KGE_GRAPHICS_MATERIAL_H__
#define __KGE_GRAPHICS_MATERIAL_H__

#include "core/object.h"
#include "texture.h"
#include "shader.h"
#include "color.h"
#include "math/matrix.hpp"

#if KGE_GLES
#include "material_GLES.h"
#endif


namespace kge
{
	class Camera;

#if KGE_GLES
	class Material: public MaterialGLES
#endif
	{
	public:
		static Ref<Material> Create(const std::string& shader_name);
		const Ref<Shader>& GetShader()const { return _shader; }

		void SetMatrix(const std::string & name, const Matrix& v);
		const Matrix& GetMatrix(const std::string& name);

		void SetVector(const std::string & name, const Vector4& v);
		const Vector4& GetVector(const std::string& name);
		bool HasVector(const std::string& name)const;

		void SetColor(const std::string& name, const Color& v);
		const Color& GetColor(const std::string& name) ;

		void SetMainColor(const Color& v);
		const Color& GetMainColor();

		void SetVectorArray(const std::string& name, const std::vector<Vector4>& v);
		const std::vector<Vector4>& GetVectorArray(const std::string& name);

		void SetMainTexture(const Ref<Texture>& v);
		const Ref<Texture>& GetMainTexture();
		bool HasMainTexture();

		void SetTexture(const std::string & name, const Ref<Texture>& v);

		const std::map<std::string, Ref<Texture>>& GetTextures() const { return _textures; }

		void SetMainTexTexelSize(const Ref<Texture>& tex);
		void SetZBufferParams(const Ref<Camera>& camera);
		void SetProjectionParams(const Ref<Camera>& camera);

		void UpdateUniforms(uint32 pass_index);
	private:
		Material();

	private:
		Ref<Shader> _shader;
		std::map<std::string, Ref<Texture>> _textures;
		std::map<std::string, Matrix> _matrices;
		std::map<std::string, Vector4> _vectors;
		std::map<std::string, std::vector<Vector4>> _vector_arrays;
		std::map<std::string, Color> _colors;
	};
}

#endif // __KGE_GRAPHICS_MATERIAL_H__