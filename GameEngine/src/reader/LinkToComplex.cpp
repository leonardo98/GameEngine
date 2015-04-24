#include "LinkToComplex.h"

LinkToComplex::~LinkToComplex() {
}

LinkToComplex::LinkToComplex(pugi::xml_node xe) 
: BeautyBase(xe)
{
	_id = xe.attribute("complex").value();
	_complex = ComplexManager::getComplex(_id);
	_shiftX = xe.attribute("shiftX").as_float();
	_shiftY = xe.attribute("shiftY").as_float();
}

LinkToComplex::LinkToComplex(const LinkToComplex &b)
: BeautyBase(b)
{
	_id = b._id;
	_shiftX = b._shiftX;
	_shiftY = b._shiftY;
	_complex = ComplexManager::getComplex(_id);
}

//void LinkToComplex::Draw() {
//    oxygine::Vector2 a(- Width() / 2, 0);
//    oxygine::Vector2 b(Width() / 2, 0);
//	Render::PushMatrix();
//	Render::MatrixMove(_pos.x, _pos.y);
//    Render::GetCurrentMatrix().Mul(a);
//    Render::GetCurrentMatrix().Mul(b);
//    if (a.x > Render::ScreenWidth() || b.x < 0)
//    {
//        Render::PopMatrix();
//        return;
//    }
////	Render::MatrixRotate(_angle);
////	Render::MatrixScale(_sx, _sy);
//	Render::MatrixMove(_shiftX, _shiftY);
//	//_drawMatrix = Render::GetCurrentMatrix();
//	Render::PushColorAndMul(_color);
//	_complex->Draw();
//	Render::PopColor();
//	Render::PopMatrix();
//}

std::string LinkToComplex::Type() const { 
	return "LinkToComplex"; 
}

void LinkToComplex::GetBeautyList(BeautyList &list) const {
	BeautyList ls;
	_complex->GetBeautyList(ls);

	oxygine::Vector2 tmp;
	for (BeautyList::iterator i = ls.begin(), e = ls.end(); i != e; ++i) {
		tmp = (*i)->getPosition();
		tmp.x  += _shiftX;
		tmp.y  += _shiftY;
		// надо реализовать метод Rotation для Vector2
		//(*i)->setPosition(*(tmp.Rotate(getRotation())) + getPosition());
		(*i)->setRotation((*i)->getRotation() + getRotation());
		list.push_back(*i);
	}
}
