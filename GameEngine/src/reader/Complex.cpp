#include "Complex.h"
#include "Beauty.h"
#include "ClickArea.h"
//#include "AnimationArea.h"
#include "Spline.h"
#include "Line.h"
#include "TexturedMesh.h"
#include "LinkToComplex.h"


BeautyBase *MakeCopy(const BeautyBase *origin)
{
	std::string typeName;
	BeautyBase *beauty;
	typeName = origin->Type();
	if (typeName == "ClickArea") {
		beauty = new ClickArea(*(ClickArea *)(origin));
	} else if (typeName == "Beauty") {
		beauty = new Beauty(*(Beauty *)(origin));
	//} else if (typeName == "Animation") {
	//	beauty = new AnimationArea(*(AnimationArea *)(origin));
	} else if (typeName == "LinkToComplex") {
		beauty = new LinkToComplex(*(LinkToComplex *)(origin));
	} else if (typeName == "SolidGroundLine") {
		beauty = new Spline(*(Spline *)(origin));
	} else if (typeName == "GroundLine") {
		beauty = new Line(*(Line *)(origin));
	} else {
		beauty = new TexturedMesh(*(TexturedMesh *)(origin));
	}
	return beauty;
}

//void Complex::Clear() {
//	for (unsigned int i = 0; i < beauties.size(); ++i) {
//		delete beauties[i];
//	}
//	beauties.clear();
//}

//void Complex::Draw() {
//	for (unsigned int i = 0; i < beauties.size(); ++i) {
//		beauties[i]->Draw();
//	}
//}

//void Complex::GetBeautyList(BeautyList &list) const {
//	for (unsigned int i = 0; i < beauties.size(); ++i) {
//		beauties[i]->GetBeautyList(list);
//	}
//}
//
//void Complex::LoadFromXml(pugi::xml_node xe) {
//	Clear();
//	assert(xe != NULL);
//	// loading
//	pugi::xml_node beautyList = xe.child("Beauties");
//	if (beautyList) {
//		pugi::xml_node elem = beautyList.first_child();
//		std::string typeName;
//		BeautyBase *beauty;
//		while (elem != NULL) {
//			typeName = elem.value();
//			if (typeName == "ClickArea") {
//				beauty = new ClickArea(elem);
//			} else if (typeName == "Beauty") {
//				beauty = new Beauty(elem, &res);
//			//} else if (typeName == "Animation") {
//			//	beauty = new AnimationArea(elem);
//			} else if (typeName == "SolidGroundLine") {
//				beauty = new Spline(elem);
//			} else if (typeName == "GroundLine") {
//				beauty = new Line(elem);
//			} else {
//				beauty = new TexturedMesh(elem, &res);
//			}
//			beauties.push_back(beauty);
//			elem = elem.next_sibling();
//		}
//	}
//}
//
//Complex::~Complex() {
//	Clear();
//}


//bool ComplexManager::Load(const std::string &fileName) {
//	pugi::xml_document doc;
//	if (!doc.load_file(fileName.c_str())) {
//		LOG("File " + fileName + " not found!");
//		return false;
//	}
//	pugi::xml_node xe = doc.first_child().child("Complex");
//	while (xe != NULL) {
//		const char *name = xe.attribute("name").value();
//		Complex *c = new Complex();
//		c->LoadFromXml(xe);
//		assert(_complexes.find(name) == _complexes.end());
//		_complexes[name] = c;
//		xe = xe.next_sibling("Complex");
//	}
//	return true;
//}

//
// Выгрузить все анимации и текстуры из памяти
//
void ComplexManager::UnloadAll() {
	for (ComplexMap::iterator it = _complexes.begin(); it != _complexes.end(); it++) {
		delete (*it).second;
	}
	_complexes.clear();
}

//
// Получить указатель на комплекс по complexId - это имя комплекса в редакторе 
// (ВАЖНО! нельзя вызывать delete для нее!)
//
LevelSet *ComplexManager::getComplex(const std::string &complexId) {
	ComplexMap::iterator it_find = _complexes.find(complexId);
	if (it_find != _complexes.end()) {
		return (*it_find).second;
	}
	LevelSet *level = new LevelSet();
	if (level->LoadFromFile(complexId.c_str()))
	{
		_complexes[complexId] = level;
		return level;
	}
	delete level;
    LOG("complex " + complexId + " not found.");
	return NULL;
}

bool ComplexManager::isComplex(const std::string &complexId) {
	ComplexMap::iterator it_find = _complexes.find(complexId);
	return (it_find != _complexes.end());
}

void ComplexManager::deleteComplex(const std::string &complexId)
{
	assert(_complexes.find(complexId) != _complexes.end());
	delete _complexes[complexId];
	_complexes.erase(_complexes.find(complexId));
}

ComplexManager::ComplexMap ComplexManager::_complexes;

//
//void Complex::MakeCopyByInGameTypeHard(const std::string &keyWorld, BeautyList &list, bool appendAtEnd) const {
//	if (!appendAtEnd) {
//		for (BeautyList::iterator i = list.begin(), e = list.end(); i != e; ++i) 
//		{
//			delete (*i);
//		}
//		list.clear();
//	}
//	for (unsigned int i = 0; i < beauties.size(); ++i) {
//		if (beauties[i]->Type() == "LinkToComplex")
//		{
//			assert(false);
//			LinkToComplex *link = static_cast<LinkToComplex *>(beauties[i]);
//			BeautyList inner;
//			link->GetComplex()->MakeCopyByInGameTypeHard(keyWorld, inner);
//			for (unsigned int i = 0; i < inner.size(); ++i) {
//				inner[i]->ShiftTo(-link->getPosition().x, -link->getPosition().y);
//				if (inner[i]->ParentBeauty() == NULL)
//				{
//					inner[i]->ParentBeauty() = link;
//				}
//				list.push_back(inner[i]);
//			}
//		}
//		else if (beauties[i]->UserString() == keyWorld)
//		{
//			list.push_back(MakeCopy(beauties[i]));
//		}
//	}
//}
//
