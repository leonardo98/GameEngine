#include "LevelSet.h"
#include "Beauty.h"
#include "ClickArea.h"
//#include "AnimationArea.h"
#include "Spline.h"
#include "Line.h"
#include "TexturedMesh.h"
#include "LinkToComplex.h"

void LevelSet::Clear() {
	for (unsigned int i = 0; i < beauties.size(); ++i) {
		delete beauties[i];
	}
	beauties.clear();
}

void LevelSet::LoadFromXml(pugi::xml_node xe) {
	Clear();
	assert(xe != NULL);
	id = xe.attribute("id").value();
	// level loading
	pugi::xml_node beautyList = xe.child("Beauties");
	if (!beautyList.empty()) {
		pugi::xml_node elem = beautyList.first_child();
		std::string typeName;
		BeautyBase *beauty;	
		while (!elem.empty()) {
			typeName = elem.name();// Attribute("type");
			if (typeName == "ClickArea") {
				beauty = new ClickArea(elem);
			} else if (typeName == "Beauty") {
				beauty = new Beauty(elem, &res);
			//} else if (typeName == "Animation") {
			//	beauty = new AnimationArea(elem);
			} else if (typeName == "LinkToComplex") {
				LinkToComplex *link = new LinkToComplex(elem);
				beauty = link;
			} else if (typeName == "SolidGroundLine") {
				beauty = new Spline(elem);
			} else if (typeName == "GroundLine") {
				beauty = new Line(elem);
			} else {
				beauty = new TexturedMesh(elem, &res);
			}
			{
				addChild(beauty);
			}
			elem = elem.next_sibling();
		}
	}
	BuildLinks();
}

void LevelSet::PushFileNamesToMap(const char *fileName, NameList &names)
{
	names.clear();
	pugi::xml_document doc;
	if (!doc.load_file(fileName)) {
		return;
	}

	pugi::xml_node xe = doc.first_child();
	std::string s;
	if (xe)
	{
        pugi::xml_node xeBeautyList = xe.first_child();
		if (xeBeautyList)
		{
            pugi::xml_node xeBeauty = xeBeautyList.first_child();
			while (xeBeauty)
			{
                pugi::xml_attribute tmp = xeBeauty.attribute("texture");
				if (tmp)
				{
					names.insert(tmp.value());
				}
                xeBeauty = xeBeauty.next_sibling();
			}
		}
	}
}

bool LevelSet::LoadFromFile(const char *fileName)
{
	// если есть ресурсы - освобождаем
	res.free();

	std::string resFileName("oxy_res/");
	resFileName += fileName;

	// if not exist - generate resource descriptions
	if (!oxygine::file::exists(resFileName.c_str()))
	{
		NameList files;
		PushFileNamesToMap(fileName, files);
        GenOxygineResourceFile(files, resFileName.c_str());
	}

	// loading resources
	res.loadXML(resFileName.c_str());
	// loading data
	pugi::xml_document doc;
	if (!doc.load_file(fileName)) {
		return false;
	}
	pugi::xml_node xe = doc.first_child();
	LoadFromXml(xe);
	return true;
}

void LevelSet::GenOxygineResourceFile(NameList &files, const char *fileName)
{
    pugi::xml_document doc;

  //  {
  //      pugi::xml_node decl;// = doc.allocate_node(rapidxml::node_declaration);
		//decl.append_attribute("version") = "1.0";
  //      decl.append_attribute("encoding") = "utf-8";
  //      decl.append_attribute("standalone") = "no";
		//doc.append_child(decl);
  //  }
    pugi::xml_node root = doc.append_child("resources");
	root.append_attribute("do_not_use_path") = "";
    pugi::xml_node atlasXml = root.append_child("atlas");

	for (NameList::iterator i = files.begin(), e = files.end(); i != e; ++i)
    {
        pugi::xml_node sprite = atlasXml.append_child("image");
        sprite.append_attribute("id") = (*i).c_str();
        sprite.append_attribute("file") = (*i).c_str();
    }

    // Save to file
	doc.save_file(fileName);
}

