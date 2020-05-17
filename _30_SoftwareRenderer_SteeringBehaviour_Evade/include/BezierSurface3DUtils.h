#pragma once
#include <math.h>
#include "Mesh.h"
#include "Vector4f.h"
#include "Vertex.h"

class BezierSurface3DUtils
{
	public:
		static Mesh*		GenerateBezierSurface();
		static double		BezierBlend(int k, float t, int n);
};

/*
#define NI 5
#define NJ 4
Vector4f inp[NI + 1][NJ + 1];
#define RESOLUTIONI 10*NI
#define RESOLUTIONJ 10*NJ
Vector4f outp[RESOLUTIONI][RESOLUTIONJ];

Mesh* BezierSurface3DUtils::GenerateBezierSurfaceOrig()
{
	int i, j, ki, kj;
	double mui, muj, bi, bj;

	// Create a random surface
	srand(1111);
	for (i = 0; i <= NI; i++) 
	{
		for (j = 0; j <= NJ; j++) 
		{
			inp[i][j].x = i;
			inp[i][j].y = j;
			inp[i][j].z = (rand() % 100) / 50.0 - 1;
		}
	}

	for (i = 0; i < RESOLUTIONI; i++) 
	{
		mui = i / (double)(RESOLUTIONI - 1);
		for (j = 0; j < RESOLUTIONJ; j++) 
		{
			muj = j / (double)(RESOLUTIONJ - 1);
			outp[i][j].x = 0;
			outp[i][j].y = 0;
			outp[i][j].z = 0;
			for (ki = 0; ki <= NI; ki++) 
			{
				bi = BezierBlend(ki, mui, NI);
				for (kj = 0; kj <= NJ; kj++) 
				{
					bj = BezierBlend(kj, muj, NJ);
					outp[i][j].x += (inp[ki][kj].x * bi * bj);
					outp[i][j].y += (inp[ki][kj].y * bi * bj);
					outp[i][j].z += (inp[ki][kj].z * bi * bj);
				}
			}
		}
	}

	//printLine_("LIST");
	//
	// Display the surface, in this case in OOGL format for GeomView
	//printLine_("{ = CQUAD");
	//for (i = 0; i<RESOLUTIONI - 1; i++) 
	//{
	//	for (j = 0; j<RESOLUTIONJ - 1; j++) 
	//	{
	//		printLine_("%g %g %g 1 1 1 1",	outp[i][j].x, outp[i][j].y, outp[i][j].z);
	//		printLine_("%g %g %g 1 1 1 1",	outp[i][j + 1].x, outp[i][j + 1].y, outp[i][j + 1].z);
	//		printLine_("%g %g %g 1 1 1 1",	outp[i + 1][j + 1].x, outp[i + 1][j + 1].y, outp[i + 1][j + 1].z);
	//		printLine_("%g %g %g 1 1 1 1",	outp[i + 1][j].x, outp[i + 1][j].y, outp[i + 1][j].z);
	//	}
	//}
	//printLine_("}\n");
	//
	// Control point polygon
	//for (i = 0; i<NI; i++)
	//{
	//	for (j = 0; j<NJ; j++)
	//	{
	//		printLine_("{ = SKEL 4 1");
	//		printLine_("%g %g %g", inp[i][j].x, inp[i][j].y, inp[i][j].z);
	//		printLine_("%g %g %g", inp[i][j + 1].x, inp[i][j + 1].y, inp[i][j + 1].z);
	//		printLine_("%g %g %g", inp[i + 1][j + 1].x, inp[i + 1][j + 1].y, inp[i + 1][j + 1].z);
	//		printLine_("%g %g %g", inp[i + 1][j].x, inp[i + 1][j].y, inp[i + 1][j].z);
	//		printLine_("5 0 1 2 3 0");
	//		printLine_("}\n");
	//	}
	//}

	std::vector<Vertex> vVertices;
	for (i = 0; i < RESOLUTIONI - 1; i++)
	{
		for (j = 0; j < RESOLUTIONJ - 1; j++)
		{
			Vector4f v1Pos = Vector4f(outp[i][j].x, outp[i][j].y, outp[i][j].z, 1.0f);
			Vector4f v3Pos = Vector4f(outp[i + 1][j].x, outp[i + 1][j].y, outp[i + 1][j].z, 1.0f);
			Vector4f v2Pos = Vector4f(outp[i][j + 1].x, outp[i][j + 1].y, outp[i][j + 1].z, 1.0f);

			Vector4f vector1 = v3Pos.sub(v2Pos);
			Vector4f vector2 = v3Pos.sub(v1Pos);
			Vector4f vNormal = vector1.cross(vector2).normalized();

			Vertex v1(	v1Pos,
						Vector4f(0.0f, 0.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v2(	v2Pos,
						Vector4f(0.0f, 1.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v3(	v3Pos,
						Vector4f(1.0f, 1.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());
			vVertices.push_back(v1);
			vVertices.push_back(v2);
			vVertices.push_back(v3);

			Vector4f v4Pos = Vector4f(outp[i + 1][j].x, outp[i + 1][j].y, outp[i + 1][j].z, 1.0f);
			Vector4f v6Pos = Vector4f(outp[i + 1][j + 1].x, outp[i + 1][j + 1].y, outp[i + 1][j + 1].z, 1.0f);
			Vector4f v5Pos = Vector4f(outp[i][j + 1].x, outp[i][j + 1].y, outp[i][j + 1].z, 1.0f);

			vector1 = v6Pos.sub(v5Pos);
			vector2 = v6Pos.sub(v4Pos);
			vNormal = vector1.cross(vector2).normalized();

			Vertex v4(	v4Pos,
						Vector4f(0.0f, 0.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v5(	v5Pos,
						Vector4f(1.0f, 0.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v6(	v6Pos,
						Vector4f(1.0f, 1.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());
			vVertices.push_back(v4);
			vVertices.push_back(v5);
			vVertices.push_back(v6);
		}
	}

	Mesh* pBezierSurface = new Mesh(vVertices);
	return pBezierSurface;
}
//*/

