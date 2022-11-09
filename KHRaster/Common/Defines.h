#pragma once
#include <memory>
#include <vector>
#include <Windows.h>

// colors
#define RED 0xffff0000
#define GREEN 0xff00ff00
#define BLUE 0xff0000ff
#define WHITE 0xffffffff
#define BLACK 0xff000000
#define YELLOW (RED | GREEN)
#define MAGENTA (RED | BLUE)
#define CYAN (BLUE | GREEN)
#define PURPLE 0xff8a2be2

static constexpr double ElapsedTime = 0.0;
static constexpr double Frequency = 1 / 60.0f;

inline int Flatten2DTo1D(int x, int y, int width)
{
	return y * width + x;
}

template <typename T>
struct Texture2D
{
	Texture2D(UINT Width, UINT Height, UINT MipLevels = 1, const UINT *pMipLevelOffsets = nullptr, const T *pData = nullptr)
		: Width(Width), Height(Height), MipLevels(MipLevels), NumPixels(UINT64(Width) * UINT64(Height)), MiplevelOffsets(MipLevels)
	{
		if (pMipLevelOffsets)
		{
			for (UINT i = 0; i < MipLevels; ++i)
			{
				MiplevelOffsets[i] = pMipLevelOffsets[i];
			}
		}

		Pixels = std::make_unique<T[]>(NumPixels);
		if (pData)
		{
			memcpy(Pixels.get(), pData, NumPixels * sizeof(T));
		}
	}

	operator auto()
	{
		return &Pixels[0];
	}

	Texture2D<T> &operator=(const Texture2D<T> &rhs)
	{
		assert(NumPixels == rhs.NumPixels);
		if (this != &rhs)
		{
			memcpy(Pixels.get(), rhs.Pixels.get(), NumPixels * sizeof(T));
		}

		return *this;
	}

	T At(UINT Index)
	{
		if (Index >= 0 && Index < NumPixels)
			return Pixels[Index];
		return {};
	}

	void Clear(T Value = (T)0)
	{
		for (UINT64 i = 0; i < NumPixels; ++i)
		{
			Pixels[i] = Value;
		}
	}

	bool IsWithinBounds(UINT X, UINT Y) const
	{
		return (X >= 0 && X < Width) && (Y >= 0 && Y < Height);
	}

	void SetPixel(UINT X, UINT Y, T Color)
	{
		if (IsWithinBounds(X, Y))
		{
			Pixels[Y * Width + X] = Color;
		}
	}

	T GetPixel(UINT X, UINT Y) const
	{
		if (IsWithinBounds(X, Y))
		{
			return Pixels[Y * Width + X];
		}
		return {};
	}

	void BLIT(const T *pSource, UINT SourceWidth, const RECT &BlockRect, UINT DstX, UINT DstY)
	{
		UINT BlockWidth = BlockRect.right - BlockRect.left;
		UINT BlockHeight = BlockRect.bottom - BlockRect.top;

		for (UINT y = 0; y < BlockHeight; y++)
		{
			for (UINT x = 0; x < BlockWidth; x++)
			{
				auto sourceIdx = Flatten2DTo1D((BlockRect.left + x), (BlockRect.top + y), SourceWidth);

				// if alpha 0 dont blit
				if (!IsWithinBounds(DstX + x, DstY + y))
					continue;

				T SrcPixel = pSource[sourceIdx];
				T DstPixel = GetPixel(DstX + x, DstY + y);

				// Color correction
				// extract channel into 8 bit value from source
				UINT SrcRed = (SrcPixel & 0x0000ff00) >> 8;
				UINT SrcGreen = (SrcPixel & 0x00ff0000) >> 16;
				UINT SrcBlue = (SrcPixel & 0xff000000) >> 24;
				UINT SrcAlpha = SrcPixel & 0x000000ff;
				// Alpha Blend
				// extract channel into 8 bit value from raster
				UINT DstRed = (DstPixel & 0x00ff0000) >> 16;
				UINT DstGreen = (DstPixel & 0x0000ff00) >> 8;
				UINT DstBlue = (DstPixel & 0x000000ff);

				// Shrubberies
				// rearranged formula from the previous formula: ((source * alpha) + (destination * (255 - alpha))) >> 8
				// Explanation: if you rearrange the formula you will get a denominator of 255, which is equivalent to shifting 8 bits to the right
				UINT NewRed = ((SrcRed * SrcAlpha) + (DstRed * (255 - SrcAlpha))) >> 8;
				UINT NewGreen = ((SrcGreen * SrcAlpha) + (DstGreen * (255 - SrcAlpha))) >> 8;
				UINT NewBlue = ((SrcBlue * SrcAlpha) + (DstBlue * (255 - SrcAlpha))) >> 8;
				UINT NewColor = NewRed << 16 | NewGreen << 8 | NewBlue;
				SetPixel(DstX + x, DstY + y, NewColor);
			}
		}
	}

	std::unique_ptr<T[]> Pixels;
	UINT Width, Height, MipLevels;
	UINT64 NumPixels;
	std::vector<UINT> MiplevelOffsets;
};

struct RenderTarget
{
	RenderTarget(UINT Width, UINT Height)
		: RT1(Width, Height), DepthBuffer(Width, Height), Width(Width), Height(Height), NumPixels(UINT64(Width) * UINT64(Height))
	{
	}

	void SetPixel(UINT X, UINT Y, UINT Color, FLOAT Depth)
	{
		if (!RT1.IsWithinBounds(X, Y) && !DepthBuffer.IsWithinBounds(X, Y))
		{
			return;
		}

		if (DepthEnable)
		{
			if (Depth <= DepthBuffer.GetPixel(X, Y))
			{
				RT1.SetPixel(X, Y, Color);
				DepthBuffer.SetPixel(X, Y, Depth);
			}
		}
		else
		{
			RT1.SetPixel(X, Y, Color);
		}
	}

	void Clear(UINT Color = 0, FLOAT Depth = 1.0f)
	{
		RT1.Clear(Color);
		DepthBuffer.Clear(Depth);
	}

	BOOL DepthEnable = FALSE;

	Texture2D<UINT> RT1;
	Texture2D<FLOAT> DepthBuffer;
	UINT Width, Height;
	UINT64 NumPixels;
};

int Save(const Texture2D<UINT> &Image, int NumChannels);