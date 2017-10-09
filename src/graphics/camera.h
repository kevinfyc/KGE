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
#include "math/rect.h"
#include "math/frustum.h"

#include "frame_buffer.h"

#include "camera_clear_flag.h"

namespace kge
{
	class RenderPass;

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

		uint32 GetDepth() const { return _depth; }
		void SetDepth(uint32 depth);

		CameraClearFlags GetClearFlags() const { return _clear_flags; }
		void SetClearFlags(CameraClearFlags flags) { _clear_flags = flags; }

		void SetClearColor(const Color& color) { _clear_color = color; }
		const Color& GetClearColor()const { return _clear_color; }

		void SetOrthographic(bool value) { _orthographic = value; }
		bool IsOrthographic()const { return _orthographic; }

		float GetOrthographicSize() const { return _orthographic_size; }
		void SetOrthographicSize(float size) { _orthographic_size = size; }

		float GetFieldOfView() const { return _field_of_view; }
		void SetFieldOfView(float fov) { _field_of_view = fov; }

		float GetClipNear() const { return _clip_near; }
		void SetClipNear(float value) { _clip_near = value; }

		float GetClipFar() const { return _clip_far; }
		void SetClipFar(float value) { _clip_far = value; }

		const Rect& GetRect() const { return _rect; }
		void SetRect(const Rect& rect) { _rect = rect; }

		int32 GetCullingMask() const { return _culling_mask; }
		void SetCullingMask(uint32 mask);

		bool CanRender() const;
		bool IsCulling(const Ref<GameObject>& obj) const;

		const Matrix& GetViewMatrix();
		const Matrix& GetProjectionMatrix();
		const Matrix& GetViewProjectionMatrix();
		const Frustum& GetFrustum();

		void SetFrameBuffer(const Ref<FrameBuffer>& frame_buffer) { _matrix_dirty = true; _frame_buffer = frame_buffer; };
		uint32 GetTargetWidth() const;
		uint32 GetTargetHeight() const;

	protected:
		virtual void OnTransformChanged() override;

	private:
		Camera();
		void Prepare();
		void Render();

		void UpdateMatrix();

	private:
		static std::list<Camera*> _cameras;
		static Camera* _current;
		Color _clear_color;
		CameraClearFlags _clear_flags;
		bool _orthographic;
		float _orthographic_size;

		float _field_of_view;
		float _clip_near;
		float _clip_far;
		Rect _rect;

		Ref<FrameBuffer> _frame_buffer;

		bool _matrix_dirty;
		Matrix _view_matrix;
		Matrix _projection_matrix;
		Matrix _view_projection_matrix;
		Frustum _frustum;

		Ref<RenderPass> _render_pass;

		int32 _culling_mask;
		uint32 _depth;
	};
}

#endif // __KGE_GRAPHICS_CAMERA_H__