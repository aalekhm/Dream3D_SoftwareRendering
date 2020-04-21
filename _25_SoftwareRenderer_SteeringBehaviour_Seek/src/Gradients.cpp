#include "Gradients.h"

Gradients::Gradients(Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert, float fGlobalAmbientLightIntensity)
{
	// Note that the 'W' component is the perspective 'Z' value;
	// The 'Z' component is the Occlusion 'Z' value.
	m_fOneOverZ[0] = 1.0f / vMinYVert.GetPosition().getW();
	m_fOneOverZ[1] = 1.0f / vMidYVert.GetPosition().getW();
	m_fOneOverZ[2] = 1.0f / vMaxYVert.GetPosition().getW();

	m_fTexCoordX[0] = vMinYVert.GetTexCoords().getX() * m_fOneOverZ[0];
	m_fTexCoordX[1] = vMidYVert.GetTexCoords().getX() * m_fOneOverZ[1];
	m_fTexCoordX[2] = vMaxYVert.GetTexCoords().getX() * m_fOneOverZ[2];

	m_fTexCoordY[0] = vMinYVert.GetTexCoords().getY() * m_fOneOverZ[0];
	m_fTexCoordY[1] = vMidYVert.GetTexCoords().getY() * m_fOneOverZ[1];
	m_fTexCoordY[2] = vMaxYVert.GetTexCoords().getY() * m_fOneOverZ[2];
	
	m_fDepth[0] = vMinYVert.GetPosition().getZ();
	m_fDepth[1] = vMidYVert.GetPosition().getZ();
	m_fDepth[2] = vMaxYVert.GetPosition().getZ();

	float fLightIntensity = 0.9f;
	Vector4f lightDir(0, 0, -1, 0);
	m_fLightAmt[0] = Saturate(vMinYVert.GetNormal().dot(lightDir)) * fLightIntensity + fGlobalAmbientLightIntensity;
	m_fLightAmt[1] = Saturate(vMidYVert.GetNormal().dot(lightDir)) * fLightIntensity + fGlobalAmbientLightIntensity;
	m_fLightAmt[2] = Saturate(vMaxYVert.GetNormal().dot(lightDir)) * fLightIntensity + fGlobalAmbientLightIntensity;

	m_fColourR[0] = vMinYVert.GetR();
	m_fColourR[1] = vMidYVert.GetR();
	m_fColourR[2] = vMaxYVert.GetR();

	m_fColourG[0] = vMinYVert.GetG();
	m_fColourG[1] = vMidYVert.GetG();
	m_fColourG[2] = vMaxYVert.GetG();

	m_fColourB[0] = vMinYVert.GetB();
	m_fColourB[1] = vMidYVert.GetB();
	m_fColourB[2] = vMaxYVert.GetB();

	m_fColourA[0] = vMinYVert.GetA();
	m_fColourA[1] = vMidYVert.GetA();
	m_fColourA[2] = vMaxYVert.GetA();

	float fOneOverdX = 1.0f / (
								(	(vMidYVert.GetX() - vMaxYVert.GetX()) *
									(vMinYVert.GetY() - vMaxYVert.GetY())
								) -
								(	(vMinYVert.GetX() - vMaxYVert.GetX()) *
									(vMidYVert.GetY() - vMaxYVert.GetY())
								));

	float fOneOverdY = -fOneOverdX;

	m_fTexCoordXXStep = CalcXStep(m_fTexCoordX, vMinYVert, vMidYVert, vMaxYVert, fOneOverdX);
	m_fTexCoordXYStep = CalcYStep(m_fTexCoordX, vMinYVert, vMidYVert, vMaxYVert, fOneOverdY);

	m_fTexCoordYXStep = CalcXStep(m_fTexCoordY, vMinYVert, vMidYVert, vMaxYVert, fOneOverdX);
	m_fTexCoordYYStep = CalcYStep(m_fTexCoordY, vMinYVert, vMidYVert, vMaxYVert, fOneOverdY);

	m_fOneOverZXStep = CalcXStep(m_fOneOverZ, vMinYVert, vMidYVert, vMaxYVert, fOneOverdX);
	m_fOneOverZYStep = CalcYStep(m_fOneOverZ, vMinYVert, vMidYVert, vMaxYVert, fOneOverdY);

	m_fDepthXStep = CalcXStep(m_fDepth, vMinYVert, vMidYVert, vMaxYVert, fOneOverdX);
	m_fDepthYStep = CalcYStep(m_fDepth, vMinYVert, vMidYVert, vMaxYVert, fOneOverdY);

	m_fLightAmtXStep = CalcXStep(m_fLightAmt, vMinYVert, vMidYVert, vMaxYVert, fOneOverdX);
	m_fLightAmtYStep = CalcYStep(m_fLightAmt, vMinYVert, vMidYVert, vMaxYVert, fOneOverdY);

	m_fColourRXStep = CalcXStep(m_fColourR, vMinYVert, vMidYVert, vMaxYVert, fOneOverdX);
	m_fColourRYStep = CalcYStep(m_fColourR, vMinYVert, vMidYVert, vMaxYVert, fOneOverdY);

	m_fColourGXStep = CalcXStep(m_fColourG, vMinYVert, vMidYVert, vMaxYVert, fOneOverdX);
	m_fColourGYStep = CalcYStep(m_fColourG, vMinYVert, vMidYVert, vMaxYVert, fOneOverdY);

	m_fColourBXStep = CalcXStep(m_fColourB, vMinYVert, vMidYVert, vMaxYVert, fOneOverdX);
	m_fColourBYStep = CalcYStep(m_fColourB, vMinYVert, vMidYVert, vMaxYVert, fOneOverdY);

	m_fColourAXStep = CalcXStep(m_fColourA, vMinYVert, vMidYVert, vMaxYVert, fOneOverdX);
	m_fColourAYStep = CalcYStep(m_fColourA, vMinYVert, vMidYVert, vMaxYVert, fOneOverdY);
}

