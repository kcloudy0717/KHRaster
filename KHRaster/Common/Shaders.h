#pragma once
#include "MathFunction.h"

// shader variables
float scaleX = 1.0f;
float scaleY = 1.0f;
float scaleZ = 1.0f;
float translateX = 0.0f;
float translateY = 0.0f;
float translateZ = 0.0f;
float angle = 0.0f;

enum FILTER
{
	None,
	Bilinear
};

struct ConstantBuffer
{
	Matrix4x4 World = Matrix_Identity();
	Texture2D<UINT> *pTexture = nullptr;
	FILTER Filter = None;
	UINT SelectedMip = 0;

	// Light stuff
	Vertex light = {0};
	Vertex pointLight = {0};
	float lightRadius = 1.0f;
} ConstantBuffer;

struct Camera
{
	Matrix4x4 View()
	{
		return Matrix_Inverse(World);
	}

	Matrix4x4 Projection()
	{
		float yScale = 1.0f / tanf(Degrees_To_Radians(FOV) * 0.5f);
		float xScale = yScale * AspectRatio;

		return {
			xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f, (Far / (Far - Near)), 1.0f,
			0.0f, 0.0f, (-(Far * Near) / (Far - Near)), 0.0f};
	}

	// all matrices
	Matrix4x4 World = Matrix_Matrix_Multiply(Matrix_Create_Translation(0.0f, 0.0f, -1.0f), Matrix_Create_Rotation_X(-18.0f));

	float Near = 0.1f;
	float Far = 10.0f;
	float FOV = 90.0f;
	float AspectRatio = 1.0f;
} Camera;

inline unsigned int ColorBlend(Vertex Src, Vertex Dst, float ratio)
{
	unsigned int startAlpha = (Src.color & 0xff000000) >> 24;
	unsigned int startRed = (Src.color & 0x00ff0000) >> 16;
	unsigned int startGreen = (Src.color & 0x0000ff00) >> 8;
	unsigned int startBlue = (Src.color & 0x000000ff);

	unsigned int endAlpha = (Dst.color & 0xff000000) >> 24;
	unsigned int endRed = (Dst.color & 0x00ff0000) >> 16;
	unsigned int endGreen = (Dst.color & 0x0000ff00) >> 8;
	unsigned int endBlue = (Dst.color & 0x000000ff);

	float resultAlpha = LinearInterpolation(static_cast<float>(startAlpha), static_cast<float>(endAlpha), ratio);
	float resultRed = LinearInterpolation(static_cast<float>(startRed), static_cast<float>(endRed), ratio);
	float resultGreen = LinearInterpolation(static_cast<float>(startGreen), static_cast<float>(endGreen), ratio);
	float resultBlue = LinearInterpolation(static_cast<float>(startBlue), static_cast<float>(endBlue), ratio);

	return static_cast<unsigned int>(resultAlpha) << 24 | static_cast<unsigned int>(resultRed) << 16 | static_cast<unsigned int>(resultGreen) << 8 | static_cast<unsigned int>(resultBlue);
}

inline unsigned int ColorBlendBGRA(unsigned int a, unsigned int b, float ratio)
{
	unsigned int startBlue = (a & 0xff000000) >> 24;
	unsigned int startGreen = (a & 0x00ff0000) >> 16;
	unsigned int startRed = (a & 0x0000ff00) >> 8;
	unsigned int startAlpha = (a & 0x000000ff);

	unsigned int endBlue = (b & 0xff000000) >> 24;
	unsigned int endGreen = (b & 0x00ff0000) >> 16;
	unsigned int endRed = (b & 0x0000ff00) >> 8;
	unsigned int endAlpha = (b & 0x000000ff);

	unsigned int resultBlue = static_cast<unsigned int>(LinearInterpolation(static_cast<float>(startBlue), static_cast<float>(endBlue), ratio));
	unsigned int resultGreen = static_cast<unsigned int>(LinearInterpolation(static_cast<float>(startGreen), static_cast<float>(endGreen), ratio));
	unsigned int resultRed = static_cast<unsigned int>(LinearInterpolation(static_cast<float>(startRed), static_cast<float>(endRed), ratio));
	unsigned int resultAlpha = static_cast<unsigned int>(LinearInterpolation(static_cast<float>(startAlpha), static_cast<float>(endAlpha), ratio));

	return (resultBlue << 24) | (resultGreen << 16) | (resultRed << 8) | resultAlpha;
}

