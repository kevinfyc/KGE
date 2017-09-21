
#include "core/main_def.h"

class App : public kge::IApplication
{
public:
	App()
	{
		SetName("App");
	}

};

KGE_MAIN(App);
