#ifndef SPLINE_H
#define SPLINE_H

#include "Line.h"
#include "math/Vector2.h"

class Besier {
public:
	std::vector<oxygine::Vector2> origin;
	void Recalc(std::vector<oxygine::Vector2> &screen, int sectorNumber) const;
	float NearestGradient(const oxygine::Vector2 &p, float &gradient);
	void Path(float f, oxygine::Vector2 &pos);
	float length() const;
	void RecalcWithNumber(std::vector<oxygine::Vector2> &screen, int dotNumber) const;
};


class Spline : public LineBase
{

protected:	
	
	Besier _besier;

public:


	Spline(pugi::xml_node xe);

	virtual std::string Type() const;

	virtual void CreateDots(DotsList &dots);
	void ExportToLines(std::vector<oxygine::Vector2> &lineDots, int divisionBy = 6);

	virtual oxygine::Vector2 GetStart();
	virtual oxygine::Vector2 GetFinish();

	const Besier &GetBesier() { return _besier; }

	virtual float NearestGradient(const oxygine::Vector2 &p, float &gradient);

};

#endif//SPLINE_H