LevelSet::LevelSet(const LevelSet &l) 
	: id(l.id)
{
	assert(false);
	for (unsigned int i = 0; i < l.beauties.size(); ++i) {

		beauties.push_back(MakeCopy(l.beauties[i]));
	}
}

void LevelSet::GetBeautyList(BeautyList &list) const 
{
	for (unsigned int i = 0; i < beauties.size(); ++i) {
		beauties[i]->GetBeautyList(list);
	}
}

LevelSet::~LevelSet()
{
	Clear();
}

const LevelSet &LevelSet::operator=(const LevelSet &l) 
{
	assert(false);
	Clear();
	id = l.id;
	for (unsigned int i = 0; i < l.beauties.size(); ++i) {
		beauties.push_back(MakeCopy(l.beauties[i]));
	}
	BuildLinks();
	return *this;
}

void LevelSet::SelectByUserString(const std::string &keyWorld, BeautyList &list, bool appendAtEnd) const {
	if (!appendAtEnd) {
		list.clear();
	}
	spActor i = getFirstChild();
	while (i)
	{
		BeautyBase *b = dynamic_cast<BeautyBase *>(i.get());
		if (b)
		{
			if (b->UserString().find(keyWorld) == 0) {
				list.push_back(b);
			}
		}
		i = i->getNextSibling();
	}
}

void LevelSet::SelectByUserString(const std::string &keyWorld, spBeautyBase &beauty) const 
{
	beauty = NULL;
	spActor i = getFirstChild();
	while (i)
	{
		BeautyBase *b = dynamic_cast<BeautyBase *>(i.get());
		if (b)
		{
			if (b->UserString().find(keyWorld) == 0) {
				beauty = b;
				return;
			}
		}
		i = i->getNextSibling();
	}
}

//void LevelSet::SelectIndexesByInGameType(const std::string &keyWorld, std::vector<unsigned int> &list, bool appendAtEnd) const {
//	if (!appendAtEnd) {
//		list.clear();
//	}
//	for (unsigned int i = 0; i < beauties.size(); ++i) {
//		if (beauties[i]->UserString().find(keyWorld) == 0) {
//			list.push_back(i);
//		}
//	}
//}

//void LevelSet::SelectByInGameTypeHard(const std::string &keyWorld, BeautyList &list, bool appendAtEnd) const {
//	if (!appendAtEnd) {
//		list.clear();
//	}
//	for (unsigned int i = 0; i < beauties.size(); ++i) {
//		if (beauties[i]->UserString() == keyWorld) {
//			list.push_back(beauties[i]);
//		}
//	}
//}

//void LevelSet::SelectByInGameTypeHard(const std::string &keyWorld, BeautyBase *&beauty, bool cutOrigin) {
//	beauty = NULL;
//
//	for (BeautyList::iterator i = beauties.begin(), e = beauties.end(); i != e; ++i) {
//		if ((*i)->UserString() == keyWorld) {
//			beauty = (*i);
//			if (cutOrigin)
//			{
//				beauties.erase(i);
//			}
//			return;
//		}
//	}
//}

//void LevelSet::SelectByInGameTypeSoft(const std::string &keyWorld, BeautyList &list, bool appendAtEnd) const {
//	if (!appendAtEnd) {
//		list.clear();
//	}
//	for (unsigned int i = 0; i < beauties.size(); ++i) {
//		if (beauties[i]->UserString().find(keyWorld) != std::string::npos) {
//			list.push_back(beauties[i]);
//		}
//	}
//}

//void LevelSet::SelectIndexesByInGameTypeSoft(const std::string &keyWorld, std::vector<unsigned int> &list, bool appendAtEnd) const {
//	if (!appendAtEnd) {
//		list.clear();
//	}
//	for (unsigned int i = 0; i < beauties.size(); ++i) {
//		if (beauties[i]->UserString().find(keyWorld) != std::string::npos) {
//			list.push_back(i);
//		}
//	}
//}

