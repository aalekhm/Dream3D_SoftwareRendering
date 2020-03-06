#include "Sprite.h"
#include <vcruntime_string.h>

Sprite::Sprite()
{
	m_pColData = nullptr;
	m_iWidth = 0;
	m_iHeight = 0;
}

Sprite::Sprite(uint32_t iWidth, uint32_t iHeight)
{
	if (m_pColData != nullptr)
		delete[] m_pColData;

	m_iWidth = iWidth;
	m_iHeight = iHeight;

	m_pColData = new Pixel[iWidth * iHeight];
	for (uint32_t i = 0; i < iWidth * iHeight; i++)
	{
		m_pColData[i] = Pixel();
	}
}

Pixel Sprite::GetPixel(int32_t x, int32_t y)
{
	if (x >= 0 && x < m_iWidth && y >= 0 && y < m_iHeight)
		return m_pColData[y*m_iWidth + x];
	else
		return Pixel(0, 0, 0, 0);
}

bool  Sprite::SetPixel(int32_t x, int32_t y, Pixel p)
{
	if (x >= 0 && x < m_iWidth && y >= 0 && y < m_iHeight)
	{
		m_pColData[y*m_iWidth + x] = p;
		return true;
	}

	return false;
}

void Sprite::Clear(Pixel p)
{
	uint32_t iSize = m_iWidth * m_iHeight;
	int32_t iPixel = p.GetPixel();
	for (int32_t i = 0; i < iSize; i++)
	{
		memcpy(&m_pColData[i], &iPixel, sizeof(Pixel));
	}
}

uint32_t Sprite::GetWidth()
{
	return m_iWidth;
}

uint32_t Sprite::GetHeight()
{
	return m_iHeight;
}


Pixel* Sprite::GetData()
{
	return m_pColData;
}

Sprite::~Sprite()
{
	if (m_pColData)
		delete[] m_pColData;
}
