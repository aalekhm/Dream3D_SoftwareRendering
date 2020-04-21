#pragma once
#include <stdint.h>
#include <windows.h>

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#define	COLOR_BLACK			0
#define	COLOR_WHITE			1
#define	COLOR_RED			2
#define	COLOR_GREEN			3
#define	COLOR_BLUE			4
#define	COLOR_GREY			5
#define	COLOR_PINK			6
#define	COLOR_CYAN			7
#define	COLOR_YELLOW		8

#define u8 unsigned char
#define s8 char
#define u16 unsigned short int
#define s16 short int
#define u32 unsigned int
#define s32 int
#define s64 long long
#define u64 unsigned long long

#define REAL_EPSILON        1.192092896e-07F        /* FLT_EPSILON */
typedef float REAL;

class SizeF
{
public:
    SizeF()
    {
        Width = Height = 0.0f;
    }

    SizeF(IN const SizeF& size)
    {
        Width = size.Width;
        Height = size.Height;
    }

    SizeF(IN REAL width,
          IN REAL height)
    {
        Width = width;
        Height = height;
    }

    SizeF operator+(IN const SizeF& sz) const
    {
        return SizeF(Width + sz.Width,
                     Height + sz.Height);
    }

    SizeF operator-(IN const SizeF& sz) const
    {
        return SizeF(Width - sz.Width,
                     Height - sz.Height);
    }

public:

    REAL Width;
    REAL Height;
};

class Size
{
public:
    Size()
    {
        Width = Height = 0;
    }

    Size(IN const Size& size)
    {
        Width = size.Width;
        Height = size.Height;
    }

    Size(IN INT width,
         IN INT height)
    {
        Width = width;
        Height = height;
    }

    Size operator+(IN const Size& sz) const
    {
        return Size(Width + sz.Width,
                    Height + sz.Height);
    }

    Size operator-(IN const Size& sz) const
    {
        return Size(Width - sz.Width,
                    Height - sz.Height);
    }

    BOOL Equals(IN const Size& sz) const
    {
        return (Width == sz.Width) && (Height == sz.Height);
    }

    BOOL Empty() const
    {
        return (Width == 0 && Height == 0);
    }

public:

    INT Width;
    INT Height;
};

class PointF
{
public:
   PointF()
   {
       X = Y = 0.0f;
   }

   PointF(IN const PointF &point)
   {
       X = point.X;
       Y = point.Y;
   }

   PointF(IN const SizeF &size)
   {
       X = size.Width;
       Y = size.Height;
   }

   PointF(IN REAL x,
          IN REAL y)
   {
       X = x;
       Y = y;
   }

   PointF operator+(IN const PointF& point) const
   {
       return PointF(X + point.X,
                     Y + point.Y);
   }

   PointF operator-(IN const PointF& point) const
   {
       return PointF(X - point.X,
                     Y - point.Y);
   }
public:

    REAL X;
    REAL Y;
};

class Point
{
public:
   Point()
   {
       X = Y = 0;
   }

   Point(IN const Point &point)
   {
       X = point.X;
       Y = point.Y;
   }

   Point(IN const Size &size)
   {
       X = size.Width;
       Y = size.Height;
   }

   Point(IN INT x,
         IN INT y)
   {
       X = x;
       Y = y;
   }

   Point operator+(IN const Point& point) const
   {
       return Point(X + point.X,
                    Y + point.Y);
   }

   Point operator-(IN const Point& point) const
   {
       return Point(X - point.X,
                    Y - point.Y);
   }

   BOOL Equals(IN const Point& point)
   {
       return (X == point.X) && (Y == point.Y);
   }

public:

    INT X;
    INT Y;
};

class RectF
{
public:

    RectF()
    {
        X = Y = Width = Height = 0.0f;
    }

    RectF(IN REAL x,
          IN REAL y,
          IN REAL width,
          IN REAL height)
    {
        X = x;
        Y = y;
        Width = width;
        Height = height;
    }

    RectF(IN const PointF& location,
          IN const SizeF& size)
    {
        X = location.X;
        Y = location.Y;
        Width = size.Width;
        Height = size.Height;
    }

    RectF* Clone() const
    {
        return new RectF(X, Y, Width, Height);
    }

    void GetSize(OUT SizeF* size) const
    {
        size->Width = Width;
        size->Height = Height;
    }

    void GetBounds(OUT RectF* rect) const
    {
        rect->X = X;
        rect->Y = Y;
        rect->Width = Width;
        rect->Height = Height;
    }

    REAL GetLeft() const
    {
        return X;
    }

    REAL GetTop() const
    {
        return Y;
    }

    REAL GetRight() const
    {
        return X+Width;
    }

    REAL GetBottom() const
    {
        return Y+Height;
    }

    BOOL Intersect(IN const RectF& rect)
    {
        return Intersect(*this, *this, rect);
    }

    static BOOL Intersect(OUT RectF& c,
                          IN const RectF& a,
                          IN const RectF& b)
    {
        REAL right = min(a.GetRight(), b.GetRight());
        REAL bottom = min(a.GetBottom(), b.GetBottom());
        REAL left = max(a.GetLeft(), b.GetLeft());
        REAL top = max(a.GetTop(), b.GetTop());

        c.X = left;
        c.Y = top;
        c.Width = right - left;
        c.Height = bottom - top;
        return !c.IsEmptyArea();
    }

