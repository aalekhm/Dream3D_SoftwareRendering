#pragma once
#include "Actor2D.h"
#include "Vector4f.h"
#include <vector>
#include "Sprite.h"

class Utils2D
{
	public:
		static Actor2D* create2DSwastikaShip(Vector4f vColour)
		{
			Actor2D* pShapeSpaceShip2D = nullptr;
			std::vector<Shape2DVertex> vTriVertices;
			{
				// Anticlockwise Clockwise winding
				vTriVertices.push_back(Shape2DVertex(Vector4f(-24, 24, 1, 1),	Vector4f(0.0, 1.0, 1, 1),	vColour));
				vTriVertices.push_back(Shape2DVertex(Vector4f(24, -25, 1, 1),	Vector4f(1.0, 0.0, 1, 1),	vColour));
				vTriVertices.push_back(Shape2DVertex(Vector4f(-24, -25, 1, 1),	Vector4f(0.0, 0.0, 1, 1),	vColour));

				vTriVertices.push_back(Shape2DVertex(Vector4f(-24, 24, 0, 1),	Vector4f(0.0, 1.0, 0, 1),	vColour));
				vTriVertices.push_back(Shape2DVertex(Vector4f(24, 25, 0, 1),	Vector4f(1.0, 1.0, 0, 1),	vColour));
				vTriVertices.push_back(Shape2DVertex(Vector4f(24, -25, 0, 1),	Vector4f(1.0, 0.0, 0, 1),	vColour));

				pShapeSpaceShip2D = new Actor2D(vTriVertices, "../Content/Swastik.tga");
				pShapeSpaceShip2D->GetTexture()->SetTextureAlpha(vColour.getW());
			}

			return pShapeSpaceShip2D;
		}
};