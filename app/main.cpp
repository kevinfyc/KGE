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

#include "ui/ui_canvas_renderer.h"
#include "ui/ui_image.h"

using namespace kge;

class App : public IApplication
{
public:
	App();
	virtual void Start() override;
	virtual void Update() override;
	void App::TestUI();

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
	TestUI();

	auto camera = GameObject::Create("camera")->AddComponent<Camera>();
	camera->GetTransform()->SetWorldPosition(Vector3(0, 1, -4));
	camera->SetCullingMask(1 << 0);

	_camera = camera;

	m_rotate_deg = 0.1f;

	_gameObject = Resource::LoadGameObject("Assets/AppMesh/plane.prefab");

}

void App::TestUI()
{
	auto camera = GameObject::Create("camera")->AddComponent<Camera>();
	camera->SetClipNear(-1);
	camera->SetClipFar(1);
	camera->SetDepth(1);
	camera->SetClearFlags(CameraClearFlags::Nothing);
	camera->SetCullingMask(1 << 1);
	camera->SetOrthographic(true);
	camera->SetOrthographicSize(camera->GetTargetHeight() / 2.0f);

	auto canvas = GameObject::Create("canvas")->AddComponent<UICanvasRenderer>();
	canvas->GetTransform()->SetParent(camera->GetTransform());
	canvas->SetSize(Vector2((float)camera->GetTargetWidth(), (float)camera->GetTargetHeight()));

	auto sp = Resource::read_sprite_group("Assets/AppFlappyBird/atlas.png.atlas");

	auto img = GameObject::Create("img")->AddComponent<UIImage>();
	img->GetTransform()->SetParent(canvas->GetTransform());
	img->SetSpriteGroup(sp);
	img->SetSpriteName("bg_day");
	img->OnAnchor();

	canvas->GetGameObject()->SetLayerRecursively(1);
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
