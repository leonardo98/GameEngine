#ifndef STRETCH_TEXTURE_H
#define STRETCH_TEXTURE_H

#include "Spline.h"


struct VertexBuffer
{
	std::vector<vertexPCT2> v;
	//std::vector<oxygine::Vector2> uv;
	std::vector<oxygine::Color> color;
	Texture *tex;
	//std::vector<unsigned short> index;
	VertexBuffer() : size(0) {}
	unsigned short size;
	void Init(unsigned int vert, unsigned int tri)
	{
		assert(vert > 0);
		assert(tri > 0);
		size = vert;
		v.resize(vert);
		//uv.resize(vert);
		color.resize(vert);
		//index.resize(tri);
	}
	//void AddQuad(CoreQuad &q)
	//{
	//	v.resize(v.size() + 4);
	//	uv.resize(uv.size() + 4);
	//	color.resize(color.size() + 4);
	//	unsigned int count = index.size();
	//	index.resize(index.size() + 6);
	//	v[size]     = q.v[0];
	//	v[size + 1] = q.v[1];
	//	v[size + 2] = q.v[2];
	//	v[size + 3] = q.v[3];
	//	uv[size]     = q.uv[0];
	//	uv[size + 1] = q.uv[1];
	//	uv[size + 2] = q.uv[2];
	//	uv[size + 3] = q.uv[3];
	//	index[count + 3] = index[count] = size;
	//	index[count + 1] = index[count + 5] = size + 2;
	//	index[count + 2] = size + 1;
	//	index[count + 4] = size + 3;
	//	size += 4;
	//}
};

class TexturedMesh : public BeautyBase
{
public:
	TexturedMesh(pugi::xml_node xe, Resources *res);
	TexturedMesh(const TexturedMesh &l);
	//TexturedMesh(const std::string &textureName, const oxygine::Vector2 pos, const Besier &besier);
	virtual std::string Type() const;
	//virtual bool PixelCheck(const oxygine::Vector2 &point);
	//virtual void Draw();
	void ShiftU(float u);
	virtual void doRender(RenderState const& parentRenderState);
private:
	bool _texture;
	std::string _fileName;
	VertexBuffer _vb;
	std::vector<vertexPCT2> _vertices;
	std::vector<Vector2> _origin;
};

#endif

