
#include "core/main_def.h"
#include "core/game_object.h"
#include "graphics/camera.h"

using namespace kge;

class App : public IApplication
{
public:
	App();
	virtual void Start() override;
	virtual void Update() override;

private:
	WeakRef<Camera> _camera;
};

App::App()
{
	SetName("App Test");
	SetInitSize(800, 600);
}

void App::Start()
{
	auto camera = GameObject::Create("camera")->AddComponent<Camera>();
	camera->SetClearColor(Color(1, 0, 0, 1));

	_camera = camera;
}

void App::Update()
{
	auto a = _camera.lock()->GetTransform();
	Vector3 pos = a->GetLocalPosition();

	float speed = 0.01f;

	_camera.lock()->GetTransform()->SetLocalPosition(Vector3(pos.x, pos.y, pos.z + speed));
}

KGE_MAIN(App);
