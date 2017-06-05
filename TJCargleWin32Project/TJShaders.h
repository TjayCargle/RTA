#pragma once
#include "EngineMath.h"
#include "greendragon.h"
unsigned int Convert2DCoordInto1D(int coordX, int coordY, int width)
{
	return (coordY * width) + coordX;

}
unsigned int lerp(int B, int A, float ratio)
{
	return ((B - A) * ratio) + A;
}
unsigned int BGRA2ARGB(unsigned int aColor)
{

	unsigned int tempColor = aColor;
	// 0xBBGGRRAA
	unsigned int RedVal = tempColor & 0x0000FF00;
	unsigned int BlueVal = tempColor & 0xFF000000;
	unsigned int GreenVal = tempColor & 0x00FF0000;
	unsigned int AVal = tempColor & 0x000000FF;
	RedVal = (RedVal >> 8);
	BlueVal = (BlueVal >> 24);
	GreenVal = (GreenVal >> 16);
	tempColor = 0;
	tempColor = AVal;
	tempColor = tempColor << 8;
	tempColor |= RedVal;
	tempColor = tempColor << 8;
	tempColor |= GreenVal;
	tempColor = tempColor << 8;
	tempColor |= BlueVal;
	return tempColor;
}
unsigned int BlendColors(unsigned int color1, unsigned int color2, float ratio)
{
	unsigned int color = 0;
	int oldRedVal = color1 & 0x00FF0000;
	int oldGreenVal = color1 & 0x0000FF00;
	int oldBlueVal = color1 & 0x000000FF;


	int RedVal = color2 & 0x00FF0000;
	int GreenVal = color2 & 0x0000FF00;
	int BlueVal = color2 & 0x000000FF;

	int tempRed = 0;
	int tempGreen = 0;
	int tempBlue = 0;

	tempRed = lerp(RedVal, oldRedVal, ratio);
	tempGreen = lerp(GreenVal, oldGreenVal, ratio);
	tempBlue = lerp(BlueVal, oldBlueVal, ratio);


	tempRed = (tempRed >> 16);
	tempGreen = (tempGreen >> 8);

	color = 0x00000000;
	color |= tempRed;
	color = color << 8;
	color |= tempGreen;
	color = color << 8;
	color |= tempBlue;

	return color;
}

// The active vertex shader. Modifies an incoming vertex. Pre-Rasterization. 
void(*VertexShader)(TJVertex&) = 0;
// The active pixel shader. Modifies an outgoing pixel. Post-Rasterization.
void(*PixelShader)(TJVertex &multiplyMe, unsigned int& aColor) = 0;
void(*TextureShader)(unsigned int & aColor, float uLoc, float vLoc, unsigned int width, unsigned int height, const unsigned int * source, const unsigned int * Raster, unsigned int RasterWidth, unsigned int RasterHeight) = 0;

void(*TextureShader2)(unsigned int & aColor, float xLoc, float yLoc, unsigned int width, const unsigned int * source) = 0;

// All Shader Variables (Always Pre-fixed by “SV_”)
TMATRIX SV_WorldMatrix;
TMATRIX SV_viewMatrix;
TMATRIX SV_projectionMatrix;

void SetPixelColor(TJVertex * aVertex, unsigned int value)
{
	aVertex->myColor = value;

}
void SetPixelColor(unsigned int * aColor, unsigned int value)
{
	*aColor =  value;

}
// Various custom vertex and pixel shaders, (Pre-fixed by “VS_” & “PS_”)
// Can be swapped using above function pointers as needed for flexibility. 

// Applys the current world matrix to all
void VS_World(TJVertex &tVertex)
{
	
	Vector_Matrix_Multiply(&tVertex, SV_WorldMatrix);
	Vector_Matrix_Multiply(&tVertex, SV_viewMatrix);
	Vector_Matrix_Multiply(&tVertex, SV_projectionMatrix);
	


}
unsigned int bilinearFilter(const unsigned int * myRaster, const unsigned int * Picture, unsigned int PictureWidth, unsigned int PictureHeight, unsigned int RasterWidth, unsigned int RasterHeight, float Uval, float Vval)
{
	float u = Uval;
	float v = Vval;
	int trueX = 0;
	int trueY = 0;
	const unsigned int * tex = Picture;
	unsigned int colortopleft = 0x00000000;
	unsigned int colortopright = 0x00000000;
	unsigned int colorbottomleft = 0x00000000;
	unsigned int colorbottomright = 0x00000000;
	unsigned int colorCurr = 0x00000000;
	float xR = 0.0f;
	float yR = 0.0f;

	trueX = floor(u);
	trueY = floor(v);

	xR = u - trueX;
	yR = v - trueY;
	if (Convert2DCoordInto1D(trueX + 1, trueY + 1, PictureWidth) < PictureWidth*PictureHeight)
	{
		colorCurr = BGRA2ARGB(tex[Convert2DCoordInto1D(trueX, trueY, PictureWidth)]);
		colortopleft = BGRA2ARGB(tex[Convert2DCoordInto1D(trueX + 1, trueY, PictureWidth)]);
		colortopright = BGRA2ARGB(tex[Convert2DCoordInto1D(trueX, trueY + 1, PictureWidth)]);
		colorbottomleft = BGRA2ARGB(tex[Convert2DCoordInto1D(trueX + 1, trueY + 1, PictureWidth)]);


		colorCurr = BlendColors(colorCurr, colortopleft, xR);

		colortopright = BlendColors(colortopright, colorbottomleft, xR);

		colorCurr = BlendColors(colorCurr, colortopright, yR);

		return colorCurr;
	}
	else
		return 0;

}

void VS_Color(unsigned int & aColor, float aU, float aV ,unsigned int width, unsigned int height,const unsigned int * source,const unsigned int * Raster, unsigned int RasterWidth, unsigned int RasterHeight)
{
	unsigned int valColor = bilinearFilter(Raster, source, width, height, RasterWidth, RasterHeight, aU,aV);
	SetPixelColor(&aColor, valColor);
}
void VS_Color(unsigned int & aColor, float xLoc, float yLoc, unsigned int width, const unsigned int * source)
{
	//color = bilinearFilter(Raster, sourceArray, sourceWidth, sourceHeight, RasterWidth, RasterHeight, )
	SetPixelColor(&aColor, source[Convert2DCoordInto1D(xLoc, yLoc, width)]);
}
void VS_Color(TJVertex &multiplyMe, unsigned int& aColor)
{
	SetPixelColor(&multiplyMe, aColor);
}