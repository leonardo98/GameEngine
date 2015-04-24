#include "BeautyBase.h"


bool Inside(const oxygine::Vector2 &m, const std::vector<oxygine::Vector2> &dots) {
	// проверим находиться ли центр треугольника внтури области
	int counter = 0;
	int n = dots.size();
	for (int j = 0; j < n; ++j) {
		const oxygine::Vector2 *a2 = &dots[j];
		const oxygine::Vector2 *b2 = &dots[(j + 1) % dots.size()];
		if ((a2->x < b2->x && a2->x < m.x && m.x <= b2->x)
			|| (a2->x > b2->x && a2->x >= m.x && m.x > b2->x)
			) {// найти точку пересечения луча из М и отрезка a2b2
			float k = (a2->y - b2->y) / (a2->x - b2->x);
			float b = a2->y - a2->x * k;
			float y = k * m.x + b;
			if (y > m.y) {
				++counter;
			}
		}
	}
	return (counter % 2 == 1);
}


unsigned int __beauty_counter = 0;

BeautyBase::~BeautyBase() {
	--__beauty_counter;
}

BeautyBase::BeautyBase(const BeautyBase &b) 
	: _linked(NULL)
{
	++__beauty_counter;
	_parentBeauty = NULL;
	setVisible(true);
	_userPoint = b._userPoint;
	_userFloat = b._userFloat;

	_userString = b._userString;
	_uid = b._uid;
	_width = b._width;
	_height = b._height;

}

oxygine::Color ReadColor(std::string color) {
	assert(color.substr(0, 2) == "0x" && color.size() == 10);
	oxygine::Color result = oxygine::Color::White;
	unsigned char c;
	unsigned int k = 0;
	unsigned int count = 0;
	for (unsigned int i = 2; i < color.size(); ++i) {
		if (color[i] >= '0' && color[i] <= '9') {
			c = color[i] - '0';
		} else if (color[i] >= 'A' && color[i] <= 'F') {
			c = 10 + (color[i] - 'A');
		} else if (color[i] >= 'a' && color[i] <= 'f') {
			c = 10 + (color[i] - 'a');
		} else {
			assert(false);
		}
		k = (k << 4) + static_cast<unsigned int>(c);
		count++;
		if (count == 2)
		{
			count = 0;
			if (i <= 3)
			{
				result.a = k;
			}
			else if (i <= 5)
			{
				result.r = k;
			}
			else if (i <= 7)
			{
				result.g = k;
			}
			else if (i <= 9)
			{
				result.b = k;
			}
			k = 0;
		}
	}
	return result;
}

BeautyBase::BeautyBase(const std::string userString, const oxygine::Vector2 &pos1, const oxygine::Vector2 &pos2) 
	: _linked(NULL)
{
	++__beauty_counter;
	_parentBeauty = NULL;
	setVisible(true);
	setPosition(pos1);
	_userPoint.x = 0;
	_userPoint.y = 0;
	_userFloat = 0;
	_width = fabs(pos1.x - pos2.x);
	_height = fabs(pos1.y - pos2.y);

	_userString = userString;
	_uid = "";
}


BeautyBase::BeautyBase(pugi::xml_node xe) 
	: _linked(NULL)
{
	setInputEnabled(false);
	++__beauty_counter;
	_parentBeauty = NULL;
	setVisible(true);
	setPosition(xe.attribute("x").as_float()
				, xe.attribute("y").as_float());
	_userPoint.x = xe.attribute("pax").as_float();
	_userPoint.y = xe.attribute("pay").as_float();

	_width = xe.attribute("width").as_int();
	_height = xe.attribute("height").as_int();

	setAnchor((_width / 2) / static_cast<float>(_width), (_height / 2) / static_cast<float>(_height));

	_userFloat = xe.attribute("scalar").as_float();
		
	setColor(ReadColor(xe.attribute("color").value()));
		
	setRotation(xe.attribute("angle").as_float());

	setScale(xe.attribute("sx").as_float()
				, xe.attribute("sy").as_float());

	pugi::xml_attribute tmp = xe.attribute("inGameType");
	if (tmp) {
		_userString = tmp.value();
	}
	tmp = xe.attribute("uid");
	if (tmp) {
		_uid = tmp.value();
	}
}

void BeautyBase::ShiftTo(float dx, float dy) 
{
	setPosition(getPosition().x + dx, getPosition().y + dy);
}
