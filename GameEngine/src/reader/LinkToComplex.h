#ifndef LINK_TO_COMPLEX_H
#define LINK_TO_COMPLEX_H

#include "Complex.h"
#include "BeautyBase.h"

class LinkToComplex : public BeautyBase
{
private:
	//Matrix _drawMatrix;
	std::string _id;

	LevelSet *_complex;
	float _shiftX;
	float _shiftY;

public:

	float GetShiftX() const { return _shiftX; }
	float GetShiftY() const { return _shiftY; }
	virtual ~LinkToComplex();
	LinkToComplex(const LinkToComplex &b);
	LinkToComplex(pugi::xml_node xe);
	virtual void GetBeautyList(BeautyList &list) const;

	//virtual void Draw();

	virtual std::string Type() const;
	const std::string &Name() const { return _id; }

	const LevelSet *GetComplex() { return _complex; }
};


#endif//LINK_TO_COMPLEX_H