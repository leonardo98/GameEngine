#include "TapButton.h"
#include "actor.h"

using namespace oxygine;

void TapButton::Init(const LevelSet &levelSet, const char *id, bool invisible)
{
	BeautyList list;
	levelSet.SelectByUserString(id, list);
	_touchArea = NULL;
	for(BeautyList::iterator i = list.begin(); i != list.end(); ++i)
	{
		if ((*i)->Type() == "ClickArea")
		{
			assert(_touchArea == NULL);
			_touchArea = *i;
		}
		else
		{
            //(*i)->TweenCheck();
			_btns.push_back(*i);
			_poses.push_back(Poses());
			_poses.back().InitWithBeauty(*i);
		}
	}

	assert((invisible || _btns.size() > 0) && _touchArea != NULL);

	_isTouch = false;
}

TapButton::TapButton(const LevelSet &levelSet, const char *id, LevelSet *recever, const char *message, bool invisible)
	: _visible(true)
	, _enable(true)
	, _message(message)
	, _eventReceiver(recever)
	, _checked(true)
{
	Init(levelSet, id, invisible);
}

void TapButton::SetVisible(bool visible)
{
	_visible = visible;
	for (BeautyList::iterator b = _btns.begin(), e = _btns.end(); b != e; ++b)
	{
		(*b)->setVisible(_visible);
	}
}

void TapButton::SetChecked(bool checked)
{
	_checked = checked;
	assert(_btns.size() > 1);
	
	if (!_checked)
		_btns.back()->setAlpha(0);
	else
		_poses.back().Reset();
}

void TapButton::Command(const std::string &cmd)
{
	for (BeautyList::iterator b = _btns.begin(), e = _btns.end(); b != e; ++b)
	{
		(*b)->Command(cmd);
	}
}

bool TapButton::Check(const Vector2 &pos)
{
	if (!_visible || !_enable)
	{
		return false;
	}
	bool oldValueIsTouch = _isTouch;
	_isTouch = _touchArea->PixelCheck(pos);
	if (!oldValueIsTouch && _isTouch)
	{
		static float MaxScale = 1.1f;
		for (unsigned int i = 0; i < _btns.size(); ++i)
		{
			_btns[i]->addTween(TweenScale(_poses[i].scale * MaxScale), 100);
		}
		//SoundEffect("button");
	}
	else if (oldValueIsTouch && !_isTouch)
	{
		for (unsigned int i = 0; i < _btns.size(); ++i)
		{
			_btns[i]->addTween(TweenScale(_poses[i].scale), 100);
		}
	}
	return _isTouch;
}

bool TapButton::UnderPos(const Vector2 &pos)
{
	return _enable && _visible && _touchArea->PixelCheck(pos);
}

void TapButton::EndTouch()
{
	if (_enable && _isTouch)
	{
		_eventReceiver->TapEvent(_message);
		CancelTouch();
	}
}

void TapButton::CancelTouch()
{
	if (_isTouch)
	{
		for (unsigned int i = 0; i < _btns.size(); ++i)
		{
			_btns[i]->removeTweens();
			_btns[i]->addTween(TweenScale(_poses[i].scale), 75);
		}
		_isTouch = false;
	}
}

void TapButton::Reset()
{
	_enable = true;
	_isTouch = false;
    for (BeautyList::iterator b = _btns.begin(), e = _btns.end(); b != e; ++b)
	{
		(*b)->removeTweens();
	}
	for (unsigned int i = 0, e = _poses.size(); i != e; ++i)
	{
		_poses[i].Reset();
	}
}

void TapButton::TweenEvent()
{
	SetVisible(false);
}

void TapButton::HideTo(float x, float y, float time)
{
	Reset();
	_enable = false;
	Vector2 shift(x, y);
    for (BeautyList::iterator b = _btns.begin(), e = _btns.end(); b != e; ++b)
	{
		(*b)->addTween(TweenPosition((*b)->getPosition() + shift), time * 1000);
		(*b)->addTween(TweenAlpha(0), time * 1000);
	}
}

void TapButton::ShowFrom(float x, float y, float time)
{
	SetVisible(true);
	Reset();
	_enable = true;
	Vector2 shift(x, y);
    for (BeautyList::iterator b = _btns.begin(), e = _btns.end(); b != e; ++b)
	{
		Vector2 pos((*b)->getPosition());
		(*b)->setPosition(pos + shift);
		(*b)->addTween(TweenPosition(pos), time, 1, false, 0, Tween::ease_inBounce);

		float alpha = (*b)->getAlpha();
		(*b)->setAlpha(0);
		(*b)->addTween(TweenAlpha(alpha), time);		
	}
}

//BeautyText *TapButton::GetBeautyText()
//{
//    for (BeautyList::iterator b = _btns.begin(), e = _btns.end(); b != e; ++b)
//	{
//		if ((*b)->Type() == "BeautyText")
//		{
//			return static_cast<BeautyText *>(*b);
//		}
//	}
//	return NULL;
//}

TapButtonInterface::TapButtonInterface()
{
	Clear();
	setTouchEnabled(true);
	addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &TapButtonInterface::OnTouchMove));
	addEventListener(TouchEvent::MOVE, CLOSURE(this, &TapButtonInterface::OnTouchMove));
	addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &TapButtonInterface::OnTouchEnd));
}

TapButton * TapButtonInterface::AddButton(const LevelSet &levelSet, const char *id, LevelSet *receiver, const char *message, bool invisible)
{
	TapButton *btn = new TapButton(levelSet, id, receiver, message, invisible);
	_list.push_back(btn);
	return btn;
}

void TapButtonInterface::OnTouchMove(Event *ev)
{
	if (!getPressed())
		return;

	TouchEvent *t = dynamic_cast<TouchEvent *>(ev);
	if (t == NULL)
		return;

	Vector2 pos(t->localPosition.x, t->localPosition.y);
	if (_state == touch_canceled)
	{
		return;
	}
	for (TapButtonList::iterator i = _list.begin(), e = _list.end(); i != e; ++i)
	{
		(*i)->Check(pos);
	}
}

void TapButtonInterface::OnTouchEnd(Event *ev)
{
	_state = touch_none;
	for (TapButtonList::iterator i = _list.begin(), e = _list.end(); i != e; ++i)
	{
		(*i)->EndTouch();
	}
}

void TapButtonInterface::TouchCancel()
{
	_state = touch_canceled;
	for (TapButtonList::iterator i = _list.begin(), e = _list.end(); i != e; ++i)
	{
		(*i)->CancelTouch();
	}
}

void TapButtonInterface::Reset()
{
	_state = touch_none;
	for (TapButtonList::iterator i = _list.begin(), e = _list.end(); i != e; ++i)
	{
		(*i)->Reset();
	}
}

void TapButtonInterface::Clear()
{
	_state = touch_none;
	for (TapButtonList::iterator i = _list.begin(), e = _list.end(); i != e; ++i)
	{
		delete  (*i);
	}
	_list.clear();
}

TapButtonInterface::~TapButtonInterface()
{
	Clear();
}

bool TapButtonInterface::UnderPos(const Vector2 &pos)
{
	for (TapButtonList::iterator i = _list.begin(), e = _list.end(); i != e; ++i)
	{
		if ((*i)->UnderPos(pos))
		{
			return true;
		}
	}
	return false;
}

bool TapButtonInterface::isOn(const Vector2 &localPosition)
{
	return true;
}
