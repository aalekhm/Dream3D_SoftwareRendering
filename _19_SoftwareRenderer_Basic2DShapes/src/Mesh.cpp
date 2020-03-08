#include "Mesh.h"
#include "RenderContext.h"
#include "MeshObjLoader.h"

Mesh::Mesh(const char* sFileName)
{
	m_pMeshObjLoader = new MeshObjLoader();
	if (m_pMeshObjLoader != nullptr)
	{
		m_pMeshObjLoader->loadObject(sFileName);
		LoadInInternalFormat();
	}
}

void Mesh::LoadInInternalFormat()
{	
	for (int i = 0; i < m_pMeshObjLoader->getNumFaces(); i++)
	{
		SFace* pFace = m_pMeshObjLoader->getFace(i);

		SVector3* sV1Pos = m_pMeshObjLoader->getVertex(pFace->vertexIndex[0] - 1);
		SVector2* sV1TexCoord = m_pMeshObjLoader->getTexCoord(pFace->texCoordIndex[0] - 1);
		SVector3* sV1Normal = m_pMeshObjLoader->getNormal(pFace->vertexNormalIndex[0] - 1);
		Vertex v1(	Vector4f(sV1Pos->x, sV1Pos->y, sV1Pos->z, 1.0f), 
					Vector4f(sV1TexCoord->x, sV1TexCoord->y, 0.0f, 0.0f), 
					Vector4f(sV1Normal->x, sV1Normal->y, sV1Normal->z, 0.0f),
					Vector4f());

		SVector3* sV2Pos = m_pMeshObjLoader->getVertex(pFace->vertexIndex[1] - 1);
		SVector2* sV2TexCoord = m_pMeshObjLoader->getTexCoord(pFace->texCoordIndex[1] - 1);
		SVector3* sV2Normal = m_pMeshObjLoader->getNormal(pFace->vertexNormalIndex[1] - 1);
		Vertex v2(	Vector4f(sV2Pos->x, sV2Pos->y, sV2Pos->z, 1.0f), 
					Vector4f(sV2TexCoord->x, sV2TexCoord->y, 0.0f, 0.0f), 
					Vector4f(sV2Normal->x, sV2Normal->y, sV2Normal->z, 0.0f),
					Vector4f());

		SVector3* sV3Pos = m_pMeshObjLoader->getVertex(pFace->vertexIndex[2] - 1);
		SVector2* sV3TexCoord = m_pMeshObjLoader->getTexCoord(pFace->texCoordIndex[2] - 1);
		SVector3* sV3Normal = m_pMeshObjLoader->getNormal(pFace->vertexNormalIndex[2] - 1);
		Vertex v3(	Vector4f(sV3Pos->x, sV3Pos->y, sV3Pos->z, 1.0f), 
					Vector4f(sV3TexCoord->x, sV3TexCoord->y, 0.0f, 0.0f), 
					Vector4f(sV3Normal->x, sV3Normal->y, sV3Normal->z, 0.0f),
					Vector4f());

		{
			m_Vertices.push_back(v1);
			m_Vertices.push_back(v2);
			m_Vertices.push_back(v3);
		}
	}
}

int32_t Mesh::GetNumFaces()
{
	return m_pMeshObjLoader->getNumFaces();
}

void Mesh::Draw(RenderContext* pTarget, Matrix4f& matViewProjection, Matrix4f& matTransform, Sprite* pBitmapTexture)
{
	Matrix4f mvp = matViewProjection.mul(matTransform);
	for(int i = 0; i < GetNumFaces(); i++)
	{
		Vertex* v1 = GetVertex(i * 3 + 0);
		Vertex* v2 = GetVertex(i * 3 + 1);
		Vertex* v3 = GetVertex(i * 3 + 2);
		
		pTarget->DrawTriangle(	v1->Transform(mvp, matTransform),
								v2->Transform(mvp, matTransform),
								v3->Transform(mvp, matTransform),
								pBitmapTexture);
	}
}