#include "Mesh.h"

Mesh::Mesh(const char* sFileName)
{
	m_pMeshObjLoader = new MeshObjLoader();
	if (m_pMeshObjLoader != nullptr)
	{
		m_pMeshObjLoader->loadObject(sFileName);
		load();
	}
}

void Mesh::load()
{
	for (int i = 0; i < m_pMeshObjLoader->getNumFaces(); i++)
	{
		SFace* pFace = m_pMeshObjLoader->getFace(i);

		SVector3* sV1Pos = m_pMeshObjLoader->getVertex(pFace->vertexIndex[0] - 1);
		SVector2* sV1TexCoord = m_pMeshObjLoader->getTexCoord(pFace->texCoordIndex[0] - 1);
		Vertex v1(Vector4f(sV1Pos->x, sV1Pos->y, sV1Pos->z, 1.0f), Vector4f(sV1TexCoord->x, sV1TexCoord->y, 0.0f, 0.0f));

		SVector3* sV2Pos = m_pMeshObjLoader->getVertex(pFace->vertexIndex[1] - 1);
		SVector2* sV2TexCoord = m_pMeshObjLoader->getTexCoord(pFace->texCoordIndex[1] - 1);
		Vertex v2(Vector4f(sV2Pos->x, sV2Pos->y, sV2Pos->z, 1.0f), Vector4f(sV2TexCoord->x, sV2TexCoord->y, 0.0f, 0.0f));

		SVector3* sV3Pos = m_pMeshObjLoader->getVertex(pFace->vertexIndex[2] - 1);
		SVector2* sV3TexCoord = m_pMeshObjLoader->getTexCoord(pFace->texCoordIndex[2] - 1);
		Vertex v3(Vector4f(sV3Pos->x, sV3Pos->y, sV3Pos->z, 1.0f), Vector4f(sV3TexCoord->x, sV3TexCoord->y, 0.0f, 0.0f));

		{
			m_Vertices.push_back(v1);
			m_Vertices.push_back(v2);
			m_Vertices.push_back(v3);
		}
	}
}
