#include "Defines.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

int Save(const Texture2D<UINT>& Image, int NumChannels)
{
	// Saves a input image as a png using stb
	std::unique_ptr<BYTE[]> Pixels = std::make_unique<BYTE[]>(Image.Width * Image.Height * NumChannels);

	BYTE* pDst = Pixels.get();

	int index = 0;
	for (int y = 0; y < int(Image.Height); ++y)
	{
		for (int x = 0; x < int(Image.Width); ++x)
		{
			auto color = Image.GetPixel(x, y);

			unsigned int R = (color & 0x00ff0000) >> 16;
			unsigned int G = (color & 0x0000ff00) >> 8;
			unsigned int B = (color & 0x000000ff);

			pDst[0] = R;
			pDst[1] = G;
			pDst[2] = B;
			pDst += 3;
		}
	}

#ifdef _DEBUG
	const char* Name = "Debug.png";
#else
	const char* Name = "Release.png";
#endif
	if (stbi_write_png(Name, Image.Width, Image.Height, 3, Pixels.get(), Image.Width * NumChannels))
	{
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}