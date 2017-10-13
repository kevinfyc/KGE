//
//  camera.cpp
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#include "camera.h"
#include "graphics.h"
#include "display.h"
#include "core/transform.h"
#include "render_texture.h"
#include "render_pass.h"
#include "renderer.h"
#include "core/game_object.h"

namespace kge
{
	DEFINE_COM_CLASS(Camera);

	Camera* Camera::_current;
	std::list<Camera*> Camera::_cameras;

	bool Camera::Init()
	{
		return true;
	}

	void Camera::Fini()
	{
		_cameras.clear();
	}
	
	void Camera::DeepCopy(const Ref<Object>& source)
	{
		assert(!"can not copy a camera");
	}

	bool Camera::IsValidCamera(Camera* camera)
	{
		bool found = false;
		for (Camera* v : _cameras)
		{
			if (v == camera)
			{
				found = true;
				break;
			}
		}

		return found;
	}

	Camera::Camera() :_matrix_dirty(true)
		,_culling_mask(-1)
	{
		_cameras.push_back(this);

		SetClearFlags(CameraClearFlags::Color);
		SetClearColor(Color(0, 0, 0, 1));
		SetDepth(0);

		SetOrthographic(false);
		SetOrthographicSize(1);

		SetFieldOfView(60);
		SetClipNear(0.3f);
		SetClipFar(1000.0f);

		SetRect(Rect(0, 0, 1, 1));
	}

	Camera::~Camera()
	{
		_cameras.remove(this);
	}

	void Camera::SetCullingMask(uint32 mask)
	{
		if (_culling_mask != mask)
		{
			_culling_mask = mask;

			Renderer::SetCullingDirty(this);
		}
	}

	bool Less(const Camera* c1, Camera* c2)
	{
		return c1->GetDepth() < c2->GetDepth();
	}

	void Camera::OnTransformChanged()
	{
		_matrix_dirty = true;

		Renderer::SetCullingDirty(this);
	}
	
	void Camera::OnResize(uint32 width, uint32 height)
	{
		for (auto i : _cameras)
		{
			i->_render_pass.reset();
			i->_matrix_dirty = true;

			Renderer::SetCullingDirty(i);

			if (i->IsOrthographic())
			{
				i->SetOrthographicSize(height * 0.5f);
			}
		}

		Renderer::OnResize(width, height);
	}

	void Camera::SetDepth(uint32 depth)
	{
		_depth = depth;

		_cameras.sort(Less);
	}

	bool Camera::CanRender() const
	{
		return GetGameObject()->IsActiveInHierarchy() && IsEnable();
	}

	bool Camera::IsCulling(const Ref<GameObject>& obj) const
	{
		return (GetCullingMask() & (1 << obj->GetLayer())) == 0;
	}

	void Camera::PrepareAll()
	{
		for (Camera* camera : _cameras)
		{
			if (camera->CanRender())
			{
				_current = camera;
				camera->Prepare();
			}
		}

		_current = nullptr;
	}

	void Camera::RenderAll()
	{
		for (Camera* camera : _cameras)
		{
			if (camera->CanRender())
			{
				_current = camera;
				camera->Render();
			}
		}

		_current = nullptr;
	}

	void Camera::Prepare()
	{
		if (!_render_pass)
			_render_pass = RenderPass::Create(Ref<RenderTexture>(), Ref<RenderTexture>(), GetClearFlags(), true, GetRect());

		_render_pass->Bind();

		Renderer::PrepareAllPass();

		_render_pass->UnBind();
	}

	void Camera::Render()
	{
		_render_pass->Begin(GetClearColor());

		Renderer::RenderAllPass();

		_render_pass->End();
	}

	void Camera::UpdateMatrix()
	{
		_matrix_dirty = false;

		uint32 width = Graphics::GetDisplay()->GetWidth();
		uint32 height = Graphics::GetDisplay()->GetHeight();

		Ref<Transform> transform = GetTransform();

		_view_matrix = Matrix();
		_view_matrix.lookAt(transform->GetWorldPosition(), transform->GetForward(), transform->GetUp());

		_projection_matrix = Matrix::identity;
		if (!IsOrthographic())
		{
			_projection_matrix.perspectiveProjection(GetFieldOfView(), width / (float)height, GetClipNear(), GetClipFar());
		}
		else
		{
			float ortho_size = GetOrthographicSize();
			auto rect = GetRect();

			float top = ortho_size;
			float bottom = -ortho_size;
			float plane_h = ortho_size * 2;
			float plane_w = plane_h * (width * rect.width) / (height * rect.height);
			_projection_matrix.orthogonalProjection(-plane_w / 2, plane_w / 2, bottom, top, GetClipNear(), GetClipFar());
		}

		_view_projection_matrix.setIdentity();
		_view_projection_matrix.multiply(_projection_matrix, _view_matrix);
	}

	const Matrix& Camera::GetViewMatrix()
	{
		if (_matrix_dirty)
			UpdateMatrix();

		return _view_matrix;
	}

	const Matrix& Camera::GetProjectionMatrix()
	{
		if (_matrix_dirty)
			UpdateMatrix();

		return _projection_matrix;
	}

	const Matrix& Camera::GetViewProjectionMatrix()
	{
		if (_matrix_dirty)
			UpdateMatrix();

		return _view_projection_matrix;
	}
	
	const Frustum& Camera::GetFrustum()
	{
		if (_matrix_dirty)
		{
			UpdateMatrix();

			_frustum = Frustum(_view_projection_matrix);
		}

		return _frustum;
	}

	uint32 Camera::GetTargetWidth() const
	{
		return _frame_buffer ? _frame_buffer->color_texture->GetWidth() : Graphics::GetDisplay()->GetWidth();
	}

	uint32 Camera::GetTargetHeight() const
	{
		return _frame_buffer ? _frame_buffer->color_texture->GetHeight() : Graphics::GetDisplay()->GetHeight();
	}
}