    BOOL IsEmptyArea() const
    {
        return (Width <= REAL_EPSILON) || (Height <= REAL_EPSILON);
    }

	void Offset(IN const PointF& point)
    {
        Offset(point.X, point.Y);
    }

    void Offset(IN REAL dx,
                IN REAL dy)
    {
        X += dx;
        Y += dy;
    }

public:

    REAL X;
    REAL Y;
    REAL Width;
    REAL Height;
};

class Rect
{
public:

    Rect()
    {
        X = Y = Width = Height = 0;
    }

    Rect(IN INT x,
         IN INT y,
         IN INT width,
         IN INT height)
    {
        X = x;
        Y = y;
        Width = width;
        Height = height;
    }

    Rect(IN const Point& location,
         IN const Size& size)
    {
        X = location.X;
        Y = location.Y;
        Width = size.Width;
        Height = size.Height;
    }

    Rect* Clone() const
    {
        return new Rect(X, Y, Width, Height);
    }

    VOID GetLocation(OUT Point* point) const
    {
        point->X = X;
        point->Y = Y;
    }

    VOID GetSize(OUT Size* size) const
    {
        size->Width = Width;
        size->Height = Height;
    }

    VOID GetBounds(OUT Rect* rect) const
    {
        rect->X = X;
        rect->Y = Y;
        rect->Width = Width;
        rect->Height = Height;
    }

    INT GetLeft() const
    {
        return X;
    }

    INT GetTop() const
    {
        return Y;
    }

    INT GetRight() const
    {
        return X+Width;
    }

    INT GetBottom() const
    {
        return Y+Height;
    }

    BOOL IsEmptyArea() const
    {
        return (Width <= 0) || (Height <= 0);
    }

    BOOL Equals(IN const Rect & rect) const
    {
        return X == rect.X &&
               Y == rect.Y &&
               Width == rect.Width &&
               Height == rect.Height;
    }

    BOOL Contains(IN INT x,
                  IN INT y) const
    {
        return x >= X && x < X+Width &&
               y >= Y && y < Y+Height;
    }

    BOOL Contains(IN const Point& pt) const
    {
        return Contains(pt.X, pt.Y);
    }

    BOOL Contains(IN Rect& rect) const
    {
        return (X <= rect.X) && (rect.GetRight() <= GetRight()) &&
               (Y <= rect.Y) && (rect.GetBottom() <= GetBottom());
    }

    VOID Inflate(IN INT dx,
                 IN INT dy)
    {
        X -= dx;
        Y -= dy;
        Width += 2*dx;
        Height += 2*dy;
    }

    VOID Inflate(IN const Point& point)
    {
        Inflate(point.X, point.Y);
    }

    BOOL Intersect(IN const Rect& rect)
    {
        return Intersect(*this, *this, rect);
    }

    static BOOL Intersect(OUT Rect& c,
                          IN const Rect& a,
                          IN const Rect& b)
    {
        INT right = min(a.GetRight(), b.GetRight());
        INT bottom = min(a.GetBottom(), b.GetBottom());
        INT left = max(a.GetLeft(), b.GetLeft());
        INT top = max(a.GetTop(), b.GetTop());

        c.X = left;
        c.Y = top;
        c.Width = right - left;
        c.Height = bottom - top;
        return !c.IsEmptyArea();
    }

    BOOL IntersectsWith(IN const Rect& rect) const
    {
        return (GetLeft() < rect.GetRight() &&
                GetTop() < rect.GetBottom() &&
                GetRight() > rect.GetLeft() &&
                GetBottom() > rect.GetTop());
    }

    static BOOL Union(OUT Rect& c,
                      IN const Rect& a,
                      IN const Rect& b)
    {
        INT right = max(a.GetRight(), b.GetRight());
        INT bottom = max(a.GetBottom(), b.GetBottom());
        INT left = min(a.GetLeft(), b.GetLeft());
        INT top = min(a.GetTop(), b.GetTop());

        c.X = left;
        c.Y = top;
        c.Width = right - left;
        c.Height = bottom - top;
        return !c.IsEmptyArea();
    }

    VOID Offset(IN const Point& point)
    {
        Offset(point.X, point.Y);
    }

    VOID Offset(IN INT dx,
                IN INT dy)
    {
        X += dx;
        Y += dy;
    }

public:

    INT X;
    INT Y;
    INT Width;
    INT Height;
};

class Color {
	public:
		float r;
		float g;
		float b;
		float a;

		Color() {
			r = g = b = 0;
			a = 255;
		}

		Color(float rRed, float gGreen, float bBlue, float aAlpha) {
			r = rRed;
			g = gGreen;
			b = bBlue;
			a = aAlpha;
		}

		float getRed() { return r; }
		float getGreen() { return g; }
		float getBlue() { return b; }
		float getAlpha() { return a; }

		void setRed(float rRed) { r = rRed; }
		void setGreen(float gGreen) { g = gGreen; }
		void setBlue(float bBlue) { b = bBlue; }
		void setAlpha(float aAlpha) { a = aAlpha; }
};

struct ColorUV
{
	uint32_t	u, v;
};

struct VertexT2F_C4UB_V3F
{
	float		tu, tv;
	uint32_t	color;
	float		vx, vy, vz;
};

