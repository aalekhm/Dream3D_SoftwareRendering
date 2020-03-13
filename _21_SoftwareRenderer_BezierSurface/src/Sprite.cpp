#include "Sprite.h"
#include <vcruntime_string.h>
#include <string.h>
#include "Engine/Image.h"

Sprite::Sprite()
{
	m_pColData = nullptr;
	m_iWidth = 0;
	m_iHeight = 0;

	m_iTextureAlpha = 0xFF;
}

Sprite::Sprite(uint32_t iWidth, uint32_t iHeight)
{
	if (m_pColData != nullptr)
		delete[] m_pColData;

	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iTextureAlpha = 0xFF;

	m_pColData = new Pixel[iWidth * iHeight];
	for (uint32_t i = 0; i < iWidth * iHeight; i++)
	{
		m_pColData[i] = Pixel();
	}
}

Sprite::Sprite(const char* sFileName)
{
	if (m_pColData != nullptr)
		delete[] m_pColData;

	if (strlen(sFileName) > 0)
	{
		Image* pImage = Image::createImage(sFileName);
		if (pImage != nullptr)
		{
			m_iWidth = pImage->getWidth();
			m_iHeight = pImage->getHeight();
			m_iTextureAlpha = 0xFF;

			m_pColData = new Pixel[m_iWidth * m_iHeight];
			switch (pImage->getFormat())
			{
				case Image::RGB:
				{
					unsigned char* pPixelData = pImage->getPixelData();
					int32_t iSize = m_iWidth * m_iHeight;
					for (int32_t i = 0; i < iSize; i++)
					{
						Pixel* pPixel = &m_pColData[i];

						pPixel->m_iRed		= pPixelData[i * 3 + 0];
						pPixel->m_iGreen	= pPixelData[i * 3 + 1];
						pPixel->m_iBlue		= pPixelData[i * 3 + 2];
						pPixel->m_iAlpha	= m_iTextureAlpha;
					}
				}
				break;
				case Image::RGBA:
				{
					memcpy(m_pColData, pImage->getPixelData(), sizeof(char) * m_iWidth * m_iHeight);
				}
				break;
			}

			delete pImage;
		}
	}
}

Pixel Sprite::GetPixel(int32_t x, int32_t y)
{
	return m_pColData[y*m_iWidth + x];
}

static float gOneOver255 = 1.0f / 255.0f;
Pixel Sprite::Blend(Pixel pSrcPixel, Pixel pDstPixel)
{
	Pixel pResultColour;
	{
		float fSrcAlpha = pSrcPixel.m_iAlpha * gOneOver255;

		pResultColour.m_iRed	= (uint8_t)( ((pDstPixel.m_iRed		* (1 - fSrcAlpha)) + (pSrcPixel.m_iRed	* fSrcAlpha)) );
		pResultColour.m_iGreen	= (uint8_t)( ((pDstPixel.m_iGreen	* (1 - fSrcAlpha)) + (pSrcPixel.m_iGreen* fSrcAlpha)) );
		pResultColour.m_iBlue	= (uint8_t)( ((pDstPixel.m_iBlue	* (1 - fSrcAlpha)) + (pSrcPixel.m_iBlue	* fSrcAlpha)) );
		pResultColour.m_iAlpha	= (uint8_t)( ((pDstPixel.m_iAlpha	* (1 - fSrcAlpha)) + (pSrcPixel.m_iAlpha* fSrcAlpha)) );
	}

	return pResultColour;
}

void Sprite::SetPixel(int32_t x, int32_t y, Pixel pSrcPixel, bool bForce)
{
	if (bForce)
	{
		m_pColData[y*m_iWidth + x] = pSrcPixel;
	}
	else
	{
		Pixel pDstPixel = m_pColData[y*m_iWidth + x];
		Pixel pResultColour = Blend(pSrcPixel, pDstPixel);
		m_pColData[y*m_iWidth + x] = pResultColour;
	}
}

void Sprite::Clear(Pixel p)
{
	uint32_t iSize = m_iWidth * m_iHeight;
	uint32_t iPixel = p.GetPixel();
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

uint32_t Sprite::GetTextureAlpha()
{
	return m_iTextureAlpha;
}

void Sprite::SetTextureAlpha(float fTextureAlpha)
{
	m_iTextureAlpha = (uint32_t)(0xFF * fTextureAlpha);
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
