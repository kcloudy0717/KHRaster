#pragma once
#include "EngineMath.h"

struct Vertex2D
{
	Vec2 position = {};
	unsigned int color = 0;
};

struct Vertex
{
	Vec4 position = {};
	unsigned int color = 0;
	Vec2 uv = {};
	Vec4 normal = {};
};

float LinearInterpolation(float Src, float Dst, float Ratio)
{
	return (Dst - Src) * Ratio + Src;
}

Vec2 LinearInterpolation(const Vec2 &Src, const Vec2 &Dst, float Ratio)
{
	return {
		LinearInterpolation(Src.x, Dst.x, Ratio),
		LinearInterpolation(Src.y, Dst.y, Ratio)};
}

Vec3 LinearInterpolation(const Vec3 &Src, const Vec3 &Dst, float Ratio)
{
	return {
		LinearInterpolation(Src.x, Dst.x, Ratio),
		LinearInterpolation(Src.y, Dst.y, Ratio),
		LinearInterpolation(Src.z, Dst.z, Ratio)};
}

Vec4 LinearInterpolation(const Vec4 &Src, const Vec4 &Dst, float Ratio)
{
	return {
		LinearInterpolation(Src.x, Dst.x, Ratio),
		LinearInterpolation(Src.y, Dst.y, Ratio),
		LinearInterpolation(Src.z, Dst.z, Ratio),
		LinearInterpolation(Src.w, Dst.w, Ratio)};
}

Vertex LinearInterpolation(const Vertex &Src, const Vertex &Dst, float Ratio)
{
	return {
		LinearInterpolation(Src.position, Dst.position, Ratio),
		(unsigned int)LinearInterpolation(Src.color, Dst.color, Ratio),
		LinearInterpolation(Src.uv, Dst.uv, Ratio),
		LinearInterpolation(Src.normal, Dst.normal, Ratio),
	};
}

float BarycentricInterpolation(float a, float b, float c, const Vec3 &barycentrics)
{
	return a * barycentrics.x + b * barycentrics.y + c * barycentrics.z;
}

Vec2 BarycentricInterpolation(const Vec2 &a, const Vec2 &b, const Vec2 &c, const Vec3 &barycentrics)
{
	return {
		BarycentricInterpolation(a.x, b.x, c.x, barycentrics),
		BarycentricInterpolation(a.y, b.y, c.y, barycentrics)};
}

Vec3 BarycentricInterpolation(const Vec3 &a, const Vec3 &b, const Vec3 &c, const Vec3 &barycentrics)
{
	return {
		BarycentricInterpolation(a.x, b.x, c.x, barycentrics),
		BarycentricInterpolation(a.y, b.y, c.y, barycentrics),
		BarycentricInterpolation(a.z, b.z, c.z, barycentrics)};
}

Vec4 BarycentricInterpolation(const Vec4 &a, const Vec4 &b, const Vec4 &c, const Vec3 &barycentrics)
{
	return {
		BarycentricInterpolation(a.x, b.x, c.x, barycentrics),
		BarycentricInterpolation(a.y, b.y, c.y, barycentrics),
		BarycentricInterpolation(a.z, b.z, c.z, barycentrics),
		BarycentricInterpolation(a.w, b.w, c.w, barycentrics)};
}

Vertex BarycentricInterpolation(const Vertex &V0, const Vertex &V1, const Vertex &V2, const Vec3 &barycentrics)
{
	return {
		BarycentricInterpolation(V0.position, V1.position, V2.position, barycentrics),
		(unsigned int)BarycentricInterpolation(V0.color, V1.color, V2.color, barycentrics),
		BarycentricInterpolation(V0.uv, V1.uv, V2.uv, barycentrics),
		BarycentricInterpolation(V0.normal, V1.normal, V2.normal, barycentrics),
	};
}

float Slope(Vec2 start, Vec2 end)
{
	return (end.y - start.y) / (end.x - start.x);
}

float ImplicitLineEquation(Vec4 start, Vec4 end, Vec4 position)
{
	return (start.y - end.y) * position.x + (end.x - start.x) * position.y + start.x * end.y - start.y * end.x;
}

Vec3 BarycentricCoordinates(Vec4 a, Vec4 b, Vec4 c, Vec4 p)
{
	// find max distances
	float maxC = ImplicitLineEquation(a, b, c);
	float maxB = ImplicitLineEquation(a, c, b);
	float maxA = ImplicitLineEquation(c, b, a);

	float subC = ImplicitLineEquation(a, b, p);
	float subB = ImplicitLineEquation(a, c, p);
	float subA = ImplicitLineEquation(c, b, p);

	return {subA / maxA, subB / maxB, subC / maxC};
}

void NDCToRaster(Vec4 &NDC, unsigned int Width, unsigned int Height)
{
	NDC.x = (NDC.x + 1.0f) * (Width >> 1);
	NDC.y = (1.0f - NDC.y) * (Height >> 1);
}

void PerspectiveDivide(Vec4 &position)
{
	if (position.w != 0.0f)
	{
		position.x = position.x / position.w;
		position.y = position.y / position.w;
		position.z = position.z / position.w;
	}
}

float Saturate(float number)
{
	return (number < 0.0f) ? 0.0f : ((number > 1.0f) ? 1.0f : number);
}