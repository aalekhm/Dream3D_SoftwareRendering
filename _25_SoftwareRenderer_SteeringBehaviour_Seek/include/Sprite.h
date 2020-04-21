#pragma once

#include "Pixel.h"

class Sprite
{
	public:
							Sprite();
							Sprite(uint32_t iWidth, uint32_t iHeight);
							Sprite(const char* sFileName);
		virtual				~Sprite();

		Pixel				GetPixel(int32_t x, int32_t y);
		void				SetPixel(int32_t x, int32_t y, Pixel p, bool bForce = false);
		static Pixel		Blend(Pixel pSrcPixel, Pixel pDstPixel);

		Pixel*				GetData();
		void				Clear(Pixel p);

		uint32_t			GetWidth();
		uint32_t			GetHeight();

		uint32_t			GetTextureAlpha();
		void				SetTextureAlpha(float fTextureAlpha);
	protected:
	private:
		uint32_t			m_iWidth;
		uint32_t			m_iHeight;

		Pixel*				m_pColData = nullptr;

		uint32_t			m_iTextureAlpha;
};
