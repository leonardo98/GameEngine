#include "TexturedMesh.h"
#include <algorithm>

TexturedMesh::TexturedMesh(pugi::xml_node xe, Resources *res)
	: BeautyBase(xe)
{
	if (xe.attribute("texture"))
	{
		_fileName = xe.attribute("texture").value();
		setResAnim(res->getResAnim(_fileName));
		setAnchor(0, 0);
		_texture = true;
	}
	else
	{
		_texture = false;
	}
	pugi::xml_node mesh = xe.child("mesh");
	if (mesh)
	{
		_vb.Init(mesh.attribute("vert").as_int(), mesh.attribute("ind").as_int());
		unsigned int count = 0;
		unsigned int color = getColor().rgba();// << 24 | getColor().g << 16 | getColor().b << 8 | getColor().a;
		for (pugi::xml_node i = mesh.first_child(); i; i = i.next_sibling())
		{
			_vb.v[count].x = i.attribute("x").as_float();
			_vb.v[count].y = i.attribute("y").as_float();
			_vb.v[count].u = i.attribute("tx").as_float();
			_vb.v[count].v = i.attribute("ty").as_float();
			_vb.v[count].color = color;
			_vb.v[count].z = 0.f;
			count++;
		}
		pugi::xml_node indexesXml = xe.child("indexes");
		assert(indexesXml);
		for (pugi::xml_node i = indexesXml.first_child(); i; i = i.next_sibling())
		{
			pugi::xml_attribute v3 = i.attribute("v3");
			if (!v3.empty())
			{
				unsigned short v0 = i.attribute("v0").as_int();
				unsigned short v1 = i.attribute("v1").as_int();
				unsigned short v2 = i.attribute("v2").as_int();
				unsigned short v3 = i.attribute("v3").as_int();
				_vertices.push_back(_vb.v[v0]);
				_vertices.push_back(_vb.v[v1]);
				_vertices.push_back(_vb.v[v3]);
				_vertices.push_back(_vb.v[v2]);
			}
			else
			{
				unsigned short v0 = i.attribute("v0").as_int();
				unsigned short v1 = i.attribute("v1").as_int();
				unsigned short v2 = i.attribute("v2").as_int();
				_vertices.push_back(_vb.v[v0]);
				_vertices.push_back(_vb.v[v1]);
				_vertices.push_back(_vb.v[v2]);
				_vertices.push_back(_vb.v[v1]);
			}
		}
		for (unsigned int i = 0; i < _vertices.size(); ++i)
		{
			_origin.push_back(Vector2(_vertices[i].x, _vertices[i].y));
		}
	}
	else
	{
		assert(false);
	}
}

TexturedMesh::TexturedMesh(const TexturedMesh &l)
	: BeautyBase(l)
{
	_fileName = l._fileName;
	_texture = l._texture;
	_vb = l._vb;
}

//TexturedMesh::TexturedMesh(const std::string &textureName, const oxygine::Vector2 pos, const Besier &besier)
//	: BeautyBase("", pos, oxygine::Vector2(pos.x + 1, pos.y))
//{
//	_fileName = textureName;
//	_texture = Core::getTexture(_fileName);
//
//	std::vector<oxygine::Vector2> dots;
//	float ln = besier.length();
//	int n = std::max(2, Math::round(ln / _texture->Width() * 2));
//	besier.RecalcWithNumber(dots, n);
//	Render::DrawStripe(dots, _texture, 0.f, 1.f, 0.f, 0.5f, 0, dots.size(), &_vb);
//	_vb.tex = _texture->GetTexture();
//}

std::string TexturedMesh::Type() const 
{
	return "TexturedMesh"; 
}

//void TexturedMesh::Draw()
//{
//
//	Render::PushMatrix();
//	Render::MatrixMove(_pos.x, _pos.y);
//
//	Render::PushColorAndMul(_color);
//	DWORD color = Render::GetColor();
//	for (unsigned short i = 0; i < _vb.size; ++i)
//	{
//		_vb.color[i] = color;
//	}
//	Render::DrawVertexBuffer(_vb);
//	Render::PopColor();
//
//	Render::PopMatrix();
//
//}

void TexturedMesh::ShiftU(float u)
{
	for (unsigned int i = 0; i < _vb.size; ++i)
	{
		_vb.v[i].u += u;
	}
}

//
//bool ColoredPolygon::PixelCheck(const oxygine::Vector2 &point) { 
//	if (Math::Inside(point, _screenDots)) {
//		return true;
//	}
//	return false;
//}

const VertexDeclaration *vdecl;

void TexturedMesh::doRender(RenderState const& rs)
{
	Vector2 p;
	for (unsigned int i = 0; i < _origin.size(); ++i)
	{
		p = rs.transform.transform(_origin[i]);
		_vertices[i].x = p.x;
		_vertices[i].y = p.y;
	}

	//Sprite::doRender(rs);
	_vstyle._apply(rs);
	if (!_texture)
	{
		Diffuse df;
		df.alpha = 0;
		df.base = 0;
		df.premultiplied = false;
//		rs.renderer->setDiffuse(df);
	}
	else
	{
		const Diffuse &df = _frame.getDiffuse();
		if (df.base)
		{
			//df.base->setWrapMode(false);
			//rs.renderer->setDiffuse(df);
		}
	}

//	rs.renderer->draw(&_vertices[0], _vertices.size() * sizeof(vertexPCT2), VERTEX_PCT2);			


}