inline unsigned int ColorBlend(const Vertex &V0, const Vertex &V1, const Vertex &V2, const Vec3 &barycentrics)
{
	unsigned int aAlpha = (V0.color & 0xff000000) >> 24;
	unsigned int aRed = (V0.color & 0x00ff0000) >> 16;
	unsigned int aGreen = (V0.color & 0x0000ff00) >> 8;
	unsigned int aBlue = (V0.color & 0x000000ff);

	unsigned int bAlpha = (V1.color & 0xff000000) >> 24;
	unsigned int bRed = (V1.color & 0x00ff0000) >> 16;
	unsigned int bGreen = (V1.color & 0x0000ff00) >> 8;
	unsigned int bBlue = (V1.color & 0x000000ff);

	unsigned int cAlpha = (V2.color & 0xff000000) >> 24;
	unsigned int cRed = (V2.color & 0x00ff0000) >> 16;
	unsigned int cGreen = (V2.color & 0x0000ff00) >> 8;
	unsigned int cBlue = (V2.color & 0x000000ff);

	float resultAlpha = BarycentricInterpolation(static_cast<float>(aAlpha), static_cast<float>(bAlpha), static_cast<float>(cAlpha), barycentrics);
	float resultRed = BarycentricInterpolation(static_cast<float>(aRed), static_cast<float>(bRed), static_cast<float>(cRed), barycentrics);
	float resultGreen = BarycentricInterpolation(static_cast<float>(aGreen), static_cast<float>(bGreen), static_cast<float>(cGreen), barycentrics);
	float resultBlue = BarycentricInterpolation(static_cast<float>(aBlue), static_cast<float>(bBlue), static_cast<float>(cBlue), barycentrics);

	return static_cast<unsigned int>(resultAlpha) << 24 | static_cast<unsigned int>(resultRed) << 16 | static_cast<unsigned int>(resultGreen) << 8 | static_cast<unsigned int>(resultBlue);
}

inline unsigned int ColorModulate(unsigned a, unsigned b)
{
	float startBlue = ((a & 0xff000000) >> 24) / 255.0f;
	float startGreen = ((a & 0x00ff0000) >> 16) / 255.0f;
	float startRed = ((a & 0x0000ff00) >> 8) / 255.0f;
	float startAlpha = (a & 0x000000ff) / 255.0f;

	float endBlue = ((b & 0xff000000) >> 24) / 255.0f;
	float endGreen = ((b & 0x00ff0000) >> 16) / 255.0f;
	float endRed = ((b & 0x0000ff00) >> 8) / 255.0f;
	float endAlpha = (b & 0x000000ff) / 255.0f;

	unsigned int resultBlue = static_cast<unsigned int>(startBlue * endBlue * 255.0f);
	unsigned int resultGreen = static_cast<unsigned int>(startGreen * endGreen * 255.0f);
	unsigned int resultRed = static_cast<unsigned int>(startRed * endRed * 255.0f);
	unsigned int resultAlpha = static_cast<unsigned int>(startAlpha * endAlpha * 255.0f);

	return (resultBlue << 24) | (resultGreen << 16) | (resultRed << 8) | resultAlpha;
}

inline unsigned int ColorCombine(unsigned a, unsigned b)
{
	float startBlue = ((a & 0xff000000) >> 24) / 255.0f;
	float startGreen = ((a & 0x00ff0000) >> 16) / 255.0f;
	float startRed = ((a & 0x0000ff00) >> 8) / 255.0f;
	float startAlpha = (a & 0x000000ff) / 255.0f;

	float endBlue = ((b & 0xff000000) >> 24) / 255.0f;
	float endGreen = ((b & 0x00ff0000) >> 16) / 255.0f;
	float endRed = ((b & 0x0000ff00) >> 8) / 255.0f;
	float endAlpha = (b & 0x000000ff) / 255.0f;

	unsigned int resultBlue = static_cast<unsigned int>(Saturate(startBlue + endBlue) * 255.0f);
	unsigned int resultGreen = static_cast<unsigned int>(Saturate(startGreen + endGreen) * 255.0f);
	unsigned int resultRed = static_cast<unsigned int>(Saturate(startRed + endRed) * 255.0f);
	unsigned int resultAlpha = static_cast<unsigned int>(Saturate(startAlpha + endAlpha) * 255.0f);

	return (resultBlue << 24) | (resultGreen << 16) | (resultRed << 8) | resultAlpha;
}

void VertexShader(Vertex &V)
{
	// World space
	V.position = Vector_Matrix_Multiply(V.position, ConstantBuffer.World);
	V.normal = Vector_Matrix_Multiply(V.normal, ConstantBuffer.World);

	// View space
	V.position = Vector_Matrix_Multiply(V.position, Camera.View());

	// Projection space
	V.position = Vector_Matrix_Multiply(V.position, Camera.Projection());
}

