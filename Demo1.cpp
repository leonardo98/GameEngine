#include "oxygine-framework.h"
#include "buttons/TapButton.h"

DECLARE_SMART(MainMenu, spMainMenu);

// define window option
void game_preinit(oxygine::core::init_desc *desc)
{
	if (desc)
	{
		desc->w = 1064;
		desc->h = 600;
#ifdef _DEBUG
		desc->showDebugActor = true;
#else
		desc->showDebugActor = false;
#endif
	}
	oxygine::core::setClearColor(oxygine::Color(0, 0, 127, 255));
}

class MainMenu : public virtual TapButtonInterface
{
public:
	virtual void TapEvent(const std::string &msg)
	{
		if (msg == "start")
		{
			// todo: do some usefull staff
		}
	}
	MainMenu()
	{
		LoadFromFile("interface/description/mainscreen.xml");
		AddButton(*this, "start_btn", this, "start");
	}
};

// game init at start
void game_init()
{
	oxygine::getStage()->addChild(new MainMenu());
}

void game_destroy()
{
}

void game_update()
{

}
