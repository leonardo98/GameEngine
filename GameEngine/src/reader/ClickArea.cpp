#include "ClickArea.h"

ClickArea::~ClickArea() {
}

ClickArea::ClickArea(const ClickArea &c) 
: BeautyBase(c)
{
	_dots = c._dots;
	_mouseDown = false;
}

ClickArea::ClickArea(pugi::xml_node xe)
: BeautyBase(xe)
{
	pugi::xml_node dot = xe.first_child();
	while (dot != NULL) {
		_dots.push_back(oxygine::Vector2(dot.attribute("x").as_float(), dot.attribute("y").as_float()));
		dot = dot.next_sibling();
	}
	_mouseDown = false;
}

//void ClickArea::Draw() {
//
//	Render::PushMatrix();
//	Render::MatrixMove(_pos.x, _pos.y);
//
//	_screenDots = _dots;
//	for (unsigned int i = 0; i < _dots.size(); ++i) {
//		Render::GetCurrentMatrix().Mul(_screenDots[i]);	
//		//Render::Line(_dots[i].x, _dots[i].y, _dots[(i + 1) % _dots.size()].x, _dots[(i + 1) % _dots.size()].y, 0xFFFFFFFF);
//	}
//
//	Render::PopMatrix();
//}

//bool ClickArea::IsOnScreen()
//{
//	for (unsigned int i = 0; i < _screenDots.size(); ++i) {
//		if (_screenDots[i].x > 0 
//			&& _screenDots[i].x < Render::ScreenWidth() 
//			&& _screenDots[i].y > 0
//			&& _screenDots[i].y < Render::ScreenHeight())
//		{
//			return true;
//		}
//	}
//	return false;
//}

bool ClickArea::PixelCheck(const oxygine::Vector2 &point) { 
	if (Inside(point, _screenDots)) {
		return true;
	}
	return false;
}

std::string ClickArea::Type() const { 
	return "ClickArea"; 
}

oxygine::Vector2 ClickArea::CalcCenter() {
	oxygine::Vector2 pos(0.f, 0.f);
	for (std::vector<oxygine::Vector2>::iterator i = _dots.begin(), e = _dots.end(); i != e; ++i) {
		pos += (*i);
	}
	return getPosition() + pos * (1.f / _dots.size());
}

bool ClickArea::WorldPointCheck(const oxygine::Vector2 &point) const {
	return Inside(point - getPosition(), _dots);
}
