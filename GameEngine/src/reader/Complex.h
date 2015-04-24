#ifndef COMPLEX_H
#define COMPLEX_H

//#include "BeautyBase.h"
//#include "Line.h"
#include "LevelSet.h"

//typedef std::vector<LineBase *> LevelGroundLines;
//
//struct Complex {
//
//private:
//
//	BeautyList beauties;
//	void Clear();
//	Complex() {}
//	virtual ~Complex();
//
//	friend class ComplexManager;
//
//public:
//
//	//void Draw();
//	void LoadFromXml(pugi::xml_node xe);
//	void GetBeautyList(BeautyList &list) const;
//	//void MakeCopyByInGameTypeHard(const std::string &keyWorld, BeautyList &list, bool appendAtEnd = false) const;
//
//};


class ComplexManager {

	typedef std::map<std::string, LevelSet *> ComplexMap;
	static ComplexMap _complexes;

public:

	static bool isComplex(const std::string &complexId);
	static void deleteComplex(const std::string &complexId);
	static void UnloadAll();
	static LevelSet *getComplex(const std::string &complexId);

};

BeautyBase *MakeCopy(const BeautyBase *);

#endif//COMPLEX_H