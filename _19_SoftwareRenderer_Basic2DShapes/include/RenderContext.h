#pragma once
#include <windows.h>
#include <cstdint>
#include "Pixel.h"
#include "Vertex.h"
#include <chrono>
#include "Edge.h"
#include "Mesh.h"
#include "Transform.h"
#include <functional>

class Sprite;
struct Shape2D;
class RenderContext
{
	public:
															RenderContext(uint32_t iWidth, uint32_t iHeight, uint32_t iPixelWidth, uint32_t iPixelHeight, std::function<void(uint32_t)>* fRenderCallback);
															~RenderContext();

		uint32_t											GetWidth();
		uint32_t											GetHeight();

		void												Render();

		void												Clear(Pixel p, float fDepth);
		void												DrawTriangle(Vertex v1, Vertex v2, Vertex v3, Sprite* pBitmapTexture);		

		void												FillTriangle3D(Vertex v1, Vertex v2, Vertex v3, Sprite* pTexture);
		void												FillTriangle2D(Vertex v1, Vertex v2, Vertex v3, Sprite* pTexture);

		void												SetAmbientLightIntensity(float fAmbientLightIntensity);
		float												GetAmbientLightIntensity();
	protected:
		void												SetPixel(int destX, int destY, Pixel p, bool bForce = false);

		bool												ClipPolygonAxis(std::vector<Vertex>& vVertices, std::vector<Vertex>& vAuxillaryList, AXIS_COMPONENT eComponentIndex);
		void												ClipPolygonComponent(std::vector<Vertex>& vVertices, AXIS_COMPONENT eComponentIndex, float fComponentFactor, std::vector<Vertex>& vResult);

		void												ScanTriangle(Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert, bool bHandedness, Sprite* pTexture);
		void												ScanEdges(Gradients* pGradients, Edge* eA, Edge* eB, bool handedness, Sprite* pTexture);
		void												DrawScanLine(Gradients* pGradients, Edge* eLeft, Edge* eRight, int j, Sprite* pTexture);
	private:
		Sprite*												m_pFrameBuffer = nullptr;
		float*												m_pDepthBuffer = nullptr;
		std::function<void(uint32_t)>*						m_fRenderCallback = nullptr;

		uint32_t											m_iWidth;
		uint32_t											m_iHeight;
		uint32_t											m_iPixelWidth;
		uint32_t											m_iPixelHeight;

		float												m_fAmbientLightIntensity;

		uint32_t											m_iDeltaTimeMs;
		std::chrono::high_resolution_clock::time_point		m_tpLastUpdate;
};