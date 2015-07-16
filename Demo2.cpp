#include "oxygine-framework.h"
#include "skelet_models/Model3D.h"

DECLARE_SMART(MainMenu, spMainMenu);

void game_preinit(oxygine::core::init_desc *desc)
{
	if (desc)
	{
		desc->w = 1064;
		desc->h = 600;
		desc->showDebugActor = false;
#ifndef _DEBUG
		desc->showDebugActor = false;
#endif
	}
	oxygine::core::setClearColor(oxygine::Color(0, 0, 127, 255));
}

class MainObject : public virtual Model3D//SkeletalModel3D
{
public:
	virtual void TapEvent(const std::string &msg)
	{
		if (msg == "start")
		{
		}
	}
	MainObject()
	{
		LoadFromFile("C:/Dropbox/Projects/Moon/pers/walking.dae");
		//AddButton(*this, "start_btn", this, "start");
	}
};

void game_init()
{
	oxygine::getStage()->addChild(new MainObject());
}

void game_destroy()
{
}

void game_update()
{

}
