#ifndef LINE_H
#define LINE_H

#include "BeautyBase.h"

typedef std::vector<oxygine::Vector2> DotsList;

class LineBase : public BeautyBase
{
public:
	LineBase(const LineBase &g)
		: BeautyBase(g)
	{}
	LineBase(pugi::xml_node xe)
		: BeautyBase(xe)
	{}
	LineBase(const std::string &igt, const oxygine::Vector2 &pos1,  const oxygine::Vector2 &pos2)
		: BeautyBase(igt, pos1, pos2)
	{}
	virtual ~LineBase();
	virtual void CreateDots(DotsList &dots) = 0;
	virtual oxygine::Vector2 GetStart() = 0;
	virtual oxygine::Vector2 GetFinish() = 0;
	//virtual float NearestGradient(const oxygine::Vector2 &p, float &gradient) = 0;
};

class Line : public LineBase
{
private:

	std::vector<oxygine::Vector2> _dots; // точки для редактирования
public:
	void DebugDraw();
	virtual ~Line();
	Line(pugi::xml_node xe);
	Line(const std::string &igt, const oxygine::Vector2 &pos1,  const oxygine::Vector2 &pos2);
	Line(const Line &c);

	virtual std::string Type() const;

	virtual void CreateDots(DotsList &dots);
	virtual oxygine::Vector2 GetStart();
	virtual oxygine::Vector2 GetFinish();
	//virtual float NearestGradient(const oxygine::Vector2 &p, float &gradient);
};


#endif//LINE_H