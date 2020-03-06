#pragma once
#include <cstdint>
#include <stdlib.h>
#include "Sprite.h"
#include <chrono>

void printLine_(const char* lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);
	{
		CCString cBuff;
		char sBuffer[512];
		_vsnprintf(sBuffer, 511, lpszFormat, args);

		cBuff = sBuffer;
		cBuff += "\n";

		OutputDebugString(cBuff.c_str());
	}
	va_end(args);
}

class Stars3D
{
	public:
		Stars3D(Sprite* pTarget, float fFOV, uint32_t iNumStars, float fSpread, float fSpeed)
		{
			m_fFOVDegrees = fFOV;
			m_pTarget = pTarget;
			m_iNumStars = iNumStars;
			m_fSpread = fSpread;
			m_fSpeed = fSpeed;
			
			m_fStarX = new float[iNumStars];
			m_fStarY = new float[iNumStars];
			m_fStarZ = new float[iNumStars];

			for (uint32_t i = 0; i < iNumStars; i++)
			{
				InitStar(i);
			}

			m_iDeltaTime = 0;
			m_tpLastUpdate = std::chrono::high_resolution_clock::now();
		}
		
		void UpdateAndRender()
		{
			auto now = std::chrono::high_resolution_clock::now();
			m_iDeltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_tpLastUpdate).count();
			m_tpLastUpdate = now;

			float fDelta = m_iDeltaTime / 1000.0f;
			m_pTarget->Clear(Pixel(0, 0, 0, 255));

			float fTanHalfFOV = tanf((m_fFOVDegrees / 2.0f) * 3.142f / 180.0f);
			uint32_t iHalfWidth = m_pTarget->GetWidth() * 0.5f;
			uint32_t iHalfHeight = m_pTarget->GetHeight() * 0.5f;
			for (uint32_t i = 0; i < m_iNumStars; i++)
			{
				m_fStarZ[i] -= (fDelta * m_fSpeed);

				if (m_fStarZ[i] <= 0)
				{
					InitStar(i);
				}

				int x = (int)((m_fStarX[i] / (m_fStarZ[i] * fTanHalfFOV)) * iHalfWidth + iHalfWidth);
				int y = (int)((m_fStarY[i] / (m_fStarZ[i] * fTanHalfFOV)) * iHalfHeight + iHalfHeight);

				//printLine_("[%d] = %d, %d", i, x, y);
				if (	(x < 0 || x >= m_pTarget->GetWidth())
						||
						(y < 0 || y >= m_pTarget->GetHeight())
				) {
					InitStar(i);
				}
				else
				{
					m_pTarget->SetPixel(x, y, Pixel(255, 255, 255, 255));
				}
			}
		}
	protected:
	private:
		void InitStar(uint32_t iIndex)
		{
			// Random number from 0.0f to 1.0f
			float fRandX = (rand() % 1000) * 0.001f;
			float fRandY = (rand() % 1000) * 0.001f;
			float fRandZ = (rand() % 1000) * 0.001f;
			
			m_fStarX[iIndex] = 2 * ((float)fRandX - 0.5f) * m_fSpread;
			m_fStarY[iIndex] = 2 * ((float)fRandY - 0.5f) * m_fSpread;
			m_fStarZ[iIndex] = ((float)fRandZ + 0.00001f) * m_fSpread;
		}

		Sprite*												m_pTarget;
		uint32_t 											m_iNumStars;
		float 												m_fSpread;
		float 												m_fSpeed;

		float*												m_fStarX;
		float*												m_fStarY;
		float*												m_fStarZ;

		float												m_fFOVDegrees;

		uint32_t											m_iDeltaTime;
		std::chrono::high_resolution_clock::time_point		m_tpLastUpdate;
};