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

int main(int argc, char** argv)
{
	const UINT64 Width = 500;
	const UINT64 Height = 500;

	RenderTarget RenderTarget(Width, Height);
	Rasterizer Rasterizer(&RenderTarget);

	const Vertex vertices[3] =
	{
		{ { 0.0f, 0.5f, 0.0f, 1.0f }, RED },
		{ { -0.5f, 0.0f, 0.0f, 1.0f }, GREEN },
		{ { 0.5f, 0.0f, 0.0f, 1.0f }, BLUE }
	};

	XTime XTime;
	XTime.Restart();
	RS_Initialize(Width, Height);
	do
	{
		XTime.Signal();

		RenderTarget.Clear();
		Rasterizer.VS = VertexShader;
		Rasterizer.DrawParametricLine(vertices[0], vertices[1]);
		Rasterizer.DrawParametricLine(vertices[1], vertices[2]);
		Rasterizer.DrawParametricLine(vertices[2], vertices[0]);
		Rasterizer.FillTriangleBetterBrute(vertices[0], vertices[1], vertices[2]);
		RenderTarget.RT1.SetPixel(NDCXToRasterCoord(vertices[0].position.x, RenderTarget.Width), NDCYToRasterCoord(vertices[0].position.y, RenderTarget.Height), vertices[0].color);
		RenderTarget.RT1.SetPixel(NDCXToRasterCoord(vertices[1].position.x, RenderTarget.Width), NDCYToRasterCoord(vertices[1].position.y, RenderTarget.Height), vertices[1].color);
		RenderTarget.RT1.SetPixel(NDCXToRasterCoord(vertices[2].position.x, RenderTarget.Width), NDCYToRasterCoord(vertices[2].position.y, RenderTarget.Height), vertices[2].color);

		angle++;
	} while (RS_Update(RenderTarget.RT1, RenderTarget.NumPixels));
	RS_Shutdown();

	return Save(RenderTarget.RT1, 3);
}