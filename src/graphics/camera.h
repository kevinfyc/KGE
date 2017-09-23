//
//  camera.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_GRAPHICS_CAMERA_H__
#define __KGE_GRAPHICS_CAMERA_H__

#include "core/component.h"
#include "color.h"

#include "math/math_def.h"

namespace kge
{
	class Camera : public Component
	{
		DECLARE_COM_CLASS(Camera, Component)

	public:
		static bool Init();
		static void Fini();
		static void PrepareAll();
		static void RenderAll();
		static Camera* Current() { return _current; }
		static bool IsValidCamera(Camera* camera);

		virtual ~Camera();

		void SetClearColor(const Color& color) { _clear_color = color; }
		const Color& GetClearColor()const { return _clear_color; }

		void SetOrthographic(bool value) { _orthographic = value; }
		bool IsOrthographic()const { return _orthographic; }


		float GetFieldOfView() const { return _field_of_view; }
		void SetFieldOfView(float fov) { _field_of_view = fov; }

		float GetClipNear() const { return _clip_near; }
		void SetClipNear(float value) { _clip_near = value; }

		float GetClipFar() const { return _clip_far; }
		void SetClipFar(float value) { _clip_far = value; }

		const Matrix& GetViewMatrix();
		const Matrix& GetProjectionMatrix();
		const Matrix& GetViewProjectionMatrix();

	private:
		Camera();
		void Prepare();
		void Render();

		void UpdateMatrix();

	private:
		static std::list<Camera*> _cameras;
		static Camera* _current;
		Color _clear_color;
		bool _orthographic;

		float _field_of_view;
		float _clip_near;
		float _clip_far;

		bool _matrix_dirty;
		Matrix _view_matrix;
		Matrix _projection_matrix;
		Matrix _view_projection_matrix;
	};
}

#endif // __KGE_GRAPHICS_CAMERA_H__