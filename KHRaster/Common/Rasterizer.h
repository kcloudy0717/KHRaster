#pragma once
#include "MathFunction.h"

struct Rasterizer
{
	using PFN_VS = void (*)(Vertex &);
	using PFN_PS = void (*)(UINT &, Vertex &);

	Rasterizer(RenderTarget *pRenderTarget)
		: pRenderTarget(pRenderTarget)
	{
	}

	void DrawPoint(Vertex V)
	{
		if (VS)
		{
			VS(V);
		}

		if (V.position.z < 0.0f)
		{
			return;
		}

		PerspectiveDivide(V.position);

		NDCToRaster(V.position, pRenderTarget->Width, pRenderTarget->Height);
		V.color = WHITE;
		if (PS)
		{
			PS(V.color, V);
		}

		pRenderTarget->SetPixel(static_cast<int>(V.position.x), static_cast<int>(V.position.y), V.color, V.position.z);
	}

	void DrawParametricLine(Vertex Src, Vertex Dst)
	{
		if (VS)
		{
			VS(Src);
			VS(Dst);
		}
		// 3D line clipping in the near plane
		if (NearPlaneLineClipping(Src, Dst) == 0)
		{
			return;
		}

		// perspective divide
		PerspectiveDivide(Src.position);
		PerspectiveDivide(Dst.position);

		// conversion from NDC coord to raster
		NDCToRaster(Src.position, pRenderTarget->Width, pRenderTarget->Height);
		NDCToRaster(Dst.position, pRenderTarget->Width, pRenderTarget->Height);

		// x = (B - A) * R + A
		float dx = fabs(Dst.position.x - Src.position.x);
		float dy = fabs(Dst.position.y - Src.position.y);
		int tp = static_cast<int>(fmaxf(dx, dy));

		// loop
		for (int i = 0; i < tp; ++i)
		{
			float r = i / static_cast<float>(tp);
			int px = static_cast<int>(floor(((Dst.position.x - Src.position.x) * r + Src.position.x) + 0.5f));
			int py = static_cast<int>(floor(((Dst.position.y - Src.position.y) * r + Src.position.y) + 0.5f));
			float depth = LinearInterpolation(Src.position.w, Dst.position.w, r);
			unsigned int color = ColorBlend(Src, Dst, r);

			pRenderTarget->SetPixel(px, py, color, depth);
		}
	}

	void FillTriangleBetterBrute(Vertex V0, Vertex V1, Vertex V2)
	{
		if (VS)
		{
			VS(V0);
			VS(V1);
			VS(V2);
		}

		// perspective correct interpolation
		float rZA = 1.0f / V0.position.w;
		float rZB = 1.0f / V1.position.w;
		float rZC = 1.0f / V2.position.w;
		V0.uv = {V0.uv.x / V0.position.w, V0.uv.y / V0.position.w};
		V1.uv = {V1.uv.x / V1.position.w, V1.uv.y / V1.position.w};
		V2.uv = {V2.uv.x / V2.position.w, V2.uv.y / V2.position.w};
		// perspective divide
		PerspectiveDivide(V0.position);
		PerspectiveDivide(V1.position);
		PerspectiveDivide(V2.position);

		// conversion from NDC coord to raster
		NDCToRaster(V0.position, pRenderTarget->Width, pRenderTarget->Height);
		NDCToRaster(V1.position, pRenderTarget->Width, pRenderTarget->Height);
		NDCToRaster(V2.position, pRenderTarget->Width, pRenderTarget->Height);

		float startX = Min(Min(V0.position.x, V1.position.x), V2.position.x);
		float startY = Min(Min(V0.position.y, V1.position.y), V2.position.y);
		float endX = Max(Max(V0.position.x, V1.position.x), V2.position.x);
		float endY = Max(Max(V0.position.y, V1.position.y), V2.position.y);
		// Fill triangle using barycentric coordinates
		for (int y = static_cast<int>(startY); y <= endY; y++)
		{
			for (int x = static_cast<int>(startX); x <= endX; x++)
			{
				Vec3 barycentrics = BarycentricCoordinates(V0.position, V1.position, V2.position, {static_cast<float>(x), static_cast<float>(y), 0, 0});
				if (barycentrics.x >= 0.0f && barycentrics.x <= 1.0f && barycentrics.y >= 0.0f && barycentrics.y <= 1.0f && barycentrics.z >= 0.0f && barycentrics.z <= 1.0f)
				{
					float finalRZ = BarycentricInterpolation(rZA, rZB, rZC, barycentrics);
					Vertex v = BarycentricInterpolation(V0, V1, V2, barycentrics);
					v.uv.x /= finalRZ;
					v.uv.y /= finalRZ;

					unsigned int color = ColorBlend(V0, V1, V2, barycentrics);
					float depth = v.position.z;

					float mipLevel = (depth - Camera.Near) / (Camera.Far - Camera.Near) * ConstantBuffer.pTexture->MipLevels;
					ConstantBuffer.SelectedMip = mipLevel;

					if (PS)
					{
						PS(color, v);
					}

					pRenderTarget->SetPixel(x, y, color, depth);
				}
			}
		}
	}

	///////////////////////////////////////////////////
	//	0 : Line is hidden behind near plane
	//	1 : One vertex is clipped
	//	2 : Both vertex is present in the view frustrum
	///////////////////////////////////////////////////
	int NearPlaneLineClipping(Vertex &Src, Vertex &Dst)
	{
		// both vertex is hidden
		if (Src.position.z < 0.0f &&
			Dst.position.z < 0.0f)
			return 0;
		// both vertex is present
		if (Src.position.z >= 0.0f &&
			Dst.position.z >= 0.0f)
			return 2;
		float zBehindNearP = 0.0f;
		float deltaZ = fabs(Dst.position.z - Src.position.z);
		float zRatio = 1.0f;
		// if vertex a.z is behind the near plane
		if (Src.position.z < 0.0f)
		{
			zBehindNearP = fabs(Src.position.z);
			zRatio = zBehindNearP / deltaZ;
			LerpAllAttributes(Src, Dst, zRatio);
			return 1;
		}
		// if vertex b.z is behind near plane
		else if (Dst.position.z < 0.0f)
		{
			zBehindNearP = fabs(Dst.position.z);
			zRatio = zBehindNearP / deltaZ;
			LerpAllAttributes(Dst, Src, zRatio);
			return 1;
		}
		return -1;
	}

	RenderTarget *pRenderTarget = nullptr;
	PFN_VS VS = nullptr;
	PFN_PS PS = nullptr;
};