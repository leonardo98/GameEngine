#ifndef BEAUTY_BASE_H
#define BEAUTY_BASE_H

#include <vector>
#include "oxygine-framework.h"
#include "math/Vector2.h"
#include "Sprite.h"
#include "Matrix3x3.h"
#include "shared.h"

#define LOG(a) OX_ASSERT(false)

//typedef unsigned int Texture;

bool Inside(const oxygine::Vector2 &m, const std::vector<oxygine::Vector2> &dots);

//
// Базовый класс для украшений - для статичной картинки или анимации или цветного многоугольника
//

DECLARE_SMART(BeautyBase, spBeautyBase);

typedef std::vector<BeautyBase *> BeautyList;

class BeautyBase : public oxygine::Sprite
{
protected:
	// эти переменные общие для украшений
	std::string _userString;
	oxygine::Vector2 _userPoint;
	float _userFloat;

	std::string _uid;
	BeautyBase *_parentBeauty;
	// чтобы не дублировать в наследниках
	BeautyBase *_linked;
	int _width;
	int _height;

public:
	virtual int Width() { return _width; }
	virtual int Height() { return _height; }
	std::string &UID() { return _uid; }
	void LinkWith(BeautyBase *beauty) { _linked = beauty; }
	BeautyBase *Linked() { return _linked; }
	BeautyBase *&ParentBeauty() { return _parentBeauty; }
	virtual ~BeautyBase();
	BeautyBase(const BeautyBase &b);
	BeautyBase(pugi::xml_node xe);
	BeautyBase(const std::string userString, const oxygine::Vector2 &, const oxygine::Vector2 &);

	virtual bool PixelCheck(const oxygine::Vector2 &point) { return false; }
	virtual bool Command(const std::string &cmd) { return false; }
	virtual void GetBeautyList(BeautyList &list) const {}
	virtual void GetLocalMatrix(Matrix3x3 &m) { m.Unit(); }
	virtual void ShiftU(float u) {}

	virtual std::string Type() const = 0;

	virtual void ShiftTo(float dx, float dy);// для перемещения на разных разрешениях экрана

	const std::string &UserString() const { return _userString; }
	float &UserFloat() { return _userFloat; }
	oxygine::Vector2 &UserPoint() { return _userPoint; }
};

#endif//BEAUTY_BASE_H