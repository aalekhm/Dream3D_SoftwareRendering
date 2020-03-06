#pragma once
#include "Vertex.h"
#include <vector>
#include "MeshObjLoader.h"

class Mesh
{
	public:
								Mesh(const char* sFileName);
		Vertex* 				GetVertex(int i) { return &m_Vertices.at(i); }

		MeshObjLoader*			m_pMeshObjLoader;
	protected:
	private:
		void					load();
		std::vector<Vertex>		m_Vertices;
};