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
#include "graphics/light.h"
#include "graphics/animation.h"

using namespace kge;

class AppStack : public IApplication
{
public:
	AppStack();
	virtual void Start() override;
	virtual void Update() override;

private:
	void InitUI();

	void OnTouchDownRestart(const WeakRef<Object>& obj, UIPointerEvent& e);

private:
	WeakRef<Camera> _camera;
	WeakRef<GameObject> _sky;
	WeakRef<GameObject> _base;
	WeakRef<GameObject> _current_stack;
	WeakRef<GameObject> _last_stack;

	WeakRef<UILabel> _lab_score;
	WeakRef<UILabel> _lab_combo;
	WeakRef<GameObject> _restart_go;

	std::vector<WeakRef<GameObject>> _pool;

	float _speed = 1;
	float _speed_ori = _speed;
	bool _toLeft = true;
	bool _toBack = true;
	bool _xDir = true;
	bool _pause = false;

	uint32 _count = 0;
	bool _game_over = false;

	float _step = 0.1f;
	float _height = _step * 0.5f;

	float _offset = 0.05f;

	float _base_height = -0.5f;

	uint32 _combo = 0;
};

AppStack::AppStack()
{
	SetName("AppStack");
	SetInitSize(512 * 10 / 16, 512);
}

void AppStack::Start()
{
	InitUI();

	_camera = GameObject::Create("camera")->AddComponent<Camera>();
	_camera.lock()->GetTransform()->SetLocalPosition(Vector3(-4, 2, -4));
	_camera.lock()->GetTransform()->SetLocalRotation(Quaternion::Euler(0, 45, 0));
	_camera.lock()->SetCullingMask(1 << 0);

	Resource::LoadGameObject("Assets/AppStack/sky.prefab");

	_base = Resource::LoadGameObject("Assets/AppStack/player.prefab");
	_base.lock()->GetTransform()->SetLocalPosition(Vector3(0, _base_height, 0));
	_last_stack = _base;

	_current_stack = GameObject::Instantiate(_base.lock());
	_current_stack.lock()->GetTransform()->SetLocalScale(Vector3(1, _step, 1));
	_current_stack.lock()->GetTransform()->SetLocalPosition(Vector3(0, _height, 0));
	_pool.push_back(_current_stack);
}

