#include "Spline.h"


#define SPLINE_OFFSET 30

void Besier::Path(float t, oxygine::Vector2 &pos)
{
	assert(origin.size() >= 4 && ((int)origin.size() - 1) % 3 == 0);
	if (t == 1.f)
	{
		pos = origin.back();
		return;
	}
	assert(0.f <= t && t < 1.f);
	
	int segmentAmount = origin.size() / 3;
	int segment = static_cast<int>(t * segmentAmount);

	t = t * segmentAmount - segment;

	oxygine::Vector2 &p0 = origin[segment * 3];
	oxygine::Vector2 &p1 = origin[segment * 3 + 1];
	oxygine::Vector2 &p2 = origin[segment * 3 + 2];
	oxygine::Vector2 &p3 = origin[segment * 3 + 3];

	pos = (p0 * (1 - t) * (1 - t) * (1 - t)
			+ p1 * 3 * t * (1 - t) * (1 - t)
			+ p2 * 3 * t * t * (1 - t)
			+ p3 * t * t * t);
}

float Besier::NearestGradient(const oxygine::Vector2 &p, float &gradient)
{
	assert(origin.size() >= 4 && ((int)origin.size() - 1) % 3 == 0);

	float tMin = 0.f;
	oxygine::Vector2 pMin, pMax;
	Path(tMin, pMin);
	float tMax = 1.f;
	Path(tMax, pMax);
	oxygine::Vector2 m25, m5, m75;
	while ((pMax.x - pMin.x) * (pMax.x - pMin.x) + (pMax.y - pMin.y) * (pMax.y - pMin.y) > 1)
	{
		float t25 = tMin + (tMax - tMin) * 0.25f;
		float t5  = tMin + (tMax - tMin) * 0.5f;
		float t75 = tMin + (tMax - tMin) * 0.75f;
		Path(t25, m25);
		Path(t5, m5);
		Path(t75, m75);
		if ((m25 - p).length() < (m5 - p).length() && (m25 - p).length() < (m75 - p).length())
		{
			pMax = m5;
			tMax = t5;
		}
		else if ((m5 - p).length() < (m75 - p).length())
		{
			pMin = m25;
			tMin = t25;
			pMax = m75;
			tMax = t75;
		}
		else
		{
			pMin = m5;
			tMin = t5;
		}
	}
	gradient = atan2(m75.y - m25.y, m75.x - m25.x);
	return (pMin - p).length();
}

void Besier::Recalc(std::vector<oxygine::Vector2> &screen, int sectorNumber) const
{
	assert(((int)origin.size() - 1) % 3 == 0);
	screen.clear();
	for (unsigned int i = 0; i < origin.size() / 3; ++i) {
		oxygine::Vector2 p0 = origin[i * 3];
		oxygine::Vector2 p1 = origin[i * 3 + 1];
		oxygine::Vector2 p2 = origin[i * 3 + 2];
		oxygine::Vector2 p3 = origin[i * 3 + 3];
		for (int k = 0; k < sectorNumber; ++k) {
			float t = static_cast<float>(k) / sectorNumber;
			screen.push_back(p0 * (1 - t) * (1 - t) * (1 - t)
							+ p1 * 3 * t * (1 - t) * (1 - t)
							+ p2 * 3 * t * t * (1 - t)
							+ p3 * t * t * t);
		}
	}
	screen.push_back(origin.back());
}

float Besier::length() const
{
	std::vector<oxygine::Vector2> tmp;
	Recalc(tmp, 10);
	float lenFull = 0.f;
	for (int i = 0; i < static_cast<int>(tmp.size()) - 1; ++i) 
	{
		lenFull += (tmp[i] - tmp[i + 1]).length();
	}
	return lenFull;
}

void Besier::RecalcWithNumber(std::vector<oxygine::Vector2> &screen, int dotNumber) const
{
	screen.clear();

	oxygine::Vector2 dot;
	std::vector<oxygine::Vector2> tmp;
	Recalc(tmp, 10);
	float lenFull = 0.f;
	for (int i = 0; i < static_cast<int>(tmp.size()) - 1; ++i) 
	{
		lenFull += (tmp[i] - tmp[i + 1]).length();
	}
	screen.push_back(tmp.front());
	int k = 0;
	float len = 0.f;
	for (int i = 1; i < dotNumber; ++i) 
	{
		float searchValue = i * lenFull / dotNumber;
		assert(k + 1 < static_cast<int>(tmp.size()));
		float lenCurrent = (tmp[k + 1] - tmp[k]).length();
		while (len + lenCurrent < searchValue) 
		{
			len += lenCurrent;
			++k;
			lenCurrent = (tmp[k + 1] - tmp[k]).length();
		}
		dot = (tmp[k + 1] - tmp[k]) * ((searchValue - len) / lenCurrent) + tmp[k];
		screen.push_back(dot);
	}
	screen.push_back(tmp.back());
}



void Spline::ExportToLines(std::vector<oxygine::Vector2> &lineDots, int divisionBy) {
	_besier.Recalc(lineDots, divisionBy);
}

void Spline::CreateDots(DotsList &dots) {
	ExportToLines(dots);
	for (unsigned int i = 0; i < dots.size(); ++i) {
		dots[i] = (dots[i] + getPosition());
	}
}

std::string Spline::Type() const {
	return "Spline";
}

Spline::Spline(pugi::xml_node xe) 
: LineBase(xe)
{
	bool besier = xe.attribute("besier") != NULL;
	pugi::xml_node dot = xe.first_child();
	while (dot != NULL) {
		oxygine::Vector2 p(static_cast<float>(atof(dot.attribute("x").value())), static_cast<float>(atof(dot.attribute("y").value())));
		if (!besier) {
			if (_besier.origin.size() != 0) {
				_besier.origin.push_back(_besier.origin.back() + oxygine::Vector2(SPLINE_OFFSET, 0));
				_besier.origin.push_back(p + oxygine::Vector2(-SPLINE_OFFSET, 0));
			}
			_besier.origin.push_back(p);
		} else {
			_besier.origin.push_back(p);
		}
		dot = dot.next_sibling();
	}

}

oxygine::Vector2 Spline::GetStart() {
	return _besier.origin.front() + getPosition();
}

oxygine::Vector2 Spline::GetFinish() {
	return _besier.origin.back() + getPosition();
}

float Spline::NearestGradient(const oxygine::Vector2 &p, float &gradient)
{
	float f = _besier.NearestGradient(p - getPosition(), gradient);
	return f;
}
