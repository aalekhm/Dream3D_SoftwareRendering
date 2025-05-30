#pragma once

#include "Pixel.h"

class Sprite
{
	public:
							Sprite();
							Sprite(uint32_t iWidth, uint32_t iHeight);
		virtual				~Sprite();

		Pixel				GetPixel(int32_t x, int32_t y);
		bool				SetPixel(int32_t x, int32_t y, Pixel p);

		Pixel*				GetData();
		void				Clear(Pixel p);
	protected:
	private:
		uint32_t			m_iWidth;
		uint32_t			m_iHeight;
		Pixel*				m_pColData = nullptr;
};