///*
#define CP_X 5
#define CP_Y 4
#define RESOLUTIONY 10*CP_Y
#define RESOLUTIONX 10*CP_X
inline Mesh* BezierSurface3DUtils::GenerateBezierSurface()
{
	Vector4f inp[CP_Y + 1][CP_X + 1];
	Vector4f outp[RESOLUTIONY][RESOLUTIONX];

	int y, x, ky, kx;
	double muy, mux, bx, by;

	// Create a random surface
	// Creating Control Points at pos (x, y, randomZ)
	// randomZ gives a curved effect on the surface.
	srand(1111);
	for (y = 0; y <= CP_Y; y++) 
	{
		for (x = 0; x <= CP_X; x++) 
		{
			inp[y][x].x = x;
			inp[y][x].y = (rand() % 100) / 50.0 - 1;
			inp[y][x].z = y;
		}
	}

	// 
	// x, y position on the surface act as 't' (0 <= t <= 1) in both 'u'(muy) & 'v'(mux) direction.
	// The influence of each control point (kx or ky) out of the total control points(CP_X, CP_Y) at a particular 't' (mux & muy) is calculated in the BezierBlend()
	//		& added as a cumulative blend factor for a particular x, y position.
	//
	for (y = 0; y < RESOLUTIONY; y++) 
	{
		muy = y / (double)(RESOLUTIONY - 1);
		for (x = 0; x < RESOLUTIONX; x++) 
		{
			mux = x / (double)(RESOLUTIONX - 1);
			outp[y][x].x = 0;
			outp[y][x].y = 0;
			outp[y][x].z = 0;
			for (ky = 0; ky <= CP_Y; ky++) 
			{
				by = BezierBlend(ky, muy, CP_Y);
				for (kx = 0; kx <= CP_X; kx++) 
				{
					bx = BezierBlend(kx, mux, CP_X);
					outp[y][x].x += (inp[ky][kx].x * bx * by);
					outp[y][x].y += (inp[ky][kx].y * bx * by);
					outp[y][x].z += (inp[ky][kx].z * bx * by);
				}
			}
		}
	}

	std::vector<Vertex> vVertices;
	for (y = 0; y < RESOLUTIONY - 1; y++)
	{
		for (x = 0; x < RESOLUTIONX - 1; x++)
		{																			//			X					  X + 1
			Vector4f v1Pos = Vector4f(	outp[y]		[x].x,							//		  v1Pos					v2Pos
										outp[y]		[x].y, 							//	  Y		@*********************@
										outp[y]		[x].z, 1.0f);					//			*				   *
			Vector4f v2Pos = Vector4f(	outp[y]		[x + 1].x, 						//			*				*
										outp[y]		[x + 1].y,						//			*			 *
										outp[y]		[x + 1].z, 1.0f);				//			*		  *
			Vector4f v3Pos = Vector4f(	outp[y + 1]	[x].x, 							//			*	   *
										outp[y + 1]	[x].y, 							//			*   *
										outp[y + 1]	[x].z, 1.0f);					//	Y + 1	@*
																					//		  v3Pos
			Vector4f vector1 = v2Pos.sub(v3Pos);
			Vector4f vector2 = v2Pos.sub(v1Pos);
			Vector4f vNormal = vector2.cross(vector1).normalized();

			Vertex v1(	v1Pos,
						Vector4f(0.0f, 0.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v2(	v2Pos,
						Vector4f(0.0f, 1.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v3(	v3Pos,
						Vector4f(1.0f, 1.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());
			vVertices.push_back(v1);
			vVertices.push_back(v2);
			vVertices.push_back(v3);
																					//			X					X + 1
			Vector4f v4Pos = Vector4f(	outp[y + 1]	[x].x,							//		  						v5Pos
										outp[y + 1]	[x].y, 							//	  Y							  @
										outp[y + 1]	[x].z, 1.0f);					//							   *  *
			Vector4f v5Pos = Vector4f(	outp[y]		[x + 1].x, 						//							*	  *
										outp[y]		[x + 1].y, 						//						 *		  *
										outp[y]		[x + 1].z, 1.0f); 				//					  *			  *
			Vector4f v6Pos = Vector4f(	outp[y + 1]	[x + 1].x, 						//				   *			  *
										outp[y + 1]	[x + 1].y, 						//			    *				  *
										outp[y + 1]	[x + 1].z, 1.0f);				//	Y + 1	 @*********************@
																					//		  v4Pos					 v6Pos
			vector1 = v5Pos.sub(v4Pos);
			vector2 = v5Pos.sub(v6Pos);
			vNormal = vector1.cross(vector2).normalized();

			Vertex v4(	v4Pos,
						Vector4f(0.0f, 0.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v5(	v5Pos,
						Vector4f(1.0f, 0.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v6(	v6Pos,
						Vector4f(1.0f, 1.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());
			vVertices.push_back(v4);
			vVertices.push_back(v5);
			vVertices.push_back(v6);
		}
	}

	Mesh* pBezierSurface = new Mesh(vVertices);
	return pBezierSurface;
}
//*/

/*
Mesh* BezierSurface3DUtils::GenerateBezierSurfaceMy()
{
	int32_t _CP_Y = 5;
	int32_t _CP_X = 4;
	int32_t _RESOLUTIONY = 10 * _CP_Y;
	int32_t _RESOLUTIONX = 10 * _CP_X;

	Vector4f* inp = new Vector4f[(_CP_Y + 1 ) * (_CP_X + 1)];
	Vector4f* outp = new Vector4f[_RESOLUTIONY * _RESOLUTIONX];

	int y, x, ky, kx;
	double muy, mux, bx, by;

	// Create a random surface
	srand(1111);
	for (y = 0; y <= _CP_Y; y++) 
	{
		for (x = 0; x <= _CP_X; x++) 
		{
			inp[y + x * _CP_Y].x = x;
			inp[y + x * _CP_Y].y = y;
			inp[y + x * _CP_Y].z = (rand() % 100) / 50.0 - 1;
		}
	}

	for (y = 0; y < _RESOLUTIONY; y++) 
	{
		muy = y / (double)(_RESOLUTIONY - 1);
		for (x = 0; x < _RESOLUTIONX; x++) 
		{
			mux = x / (double)(_RESOLUTIONX - 1);
			outp[y + x * _RESOLUTIONY].x = 0;
			outp[y + x * _RESOLUTIONY].y = 0;
			outp[y + x * _RESOLUTIONY].z = 0;
			for (ky = 0; ky <= _CP_Y; ky++) 
			{
				by = BezierBlend(ky, muy, _CP_Y);
				for (kx = 0; kx <= _CP_X; kx++) 
				{
					bx = BezierBlend(kx, mux, _CP_X);
					outp[y + x * _RESOLUTIONY].x += (inp[ky + kx * _CP_Y].x * bx * by);
					outp[y + x * _RESOLUTIONY].y += (inp[ky + kx * _CP_Y].y * bx * by);
					outp[y + x * _RESOLUTIONY].z += (inp[ky + kx * _CP_Y].z * bx * by);
				}
			}
		}
	}

	std::vector<Vertex> vVertices;
	for (y = 0; y < _RESOLUTIONY - 1; y++)
	{
		for (x = 0; x < _RESOLUTIONX - 1; x++)												//			X					  X + 1
		{																					//		  v1Pos					v2Pos
			Vector4f v1Pos = Vector4f(	outp[y + x * _RESOLUTIONY].x,						//	  Y		@*********************@
										outp[y + x * _RESOLUTIONY].y,						//			*				   *
										outp[y + x * _RESOLUTIONY].z, 1.0f);				//			*				*
			Vector4f v3Pos = Vector4f(	outp[(y + 1) + x * _RESOLUTIONY].x,					//			*			 *
										outp[(y + 1) + x * _RESOLUTIONY].y,					//			*		  *
										outp[(y + 1) + x * _RESOLUTIONY].z, 1.0f);			//			*	   *
			Vector4f v2Pos = Vector4f(	outp[y + (x + 1) * _RESOLUTIONY].x,					//			*   *
										outp[y + (x + 1) * _RESOLUTIONY].y,					//	Y + 1	@*
										outp[y + (x + 1) * _RESOLUTIONY].z, 1.0f);			//		  v3Pos

			Vector4f vector1 = v3Pos.sub(v2Pos);
			Vector4f vector2 = v3Pos.sub(v1Pos);
			Vector4f vNormal = vector1.cross(vector2).normalized();

			Vertex v1(	v1Pos,
						Vector4f(0.0f, 0.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v2(	v2Pos,
						Vector4f(0.0f, 1.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v3(	v3Pos,
						Vector4f(1.0f, 1.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());
			vVertices.push_back(v1);
			vVertices.push_back(v2);
			vVertices.push_back(v3);
																								//			X					X + 1
			Vector4f v4Pos = Vector4f(	outp[(y + 1) + x * _RESOLUTIONY].x,						//		  						v5Pos
										outp[(y + 1) + x * _RESOLUTIONY].y, 					//	  Y							  @
										outp[(y + 1) + x * _RESOLUTIONY].z, 1.0f);				//							   *  *
			Vector4f v6Pos = Vector4f(	outp[(y + 1) + (x + 1) * _RESOLUTIONY].x, 				//							*	  *
										outp[(y + 1) + (x + 1) * _RESOLUTIONY].y, 				//						 *		  *
										outp[(y + 1) + (x + 1) * _RESOLUTIONY].z, 1.0f);		//					  *			  *
			Vector4f v5Pos = Vector4f(	outp[y + (x + 1) * _RESOLUTIONY].x, 					//				   *			  *
										outp[y + (x + 1) * _RESOLUTIONY].y, 					//			    *				  *
										outp[y + (x + 1) * _RESOLUTIONY].z, 1.0f);				//	Y + 1	 @*********************@
																								//		  v4Pos					 v6Pos
			vector1 = v6Pos.sub(v5Pos);
			vector2 = v6Pos.sub(v4Pos);
			vNormal = vector1.cross(vector2).normalized();

			Vertex v4(	v4Pos,
						Vector4f(0.0f, 0.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v5(	v5Pos,
						Vector4f(1.0f, 0.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v6(	v6Pos,
						Vector4f(1.0f, 1.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());
			vVertices.push_back(v4);
			vVertices.push_back(v5);
			vVertices.push_back(v6);
		}
	}

	Mesh* pBezierSurface = new Mesh(vVertices);
	return pBezierSurface;
}
*/
// Courtesy : http://paulbourke.net/geometry/bezier/
//
//				N!		   k		 N-k
//	blend =	----------- * t   * (1-t)
//			 k! (N-k)!
//
// where :=
//		N = Number of control points
//		k = kth control point
//		t = 0 <= t <= 1
//
// Calculate the influence of 'k'th control point at time 't', where total number of control points is 'n'.
//
inline double BezierSurface3DUtils::BezierBlend(int k, float t, int n)
{
	int N,kn,nkn;
	double blend=1;

	N = n;
	kn = k;
	nkn = n - k;

	while (N >= 1) 
	{
		blend *= N;							// N!
		N--;

		if (kn > 1)
		{
			blend /= (double)kn;			// __1__
			kn--;							//   k!
		}

		if (nkn > 1)
		{
			blend /= (double)nkn;			// ___1___
			nkn--;							//  (N-k)!
		}
	}

	if (k > 0)
	{										//  K
		blend *= pow(t,(double)k);			// t
	}

	if (n-k > 0)
	{										//		 N-k
		blend *= pow(1-t,(double)(n-k));	// (1 - u)
	}

	return(blend);
}
