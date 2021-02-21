#include <ctime>
#include <cmath>
#include <memory>
#include <cassert>
#include <iostream>

#include <Common/Defines.h>
#include <Common/RasterSurface.h>
#include <Common/XTime.h>

struct Vector2D
{
	float x = 0.0f;
	float y = 0.0f;
};

struct Vertex2D
{
	Vector2D position;
	unsigned int color;
};

unsigned int ColorBlend(Vertex2D start, Vertex2D end, float ratio);
float Lerp(float start, float end, float ratio);
float Slope(Vector2D start, Vector2D end);
float ImplicitLineEquation(Vector2D start, Vector2D end, Vector2D position);

void DrawBresenhamLine(Vertex2D start, Vertex2D end, Texture2D<UINT>* pSurface);
void DrawMidpointLine(Vertex2D start, Vertex2D end, Texture2D<UINT>* pSurface);
void DrawParametrixLine(Vertex2D start, Vertex2D end, Texture2D<UINT>* pSurface);

void DrawBresenhamLineOptimized(Vertex2D start, Vertex2D end, Texture2D<UINT>* pSurface);
void DrawMidpointLineOptimized(Vertex2D start, Vertex2D end, Texture2D<UINT>* pSurface);
void DrawParametrixLineOptimized(Vertex2D start, Vertex2D end, Texture2D<UINT>* pSurface);

