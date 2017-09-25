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
	{
		_cameras.push_back(this);

		SetOrthographic(false);
		SetFieldOfView(60);
		SetClipNear(0.3f);
		SetClipFar(1000.0f);
	}

	Camera::~Camera()
	{
		_cameras.remove(this);
	}

	void Camera::PrepareAll()
	{
		for (Camera* camera : _cameras)
		{
			_current = camera;
			camera->Prepare();
		}

		_current = nullptr;
	}

	void Camera::RenderAll()
	{
		for (Camera* camera : _cameras)
		{
			_current = camera;
			camera->Render();
		}

		_current = nullptr;
	}

	void Camera::Prepare()
	{
		
	}

	void Camera::Render()
	{
		
	}

	void Camera::UpdateMatrix()
	{
		_matrix_dirty = false;

		uint32 width = Graphics::GetDisplay()->GetWidth();
		uint32 height = Graphics::GetDisplay()->GetHeight();

		Ref<Transform> transform = GetTransform();

		_view_matrix = Matrix();
		_view_matrix.lookAt(transform->GetWorldPosition(), transform->GetForward(), transform->GetUp());

		_projection_matrix = Matrix();
		if (!IsOrthographic())
		{
			_projection_matrix.perspectiveProjection(GetFieldOfView(), width / (float)height, GetClipNear(), GetClipFar());
		}
		else
		{
			_projection_matrix.orthogonalProjection((float)width, (float)height, GetClipNear(), GetClipFar());
		}

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

	uint32 Camera::GetTargetWidth() const
	{
		return _frame_buffer ? _frame_buffer->color_texture->GetWidth() : Graphics::GetDisplay()->GetWidth();
	}

	uint32 Camera::GetTargetHeight() const
	{
		return _frame_buffer ? _frame_buffer->color_texture->GetHeight() : Graphics::GetDisplay()->GetHeight();
	}
}

