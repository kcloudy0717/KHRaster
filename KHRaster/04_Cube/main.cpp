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
	std::cout << "Controls\n";
	std::cout << "1: Increase FOV\n";
	std::cout << "2: Decrease FOV\n";
	std::cout << "3: Reset FOV\n";

	const UINT64 Width = 500;
	const UINT64 Height = 500;

	RenderTarget RenderTarget(Width, Height);
	Rasterizer Rasterizer(&RenderTarget);
	Rasterizer.VS = VertexShader;

	unsigned int gridColor = WHITE;
	unsigned int cubeColor = GREEN;
	Vertex leftVertices[11] =
	{
		{-0.5f, 0.0f,  0.5f, 1.0f, gridColor},
		{-0.5f, 0.0f,  0.4f, 1.0f, gridColor},
		{-0.5f, 0.0f,  0.3f, 1.0f, gridColor},
		{-0.5f, 0.0f,  0.2f, 1.0f, gridColor},
		{-0.5f, 0.0f,  0.1f, 1.0f, gridColor},
		{-0.5f, 0.0f,  0.0f, 1.0f, gridColor},
		{-0.5f, 0.0f, -0.1f, 1.0f, gridColor},
		{-0.5f, 0.0f, -0.2f, 1.0f, gridColor},
		{-0.5f, 0.0f, -0.3f, 1.0f, gridColor},
		{-0.5f, 0.0f, -0.4f, 1.0f, gridColor},
		{-0.5f, 0.0f, -0.5f, 1.0f, gridColor}
	};
	Vertex rightVertices[11] =
	{
		{0.5f, 0.0f,  0.5f, 1.0f, gridColor},
		{0.5f, 0.0f,  0.4f, 1.0f, gridColor},
		{0.5f, 0.0f,  0.3f, 1.0f, gridColor},
		{0.5f, 0.0f,  0.2f, 1.0f, gridColor},
		{0.5f, 0.0f,  0.1f, 1.0f, gridColor},
		{0.5f, 0.0f,  0.0f, 1.0f, gridColor},
		{0.5f, 0.0f, -0.1f, 1.0f, gridColor},
		{0.5f, 0.0f, -0.2f, 1.0f, gridColor},
		{0.5f, 0.0f, -0.3f, 1.0f, gridColor},
		{0.5f, 0.0f, -0.4f, 1.0f, gridColor},
		{0.5f, 0.0f, -0.5f, 1.0f, gridColor}
	};
	Vertex topVertices[11] =
	{
		{ 0.5f, 0.0f, 0.5f, 1.0f, gridColor},
		{ 0.4f, 0.0f, 0.5f, 1.0f, gridColor},
		{ 0.3f, 0.0f, 0.5f, 1.0f, gridColor},
		{ 0.2f, 0.0f, 0.5f, 1.0f, gridColor},
		{ 0.1f, 0.0f, 0.5f, 1.0f, gridColor},
		{ 0.0f, 0.0f, 0.5f, 1.0f, gridColor},
		{-0.1f, 0.0f, 0.5f, 1.0f, gridColor},
		{-0.2f, 0.0f, 0.5f, 1.0f, gridColor},
		{-0.3f, 0.0f, 0.5f, 1.0f, gridColor},
		{-0.4f, 0.0f, 0.5f, 1.0f, gridColor},
		{-0.5f, 0.0f, 0.5f, 1.0f, gridColor}
	};
	Vertex bottomVertices[11] =
	{
		{ 0.5f, 0.0f, -0.5f, 1.0f, gridColor},
		{ 0.4f, 0.0f, -0.5f, 1.0f, gridColor},
		{ 0.3f, 0.0f, -0.5f, 1.0f, gridColor},
		{ 0.2f, 0.0f, -0.5f, 1.0f, gridColor},
		{ 0.1f, 0.0f, -0.5f, 1.0f, gridColor},
		{ 0.0f, 0.0f, -0.5f, 1.0f, gridColor},
		{-0.1f, 0.0f, -0.5f, 1.0f, gridColor},
		{-0.2f, 0.0f, -0.5f, 1.0f, gridColor},
		{-0.3f, 0.0f, -0.5f, 1.0f, gridColor},
		{-0.4f, 0.0f, -0.5f, 1.0f, gridColor},
		{-0.5f, 0.0f, -0.5f, 1.0f, gridColor}
	};
	Vertex cube[8] = 
	{
		{-0.25f, 0.5f, -0.25, 1.0f, cubeColor},
		{ 0.25f, 0.5f, -0.25, 1.0f, cubeColor},
		{-0.25f, 0.0f, -0.25, 1.0f, cubeColor},
		{ 0.25f, 0.0f, -0.25, 1.0f, cubeColor},

		{-0.25f, 0.5f, 0.25, 1.0f, cubeColor},
		{ 0.25f, 0.5f, 0.25, 1.0f, cubeColor},
		{-0.25f, 0.0f, 0.25, 1.0f, cubeColor},
		{ 0.25f, 0.0f, 0.25, 1.0f, cubeColor},
	};
	Vertex cube1[8] =
	{
		{-0.25f, 0.5f, -0.25, 1.0f, cubeColor},
		{ 0.25f, 0.5f, -0.25, 1.0f, cubeColor},
		{-0.25f, 0.0f, -0.25, 1.0f, cubeColor},
		{ 0.25f, 0.0f, -0.25, 1.0f, cubeColor},

		{-0.25f, 0.5f, 0.25, 1.0f, cubeColor},
		{ 0.25f, 0.5f, 0.25, 1.0f, cubeColor},
		{-0.25f, 0.0f, 0.25, 1.0f, cubeColor},
		{ 0.25f, 0.0f, 0.25, 1.0f, cubeColor},
	};
	Matrix4x4 gridMatrix = Matrix_Identity();
	Matrix4x4 cubeMatrix = Matrix_Create_Translation(0.0f, 0.0f, 0.0f);
	Matrix4x4 cube1Matrix = Matrix_Matrix_Multiply(Matrix_Create_Translation(1.5f, 0.5f, 1.5f), Matrix_Create_Scale(0.3f, 0.3f, 0.3f));
	Camera.FOV = 90.0f;
	Camera.AspectRatio = (float) Width / (float) Height;

	XTime XTime;
	XTime.Restart();
	double ElapsedTime = 0.0;
	RS_Initialize(Width, Height);
	do
	{
		XTime.Signal();
		ElapsedTime += XTime.Delta();
		if (ElapsedTime > (1.0 / 60.0))
		{
			ElapsedTime = 0.0;

			RenderTarget.Clear();

			ConstantBuffer.World = gridMatrix;
			// draw grid
			for (int i = 0; i < 11; ++i)
			{
				Rasterizer.DrawParametricLine(leftVertices[i], rightVertices[i]);
				Rasterizer.DrawParametricLine(topVertices[i], bottomVertices[i]);
			}

			ConstantBuffer.World = cubeMatrix;
			// draw cube
			// front horizontal line
			Rasterizer.DrawParametricLine(cube[0], cube[1]);
			Rasterizer.DrawParametricLine(cube[2], cube[3]);
			// front vertical line
			Rasterizer.DrawParametricLine(cube[0], cube[2]);
			Rasterizer.DrawParametricLine(cube[1], cube[3]);
			// back horizontal
			Rasterizer.DrawParametricLine(cube[4], cube[5]);
			Rasterizer.DrawParametricLine(cube[6], cube[7]);
			// back vertical line
			Rasterizer.DrawParametricLine(cube[4], cube[6]);
			Rasterizer.DrawParametricLine(cube[5], cube[7]);
			// connects the first plane with second plane
			Rasterizer.DrawParametricLine(cube[0], cube[4]);
			Rasterizer.DrawParametricLine(cube[2], cube[6]);
			Rasterizer.DrawParametricLine(cube[1], cube[5]);
			Rasterizer.DrawParametricLine(cube[3], cube[7]);

			ConstantBuffer.World = cube1Matrix;
			// draw cube 1
			// top horizontal line
			Rasterizer.DrawParametricLine(cube1[0], cube1[1]);
			Rasterizer.DrawParametricLine(cube1[2], cube1[3]);
			// bottom horizontal line
			Rasterizer.DrawParametricLine(cube1[0], cube1[2]);
			Rasterizer.DrawParametricLine(cube1[1], cube1[3]);
			// top horizontal line, in depth line
			Rasterizer.DrawParametricLine(cube1[4], cube1[5]);
			Rasterizer.DrawParametricLine(cube1[6], cube1[7]);
			// bottom horizontal line, in depth line
			Rasterizer.DrawParametricLine(cube1[4], cube1[6]);
			Rasterizer.DrawParametricLine(cube1[5], cube1[7]);
			// connects the first plane with second plane
			Rasterizer.DrawParametricLine(cube1[0], cube1[4]);
			Rasterizer.DrawParametricLine(cube1[2], cube1[6]);
			Rasterizer.DrawParametricLine(cube1[1], cube1[5]);
			Rasterizer.DrawParametricLine(cube1[3], cube1[7]);
			angle++;

			cubeMatrix = Matrix_Create_Rotation_Y(angle);
			// Shrubbery 1
			cube1Matrix = Matrix_Matrix_Multiply(Matrix_Create_Translation(1.5f, 0.5f, 1.5f), Matrix_Create_Scale(0.3f, 0.3f, 0.3f));
			cube1Matrix = Matrix_Matrix_Multiply(cube1Matrix, Matrix_Create_Rotation_Y(angle));
			cube1Matrix = Matrix_Matrix_Multiply(cube1Matrix, cubeMatrix);
			cube1Matrix = Matrix_Matrix_Multiply(cube1Matrix,Matrix_Create_Rotation_Y(angle));
			// Shrubbery 2
			if (GetAsyncKeyState('1') & 0x1)
			{
				Camera.FOV += 2.0f;
			}
			if (GetAsyncKeyState('2') & 0x1)
			{
				Camera.FOV -= 2.0f;
			}
			if (GetAsyncKeyState('3') & 0x1)
			{
				Camera.FOV = 90.0f;
			}
		}
	} while (RS_Update(RenderTarget.RT1, RenderTarget.NumPixels));
	RS_Shutdown();

	return Save(RenderTarget.RT1, 3);
}