int main(int argc, char** argv)
{
	std::cout << "Controls\n";
	std::cout << "1: Draw Bresenham Line\n";
	std::cout << "2: Draw Midpoint Line\n";
	std::cout << "3: Draw Parametric Line\n";
	std::cout << "4: Clear\n";

	const UINT64 Width = 500;
	const UINT64 Height = 500;

	srand(time(NULL));

	Texture2D<UINT> Surface(Width, Height), CopySurface(Width, Height);
	for (int i = 0; i < 5000; i++)
	{
		Surface.SetPixel(rand() % Width + 1, rand() % Height + 1, WHITE);
	}

	CopySurface = Surface;

	Vertex2D bresenhamLine[2] =
	{
		{{0.0f, 100.0f}, 0xffff0000},
		{{Width - 1.0f, Height - 21.0f}, 0xffff0000}
	};
	Vertex2D midpointLine[2] =
	{
		{{0.0f, 110.0f}, 0xff00ff00},
		{{Width - 1.0f, Height - 11.0f}, 0xff00ff00}
	};
	Vertex2D parametrixLine[2] =
	{
		{{0.0f, 120.0f}, 0xffff00ff},
		{{Width - 1.0f, Height - 1.0f}, 0xff00ffff}
	};

	Vertex2D rngBresenhamLine[2] =
	{
		{{0, 0}, 0xffff0000},
		{{0, 0}, 0xffff0000}
	};
	Vertex2D rngMidpointLine[2] =
	{
		{{0, 0}, 0xff00ff00},
		{{0, 0}, 0xff00ff00}
	};
	Vertex2D rngParametrixLine[2] =
	{
		{{0, 0}, 0xffff00ff},
		{{0, 0}, 0xff00ffff}
	};
	bool useOptimizedVersion = true;

	float randStartX = 0.0f;
	float randStartY = 0.0f;
	float randEndX = 0.0f;
	float randEndY = 0.0f;

	float randStartX1 = 0.0f;
	float randStartY1 = 0.0f;
	float randEndX1 = 0.0f;
	float randEndY1 = 0.0f;

	float randStartX2 = 0.0f;
	float randStartY2 = 0.0f;
	float randEndX2 = 0.0f;
	float randEndY2 = 0.0f;

	RS_Initialize(Width, Height);
	do
	{
		if (GetAsyncKeyState('1') & 0x1)
		{
			Surface.Clear();
			randStartX = static_cast<float>(rand() % Width + 1);
			randStartY = static_cast<float>(rand() % Height + 1);
			randEndX = static_cast<float>(rand() % Width + 1);
			randEndY = static_cast<float>(rand() % Height + 1);
			rngBresenhamLine[0].position.x = randStartX;
			rngBresenhamLine[0].position.y = randStartY;
			rngBresenhamLine[1].position.x = randEndX;
			rngBresenhamLine[1].position.y = randEndY;
		}
		if (GetAsyncKeyState('2') & 0x1)
		{
			Surface.Clear();
			randStartX1 = static_cast<float>(rand() % Width + 1);
			randStartY1 = static_cast<float>(rand() % Height + 1);
			randEndX1 = static_cast<float>(rand() % Width + 1);
			randEndY1 = static_cast<float>(rand() % Height + 1);
			rngMidpointLine[0].position.x = randStartX1;
			rngMidpointLine[0].position.y = randStartY1;
			rngMidpointLine[1].position.x = randEndX1;
			rngMidpointLine[1].position.y = randEndY1;
		}
		if (GetAsyncKeyState('3') & 0x1)
		{
			Surface.Clear();
			randStartX2 = static_cast<float>(rand() % Width + 1);
			randStartY2 = static_cast<float>(rand() % Height + 1);
			randEndX2 = static_cast<float>(rand() % Width + 1);
			randEndY2 = static_cast<float>(rand() % Height + 1);
			rngParametrixLine[0].position.x = randStartX2;
			rngParametrixLine[0].position.y = randStartY2;
			rngParametrixLine[1].position.x = randEndX2;
			rngParametrixLine[1].position.y = randEndY2;
		}
		if (GetAsyncKeyState('4') & 0x1)
		{
			Surface.Clear();
			Surface = CopySurface;
			rngBresenhamLine[0].position.x = 0;
			rngBresenhamLine[0].position.y = 0;
			rngBresenhamLine[1].position.x = 0;
			rngBresenhamLine[1].position.y = 0;
			rngMidpointLine[0].position.x = 0;
			rngMidpointLine[0].position.y = 0;
			rngMidpointLine[1].position.x = 0;
			rngMidpointLine[1].position.y = 0;
			rngParametrixLine[0].position.x = 0;
			rngParametrixLine[0].position.y = 0;
			rngParametrixLine[1].position.x = 0;
			rngParametrixLine[1].position.y = 0;
			randStartX = 0;
			randStartY = 0;
			randEndX = 0;
			randEndY = 0;
			randStartX1 = 0;
			randStartY1 = 0;
			randEndX1 = 0;
			randEndY1 = 0;
			randStartX2 = 0;
			randStartY2 = 0;
			randEndX2 = 0;
			randEndY2 = 0;
		}
		if (!useOptimizedVersion)
		{
			DrawBresenhamLine(bresenhamLine[0], bresenhamLine[1], &Surface);
			DrawMidpointLine(midpointLine[0], midpointLine[1], &Surface);
			DrawParametrixLine(parametrixLine[0], parametrixLine[1], &Surface);

			DrawBresenhamLine(rngBresenhamLine[0], rngBresenhamLine[1], &Surface);
			Surface.SetPixel(static_cast<int>(randStartX), static_cast<int>(randStartY), YELLOW);
			Surface.SetPixel(static_cast<int>(randEndX), static_cast<int>(randEndY), YELLOW);
			DrawMidpointLine(rngMidpointLine[0], rngMidpointLine[1], &Surface);
			Surface.SetPixel(static_cast<int>(randStartX1), static_cast<int>(randStartY1), YELLOW);
			Surface.SetPixel(static_cast<int>(randEndX1), static_cast<int>(randEndY1), YELLOW);
			DrawParametrixLine(rngParametrixLine[0], rngParametrixLine[1], &Surface);
			Surface.SetPixel(static_cast<int>(randStartX2), static_cast<int>(randStartY2), YELLOW);
			Surface.SetPixel(static_cast<int>(randEndX2), static_cast<int>(randEndY2), YELLOW);
		}
		else
		{
			DrawBresenhamLineOptimized(bresenhamLine[0], bresenhamLine[1], &Surface);
			DrawMidpointLineOptimized(midpointLine[0], midpointLine[1], &Surface);
			DrawParametrixLineOptimized(parametrixLine[0], parametrixLine[1], &Surface);

			DrawBresenhamLineOptimized(rngBresenhamLine[0], rngBresenhamLine[1], &Surface);
			Surface.SetPixel(static_cast<int>(randStartX), static_cast<int>(randStartY), YELLOW);
			Surface.SetPixel(static_cast<int>(randEndX), static_cast<int>(randEndY), YELLOW);
			DrawMidpointLineOptimized(rngMidpointLine[0], rngMidpointLine[1], &Surface);
			Surface.SetPixel(static_cast<int>(randStartX1), static_cast<int>(randStartY1), YELLOW);
			Surface.SetPixel(static_cast<int>(randEndX1), static_cast<int>(randEndY1), YELLOW);
			DrawParametrixLineOptimized(rngParametrixLine[0], rngParametrixLine[1], &Surface);
			Surface.SetPixel(static_cast<int>(randStartX2), static_cast<int>(randStartY2), YELLOW);
			Surface.SetPixel(static_cast<int>(randEndX2), static_cast<int>(randEndY2), YELLOW);
		}
	} while (RS_Update(Surface, Surface.NumPixels));
	RS_Shutdown();

	return Save(Surface, 3);
}

