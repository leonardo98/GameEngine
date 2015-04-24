#ifndef LEVELSET_H
#define LEVELSET_H

#include "BeautyBase.h"
#include "Spline.h"
#include "actor.h"
#include <set>

DECLARE_SMART(LevelSet, spLevelSet);

typedef std::set<std::string> NameList;

class LevelSet : public oxygine::Actor
{
private:
	void BuildLinks();
	std::string id;
	Resources res;
	static void PushFileNamesToMap(const char *fileName, NameList &names);
	static void GenOxygineResourceFile(NameList &files, const char *fileName);
public:
	const std::string &Id() { return id; }
	BeautyList beauties;
	//void Draw();
	//void Update(float dt);
	void Clear();
	void LoadFromXml(pugi::xml_node xe);
	bool LoadFromFile(const char *fileName);
	const LevelSet &operator=(const LevelSet &l);
	LevelSet(const LevelSet &l);
	~LevelSet();
	LevelSet() {}
	// search only from begining of UserString
	void SelectByUserString(const std::string &keyWorld, BeautyList &list, bool appendAtEnd = false) const;
	void SelectByUserString(const std::string &keyWorld, spBeautyBase &beauty) const;
	//void SelectIndexesByInGameType(const std::string &keyWorld, std::vector<unsigned int> &list, bool appendAtEnd = false) const;
	//// search anythere in InGameype
	//void SelectByInGameTypeSoft(const std::string &keyWorld, BeautyList &list, bool appendAtEnd = false) const;
	//void SelectIndexesByInGameTypeSoft(const std::string &keyWorld, std::vector<unsigned int> &list, bool appendAtEnd = false) const;
	//void SelectByInGameTypeHard(const std::string &keyWorld, BeautyList &list, bool appendAtEnd = false) const;
	//void SelectByInGameTypeHard(const std::string &keyWorld, BeautyBase *&beauty, bool cutOrigin = false);

	//void MakeCopyByInGameTypeHard(const std::string &keyWorld, BeautyList &list, bool appendAtEnd = false) const;
	void GetBeautyList(BeautyList &list) const;
};

//class LevelSetManager
//{ 
//protected:
//	typedef std::map<std::string, pugi::xml_document *> LevelSetMap;
//	static LevelSetMap _levelSets;
//public:
//
//	//
//	// Загрузить файл 
//	// в случае успеха - вернет "true"
//	// можно загружать несколько файлов - менеджер "коллекционирует" 
//	//
//	static bool Load(const std::string &fileName);
//	
//	//
//	// Выгрузить все из памяти
//	//
//	static void UnloadAll();
//	
//	static bool GetLevel(const std::string &libId, const std::string &id, spLevelSet ls, bool skipMessage = false);
//};

#endif//LEVELSET_H