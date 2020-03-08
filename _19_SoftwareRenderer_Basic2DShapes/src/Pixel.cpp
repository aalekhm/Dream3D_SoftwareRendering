#include "Pixel.h"

Pixel::Pixel()
{
	m_iRed = 0; m_iGreen = 0; m_iBlue = 0; m_iAlpha = 0;
}

Pixel::Pixel(uint8_t iRed, uint8_t iGreen, uint8_t iBlue, uint8_t iAlpha)
{
	m_iRed = iRed;
	m_iGreen = iGreen;
	m_iBlue = iBlue;
	m_iAlpha = iAlpha;
}

Pixel::Pixel(uint32_t p)
{
	m_iPixel = p;
}

uint32_t Pixel::GetPixel()
{
	return m_iPixel;
}

bool Pixel::operator==(const Pixel& p) const
{
	return (m_iPixel == p.m_iPixel);
}

bool Pixel::operator!=(const Pixel& p) const
{
	return (m_iPixel != p.m_iPixel);
}
