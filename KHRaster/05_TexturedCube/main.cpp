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

// Texture data
#include "celestial.h"
#include "CatMarioModel Death.h"

enum RENDER_OPTIONS
{
	WireframedCube,
	ColoredCube_NoDepth,
	ColoredCube_Depth,
	TexturedCube
};

int main(int argc, char** argv)
{
	std::cout << "Controls\n";
	std::cout << "1: Wireframed cube\n";
	std::cout << "2: Colored cube: DepthEnable = false\n";
	std::cout << "3: Colored cube: DepthEnable = true\n";
	std::cout << "4: Textured cube\n";

	const UINT64 Width = 500;
	const UINT64 Height = 500;

	RenderTarget RenderTarget(Width, Height);
	Rasterizer Rasterizer(&RenderTarget);

	Texture2D<UINT> celestial(celestial_width, celestial_height, celestial_numlevels, celestial_leveloffsets, celestial_pixels);
	Texture2D<UINT> CatMarioModel(CatMarioModel_Death_width, CatMarioModel_Death_height, CatMarioModel_Death_numlevels, CatMarioModel_Death_leveloffsets, CatMarioModel_Death_pixels);

	unsigned int cubeColor = GREEN;
	
	Vertex cube[16] =
	{
		// front face
		// top vertices, -z
		{-0.25f, 0.5f, -0.25, 1.0f, cubeColor, {0.0f, 0.0f}}, // 0
		{ 0.25f, 0.5f, -0.25, 1.0f, cubeColor, {1.0f, 0.0f}}, // 1
		// bottom vertices -z
		{-0.25f, 0.0f, -0.25, 1.0f, cubeColor, {0.0f, 1.0f}}, // 2
		{ 0.25f, 0.0f, -0.25, 1.0f, cubeColor, {1.0f, 1.0f}}, // 3
		// back face
		// top vertices, +z
		{-0.25f, 0.5f, 0.25, 1.0f, cubeColor, {1.0f, 0.0f}}, // 4
		{ 0.25f, 0.5f, 0.25, 1.0f, cubeColor, {0.0f, 0.0f}}, // 5
		// bottom vertices +z
		{-0.25f, 0.0f, 0.25, 1.0f, cubeColor, {1.0f, 1.0f}}, // 6
		{ 0.25f, 0.0f, 0.25, 1.0f, cubeColor, {0.0f, 1.0f}}, // 7
		// left face
		{-0.25f, 0.5f, 0.25, 1.0f, cubeColor, {0.0f, 0.0f}}, // 8
		{-0.25f, 0.5f, -0.25, 1.0f, cubeColor, {1.0f, 0.0f}}, // 9
		{-0.25f, 0.0f, 0.25, 1.0f, cubeColor, {0.0f, 1.0f}}, // 10
		{-0.25f, 0.0f, -0.25, 1.0f, cubeColor, {1.0f, 1.0f}}, // 11
		// right face
		{ 0.25f, 0.5f, -0.25, 1.0f, cubeColor, {0.0f, 0.0f}}, // 12
		{ 0.25f, 0.5f, 0.25, 1.0f, cubeColor, {1.0f, 0.0f}}, // 13
		{ 0.25f, 0.0f, -0.25, 1.0f, cubeColor, {0.0f, 1.0f}}, // 14
		{ 0.25f, 0.0f, 0.25, 1.0f, cubeColor, {1.0f, 1.0f}}, // 15
	};
	Vertex cube1[16] =
	{
		// front face
		// top vertices, -z
		{-0.25f, 0.5f, -0.25, 1.0f, cubeColor, {0.0f, 0.0f}}, // 0
		{ 0.25f, 0.5f, -0.25, 1.0f, cubeColor, {1.0f, 0.0f}}, // 1
		// bottom vertices -z
		{-0.25f, 0.0f, -0.25, 1.0f, cubeColor, {0.0f, 1.0f}}, // 2
		{ 0.25f, 0.0f, -0.25, 1.0f, cubeColor, {1.0f, 1.0f}}, // 3
		// back face
		// top vertices, +z
		{-0.25f, 0.5f, 0.25, 1.0f, cubeColor, {1.0f, 0.0f}}, // 4
		{ 0.25f, 0.5f, 0.25, 1.0f, cubeColor, {0.0f, 0.0f}}, // 5
		// bottom vertices +z
		{-0.25f, 0.0f, 0.25, 1.0f, cubeColor, {1.0f, 1.0f}}, // 6
		{ 0.25f, 0.0f, 0.25, 1.0f, cubeColor, {0.0f, 1.0f}}, // 7
		// left face
		{-0.25f, 0.5f, 0.25, 1.0f, cubeColor, {0.0f, 0.0f}}, // 8
		{-0.25f, 0.5f, -0.25, 1.0f, cubeColor, {1.0f, 0.0f}}, // 9
		{-0.25f, 0.0f, 0.25, 1.0f, cubeColor, {0.0f, 1.0f}}, // 10
		{-0.25f, 0.0f, -0.25, 1.0f, cubeColor, {1.0f, 1.0f}}, // 11
		// right face
		{ 0.25f, 0.5f, -0.25, 1.0f, cubeColor, {0.0f, 0.0f}}, // 12
		{ 0.25f, 0.5f, 0.25, 1.0f, cubeColor, {1.0f, 0.0f}}, // 13
		{ 0.25f, 0.0f, -0.25, 1.0f, cubeColor, {0.0f, 1.0f}}, // 14
		{ 0.25f, 0.0f, 0.25, 1.0f, cubeColor, {1.0f, 1.0f}}, // 15
	};
	Matrix4x4 gridMatrix = Matrix_Identity();
	Matrix4x4 cubeMatrix = Matrix_Create_Translation(0.0f, 0.0f, 0.0f);
	Matrix4x4 cube1Matrix = Matrix_Matrix_Multiply(Matrix_Create_Translation(0.75f, 0.5f, 0.0f), Matrix_Create_Scale(0.5f, 0.5f, 0.5f));

	RENDER_OPTIONS Option = WireframedCube;

	XTime XTime;
	XTime.Restart();
	RS_Initialize(Width, Height);
	double ElapsedTime = 0.0;
	double FPS = 1.0 / 60.0;
	do
	{
		XTime.Signal();
		ElapsedTime += XTime.Delta();
		if (ElapsedTime > FPS)
		{
			ElapsedTime = 0;

			RenderTarget.Clear();

			ConstantBuffer.World = gridMatrix;

			// Set PSO
			Rasterizer.VS = VertexShader;

			if (Option == WireframedCube)
			{
				RenderTarget.DepthEnable = false;
				ConstantBuffer.World = cubeMatrix;
				// draw cube
				// top horizontal line, -z
				Rasterizer.DrawParametricLine(cube[0], cube[1]);
				Rasterizer.DrawParametricLine(cube[2], cube[3]);
				// bottom horizontal line, -z
				Rasterizer.DrawParametricLine(cube[0], cube[2]);
				Rasterizer.DrawParametricLine(cube[1], cube[3]);
				// top horizontal line, +z
				Rasterizer.DrawParametricLine(cube[4], cube[5]);
				Rasterizer.DrawParametricLine(cube[6], cube[7]);
				// bottom horizonal line, +z
				Rasterizer.DrawParametricLine(cube[4], cube[6]);
				Rasterizer.DrawParametricLine(cube[5], cube[7]);
				// connect both planes together
				Rasterizer.DrawParametricLine(cube[0], cube[4]);
				Rasterizer.DrawParametricLine(cube[2], cube[6]);
				Rasterizer.DrawParametricLine(cube[1], cube[5]);
				Rasterizer.DrawParametricLine(cube[3], cube[7]);
			}
			if (Option == ColoredCube_NoDepth)
			{
				RenderTarget.DepthEnable = false;
				ConstantBuffer.World = cubeMatrix;
				// front face
				Rasterizer.PS = PS_Red;
				Rasterizer.FillTriangleBetterBrute(cube[0], cube[1], cube[2]);
				Rasterizer.FillTriangleBetterBrute(cube[3], cube[1], cube[2]);
				// left face
				Rasterizer.PS = PS_Green;
				Rasterizer.FillTriangleBetterBrute(cube[4], cube[0], cube[6]);
				Rasterizer.FillTriangleBetterBrute(cube[2], cube[0], cube[6]);
				// right face
				Rasterizer.PS = PS_Blue;
				Rasterizer.FillTriangleBetterBrute(cube[1], cube[3], cube[5]);
				Rasterizer.FillTriangleBetterBrute(cube[7], cube[3], cube[5]);
				// back face
				Rasterizer.PS = PS_Purple;
				Rasterizer.FillTriangleBetterBrute(cube[4], cube[5], cube[6]);
				Rasterizer.FillTriangleBetterBrute(cube[7], cube[5], cube[6]);
			}
			if (Option == ColoredCube_Depth)
			{
				RenderTarget.DepthEnable = true;
				ConstantBuffer.World = cubeMatrix;
				// front face
				Rasterizer.PS = PS_Red;
				Rasterizer.FillTriangleBetterBrute(cube[0], cube[1], cube[2]);
				Rasterizer.FillTriangleBetterBrute(cube[3], cube[1], cube[2]);
				// left face
				Rasterizer.PS = PS_Green;
				Rasterizer.FillTriangleBetterBrute(cube[4], cube[0], cube[6]);
				Rasterizer.FillTriangleBetterBrute(cube[2], cube[0], cube[6]);
				// right face
				Rasterizer.PS = PS_Blue;
				Rasterizer.FillTriangleBetterBrute(cube[1], cube[3], cube[5]);
				Rasterizer.FillTriangleBetterBrute(cube[7], cube[3], cube[5]);
				// back face
				Rasterizer.PS = PS_Purple;
				Rasterizer.FillTriangleBetterBrute(cube[4], cube[5], cube[6]);
				Rasterizer.FillTriangleBetterBrute(cube[7], cube[5], cube[6]);
			}
			if (Option == TexturedCube)
			{
				RenderTarget.DepthEnable = true;
				ConstantBuffer.World = cubeMatrix;
				ConstantBuffer.pTexture = &celestial;

				Rasterizer.PS = PS_Texture;
				// front face
				Rasterizer.FillTriangleBetterBrute(cube[0], cube[1], cube[2]);
				Rasterizer.FillTriangleBetterBrute(cube[3], cube[1], cube[2]);
				// left face
				Rasterizer.FillTriangleBetterBrute(cube[8], cube[9], cube[10]);
				Rasterizer.FillTriangleBetterBrute(cube[11], cube[9], cube[10]);
				// right face
				Rasterizer.FillTriangleBetterBrute(cube[12], cube[13], cube[14]);
				Rasterizer.FillTriangleBetterBrute(cube[15], cube[13], cube[14]);
				// back face
				Rasterizer.FillTriangleBetterBrute(cube[5], cube[4], cube[7]);
				Rasterizer.FillTriangleBetterBrute(cube[6], cube[4], cube[7]);

				ConstantBuffer.World = cube1Matrix;
				ConstantBuffer.pTexture = &CatMarioModel;

				// front face
				Rasterizer.FillTriangleBetterBrute(cube1[0], cube1[1], cube1[2]);
				Rasterizer.FillTriangleBetterBrute(cube1[3], cube1[1], cube1[2]);
				// left face				
				Rasterizer.FillTriangleBetterBrute(cube1[8], cube1[9], cube1[10]);
				Rasterizer.FillTriangleBetterBrute(cube1[11], cube1[9], cube1[10]);
				// right face				
				Rasterizer.FillTriangleBetterBrute(cube1[12], cube1[13], cube1[14]);
				Rasterizer.FillTriangleBetterBrute(cube1[15], cube1[13], cube1[14]);
				// back face				
				Rasterizer.FillTriangleBetterBrute(cube1[5], cube1[4], cube1[7]);
				Rasterizer.FillTriangleBetterBrute(cube1[6], cube1[4], cube1[7]);
			}
			if (GetAsyncKeyState('1') & 0x1)
			{
				Option = WireframedCube;
			}
			if (GetAsyncKeyState('2') & 0x1)
			{
				Option = ColoredCube_NoDepth;
			}
			if (GetAsyncKeyState('3') & 0x1)
			{
				Option = ColoredCube_Depth;
			}
			if (GetAsyncKeyState('4') & 0x1)
			{
				Option = TexturedCube;
			}

			angle++;
			cubeMatrix = Matrix_Matrix_Multiply(Matrix_Create_Translation(0.0f, 0.0f, 0.0f), Matrix_Create_Rotation_Y(angle));
			// cube 1 stuff
			cube1Matrix = Matrix_Matrix_Multiply(Matrix_Create_Translation(0.35f, 0.25f, 0.0f), Matrix_Create_Scale(0.5f, 0.5f, 0.5f));
			//cube1Matrix = Matrix_Matrix_Multiply(cube1Matrix, Matrix_Create_Rotation_Y(angle));
			cube1Matrix = Matrix_Matrix_Multiply(cube1Matrix, cubeMatrix);
			//cube1Matrix = Matrix_Matrix_Multiply(cube1Matrix, Matrix_Create_Rotation_Y(angle));

			Rasterizer.VS = nullptr;
			Rasterizer.PS = nullptr;
		}
	} while (RS_Update(RenderTarget.RT1, RenderTarget.RT1.NumPixels));
	RS_Shutdown();

	return Save(RenderTarget.RT1, 3);
}