unsigned int ColorBlend(Vertex2D start, Vertex2D end, float ratio)
{
	unsigned int startRed = (start.color & 0x00ff0000) >> 16;
	unsigned int startGreen = (start.color & 0x0000ff00) >> 8;
	unsigned int startBlue = (start.color & 0x000000ff);

	unsigned int endRed = (end.color & 0x00ff0000) >> 16;
	unsigned int endGreen = (end.color & 0x0000ff00) >> 8;
	unsigned int endBlue = (end.color & 0x000000ff);

	float resultRed = Lerp(static_cast<float>(startRed), static_cast<float>(endRed), ratio);
	float resultGreen = Lerp(static_cast<float>(startGreen), static_cast<float>(endGreen), ratio);
	float resultBlue = Lerp(static_cast<float>(startBlue), static_cast<float>(endBlue), ratio);

	return static_cast<unsigned int>(resultRed) << 16 | static_cast<unsigned int>(resultGreen) << 8 | static_cast<unsigned int>(resultBlue);
}
float Lerp(float start, float end, float ratio)
{
	return (end - start) * ratio + start;
}
float Slope(Vector2D start, Vector2D end)
{
	return (end.y - start.y) / (end.x - start.x);
}
float ImplicitLineEquation(Vector2D start, Vector2D end, Vector2D position)
{
	return (start.y - end.y) * position.x + (end.x - start.x) * position.y + start.x * end.y - start.y * end.x;
}

