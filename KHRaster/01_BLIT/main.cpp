#include <ctime>
#include <cmath>
#include <memory>
#include <cassert>
#include <iostream>

#include <Common/Defines.h>
#include <Common/Shaders.h>
#include <Common/Rasterizer.h>
#include <Common/RasterSurface.h>
#include <Common/XTime.h>

#include "tiles_12.h"
#include "teleporter_hit.h"

int main(int argc, char **argv)
{
	// Tile info
	/*
	Tiles		X		Y		WIDTH		HEIGHT
	Grass		288		128		32			32
	House       16      16      95          82
	Red house	128     32		63			66
	Mail Box    176		112		15			23
	Green house 208     32		95			64
	Tree		320		16		63			79
	Park		16		112		143			71
	Fence		212		112		39			47
	*/
	const RECT GrassBlock = {288, 128, 320, 160};
	const RECT House = {16, 16, 111, 98};
	const RECT RedHouse = {128, 32, 191, 98};
	const RECT MailBox = {176, 112, 191, 135};
	const RECT GreenHouse = {208, 32, 303, 96};
	const RECT Tree = {320, 16, 383, 98};
	const RECT Park = {16, 112, 159, 183};
	const RECT Fence = {212, 112, 251, 159};

	const UINT64 Width = 500;
	const UINT64 Height = 500;

	srand(time(NULL));

	Texture2D<UINT> Surface(Width, Height), CopySurface(Width, Height);

	// grass
	for (int y = 0; y < Height; y += 32)
	{
		for (int x = 0; x < Width; x += 32)
		{
			Surface.BLIT(tiles_12_pixels, tiles_12_width, GrassBlock, x, y);
		}
	}

	Surface.BLIT(tiles_12_pixels, tiles_12_width, House, rand() % (Width + 1 - 95), rand() % (Height + 1 - 82));
	Surface.BLIT(tiles_12_pixels, tiles_12_width, House, rand() % (Width + 1 - 95), rand() % (Height + 1 - 82));
	Surface.BLIT(tiles_12_pixels, tiles_12_width, RedHouse, rand() % (Width + 1 - 63), rand() % (Height + 1 - 66));
	Surface.BLIT(tiles_12_pixels, tiles_12_width, MailBox, rand() % (Width + 1 - 15), rand() % (Height + 1 - 23));
	Surface.BLIT(tiles_12_pixels, tiles_12_width, GreenHouse, rand() % (Width + 1 - 95), rand() % (Height + 1 - 64));
	Surface.BLIT(tiles_12_pixels, tiles_12_width, Tree, rand() % (Width + 1 - 63), rand() % (Height + 1 - 79));
	Surface.BLIT(tiles_12_pixels, tiles_12_width, Tree, rand() % (Width + 1 - 63), rand() % (Height + 1 - 79));
	Surface.BLIT(tiles_12_pixels, tiles_12_width, Park, rand() % (Width + 1 - 143), rand() % (Height + 1 - 71));
	Surface.BLIT(tiles_12_pixels, tiles_12_width, Fence, rand() % (Width + 1 - 39), rand() % (Height + 1 - 47));

	// place holder for the raster so we can use it for later
	CopySurface = Surface;

	// cell locations
	int x = 0;
	int y = 0;
	double ElapsedTime = 0.0;

	XTime XTime;
	XTime.Restart();

	RS_Initialize(Width, Height);
	do
	{
		XTime.Signal();
		ElapsedTime += XTime.Delta();
		if (ElapsedTime > (1.0 / 60.0))
		{
			ElapsedTime = 0.0;

			Surface = CopySurface;

			// draw cells each frame
			RECT Teleporter = {x, y, x + 128, y + 128};
			Surface.BLIT(teleporter_hit_pixels, teleporter_hit_width, Teleporter, Width / 2, Height / 2);

			x += 128;
			if (x == 1024)
			{
				x = 0;
				y += 128;
			}
			if (y == 1024)
			{
				y = 0;
			}
		}
	} while (RS_Update(Surface, Surface.NumPixels));
	RS_Shutdown();

	return Save(Surface, 3);
}