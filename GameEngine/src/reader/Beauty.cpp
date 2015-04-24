#include "Beauty.h"
#include "Complex.h"

Beauty::~Beauty() {
}

Beauty::Beauty(pugi::xml_node xe, Resources *res) 
: BeautyBase(xe)
{
	_fileName = xe.attribute("texture").value();
	setResAnim(res->getResAnim(_fileName));
	//_sprite = Core::getTexture(_fileName);
	//_localPos.Move(_pos.x, _pos.y);
	//_localPos.Rotate(_angle);
	//_localPos.Scale(_sx, _sy);
	//_localPos.Move(Math::round(- Width() / 2), Math::round(- Height() / 2));
}

//void Beauty::Draw() {
//	//float a = ((oldColor & 0xFF000000) >> 24) / 255.f;
//	//Render::SetAlpha(_alpha * a * 0xFF);
//	Render::PushColorAndMul(_color);
//	_sprite->Render(_localPos);
//	//Render::SetAlpha(a * 0xFF);
//	Render::PopColor();
//}

std::string Beauty::Type() const { 
	return "Beauty"; 
}

//int Beauty::Width() {
//	return _sprite->Width();
//}
//
//int Beauty::Height() {
//	return _sprite->Height();
//}

Beauty::Beauty(const Beauty &b)
: BeautyBase(b)
{
	_fileName = b._fileName;
}

void Beauty::GetBeautyList(BeautyList &list) const {
	BeautyBase *s = MakeCopy(this);
	list.push_back(s);
}