void DrawBresenhamLine(Vertex2D start, Vertex2D end, Texture2D<UINT>* pSurface)
{
	int x = static_cast<int>(start.position.x);
	int y = static_cast<int>(start.position.y);
	float m = Slope(start.position, end.position);
	float deltaX = end.position.x - start.position.x;
	float deltaY = end.position.y - start.position.y;
	// vertical line case
	if (deltaX == 0.0f)
	{
		if (end.position.y < start.position.y) // negative direction in y axis
		{
			for (y = static_cast<int>(end.position.y); y < start.position.y; ++y)
			{
				pSurface->SetPixel(x, y, start.color);
			}
		}
		else // positive direction in y axis
		{
			for (; y < static_cast<int>(end.position.y); ++y)
			{
				pSurface->SetPixel(x, y, start.color);
			}
		}
	}
	else
	{
		int adjust = m >= 0.0f ? 1 : -1;
		float error = 0.0f;
		float threshold = 0.5f;
		// if run is greater than rise
		if (m <= 1.0f && m >= -1.0f)
		{
			if (end.position.x < start.position.x) // negative direction in x axis
			{
				y = static_cast<int>(end.position.y);
				for (x = static_cast<int>(end.position.x); x < start.position.x; ++x)
				{
					pSurface->SetPixel(x, y, start.color);
					error += abs(m);
					if (error >= threshold)
					{
						y += adjust;
						threshold += 1.0f;
					}
				}
			}
			else // positive direction in x axis
			{
				for (; x < end.position.x; ++x)
				{
					pSurface->SetPixel(x, y, start.color);
					error += abs(m);
					if (error >= threshold)
					{
						y += adjust;
						threshold += 1.0f;
					}
				}
			}
		}
		// rise is greater than run
		else
		{
			m = deltaX / deltaY;
			if (end.position.y < start.position.y) // negative direction in y axis
			{
				x = static_cast<int>(end.position.x);
				for (y = static_cast<int>(end.position.y); y < start.position.y; ++y)
				{
					pSurface->SetPixel(x, y, start.color);
					error += abs(m);
					if (error >= threshold)
					{
						x += adjust;
						threshold += 1.0f;
					}
				}
			}
			else // positive direction in y axis
			{
				for (; y < end.position.y; ++y)
				{
					pSurface->SetPixel(x, y, start.color);
					error += abs(m);
					if (error >= threshold)
					{
						x += adjust;
						threshold += 1.0f;
					}
				}
			}
		}
	}
}
void DrawMidpointLine(Vertex2D start, Vertex2D end, Texture2D<UINT>* pSurface)
{
	int x = static_cast<int>(start.position.x);
	int y = static_cast<int>(start.position.y);
	int adjust = 0;
	float m = Slope(start.position, end.position);
	float deltaX = end.position.x - start.position.x;
	float implicitResult = 0.0f;
	float xOffset = 0.0f;
	float yOffset = 0.0f;
	Vector2D midPoint = { 0.0f, 0.0f };
	// vertical line case
	if (deltaX == 0.0f)
	{
		yOffset = 0.5f;
		if (end.position.y < start.position.y) // negative direction on y axis
		{
			for (y = static_cast<int>(end.position.y); y < static_cast<int>(start.position.y); ++y)
			{
				pSurface->SetPixel(x, y, start.color);
				midPoint = { static_cast<float>(x), static_cast<float>(y) + yOffset };
				implicitResult = ImplicitLineEquation(start.position, end.position, midPoint);
				if (implicitResult < 0.0f)
					++x;
			}
		}
		else // positive direction on y axis
		{
			for (y = static_cast<int>(start.position.y); y < static_cast<int>(end.position.y); ++y)
			{
				pSurface->SetPixel(x, y, start.color);
				midPoint = { static_cast<float>(x), static_cast<float>(y) + yOffset };
				implicitResult = ImplicitLineEquation(start.position, end.position, midPoint);
				if (implicitResult < 0.0f)
					++x;
			}
		}
	}
	else
	{
		// if run is greater than rise
		if (m <= 1.0f && m >= -1.0f)
		{
			adjust = end.position.y < start.position.y ? -1 : 1;
			xOffset = end.position.x < start.position.x ? -1.0f : 1.0f;
			yOffset = end.position.y < start.position.y ? -0.5f : 0.5f;
			if (end.position.x < start.position.x) // negative direction on x axis
			{
				for (; x > static_cast<int>(end.position.x); --x)
				{
					pSurface->SetPixel(x, y, start.color);
					midPoint = { static_cast<float>(x) + xOffset, static_cast<float>(y) + yOffset };
					implicitResult = ImplicitLineEquation(start.position, end.position, midPoint);
					if ((implicitResult > 0.0f && yOffset > 0.0f) || (implicitResult < 0.0f && yOffset < 0.0f))
						y += adjust;
				}
			}
			else // positive direction on x axis
			{
				for (; x < static_cast<int>(end.position.x); ++x)
				{
					pSurface->SetPixel(x, y, start.color);
					midPoint = { static_cast<float>(x) + xOffset, static_cast<float>(y) + yOffset };
					implicitResult = ImplicitLineEquation(start.position, end.position, midPoint);
					if ((implicitResult < 0.0f && yOffset > 0.0f) || (implicitResult > 0.0f && yOffset < 0.0f))
						y += adjust;
				}
			}
		}
		// rise is greater than run
		else
		{
			adjust = end.position.x < start.position.x ? -1 : 1;
			xOffset = end.position.x < start.position.x ? -0.5f : 0.5f;
			yOffset = end.position.y < start.position.y ? -1.0f : 1.0f;
			if (end.position.y < start.position.y) // negative direction on y axis
			{
				for (; y > static_cast<int>(end.position.y); --y)
				{
					pSurface->SetPixel(x, y, start.color);
					midPoint = { static_cast<float>(x) + xOffset, static_cast<float>(y) + yOffset };
					implicitResult = ImplicitLineEquation(start.position, end.position, midPoint);
					if ((implicitResult > 0.0f && xOffset < 0.0f) || (implicitResult < 0.0f && xOffset > 0.0f))
						x += adjust;
				}
			}
			else // positive direction on y axis
			{
				for (; y < static_cast<int>(end.position.y); ++y)
				{
					pSurface->SetPixel(x, y, start.color);
					midPoint = { static_cast<float>(x) + xOffset, static_cast<float>(y) + yOffset };
					implicitResult = ImplicitLineEquation(start.position, end.position, midPoint);
					if ((implicitResult < 0.0f && xOffset < 0.0f) || (implicitResult > 0.0f && xOffset > 0.0f))
						x += adjust;
				}
			}
		}
	}
}
void DrawParametrixLine(Vertex2D start, Vertex2D end, Texture2D<UINT>* pSurface)
{
	int x = static_cast<int>(start.position.x);
	int y = static_cast<int>(start.position.y);
	float deltaX = end.position.x - start.position.x;
	float deltaY = end.position.y - start.position.y;
	float ratio = 0.0f;
	float m = Slope(start.position, end.position);
	unsigned int resultColor = 0;
	// vertical line case
	if (deltaX == 0.0f)
	{
		if (end.position.y < start.position.y) // negative direction on y axis
		{
			for (y = static_cast<int>(end.position.y); y < static_cast<int>(start.position.y); ++y)
			{
				deltaY = start.position.y - end.position.y;
				ratio = (y - end.position.y) / deltaY;
				float _x = Lerp(start.position.x, end.position.x, ratio);
				resultColor = ColorBlend(end, start, ratio);
				pSurface->SetPixel(static_cast<int>(floor(_x + 0.5f)), y, resultColor);
			}
		}
		else // positive direction on y axis
		{
			for (; y < static_cast<int>(end.position.y); ++y)
			{
				deltaY = end.position.y - start.position.y;
				ratio = (y - start.position.y) / deltaY;
				float _x = Lerp(start.position.x, end.position.x, ratio);
				resultColor = ColorBlend(start, end, ratio);
				pSurface->SetPixel(static_cast<int>(floor(_x + 0.5f)), y, resultColor);
			}
		}
	}
	else
	{
		// if run is greater than rise
		if (m <= 1.0f && m >= -1.0f)
		{
			if (end.position.x < start.position.x) // negative direction on x axis
			{
				for (x = static_cast<int>(end.position.x); x < static_cast<int>(start.position.x); ++x)
				{
					deltaX = start.position.x - end.position.x;
					ratio = (x - end.position.x) / deltaX;
					float _y = Lerp(end.position.y, start.position.y, ratio);
					resultColor = ColorBlend(end, start, ratio);
					pSurface->SetPixel(x, static_cast<int>(floor(_y + 0.5f)), resultColor);
				}
			}
			else // positive direction on x axis
			{
				for (; x < static_cast<int>(end.position.x); ++x)
				{
					deltaX = end.position.x - start.position.x;
					ratio = (x - start.position.x) / deltaX;
					float _y = Lerp(start.position.y, end.position.y, ratio);
					resultColor = ColorBlend(start, end, ratio);
					pSurface->SetPixel(x, static_cast<int>(floor(_y + 0.5f)), resultColor);
				}
			}
		}
		// rise is greater than run
		else
		{
			if (end.position.y < start.position.y) // negative direction on y axis
			{
				for (y = static_cast<int>(end.position.y); y < static_cast<int>(start.position.y); ++y)
				{
					deltaY = start.position.y - end.position.y;
					ratio = (y - end.position.y) / deltaY;
					float _x = Lerp(end.position.x, start.position.x, ratio);
					resultColor = ColorBlend(end, start, ratio);
					pSurface->SetPixel(static_cast<int>(floor(_x + 0.5f)), y, resultColor);
				}
			}
			else // positive direction on y axis
			{
				for (; y < static_cast<int>(end.position.y); ++y)
				{
					deltaY = end.position.y - start.position.y;
					ratio = (y - start.position.y) / deltaY;
					float _x = Lerp(start.position.x, end.position.x, ratio);
					resultColor = ColorBlend(start, end, ratio);
					pSurface->SetPixel(static_cast<int>(floor(_x + 0.5f)), y, resultColor);
				}
			}
		}
	}
}

