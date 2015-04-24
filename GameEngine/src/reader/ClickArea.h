#ifndef CLICKAREA_H
#define CLICKAREA_H

#include "BeautyBase.h"

//
// Украшение
// Область для клика
//

class ClickArea : public BeautyBase
{
private:

	std::vector<oxygine::Vector2> _dots; // точки для редактирования
	std::vector<oxygine::Vector2> _screenDots; 
	bool _mouseDown;

public:

	oxygine::Vector2 CalcCenter();

	virtual ~ClickArea();
	ClickArea(pugi::xml_node xe);
	ClickArea(const ClickArea &c);

	//virtual void Draw();
	virtual bool PixelCheck(const oxygine::Vector2 &point);
	//virtual bool IsOnScreen();
	virtual bool WorldPointCheck(const oxygine::Vector2 &point) const;
	virtual std::string Type() const;

};


#endif//CLICKAREA_H