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

// Geometry and texture data
#include "StoneHenge.h"
#include "StoneHenge_Texture.h"

float RandomNumber(float min, float max)
{
	return ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (max - min)) + min;
}

Vertex starField[3000];
Vertex vertices[1457];

int main(int argc, char** argv)
{
	const UINT64 Width = 500;
	const UINT64 Height = 500;

	RenderTarget RenderTarget(Width, Height);
	Rasterizer Rasterizer(&RenderTarget);

	Texture2D<UINT> stoneHenge(StoneHenge_width, StoneHenge_height, StoneHenge_numlevels, StoneHenge_leveloffsets, StoneHenge_pixels);

	bool shrink = false;
	srand(time(NULL));

	for (int i = 0; i < 3000; i++)
	{
		starField[i].position.x = RandomNumber(-1.0f, 1.0f) * 50.0f;
		starField[i].position.y = RandomNumber(-1.0f, 1.0f) * 50.0f;
		starField[i].position.z = RandomNumber(-1.0f, 1.0f) * 50.0f;
		starField[i].position.w = 1.0f;
		starField[i].color = WHITE;
	}
	translateZ = -4.25f;

	Matrix4x4 Default = Matrix_Matrix_Multiply(Matrix_Create_Translation(translateX, translateY, translateZ), Matrix_Create_Rotation_X(-25.0f));
	Camera.World = Default;

	for (int i = 0; i < 1457; ++i)
	{
		vertices[i].position.x = StoneHenge_data[i].pos[0] * 0.1f;
		vertices[i].position.y = StoneHenge_data[i].pos[1] * 0.1f;
		vertices[i].position.z = StoneHenge_data[i].pos[2] * 0.1f;
		vertices[i].position.w = 1.0f;

		vertices[i].uv.x = StoneHenge_data[i].uvw[0];
		vertices[i].uv.y = StoneHenge_data[i].uvw[1];

		vertices[i].normal.x = StoneHenge_data[i].nrm[0];
		vertices[i].normal.y = StoneHenge_data[i].nrm[1];
		vertices[i].normal.z = StoneHenge_data[i].nrm[2];
		vertices[i].normal.w = 0.0f;
	}
	ConstantBuffer.light.color = 0xf0c0c0ff;
	ConstantBuffer.light.position = { 0.0f, 0.0f, 0.0f, 1.0f };
	ConstantBuffer.light.normal = Vector_Normalize({ 0.577f, 0.577f, -0.577f, 0.0f });
	ConstantBuffer.pointLight.color = 0x00ffffff;
	ConstantBuffer.pointLight.position = { -1.0f, 0.5f, 1.0f, 1.0f };

	XTime XTime;
	XTime.Restart();
	RS_Initialize(Width, Height);
	double ElapsedTime = 0.0;
	double FPS = 1.0 / 60.0;
	do
	{
		XTime.Signal();
		ElapsedTime += XTime.Delta();
		if (ElapsedTime > Frequency)
		{
			ElapsedTime = 0.0;

			RenderTarget.Clear();
			RenderTarget.DepthEnable = TRUE;

			// Set PSO
			Rasterizer.VS = VertexShader;

			ConstantBuffer.World = Matrix_Identity();
			for (auto& i : starField)
			{
				Rasterizer.DrawPoint(i);
			}

			ConstantBuffer.pTexture = &stoneHenge;
			Rasterizer.PS = PixelShader;
			for (int i = 0; i < ARRAYSIZE(StoneHenge_indicies); i += 3)
			{
				Rasterizer.FillTriangleBetterBrute(
					vertices[StoneHenge_indicies[i]],
					vertices[StoneHenge_indicies[i + 1]],
					vertices[StoneHenge_indicies[i + 2]]);
			}
			Rasterizer.PS = nullptr;

			// camera movement
			// move forward
			if (GetAsyncKeyState('W'))
			{
				Camera.World = Matrix_Matrix_Multiply(Camera.World, Matrix_Create_Rotation_X(1.0f));
			}
			// move left
			if (GetAsyncKeyState('A'))
			{
				Camera.World = Matrix_Matrix_Multiply(Camera.World, Matrix_Create_Rotation_Y(-1.0f));
			}
			// move backwards
			if (GetAsyncKeyState('S'))
			{
				Camera.World = Matrix_Matrix_Multiply(Camera.World, Matrix_Create_Rotation_X(-1.0f));
			}
			// move right
			if (GetAsyncKeyState('D'))
			{
				Camera.World = Matrix_Matrix_Multiply(Camera.World, Matrix_Create_Rotation_Y(1.0f));
			}
			// reset values
			if (GetAsyncKeyState('R') & 0x1)
			{
				Camera.World = Default;
			}

			if (ConstantBuffer.lightRadius > 10.0f)
			{
				shrink = true;
			}
			else if (ConstantBuffer.lightRadius < 1.0f)
			{
				shrink = false;
			}
			if (shrink)
			{
				ConstantBuffer.lightRadius -= 0.3f;
			}
			else
			{
				ConstantBuffer.lightRadius += 0.3f;
			}
		}
	} while (RS_Update(RenderTarget.RT1, RenderTarget.RT1.NumPixels));
	RS_Shutdown();

	return Save(RenderTarget.RT1, 3);
}