// Shrubberies
void DrawBresenhamLineOptimized(Vertex2D start, Vertex2D end, Texture2D<UINT>* pSurface)
{
	int x = static_cast<int>(start.position.x);
	int y = static_cast<int>(start.position.y);
	float m = Slope(start.position, end.position);
	float deltaX = end.position.x - start.position.x;
	float deltaY = end.position.y - start.position.y;
	int adjust = m >= 0.0f ? 1 : -1;
	float error = 0.0f;
	float threshold = 0.5f;
	int numOfPixelToDraw = 0;
	bool steepSlope = false;
	if (abs(deltaX) >= abs(deltaY)) // if change in x is greater than change in y, we draw x many pixels
		numOfPixelToDraw = static_cast<int>(abs(deltaX));
	else
		numOfPixelToDraw = static_cast<int>(abs(deltaY));
	if (deltaX == 0.0f) // vertical line case
	{
		if (end.position.y < start.position.y) // negative direction in y axis
			y = static_cast<int>(end.position.y);
	}
	else
	{
		if (m <= 1.0f && m >= -1.0f) // if run is greater than rise
		{
			if (end.position.x < start.position.x) // negative direction in x axis
			{
				x = static_cast<int>(end.position.x);
				y = static_cast<int>(end.position.y);
			}
		}
		else // rise is greater than run
		{
			steepSlope = true;
			m = deltaX / deltaY;
			if (end.position.y < start.position.y) // negative direction in y axis
			{
				x = static_cast<int>(end.position.x);
				y = static_cast<int>(end.position.y);
			}
		}
	}
	for (int i = 0; i < numOfPixelToDraw; ++i)
	{
		pSurface->SetPixel(x, y, start.color);
		error += abs(m);
		if (error >= threshold && steepSlope)
		{
			x += adjust;
			threshold += 1.0f;
		}
		else if (error >= threshold && !steepSlope)
		{
			y += adjust;
			threshold += 1.0f;
		}

		if (steepSlope || deltaX == 0.0f)
			++y;
		else if (!steepSlope)
			++x;
	}
}
void DrawMidpointLineOptimized(Vertex2D start, Vertex2D end, Texture2D<UINT>* pSurface)
{
	int x = static_cast<int>(start.position.x);
	int y = static_cast<int>(start.position.y);
	int adjust = 0;
	float m = Slope(start.position, end.position);
	float deltaX = end.position.x - start.position.x;
	float deltaY = end.position.y - start.position.y;
	float implicitResult = 0.0f;
	float xOffset = 0.0f;
	float yOffset = 0.0f;
	Vector2D midPoint = { 0.0f, 0.0f };
	int numOfPixelToDraw = 0;
	bool steepSlope = false;
	if (abs(deltaX) >= abs(deltaY)) // if change in x is greater than change in y, we draw x many pixels
		numOfPixelToDraw = static_cast<int>(abs(deltaX));
	else
		numOfPixelToDraw = static_cast<int>(abs(deltaY));
	if (deltaX == 0.0f)	// vertical line case
	{
		yOffset = 0.5f;
		if (end.position.y < start.position.y) // negative direction on y axis
			y = static_cast<int>(end.position.y);
		else // positive direction on y axis
			y = static_cast<int>(start.position.y);
	}
	else
	{
		if (m <= 1.0f && m >= -1.0f) // if run is greater than rise
		{
			adjust = end.position.y < start.position.y ? -1 : 1;
			xOffset = end.position.x < start.position.x ? -1.0f : 1.0f;
			yOffset = end.position.y < start.position.y ? -0.5f : 0.5f;
		}
		else // rise is greater than run
		{
			steepSlope = true;
			adjust = end.position.x < start.position.x ? -1 : 1;
			xOffset = end.position.x < start.position.x ? -0.5f : 0.5f;
			yOffset = end.position.y < start.position.y ? -1.0f : 1.0f;
		}
	}

	for (int i = 0; i < numOfPixelToDraw; ++i)
	{
		if (deltaX == 0.0f)	// vertical line case
		{
			if (end.position.y < start.position.y) // negative direction on y axis
			{
				pSurface->SetPixel(x, y, start.color);
				midPoint = { static_cast<float>(x), static_cast<float>(y) + yOffset };
				implicitResult = ImplicitLineEquation(start.position, end.position, midPoint);
				if (implicitResult < 0.0f)
					++x;
			}
			else // positive direction on y axis
			{
				pSurface->SetPixel(x, y, start.color);
				midPoint = { static_cast<float>(x), static_cast<float>(y) + yOffset };
				implicitResult = ImplicitLineEquation(start.position, end.position, midPoint);
				if (implicitResult < 0.0f)
					++x;
			}
		}
		else
		{
			if (m <= 1.0f && m >= -1.0f) // if run is greater than rise
			{
				if (end.position.x < start.position.x) // negative direction on x axis
				{
					pSurface->SetPixel(x, y, start.color);
					midPoint = { static_cast<float>(x) + xOffset, static_cast<float>(y) + yOffset };
					implicitResult = ImplicitLineEquation(start.position, end.position, midPoint);
					if ((implicitResult > 0.0f && yOffset > 0.0f) || (implicitResult < 0.0f && yOffset < 0.0f))
						y += adjust;
				}
				else // positive direction on x axis
				{
					pSurface->SetPixel(x, y, start.color);
					midPoint = { static_cast<float>(x) + xOffset, static_cast<float>(y) + yOffset };
					implicitResult = ImplicitLineEquation(start.position, end.position, midPoint);
					if ((implicitResult < 0.0f && yOffset > 0.0f) || (implicitResult > 0.0f && yOffset < 0.0f))
						y += adjust;
				}
			}
			else // rise greater than run
			{
				if (end.position.y < start.position.y) // negative direction on y axis
				{
					pSurface->SetPixel(x, y, start.color);
					midPoint = { static_cast<float>(x) + xOffset, static_cast<float>(y) + yOffset };
					implicitResult = ImplicitLineEquation(start.position, end.position, midPoint);
					if ((implicitResult > 0.0f && xOffset < 0.0f) || (implicitResult < 0.0f && xOffset > 0.0f))
						x += adjust;
				}
				else // positive direction on y axis
				{
					pSurface->SetPixel(x, y, start.color);
					midPoint = { static_cast<float>(x) + xOffset, static_cast<float>(y) + yOffset };
					implicitResult = ImplicitLineEquation(start.position, end.position, midPoint);
					if ((implicitResult < 0.0f && xOffset < 0.0f) || (implicitResult > 0.0f && xOffset > 0.0f))
						x += adjust;
				}
			}
		}
		if (deltaX == 0.0f)
			++y;
		else if (!steepSlope && end.position.x < start.position.x)
			--x;
		else if (!steepSlope && start.position.x < end.position.x)
			++x;
		else if (steepSlope && end.position.y < start.position.y)
			--y;
		else if (steepSlope && start.position.y < end.position.y)
			++y;
	}
}
void DrawParametrixLineOptimized(Vertex2D start, Vertex2D end, Texture2D<UINT>* pSurface)
{
	//int x = static_cast<int>(start.position.x);
	//int y = static_cast<int>(start.position.y);
	//float deltaX = end.position.x - start.position.x;
	//float deltaY = end.position.y - start.position.y;
	//float ratio = 0.0f;
	//float m = Slope(start.position, end.position);
	//unsigned int resultColor = 0;
	//int numOfPixelToDraw = 0;
	//bool steepSlope = false;
	//if (abs(deltaX) >= abs(deltaY)) // if change in x is greater than change in y, we draw x many pixels
	//	numOfPixelToDraw = static_cast<int>(abs(deltaX));
	//else
	//	numOfPixelToDraw = static_cast<int>(abs(deltaY));
	//if (deltaX == 0.0f)	// vertical line case
	//{
	//	if (end.position.y < start.position.y) // negative direction on y axis
	//	{
	//		y = static_cast<int>(end.position.y);
	//		deltaY = start.position.y - end.position.y;
	//	}
	//}
	//else
	//{
	//	if (m <= 1.0f && m >= -1.0f) // if run is greater than rise
	//	{
	//		if (end.position.x < start.position.x) // negative direction on x axis
	//		{
	//			x = static_cast<int>(end.position.x);
	//			deltaX = start.position.x - end.position.x;
	//		}
	//	}
	//	else // rise is greater than run
	//	{
	//		steepSlope = true;
	//		if (end.position.y < start.position.y) // negative direction on y axis
	//		{
	//			y = static_cast<int>(end.position.y);
	//			deltaY = start.position.y - end.position.y;
	//		}
	//	}
	//}
	//for (int i = 0; i < numOfPixelToDraw; ++i)
	//{
	//	if (steepSlope || deltaX == 0.0f)
	//	{
	//		float _x = 0.0f;
	//		if (end.position.y < start.position.y) // negative direction on y axis
	//		{
	//			ratio = (y - end.position.y) / deltaY;
	//			_x = Lerp(end.position.x, start.position.x, ratio);
	//			resultColor = ColorBlend(end, start, ratio);
	//		}
	//		else // positive direction on y axis
	//		{
	//			ratio = (y - start.position.y) / deltaY;
	//			_x = Lerp(start.position.x, end.position.x, ratio);
	//			resultColor = ColorBlend(start, end, ratio);
	//		}
	//		DrawPixel(static_cast<int>(floor(_x + 0.5f)), y, resultColor);
	//		++y;
	//	}
	//	else if (!steepSlope)
	//	{
	//		float _y = 0.0f;
	//		if (end.position.x < start.position.x) // negative direction on x axis
	//		{
	//			ratio = (x - end.position.x) / deltaX;
	//			_y = Lerp(end.position.y, start.position.y, ratio);
	//			resultColor = ColorBlend(end, start, ratio);
	//		}
	//		else // positive direction on x axis
	//		{
	//			ratio = (x - start.position.x) / deltaX;
	//			_y = Lerp(start.position.y, end.position.y, ratio);
	//			resultColor = ColorBlend(start, end, ratio);
	//		}
	//		DrawPixel(x, static_cast<int>(floor(_y + 0.5f)), resultColor);
	//		++x;
	//	}
	//}

	// x = (B - A) * R + A
	float dx = fabs(end.position.x - start.position.x);
	float dy = fabs(end.position.y - start.position.y);
	int tp = static_cast<int>(fmaxf(dx, dy));
	// loop
	for (int i = 0; i < tp; ++i)
	{
		float r = i / static_cast<float>(tp);
		int px = static_cast<int>(floor(((end.position.x - start.position.x) * r + start.position.x) + 0.5f));
		int py = static_cast<int>(floor(((end.position.y - start.position.y) * r + start.position.y) + 0.5f));
		pSurface->SetPixel(px, py, start.color);
	}
}
