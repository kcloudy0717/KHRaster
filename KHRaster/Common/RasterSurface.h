#pragma once
#include <sal.h>

// Spawns & manages a win32 window of the requested size. (the "RasterSurface")
bool RS_Initialize(_In_range_(1, 0xFFFF) unsigned int _width,
				   _In_range_(1, 0xFFFF) unsigned int _height);

// Updates the RasterSurface with a block of raw XRGB pixel data.
// Incoming data must 32bit pixels 8 bits per channel.
bool RS_Update(_In_reads_(_numPixels) const unsigned int *_xrgbPixels,
			   _In_range_(1, 0xFFFFFFFF) unsigned int _numPixels);

// Deallocates the RasterSurface and cleans up any leftover memory.
bool RS_Shutdown();