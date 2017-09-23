
#include "core/main_def.h"

class App : public kge::IApplication
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

}

KGE_MAIN(App);
