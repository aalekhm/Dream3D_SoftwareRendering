#pragma once
#include "Actor2D.h"
#include "Vector4f.h"
#include <vector>

class Utils2D
{
	public:
		static Actor2D* create2DSwastikaShip()
		{
			Actor2D* pShapeSpaceShip2D = nullptr;
			std::vector<Shape2DVertex> vTriVertices;
			{
				// Anticlockwise Clockwise winding
				vTriVertices.push_back(Shape2DVertex(Vector4f(-24, 24, 1, 1),	Vector4f(0.0, 1.0, 1, 1),	Vector4f(0.0, 0.0, 0.0, 0.0f)));
				vTriVertices.push_back(Shape2DVertex(Vector4f(24, -25, 1, 1),	Vector4f(1.0, 0.0, 1, 1),	Vector4f(0.0, 0.0, 0.0, 0.0f)));
				vTriVertices.push_back(Shape2DVertex(Vector4f(-24, -25, 1, 1),	Vector4f(0.0, 0.0, 1, 1),	Vector4f(0.0, 0.0, 0.0, 0.0f)));

				vTriVertices.push_back(Shape2DVertex(Vector4f(-24, 24, 0, 1),	Vector4f(0.0, 1.0, 0, 1),	Vector4f(0.0, 0.0, 0.0, 0.0f)));
				vTriVertices.push_back(Shape2DVertex(Vector4f(24, 25, 0, 1),	Vector4f(1.0, 1.0, 0, 1),	Vector4f(0.0, 0.0, 0.0, 0.0f)));
				vTriVertices.push_back(Shape2DVertex(Vector4f(24, -25, 0, 1),	Vector4f(1.0, 0.0, 0, 1),	Vector4f(0.0, 0.0, 0.0, 0.0f)));

				pShapeSpaceShip2D = new Actor2D(vTriVertices, "../Content/Swastik.tga");
			}

			return pShapeSpaceShip2D;
		}
};