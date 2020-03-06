#pragma once
#include <windows.h>
#include <cstdint>
#include "Pixel.h"
#include "Vertex.h"
#include <chrono>
#include "Edge.h"

class Sprite;
class RenderContext
{
	public:
															RenderContext(uint32_t iWidth, uint32_t iHeight, uint32_t iPixelWidth, uint32_t iPixelHeight);
															~RenderContext();

		uint32_t											GetWidth();
		uint32_t											GetHeight();

		void												Render();
		void												DrawPixel(int32_t iX, int32_t iY, Pixel pPixel);
		void												CopyPixel(int destX, int destY, int srcX, int srcY, Sprite* pTexture);
		void												Clear(Pixel p);

		void												FillTriangle(Vertex v1, Vertex v2, Vertex v3, Sprite* pTexture);
	protected:
		void												RenderInternal(uint32_t iDeltaTimeMs);

		void												ScanTriangle(Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert, bool bHandedness, Sprite* pTexture);
		void												ScanEdges(Gradients* pGradients, Edge* eA, Edge* eB, bool handedness, Sprite* pTexture);
		void												DrawScanLine(Gradients* pGradients, Edge* eLeft, Edge* eRight, int j, Sprite* pTexture);
	private:
		Sprite*												m_pFrameBuffer = nullptr;

		uint32_t											m_iWidth;
		uint32_t											m_iHeight;
		uint32_t											m_iPixelWidth;
		uint32_t											m_iPixelHeight;

		uint32_t											m_iDeltaTimeMs;
		std::chrono::high_resolution_clock::time_point		m_tpLastUpdate;
};