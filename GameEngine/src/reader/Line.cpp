#include "Line.h"

LineBase::~LineBase() 
{
	_linked = NULL;
}

Line::~Line() {
}

Line::Line(const Line &c) 
: LineBase(c)
{
	_dots = c._dots;
}

Line::Line(pugi::xml_node xe)
: LineBase(xe)
{
	pugi::xml_node dot = xe.first_child();
	while (dot != NULL) {
		_dots.push_back(oxygine::Vector2(static_cast<float>(atof(dot.attribute("x").value())), static_cast<float>(atof(dot.attribute("y").value()))));
		dot = dot.next_sibling();
	}
}

Line::Line(const std::string &igt, const oxygine::Vector2 &pos1,  const oxygine::Vector2 &pos2)
	: LineBase(igt, pos1, pos2)
{
	_dots.resize(2);
	if (pos1.x < pos2.x)
	{
		_dots[0] = oxygine::Vector2(0, 0);
		_dots[1] = pos2 - pos1;
	}
	else
	{
		_dots[0] = pos2 - pos1;
		_dots[1] = oxygine::Vector2(0, 0);
	}
}

std::string Line::Type() const { 
	return "Line"; 
}

oxygine::Vector2 Line::GetStart() {
	if (_dots.size() == 2 && _dots[1].x < _dots[0].x) {
		return _dots[1] + getPosition();
	} else {
		return _dots[0] + getPosition();
	}
}

oxygine::Vector2 Line::GetFinish() {
	if (_dots.size() == 2 && _dots[1].x > _dots[0].x) {
		return _dots[1] + getPosition();
	} else {
		return _dots[0] + getPosition();
	}
}

void Line::CreateDots(DotsList &dots) {
	dots = _dots;
	for (unsigned int i = 0; i < dots.size(); ++i) {
		dots[i] = (dots[i] + getPosition());
	}
}