void AppStack::InitUI()
{
	auto camera = GameObject::Create("camera")->AddComponent<Camera>();
	camera->SetClipNear(-1);
	camera->SetClipFar(1);
	camera->SetDepth(1);
	camera->SetClearFlags(CameraClearFlags::Nothing);
	camera->SetCullingMask(1 << 1);
	camera->SetOrthographic(true);
	camera->SetOrthographicSize(camera->GetTargetHeight() / 2.0f);

	int32 h = camera->GetTargetHeight();
	int32 w = camera->GetTargetWidth();

	auto font = Resource::LoadFont("Assets/font/heiti.ttf");

	auto font_canvas = GameObject::Create("combo_canvas")->AddComponent<UICanvasRenderer>();
	font_canvas->GetTransform()->SetParent(camera->GetTransform());
	font_canvas->SetSize(Vector2((float)camera->GetTargetWidth(), (float)camera->GetTargetHeight()));

	auto combo = GameObject::Create("combo")->AddComponent<UILabel>();
	combo->GetTransform()->SetParent(font_canvas->GetTransform());
	combo->SetFont(font);
	combo->SetFontSize(20);
	combo->SetColor(Color(1, 1, 1, 1));
	combo->SetText("Combo: 0");
	combo->SetRich(true);
	combo->SetAlignment(TextAlignment::UpperLeft);
	combo->SetAnchors(Vector2(0, 1), Vector2(0, 1));
	combo->SetOffsets(Vector2(0, (float)-h), Vector2((float)w, 0));
	combo->OnAnchor();
	_lab_combo = combo;

	auto score = GameObject::Create("score")->AddComponent<UILabel>();
	score->GetTransform()->SetParent(font_canvas->GetTransform());
	score->SetFont(font);
	score->SetFontSize(20);
	score->SetColor(Color(1, 1, 1, 1));
	score->SetText("Score: 0");
	score->SetRich(true);
	score->SetAlignment(TextAlignment::UpperRight);
	score->SetAnchors(Vector2(0, 1), Vector2(0, 1));
	score->SetOffsets(Vector2(0, (float)-h), Vector2((float)w, 0));
	score->OnAnchor();
	_lab_score = score;

	auto restart_canvas = GameObject::Create("restart_canvas")->AddComponent<UICanvasRenderer>();
	restart_canvas->GetTransform()->SetParent(camera->GetTransform());
	restart_canvas->SetSize(Vector2(160, 30));
	_restart_go = restart_canvas->GetGameObject();

	auto restart = GameObject::Create("restart")->AddComponent<UILabel>();
	restart->GetTransform()->SetParent(restart_canvas->GetTransform());
	restart->SetFont(font);
	restart->SetFontSize(20);
	restart->SetColor(Color(1, 1, 1, 1));
	restart->SetText("点击重新开始");
	restart->SetRich(true);
	restart->SetAlignment(TextAlignment::MiddleCenter);
	restart->SetAnchors(Vector2(0, 1), Vector2(0, 1));
	restart->SetOffsets(Vector2(0, -30), Vector2(160, 0));
	restart->OnAnchor();
	restart->event_handler.enable = true;
	restart->event_handler.on_pointer_down = std::bind(&AppStack::OnTouchDownRestart, this, std::placeholders::_1, std::placeholders::_2);
	restart_canvas->GetGameObject()->SetLayerRecursively(1);
	_restart_go.lock()->SetActive(false);

	font_canvas->GetGameObject()->SetLayerRecursively(1);
}

void AppStack::OnTouchDownRestart(const WeakRef<Object>& obj, UIPointerEvent& e)
{
	_height = _step * 0.5f;

	_combo = _count = 0;

	_speed = _speed_ori;
	
	_lab_score.lock()->SetText("Combo: 0");
	_lab_combo.lock()->SetText("Score: 0");

	for (auto go : _pool)
	{
		GameObject::Destroy(go.lock());
	}

	_game_over = false;

	_current_stack = GameObject::Instantiate(_base.lock());
	_current_stack.lock()->GetTransform()->SetLocalScale(Vector3(1, _step, 1));
	_current_stack.lock()->GetTransform()->SetLocalPosition(Vector3(0, _height, 0));
	_pool.push_back(_current_stack);

	_last_stack = _base;

	_restart_go.lock()->SetActive(false);

	_base.lock()->GetTransform()->SetLocalPosition(Vector3(0, _base_height, 0));
}

