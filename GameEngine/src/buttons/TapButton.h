#ifndef TAP_BUTTON_H
#define TAP_BUTTON_H

#include "../reader/LevelSet.h"

struct Poses
{
	oxygine::Vector2 pos;
	oxygine::Vector2 scale;
	unsigned int angle;
	float alpha;
	oxygine::Color color;
	BeautyBase *beauty;
	Poses() : beauty(NULL) {}
	void InitWithBeauty(BeautyBase *b)
	{
		beauty = b;
		pos = b->getPosition();
		scale = b->getScale();
		angle = b->getRotation();
		alpha = b->getAlpha();
		color = b->getColor();
	}
	void Reset() 
	{
		beauty->setPosition(pos);
		beauty->setScale(scale);
		beauty->setRotation(angle);
		beauty->setAlpha(alpha);
		beauty->setColor(color);
	}
};

//class EventListener
//{
//public:
//	virtual int Event(const std::string &msg) { return -1; };
//};

class TapButton : virtual public Actor
	//: TweenEventListener
{
private:
	BeautyList _btns;
	BeautyBase *_touchArea;

	LevelSet *_eventReceiver;
	std::string _message;

	std::vector<Poses> _poses;
	bool _isTouch;
	bool _checked;
	bool _visible;
	bool _enable;
	void Init(const LevelSet &levelSet, const char *id, bool invisible);
	TapButton(const LevelSet &levelSet, const char *id, LevelSet *recever, const char *message, bool invisible = false);
	bool Check(const oxygine::Vector2 &pos);
	void EndTouch();
	void CancelTouch();
	void Reset();
	bool UnderPos(const oxygine::Vector2 &pos);
	friend class TapButtonInterface;
public:
	void SetEnable(bool v) { _enable = v; }
	virtual void TweenEvent();
	void SetVisible(bool visible);
	void SetChecked(bool checked);
    bool IsVisible() { return _visible; }
    bool IsChecked() { return _checked; }
	void HideTo(float x, float y, float time);
	void ShowFrom(float x, float y, float time);
	//BeautyText *GetBeautyText();
	void Command(const std::string &cmd);
};

typedef std::list<TapButton *> TapButtonList;

class TapButtonInterface : public virtual LevelSet
{
private:
	TapButtonList _list;
	enum TouchState
	{
		touch_none,
		touch_moving,
		touch_canceled
	};
	TouchState _state;
public:
	virtual bool isOn(const Vector2 &localPosition);

	void OnTouchMove(Event *ev);
	void OnTouchEnd(Event *ev);
	
	void TouchCancel();

	void Clear();
	TapButtonInterface();
	virtual ~TapButtonInterface();
	TapButton * AddButton(const LevelSet &levelSet, const char *id, LevelSet *receiver, const char *message, bool invisible = false);
	void Reset();
	bool UnderPos(const oxygine::Vector2 &pos);
};

#endif//TAP_BUTTON_H