void PixelShader(UINT &color, Vertex &V)
{
	auto pTexture = ConstantBuffer.pTexture;
	if (pTexture && pTexture->MipLevels > 0)
	{
		const auto &textureLevelOffsets = pTexture->MiplevelOffsets;

		UINT SelectedMip = (UINT)ConstantBuffer.SelectedMip;

		// safe check
		if (SelectedMip >= pTexture->MipLevels)
			SelectedMip = pTexture->MipLevels - 1;
		if (SelectedMip < 0)
			SelectedMip = 0;

		// texture mip map based on argument level
		unsigned int newTextureWidth = pTexture->Width >> SelectedMip;
		unsigned int newTextureHeight = pTexture->Height >> SelectedMip;
		float x = V.uv.x * newTextureWidth;
		float y = V.uv.y * newTextureHeight;
		int index = static_cast<int>(y) * newTextureWidth + static_cast<int>(x);
		// convert x and y to remainders, remainders are what we use to calculate the ratio of the pixels we need to lerp
		x = x - floor(x);
		y = y - floor(y);
		// adjecent pixels in the texture raster, add offset to according to mip level

		unsigned int topLeft = pTexture->At(index + textureLevelOffsets[SelectedMip]);
		unsigned int topRight = pTexture->At(index + 1 + textureLevelOffsets[SelectedMip]);
		unsigned int bottomLeft = pTexture->At(index + newTextureWidth + textureLevelOffsets[SelectedMip]);
		unsigned int bottomRight = pTexture->At(index + newTextureWidth + 1 + textureLevelOffsets[SelectedMip]);

		unsigned int h1 = ColorBlendBGRA(topLeft, topRight, x);
		unsigned int h2 = ColorBlendBGRA(bottomLeft, bottomRight, x);
		color = ColorBlendBGRA(h1, h2, y);
	}

	float NoL = Saturate(Vector_Dot(ConstantBuffer.light.normal, V.normal));
	unsigned int color0 = ColorBlendBGRA(0x000000ff, ConstantBuffer.light.color, NoL);

	Vec4 pointLightDirection = Vector_Normalize(Vector_Sub(ConstantBuffer.pointLight.position, V.position));
	float pointLightRatio = Saturate(Vector_Dot(pointLightDirection, V.normal));
	float attenuation = 1.0f - Saturate(Vector_Length(Vector_Sub(ConstantBuffer.pointLight.position, V.position)) / ConstantBuffer.lightRadius);
	pointLightRatio = attenuation * attenuation * pointLightRatio;
	unsigned int color1 = ColorBlendBGRA(0x000000ff, ConstantBuffer.pointLight.color, pointLightRatio);

	color = ColorModulate(color, ColorCombine(color0, color1));

	color = ((color & 0xff000000) >> 24 | ((color & 0x00ff0000) >> 8) | ((color & 0x0000ff00) << 8) | ((color & 0x000000ff) << 24));
}

void PS_White(UINT &color, Vertex &v)
{
	color = WHITE;
}

void PS_Red(UINT &color, Vertex &v)
{
	color = RED;
}

void PS_Green(UINT &color, Vertex &v)
{
	color = GREEN;
}

void PS_Blue(UINT &color, Vertex &v)
{
	color = BLUE;
}

void PS_Purple(UINT &color, Vertex &v)
{
	color = PURPLE;
}

void PS_Texture(UINT &color, Vertex &v)
{
	int x = static_cast<int>(v.uv.x * ConstantBuffer.pTexture->Width);
	int y = static_cast<int>(v.uv.y * ConstantBuffer.pTexture->Height);
	color = ConstantBuffer.pTexture->GetPixel(x, y);
	color = ((color & 0xff000000) >> 24 | ((color & 0x00ff0000) >> 8) | ((color & 0x0000ff00) << 8) | ((color & 0x000000ff) << 24));
}

#pragma region Helper Functions
void ResetScale()
{
	scaleX = 1.0f;
	scaleY = 1.0f;
	scaleZ = 1.0f;
}
void ResetRotation()
{
	angle = 0.0f;
}
void ResetTranslation()
{
	translateX = 0.0f;
	translateY = 0.0f;
	translateZ = 0.0f;
}
#pragma endregion

void LerpAllAttributes(Vertex &Src, Vertex &Dst, float ratio)
{
	Src.position.x = LinearInterpolation(Src.position.x, Dst.position.x, ratio);
	Src.position.y = LinearInterpolation(Src.position.y, Dst.position.y, ratio);
	Src.position.z = LinearInterpolation(Src.position.z, Dst.position.z, ratio);
	Src.position.w = LinearInterpolation(Src.position.w, Dst.position.w, ratio);
	Src.color = ColorBlend(Src, Dst, ratio);
	Src.uv.x = LinearInterpolation(Src.uv.x, Dst.uv.x, ratio);
	Src.uv.y = LinearInterpolation(Src.uv.y, Dst.uv.y, ratio);
}