void AppStack::Update()
{
	if (_current_stack.lock() && !_pause && !_game_over)
	{
		float x, y, z;

		Vector3 pos = _current_stack.lock()->GetTransform()->GetLocalPosition();
		x = pos.x; y = pos.y; z = pos.z;

		Vector3 size = _current_stack.lock()->GetTransform()->GetLocalScale();

		if (_xDir)
		{
			if (_toLeft)
				x -= _speed * Time::GetDeltaTime();
			else
				x += _speed * Time::GetDeltaTime();

			if (x < -1.f - size.x * 0.5f)
				_toLeft = false;
			if (x > 1.f + size.x * 0.5f)
				_toLeft = true;

			_current_stack.lock()->GetTransform()->SetLocalPosition(Vector3(x, y, z));
		}
		else
		{
			if (_toBack)
				z += _speed * Time::GetDeltaTime();
			else
				z -= _speed * Time::GetDeltaTime();

			if (z < -1.f - size.z * 0.5f)
				_toBack = true;
			if (z > 1.f + size.z * 0.5f)
				_toBack = false;

			_current_stack.lock()->GetTransform()->SetLocalPosition(Vector3(x, y, z));
		}
	}

	if (Input::GetMouseButtonDown(0) && _current_stack.lock() && _last_stack.lock())
	{
		auto current_pos = _current_stack.lock()->GetTransform()->GetWorldPosition();
		auto current_size = _current_stack.lock()->GetTransform()->GetWorldScale();
		auto last_pos = _last_stack.lock()->GetTransform()->GetWorldPosition();
		auto last_size = _last_stack.lock()->GetTransform()->GetWorldScale();

		float current_left = current_pos.x - current_size.x * 0.5f;
		float current_right = current_pos.x + current_size.x * 0.5f;
		float current_front = current_pos.z - current_size.z * 0.5f;
		float current_back = current_pos.z + current_size.z * 0.5f;

		float last_left = last_pos.x - last_size.x * 0.5f;
		float last_right = last_pos.x + last_size.x * 0.5f;
		float last_front = last_pos.z - last_size.z * 0.5f;
		float last_back = last_pos.z + last_size.z * 0.5f;

		if (current_right < last_left || current_left > last_right)
		{
			GameObject::Destroy(_current_stack.lock());
			_current_stack.reset();
			_game_over = true;
			_restart_go.lock()->SetActive(true);
		}
		else
		{

			float left, right, front, back;
			if (almostEqual(current_front, last_front, _offset * current_size.z) && almostEqual(current_left, last_left, _offset * current_size.x))
			{
				front = last_front;
				back = last_back;
				left = last_left;
				right = last_right;
				_current_stack.lock()->GetTransform()->SetLocalPosition(Vector3(last_pos.x, _height, last_pos.z));
				_combo++;
			}
			else
			{
				front = max2(current_front, last_front);
				back = min2(current_back, last_back);
				left = max2(current_left, last_left);
				right = min2(current_right, last_right);
				_combo = 0;
			}
			float size_z = back - front;
			float size_x = right - left;

			if (_combo > 7)
			{
				size_x *= 1.1f;
				size_z *= 1.1f;
				_combo = 0;
			}

			float x = (left + right) * 0.5f;
			float z = (back + front) * 0.5f;

			_current_stack.lock()->GetTransform()->SetLocalScale(Vector3(size_x, current_size.y, size_z));
			_current_stack.lock()->GetTransform()->SetLocalPosition(Vector3(x, _height, z));

			_last_stack = _current_stack;
			_height = current_pos.y + _step;

			_current_stack = GameObject::Instantiate(_current_stack.lock());
			_current_stack.lock()->GetTransform()->SetLocalScale(Vector3(size_x, _step, size_z));
			_current_stack.lock()->GetTransform()->SetLocalPosition(Vector3(x, _height, z));
			_pool.push_back(_current_stack);

			_xDir = !_xDir;

			_count++;

			if (_count >= 0 && _count <= 10)
				_speed = _speed_ori * 1.1f;
			else if (_count > 10 && _count <= 30)
				_speed = _speed_ori * 1.2f;
			else if (_count > 30 && _count <= 50)
				_speed = _speed_ori * 1.5f;
			else if (_count > 50 && _count <= 80)
				_speed = _speed_ori * 1.7f;
			else if (_count > 80)
				_speed = _speed_ori * 2.f;

			if (_height >= 1.f)
			{
				float old = _step * 0.5f;
				float offset = _height - old;
				_height = old;

				for (auto go : _pool)
				{
					Vector3 pos = go.lock()->GetTransform()->GetLocalPosition();
					pos.y = pos.y - offset;
					go.lock()->GetTransform()->SetLocalPosition(pos);
				}

				Vector3 base_pos = _base.lock()->GetTransform()->GetLocalPosition();
				base_pos.y = base_pos.y - offset;
				_base.lock()->GetTransform()->SetLocalPosition(base_pos);
			}

			std::string combo;
			if (StringFromFormat(combo, "Combo: %d", _combo))
				_lab_combo.lock()->SetText(combo);
			std::string score;
			if (StringFromFormat(score, "Score: %d", _count))
				_lab_score.lock()->SetText(score);
		}
	}
}

KGE_MAIN(AppStack);