//void LevelSet::Draw() {
//    for (unsigned int i = 0; i < beauties.size(); ++i) {
//        BeautyBase *b = beauties[i];
//        if (b->Visible()) {
//            b->Draw();
//        }
//    }
//}

//void LevelSet::MakeCopyByInGameTypeHard(const std::string &keyWorld, BeautyList &list, bool appendAtEnd) const {
//	if (!appendAtEnd) {
//		for (BeautyList::iterator i = list.begin(), e = list.end(); i != e; ++i) 
//		{
//				delete (*i);
//		}
//		list.clear();
//	}
//	for (unsigned int j = 0; j < beauties.size(); ++j) {
//		if (beauties[j]->Type() == "LinkToComplex")
//		{
//			LinkToComplex *link = static_cast<LinkToComplex *>(beauties[j]);
//			BeautyList inner;
//			link->GetComplex()->MakeCopyByInGameTypeHard(keyWorld, inner);
//			for (unsigned int i = 0; i < inner.size(); ++i) {
//				inner[i]->ShiftTo(link->getPosition().x + link->GetShiftX(), link->getPosition().y + link->GetShiftY());
//				if (inner[i]->ParentBeauty() == 0)
//				{
//					inner[i]->ParentBeauty() = link;
//				}
//				list.push_back(inner[i]);
//			}
//			if (beauties[j]->Linked() == NULL && inner.size() == 1)
//			{
//				beauties[j]->LinkWith(inner[0]);
//			}
//		}
//		else if (beauties[j]->UserString() == keyWorld)
//		{
//			list.push_back(MakeCopy(beauties[j]));
//		}
//	}
//}

void LevelSet::BuildLinks()
{
	BeautyList linked;
	for (BeautyList::iterator i = beauties.begin(), e = beauties.end(); i != e; ++i) {
		if ((*i)->UID().size() && (*i)->UID()[0] == ':')
		{
			linked.push_back(*i);
		}
	}
	std::string uid;
	for (BeautyList::iterator i = linked.begin(), e = linked.end(); i != e; ++i) {
		uid = (*i)->UID().substr(1);
		for (BeautyList::iterator j = beauties.begin(), k = beauties.end(); j != k; ++j) {
			if ((*j)->UID() == uid)
			{
				(*i)->LinkWith(*j);
				break;
			}
		}
	}
}

//LevelSetManager::LevelSetMap LevelSetManager::_levelSets;
//
//bool LevelSetManager::Load(const std::string &fileName)
//{
//	OX_ASSERT(_levelSets.find(fileName) == _levelSets.end());
//	pugi::xml_document *doc = new pugi::xml_document();
//	if (doc->load_file(fileName.c_str())) {
//		_levelSets[fileName] = doc;
//		return true;
//	} 
//	delete doc;
//	return false;
//}
//
//void LevelSetManager::UnloadAll()
//{
//	for (LevelSetManager::LevelSetMap::iterator i = _levelSets.begin(), e = _levelSets.end(); i != e; ++i)
//	{
//		delete (i->second);
//	}
//	_levelSets.clear();
//}
//
//bool LevelSetManager::GetLevel(const std::string &libId, const std::string &id, spLevelSet level, bool skipMessage)
//{
//	assert(_levelSets.find(libId) != _levelSets.end());
//	pugi::xml_document *doc = _levelSets[libId];
//	level->Clear();
//	pugi::xml_node xe = doc->first_child().first_child();// RootElement()->FirstChildElement();
//	while (xe != NULL && xe.attribute("id").value() != id) {			
//		xe = xe.next_sibling();
//	}
//	if (xe != NULL)
//	{
//		level->LoadFromXml(xe);
//	}
//	else if (!skipMessage)
//	{
//		LOG("Level not found: " + id);
//	}
//	return xe != NULL;
//}
