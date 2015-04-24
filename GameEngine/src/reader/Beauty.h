#ifndef BEAUTY_H
#define BEAUTY_H

#include "BeautyBase.h"

class Beauty : public BeautyBase
{
private:
	std::string _fileName;

public:

	virtual ~Beauty();
	Beauty(const Beauty &b);
	Beauty(pugi::xml_node xe, Resources *res);

	virtual void GetBeautyList(BeautyList &list) const;

	virtual std::string Type() const;

	const std::string &FileName() const {return _fileName;}
};


#endif//BEAUTY_H