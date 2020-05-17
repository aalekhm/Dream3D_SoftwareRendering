#pragma once
#include <vector>

class Vertex;
class MeshObjLoader;
class RenderContext;
class Sprite;
class Matrix4f;

class Mesh
{
	public:
								Mesh(const char* sFileName);
								Mesh::Mesh(std::vector<Vertex> vFaces);
		Vertex* 				GetVertex(int i) { return &m_Vertices.at(i); }
		
		int32_t					GetNumFaces();
		void					Draw(RenderContext* pGraphics, Matrix4f& matViewProjection, Matrix4f& matTransform, Sprite* pBitmapTexture);
	protected:
	private:
		void					LoadInInternalFormat();
		
		std::vector<Vertex>		m_Vertices;
		MeshObjLoader*			m_pMeshObjLoader;
};