#pragma once
#include <windows.h>
#include <cstdint>
#include "Pixel.h"
#include <chrono>

class Sprite;
class RenderContext
{
	public:
															RenderContext(uint32_t iWidth, uint32_t iHeight, uint32_t iPixelWidth, uint32_t iPixelHeight);
															~RenderContext();

		void												Render();
		void												DrawPixel(int32_t iX, int32_t iY, Pixel pPixel);
		void												Clear(Pixel p);
	protected:
		void												RenderInternal(uint32_t iDeltaTimeMs);
		void												DrawScanBuffer(uint32_t iYCoord, uint32_t iXMin, uint32_t iXMax);
		void												FillShape(uint32_t iYMin, uint32_t iYMax);
	private:
		Sprite*												m_pFrameBuffer = nullptr;

		uint32_t											m_iWidth;
		uint32_t											m_iHeight;
		uint32_t											m_iPixelWidth;
		uint32_t											m_iPixelHeight;

		uint32_t											m_iDeltaTimeMs;
		std::chrono::high_resolution_clock::time_point		m_tpLastUpdate;

		uint32_t*											m_iScanBuffer = nullptr;
};