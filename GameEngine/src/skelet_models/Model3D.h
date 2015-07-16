#include "actor.h"
#include "AnimationController.h"

//
// Model3D
//
class Model3D : virtual public oxygine::Actor
{
private:
	AnimationController _controller;
public:
	Model3D() : _controller("") {}
	void LoadFromFile(const char *fileName)
	{
		_controller.Import3DFromFile(fileName);
	}
	bool Ready() { return _controller.GetMaxIndex() > 0; }
};

