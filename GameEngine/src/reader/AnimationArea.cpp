#include "AnimationArea.h"
#include "../Core/Core.h"
#include "../Core/Math.h"

AnimationArea::~AnimationArea() {
}

AnimationArea::AnimationArea(TiXmlElement *xe) 
: BeautyBase(xe)
, _animationReplace(NULL)
{
	_lib = xe->Attribute("file");
	_id = xe->Attribute("id");
	_shiftX = atoi(xe->Attribute("shiftX"));
	_shiftY = atoi(xe->Attribute("shiftY"));
	_animation = My::AnimationManager::getAnimation(_id);
	const char *tmp = xe->Attribute("start");
	_time = tmp ? atof(tmp) : 0;
	_state = state_stop;
}

AnimationArea::AnimationArea(const AnimationArea &b)
: BeautyBase(b)
, _animationReplace(NULL)
{
	_lib = b._lib;
	_id = b._id;
	_animation = My::AnimationManager::getAnimation(_id);
	_time = b._time;
	_state = state_stop;
	_shiftX = b._shiftX;
	_shiftY = b._shiftY;
}

void AnimationArea::Draw() {
    oxygine::Vector2 a(- Width() / 2 * fabs(_sx), 0);
    oxygine::Vector2 b(Width() / 2 * fabs(_sx), 0);
	Render::PushMatrix();
	Render::MatrixMove(_pos.x, _pos.y);
    Render::GetCurrentMatrix().Mul(a);
    Render::GetCurrentMatrix().Mul(b);
    if (a.x > Render::ScreenWidth() || b.x < 0)
    {
        Render::PopMatrix();
        return;
    }
	Render::MatrixRotate(_angle);
	Render::MatrixScale(_sx, _sy);
	Render::MatrixMove(_shiftX, _shiftY);
	Render::PushColorAndMul(_color);
	if (_state != state_replace)
	{
		_animation->Draw(_time);
	}
	else
	{
		_animationReplace->Draw(_time);
	}
	Render::PopColor();
	Render::PopMatrix();
}

void AnimationArea::GetLocalMatrix(Matrix &m) 
{
	m.Unit();
	m.Move(_pos.x, _pos.y);
	m.Rotate(_angle);
	m.Scale(_sx, _sy);
	//m.Move(_shiftX, _shiftY);// (?) - не уверен что это шаг нужен
}


std::string AnimationArea::Type() const { 
	return "Animation"; 
}

void AnimationArea::Update(float dt) {
	if (_state == state_loop || _state == state_play || _state == state_play_and_stop_at_last_frame) {
		_time += dt / _animation->Time();
		if (_time >= 1.f) {
			if (_state == state_play_and_stop_at_last_frame) {
				_time = 1.f;
				_state = state_last_frame;
			} else if (_state == state_play) {
				_time = 0.f;
				_state = state_stop;
			} else if (_state == state_loop) {
				while (_time >= 1.f) {
					_time -= 1.f;
				}
			} else {
				assert(false);
			}
		}
	}
	if (_state == state_replace) {
		_time += dt / _animationReplace->Time();
		if (_time >= 1.f) {
			_state = state_loop;
			_time = 0.f;
		}
	}
}

bool AnimationArea::Command(const std::string &cmd) {
	if (cmd == "stop") {
		_state = state_stop;
		return true;
	} else if (cmd == "play") {
		_state = state_play;
		_time = 0.f;
		return true;
	} else if (cmd == "reset") {
		_state = state_stop;
		_time = 0.f;
		return true;
	} else if (cmd == "play and stop") {
		_state = state_play_and_stop_at_last_frame;
		_time = 0.f;
		return true;
	} else if (cmd == "rewind") {
		_time += 0.2499f;
		if (_time > 1.f) {
			_time = 0.f;
		}
		return true;
	} else if (cmd == "loop") {
		_state = state_loop;
		return true;
	} else if (cmd.substr(0, 8) == "replace ") {
		_time = 0.f;
		_state = state_replace;
		_animationReplace = My::AnimationManager::getAnimation(cmd.substr(8));
		return true;
	}
	return (cmd == "" || BeautyBase::Command(cmd));
}