float Gradients::CalcXStep(float* pValues, Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert, float fOneOverDX)
{
	return	((	(pValues[1] - pValues[2])
				*
				(vMinYVert.GetY() - vMaxYVert.GetY())
			) -
			(	(pValues[0] - pValues[2])
				*
				(vMidYVert.GetY() - vMaxYVert.GetY())
			)) * fOneOverDX;
}

float Gradients::CalcYStep(float* pValues, Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert, float fOneOverDY)
{
	return	((	(pValues[1] - pValues[2])
				*
				(vMinYVert.GetX() - vMaxYVert.GetX())
			) -
			(	(pValues[0] - pValues[2])
				*
				(vMidYVert.GetX() - vMaxYVert.GetX())
			)) * fOneOverDY;
}

float Gradients::GetTexCoordX(int32_t iLoc) {	return m_fTexCoordX[iLoc]; }
float Gradients::GetTexCoordY(int32_t iLoc) {	return m_fTexCoordY[iLoc]; }
float Gradients::GetOneOverZ(int32_t iLoc)	{	return m_fOneOverZ[iLoc]; }
float Gradients::GetDepth(int32_t iLoc)		{	return m_fDepth[iLoc]; }
float Gradients::GetLightAmt(int32_t iLoc)	{	return m_fLightAmt[iLoc]; }
float Gradients::GetColourR(int32_t iLoc)	{	return m_fColourR[iLoc]; }
float Gradients::GetColourG(int32_t iLoc)	{	return m_fColourG[iLoc]; }
float Gradients::GetColourB(int32_t iLoc)	{	return m_fColourB[iLoc]; }
float Gradients::GetColourA(int32_t iLoc)	{	return m_fColourA[iLoc]; }

float Gradients::GetTexCoordXXStep()		{	return m_fTexCoordXXStep; }
float Gradients::GetTexCoordXYStep()		{	return m_fTexCoordXYStep; }
float Gradients::GetTexCoordYXStep()		{	return m_fTexCoordYXStep; }
float Gradients::GetTexCoordYYStep()		{	return m_fTexCoordYYStep; }
float Gradients::GetOneOverZXStep()			{	return m_fOneOverZXStep; }
float Gradients::GetOneOverZYStep()			{	return m_fOneOverZYStep; }
float Gradients::GetDepthXStep()			{	return m_fDepthXStep; }
float Gradients::GetDepthYStep()			{	return m_fDepthYStep; }
float Gradients::GetLightAmtXStep()			{	return m_fLightAmtXStep; }
float Gradients::GetLightAmtYStep()			{	return m_fLightAmtYStep; }
float Gradients::GetColourRXStep()			{	return m_fColourRXStep; }
float Gradients::GetColourRYStep()			{	return m_fColourRYStep; }
float Gradients::GetColourGXStep()			{	return m_fColourGXStep; }
float Gradients::GetColourGYStep()			{	return m_fColourGYStep; }
float Gradients::GetColourBXStep()			{	return m_fColourBXStep; }
float Gradients::GetColourBYStep()			{	return m_fColourBYStep; }
float Gradients::GetColourAXStep()			{	return m_fColourAXStep; }
float Gradients::GetColourAYStep()			{	return m_fColourAYStep; }

float Gradients::Saturate(float fVal)
{
	if (fVal > 1.0f)
	{
		return 1.0f;
	}

	if (fVal < 0.0f)
	{
		return 0.0f;
	}

	return fVal;
}

