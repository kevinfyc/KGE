#include "core/main_def.h"
#include "core/Application.h"
#include "core/game_object.h"
#include "graphics/camera.h"
#include "graphics/mesh.h"
#include "graphics/material.h"
#include "graphics/mesh_renderer.h"
#include "util/time.h"
#include "core/resource.h"
#include "core/input.h"
#include "graphics/graphics.h"
#include "graphics/display.h"

using namespace kge;

class App : public IApplication
{
public:
	App();
	virtual void Start() override;
	virtual void Update() override;

private:
	WeakRef<Camera> _camera;
	WeakRef<GameObject> _cube;
	float m_rotate_deg;
	WeakRef<GameObject> _gameObject;
};

App::App()
{
	SetName("App Test");
	SetInitSize(800, 600);
}

void App::Start()
{
	auto camera = GameObject::Create("camera")->AddComponent<Camera>();
	camera->GetTransform()->SetWorldPosition(Vector3(0, 1, -4));
	camera->SetCullingMask(1 << 0);

	_camera = camera;

	m_rotate_deg = 0.1f;

	_gameObject = Resource::LoadGameObject("Assets/AppMesh/plane.prefab");

}

void App::Update()
{
	Quaternion rot = Quaternion();
	rot.fromAngleAxis(m_rotate_deg * Deg2Rad, Vector3(0, 1, 0));
	//_cube.lock()->GetTransform()->SetLocalRotation(rot);
	_gameObject.lock()->GetTransform()->SetLocalRotation(rot);
	m_rotate_deg += 30 * Time::GetDeltaTime();

	if (Input::GetMouseButton(0))
	{
		auto mouse = Input::GetMousePosition();
		if (mouse.x > Graphics::GetDisplay()->GetWidth() * 0.5f)
		{
			auto pos = _camera.lock()->GetTransform()->GetWorldPosition();
			pos = pos + (_camera.lock()->GetTransform()->GetForward() * Time::GetDeltaTime() * 5);
			_camera.lock()->GetTransform()->SetWorldPosition(pos);
		}
		else
		{
			auto pos = _camera.lock()->GetTransform()->GetWorldPosition();
			pos = pos + (-_camera.lock()->GetTransform()->GetForward() * Time::GetDeltaTime() * 5);
			_camera.lock()->GetTransform()->SetWorldPosition(pos);
		}
	}
}

KGE_MAIN(App);
