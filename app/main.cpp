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
#include "ui/ui_label.h"

using namespace kge;

class App : public IApplication
{
public:
	App();
	virtual void Start() override;
	virtual void Update() override;
	void App::TestUI();

	void OnTouchDownBG(const WeakRef<Object>& obj, UIPointerEvent& e);

	void OnDragBeginBG(const WeakRef<Object>& obj, UIPointerEvent& e);
	void OnDragBG(const WeakRef<Object>& obj, UIPointerEvent& e);
	void OnDragEndBG(const WeakRef<Object>& obj, UIPointerEvent& e);

private:
	WeakRef<Camera> _camera;
	WeakRef<Camera> _ui_camera;
	WeakRef<GameObject> _cube;
	float m_rotate_deg;
	WeakRef<GameObject> _gameObject;
	WeakRef<GameObject> _gameObject_ui;
	uint32 click_counter = 0;
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

	_gameObject = Resource::LoadGameObject("Assets/AppTestMesh/plane.prefab");

	auto ui_camera = GameObject::Create("camera")->AddComponent<Camera>();
	ui_camera->SetOrthographic(true);
	ui_camera->SetOrthographicSize(ui_camera->GetTargetHeight() / 2.0f);
	ui_camera->SetClipNear(-1);
	ui_camera->SetClipFar(1);
	_ui_camera = camera;

	auto scale_w = camera->GetTargetWidth() / 720.0f;
	auto scale_h = camera->GetTargetHeight() / 772.0f;
	auto scale_ui = min2(scale_w, scale_h);

	auto ui = Resource::LoadGameObject("Assets/AppTestUI/ui.prefab");
	ui->GetTransform()->SetWorldScale(Vector3::one() * scale_ui);
	_gameObject_ui = ui;

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

	auto font_canvas = GameObject::Create("canvas")->AddComponent<UICanvasRenderer>();
	font_canvas->GetTransform()->SetParent(camera->GetTransform());
	font_canvas->SetSize(Vector2((float)camera->GetTargetWidth(), (float)camera->GetTargetHeight()));

	//auto sp = Resource::read_sprite_group("Assets/AppFlappyBird/atlas.png.atlas");

	auto font = Resource::LoadFont("Assets/font/heiti.ttf");

	auto fps = GameObject::Create("fps")->AddComponent<UILabel>();
	fps->GetTransform()->SetParent(font_canvas->GetTransform());
	fps->SetFont(font);
	fps->SetFontSize(20);
	fps->SetColor(Color(0, 1, 0, 1));
	fps->SetText("测试中文1&&<color=#ff0000ff>啊English</color>");// ("123abc中sdf速度\nA<color=#ff0000ff>bbb</color>c<shadow=#ff0000ff>def</shadow>cc<outline=#ff0000ff>def</outline>cc<underline>def</underline>cc<bold>def</bold>cc<italic>def</italic>");
	fps->SetRich(true);
	fps->SetAlignment(TextAlignment::UpperLeft);

	fps->SetAnchors(Vector2(0, 1), Vector2(0, 1));
	int32 h = camera->GetTargetHeight();
	int32 w = camera->GetTargetWidth();
	fps->SetOffsets(Vector2(0, (float)-h), Vector2((float)w, 0));
	fps->OnAnchor();
	font_canvas->GetGameObject()->SetLayerRecursively(1);
}

void App::OnTouchDownBG(const WeakRef<Object>& obj, UIPointerEvent& e)
{
	click_counter++;
	auto img = RefCast<UIImage>(obj.lock());
	img->SetSpriteName(click_counter % 2 ? "bg_night" : "bg_day");
}

void App::OnDragBeginBG(const WeakRef<Object>& obj, UIPointerEvent& e)
{
	KGE_LOG_DEBUG("drag begin %s (%f, %f)", obj.lock()->GetName(), e.position.x, e.position.y);
}
void App::OnDragBG(const WeakRef<Object>& obj, UIPointerEvent& e)
{
	KGE_LOG_DEBUG("drag %s (%f, %f)", obj.lock()->GetName(), e.position.x, e.position.y);
}
void App::OnDragEndBG(const WeakRef<Object>& obj, UIPointerEvent& e)
{
	KGE_LOG_DEBUG("drag end %s (%f, %f)", obj.lock()->GetName(), e.position.x, e.position.y);
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
