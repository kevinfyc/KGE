
#include "core/main_def.h"
#include "core/game_object.h"
#include "graphics/camera.h"

using namespace kge;

class App : public IApplication
{
public:
	App();
	virtual void Start() override;

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
}

KGE_MAIN(App);
