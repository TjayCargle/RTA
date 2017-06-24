#pragma once
#include <vector>
#include <DirectXMath.h>
struct Point
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;
	int parentIndex = -1;
	Point()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 1;
	}
	Point(const float &value)
	{
		x = value;
		y = value;
		z = value;
		w = 1;

	}
	Point(const float &somex, const float &somey, const float &somez, const float &somew)
	{
		x = somex;
		y = somey;
		z = somez;
		w = somew;
	}
	bool operator==(const Point &value)
	{
		bool returnedBool = true;
		if (x != value.x)
		{
			returnedBool = false;
		}
		if (y != value.y)
		{
			returnedBool = false;
		}
		if (z != value.z)
		{
			returnedBool = false;
		}
		if (w != value.w)
		{
			returnedBool = false;
		}
		return returnedBool;
	}
	Point & operator+(const unsigned int &value)
	{
		x += value;
		y += value;
		z += value;

		return *this;
	}
	Point & operator+=(const int &value)
	{
		x += value;
		y += value;
		z += value;

		return *this;
	}
	Point & operator+(const  int &value)
	{
		x += value;
		y += value;
		z += value;

		return *this;
	}
	Point & operator+=(const unsigned int &value)
	{
		x += value;
		y += value;
		z += value;

		return *this;
	}
	Point & operator-(const  int &value)
	{
		x -= value;
		y -= value;
		z -= value;

		return *this;
	}
	Point & operator-=(const unsigned int &value)
	{
		x -= value;
		y -= value;
		z -= value;

		return *this;
	}
	Point & operator+=(const float &value)
	{
		x += value;
		y += value;
		z += value;

		return *this;
	}
	Point & operator+(const float &value)
	{
		x += value;
		y += value;
		z += value;

		return *this;
	}
	Point & operator-(const float &value)
	{
		x -= value;
		y -= value;
		z -= value;

		return *this;
	}
	Point & operator-=(const float &value)
	{
		x -= value;
		y -= value;
		z -= value;

		return *this;
	}

};
struct TJColor
{
	float r = 0;
	float g = 0;
	float b = 0;
	float a = 0;
	float implicitLineEquation(Point A, Point B, Point Curr)
	{
		return ((A.y - B.y)*Curr.x + (B.x - A.x)*Curr.y + A.x*B.y - A.y*B.x);
	}
	float implicitLineEquation(Point A, Point B, float CX, float CY)
	{
		return ((A.y - B.y)*CX + (B.x - A.x)*CY + A.x*B.y - A.y*B.x);
	}
	float BLerp(float A, float a, float B, float b, float C, float y)
	{
		return (A * a) + (B * b) + (C * y);
	}
	void CreateFromUint(unsigned int aColor)
	{
		unsigned int tempColor = aColor;
		// 0xBBGGRRAA
		unsigned int AVal =     tempColor & 0xFF000000;
		unsigned int RedVal =   tempColor & 0x00FF0000;
		unsigned int GreenVal = tempColor & 0x0000FF00;
		unsigned int BlueVal =  tempColor & 0x000000FF;
		
		AVal = (AVal >> 24);
		RedVal = (RedVal >> 16);
		GreenVal = (GreenVal >> 8);

		a = AVal / 255.0f;
		r = RedVal / 255.0f;
		g = GreenVal / 255.0f;
		b = BlueVal / 255.0f;

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
};
struct TJVertex
{
	Point pos;
	//Point translation = 0;
	//Point rotation = 0;
	//Point scale = 1;
	Point normal;
	float u;
	float v;
	TJColor color;

	//TJVertex()
	//{
	//	Point pos = 0;
	////	Point translation = 0;
	////	Point rotation = 0;
	////	Point scale = 1;
	//	float u = 0;
	//	float v = 0;
	//	unsigned int color = 0xFF000000;
	//}
	//TJVertex( const float &posx, const float &posy, const float &posz, const unsigned int & aColor)
	//{
	//	pos.x = posx;
	//	pos.y = posy;
	//	pos.z = posz;
	//	color = aColor;
	//}
};
struct Line
{
	Point a;
	Point b;
};
struct PTriangle
{
	Point a;
	Point b;
	Point c;
};
struct VTriangle
{
	TJVertex a;
	TJVertex b;
	TJVertex c;
};
struct CubeFace
{
	PTriangle FT1;
	PTriangle FT2;
};

class TJCube
{
public:

	Point C1 = 0;
	Point C2 = 0;
	Point C3 = 0;
	Point C4 = 0;
	Point C5 = 0;
	Point C6 = 0;
	Point C7 = 0;
	Point C8 = 0;

	Point pointArray[8] = { 0 };

	PTriangle T1 = { 0 };
	PTriangle T2 = { 0 };
	PTriangle T3 = { 0 };
	PTriangle T4 = { 0 };
	PTriangle T5 = { 0 };
	PTriangle T6 = { 0 };
	PTriangle T7 = { 0 };
	PTriangle T8 = { 0 };
	PTriangle T9 = { 0 };
	PTriangle T10 = { 0 };
	PTriangle T11 = { 0 };
	PTriangle T12 = { 0 };

	PTriangle TriangleArray[12] = { 0 };

	CubeFace F1;
	CubeFace F2;
	CubeFace F3;
	CubeFace F4;
	CubeFace F5;
	CubeFace F6;
	void updateFaces()
	{
		F1.FT1 = T1;
		F1.FT2 = T2;

		F2.FT1 = T3;
		F2.FT2 = T4;

		F3.FT1 = T5;
		F3.FT2 = T6;

		F4.FT1 = T7;
		F4.FT2 = T8;

		F5.FT1 = T9;
		F5.FT2 = T10;

		F6.FT1 = T11;
		F6.FT2 = T12;
	}
	void updateTriangles()
	{
		T1.a = C1;
		T1.b = C5;
		T1.c = C8;
		T2.a = C1;
		T2.b = C4;
		T2.c = C8;

		T3.a = C2;
		T3.b = C1;
		T3.c = C4;

		T4.a = C2;
		T4.b = C3;
		T4.c = C4;

		T5.a = C3;
		T5.b = C7;
		T5.c = C6;
		T6.a = C3;
		T6.b = C2;
		T6.c = C6;

		T7.a = C6;
		T7.b = C5;
		T7.c = C8;
		T8.a = C6;
		T8.b = C7;
		T8.c = C8;

		T9.a = C2;
		T9.b = C6;
		T9.c = C5;
		T10.a = C2;
		T10.b = C1;
		T10.c = C5;

		T11.a = C4;
		T11.b = C8;
		T11.c = C7;
		T12.a = C4;
		T12.b = C3;
		T12.c = C7;

		setTriangleArrayFromTriangles();
		updateFaces();
	}
	void setTriangleArrayFromTriangles()
	{
		TriangleArray[0] = T1;
		TriangleArray[1] = T2;
		TriangleArray[2] = T3;
		TriangleArray[3] = T4;
		TriangleArray[4] = T5;
		TriangleArray[5] = T6;
		TriangleArray[6] = T7;
		TriangleArray[7] = T8;
		TriangleArray[8] = T9;
		TriangleArray[9] = T10;
		TriangleArray[10] = T11;
		TriangleArray[11] = T12;
	}
	void setTrianglesFromTriangleArray()
	{
		T1 = TriangleArray[0];
		T2 = TriangleArray[1];
		T3 = TriangleArray[2];
		T4 = TriangleArray[3];
		T5 = TriangleArray[4];
		T6 = TriangleArray[5];
		T7 = TriangleArray[6];
		T8 = TriangleArray[7];
		T9 = TriangleArray[8];
		T10 = TriangleArray[9];
		T11 = TriangleArray[10];
		T12 = TriangleArray[11];
	}
	void SetProceduralNDCCube()
	{
		C1.x = -0.25f;
		C1.y = 0.25f;
		C1.z = -0.25f;


		C2.x = -0.25f;
		C2.y = 0.25f;
		C2.z = 0.25f;


		C3.x = 0.25f;
		C3.y = 0.25f;
		C3.z = 0.25f;

		C4.x = 0.25;
		C4.y = 0.25f;
		C4.z = -0.25f;

		C5.x = -0.25f;
		C5.y = -0.25f;
		C5.z = -0.25f;

		C6.x = -0.25f;
		C6.y = -0.25f;
		C6.z = 0.25f;

		C7.x = 0.25f;
		C7.y = -0.25;
		C7.z = 0.25f;

		C8.x = 0.25f;
		C8.y = -0.25f;
		C8.z = -0.25f;

		pointArray[0] = C1;
		pointArray[1] = C2;
		pointArray[2] = C3;
		pointArray[3] = C4;
		pointArray[4] = C5;
		pointArray[5] = C6;
		pointArray[6] = C7;
		pointArray[7] = C8;

		updateTriangles();
	}
	void setArrayFromPoints()
	{
		pointArray[0] = C1;
		pointArray[1] = C2;
		pointArray[2] = C3;
		pointArray[3] = C4;
		pointArray[4] = C5;
		pointArray[5] = C6;
		pointArray[6] = C7;
		pointArray[7] = C8;
	}
	void setPointsFromArray()
	{
		C1 = pointArray[0];
		C2 = pointArray[1];
		C3 = pointArray[2];
		C4 = pointArray[3];
		C5 = pointArray[4];
		C6 = pointArray[5];
		C7 = pointArray[6];
		C8 = pointArray[7];
	}


};

struct Light
{
	float lightRatio = 0.0f;
	Point pos;
	Point direction;
	Point normal;
	unsigned int color = 0;
};
struct DLight : Light
{

};

struct PLight : Light
{

};
struct AnimationClip;
struct my_fbx_joint;
struct Mesh
{
	std::vector<std::vector<VTriangle>> myTriangles;
	int vertexCount;
	std::vector<unsigned int> indexBuffer;
	int boneVectorSize = 0;
	std::vector<std::vector<Point>> bones;
//	std::vector<Point> bones;
	std::string name = "yo";
	std::vector<my_fbx_joint*> fbxJoints;
	AnimationClip * myClip;
};

class alignas(16) TJMatrix
{
public:
	float e11, e12, e13, e14;
	float e21, e22, e23, e24;
	float e31, e32, e33, e34;
	float e41, e42, e43, e44;

	TJMatrix()
	{
		e11 = 1; e12 = 0; e13 = 0; e14 = 0;
		e21 = 0; e22 = 1; e23 = 0; e24 = 0;
		e31 = 0; e32 = 0; e33 = 1; e34 = 0;
		e41 = 0; e42 = 0; e43 = 0; e44 = 1;
	}
	void SetAsIdentiy()
	{
		e11 = 1; e12 = 0; e13 = 0; e14 = 0;
		e21 = 0; e22 = 1; e23 = 0; e24 = 0;
		e31 = 0; e32 = 0; e33 = 1; e34 = 0;
		e41 = 0; e42 = 0; e43 = 0; e44 = 1;
	}
	void SetAsZero()
	{
		e11 = 0; e12 = 0; e13 = 0; e14 = 0;
		e21 = 0; e22 = 0; e23 = 0; e24 = 0;
		e31 = 0; e32 = 0; e33 = 0; e34 = 0;
		e41 = 0; e42 = 0; e43 = 0; e44 = 0;
	}
	void SetAsTranslation(float x, float y, float z)
	{
		this->SetAsIdentiy();
		e41 = x;
		e42 = y;
		e43 = z;
	}
	void SetAsRotation_X(float deg)
	{
		SetAsIdentiy();
		float radians = (deg * 3.14159 / 180.0f);
		e22 = cos(radians); e23 = -sin(radians);
		e32 = sin(radians); e33 = cos(radians);
	}
	void SetAsRotation_Y(float deg)
	{
		SetAsIdentiy();
		float radians = (deg * 3.14159 / 180.0f);
		e11 = cos(radians); e13 = sin(radians);
		e31 = -sin(radians); e33 = cos(radians);
	}
	void SetAsRotation_Z(float deg)
	{
		SetAsIdentiy();
		float radians = (deg * 3.14159 / 180.0f);
		e11 = cos(radians); e12 = -sin(radians);
		e21 = sin(radians); e22 = cos(radians);
	}
	void SetAsProjectionMatrix(float znear, float zfar, float aspectRatio, float VFieldOfView)
	{
		SetAsZero();
		float yscale = 1 / tan((0.5 * VFieldOfView) * 3.14 / 180);
		float xscale = yscale * (aspectRatio);

		e11 = xscale;
		e22 = yscale;
		e33 = zfar / (zfar - znear);
		e34 = 1;
		e43 = (-1 * (zfar * znear)) / (zfar - znear);
	}
	void TransposeThisMatrix()
	{
		// TODO LAB 2: Replace with your implementation.
		TJMatrix yo;
		yo.SetAsZero();
		yo.e11 = e11;	yo.e12 = e21;   yo.e13 = e31; 	yo.e14 = e41;
		yo.e21 = e12;	yo.e22 = e22;   yo.e23 = e32;	yo.e24 = e42;
		yo.e31 = e13;	yo.e32 = e23;	yo.e33 = e33;	yo.e34 = e43;
		yo.e41 = e14;	yo.e42 = e24;	yo.e43 = e34;	yo.e44 = e44;


		e11 = yo.e11;	e12 = yo.e12;   e13 = yo.e13; 	e14 = yo.e14;
		e21 = yo.e21;	e22 = yo.e22;   e23 = yo.e23;	e24 = yo.e24;
		e31 = yo.e31;	e32 = yo.e32;	e33 = yo.e33;	e34 = yo.e34;
		e41 = yo.e41;	e42 = yo.e42;	e43 = yo.e43;	e44 = yo.e44;
	
	}
	void ScaleThisMatrix(float s)
	{

		e11 = e11 * s;	e12 = e12 * s; e13 = e13 * s; 	e14 = e14 * s;
		e21 = e21 * s;	e22 = e22 * s; e23 = e23 * s;	e24 = e24 * s;
		e31 = e31 * s;	e32 = e32 * s; e33 = e33 * s;	e34 = e34 * s;
		e41 = e41 * s;	e42 = e42 * s; e43 = e43 * s;	e44 = e44 * s;


	}
	TJMatrix InverseMatrix(TJMatrix & m)
	{
		float det = Matrix_Determinant(m);
		if (det == 0)
		{
			return m;
		}
		else
		{
			TJMatrix temp;
			temp.e11 = Matrix_Determinant(m.e22, m.e23, m.e24, m.e32, m.e33, m.e34, m.e42, m.e43, m.e44);
			temp.e21 = Matrix_Determinant(m.e12, m.e13, m.e14, m.e32, m.e33, m.e34, m.e42, m.e43, m.e44);
			temp.e31 = Matrix_Determinant(m.e12, m.e13, m.e14, m.e22, m.e23, m.e24, m.e42, m.e43, m.e44);
			temp.e41 = Matrix_Determinant(m.e12, m.e13, m.e14, m.e22, m.e23, m.e24, m.e32, m.e33, m.e34);

			temp.e12 = Matrix_Determinant(m.e21, m.e23, m.e24, m.e31, m.e33, m.e34, m.e41, m.e43, m.e44);
			temp.e22 = Matrix_Determinant(m.e11, m.e13, m.e14, m.e31, m.e33, m.e34, m.e41, m.e43, m.e44);
			temp.e32 = Matrix_Determinant(m.e11, m.e13, m.e14, m.e21, m.e23, m.e24, m.e41, m.e43, m.e44);
			temp.e42 = Matrix_Determinant(m.e11, m.e13, m.e14, m.e21, m.e23, m.e24, m.e31, m.e33, m.e34);

			temp.e13 = Matrix_Determinant(m.e21, m.e22, m.e24, m.e31, m.e32, m.e34, m.e41, m.e42, m.e44);
			temp.e23 = Matrix_Determinant(m.e11, m.e12, m.e14, m.e31, m.e32, m.e34, m.e41, m.e42, m.e44);
			temp.e33 = Matrix_Determinant(m.e11, m.e12, m.e14, m.e21, m.e22, m.e24, m.e41, m.e42, m.e44);
			temp.e43 = Matrix_Determinant(m.e11, m.e12, m.e14, m.e21, m.e22, m.e24, m.e31, m.e32, m.e34);

			temp.e14 = Matrix_Determinant(m.e21, m.e22, m.e23, m.e31, m.e32, m.e33, m.e41, m.e42, m.e43);
			temp.e24 = Matrix_Determinant(m.e11, m.e12, m.e13, m.e31, m.e32, m.e33, m.e41, m.e42, m.e43);
			temp.e34 = Matrix_Determinant(m.e11, m.e12, m.e13, m.e21, m.e22, m.e23, m.e41, m.e42, m.e43);
			temp.e44 = Matrix_Determinant(m.e11, m.e12, m.e13, m.e21, m.e22, m.e23, m.e31, m.e32, m.e33);



			temp.e12 *= -1;
			temp.e14 *= -1;
			temp.e21 *= -1;
			temp.e23 *= -1;
			temp.e32 *= -1;
			temp.e34 *= -1;
			temp.e41 *= -1;
			temp.e43 *= -1;

			temp.TransposeThisMatrix();

			det = 1 / det;
			temp.ScaleThisMatrix(det);
			m = temp;


		}
		return m;

	}
	void SetAsViewMatrix(TJMatrix & camera)
	{
		TJMatrix temp;
		temp.SetAsIdentiy();
		TJMatrix rotationMatrix = CreateRoatation_X(-18);
		TJMatrix translationMatrix = CreateTranslationMatrix(0, 0, -1);
		temp = Matrix_Matrix_Multiply(translationMatrix, rotationMatrix);
		temp = Matrix_Matrix_Multiply(temp, camera);
		temp = InverseMatrix(temp);
		
		e11 = temp.e11; e12 = temp.e12; e13 = temp.e13; e14 = temp.e14;
		e21 = temp.e21; e22 = temp.e22; e23 = temp.e23; e24 = temp.e24;
		e31 = temp.e31; e32 = temp.e32; e33 = temp.e33; e34 = temp.e34;
		e41 = temp.e41; e42 = temp.e42; e43 = temp.e43; e44 = temp.e44;
	}

	static TJMatrix CreateIdentiyMatrix()
	{
		TJMatrix temp;
		temp.SetAsIdentiy();
		return temp;
	}
	static TJMatrix CreateZeroMatrix()
	{
		TJMatrix temp;
		temp.SetAsZero();
		return temp;
	}
	static TJMatrix CreateTranslationMatrix(float x, float y, float z)
	{
		TJMatrix temp;
		temp.SetAsTranslation(x, y, z);
		return temp;
	}
	static TJMatrix CreateRoatation_X(float deg)
	{
		TJMatrix temp;
		temp.SetAsRotation_X(deg);
		return temp;
	}
	static TJMatrix CreateRoatation_Y(float deg)
	{
		TJMatrix temp;
		temp.SetAsRotation_Y(deg);
		return temp;
	}
	static TJMatrix CreateRoatation_Z(float deg)
	{
		TJMatrix temp;
		temp.SetAsRotation_Z(deg);
		return temp;
	}
	static TJMatrix CreateProjectionMatrix(float znear, float zfar, float aspectRatio, float VFieldOfView)
	{
		TJMatrix temp;
		temp.SetAsProjectionMatrix(znear, zfar, aspectRatio, VFieldOfView);
		return temp;
	}
	static TJMatrix CreateTransposedMatrix(TJMatrix & m)
	{
		TJMatrix temp;
		temp = m;
		temp.TransposeThisMatrix();
		return temp;

	}
	static TJMatrix CreateScaledMatrix(TJMatrix & m, float scale)
	{
		TJMatrix temp;
		temp = m;
		temp.ScaleThisMatrix(scale);
		return temp;
	}
	static TJMatrix CreateViewMatrix(TJMatrix & camera)
	{
		TJMatrix temp;
		temp.SetAsViewMatrix(camera);
		return temp;
	}

	static Mesh ScaleMesh(Mesh m, float s)
	{
		for (int j = 0; j < m.myTriangles.size(); j++)
		{
			for (int i = 0; i < m.myTriangles[j].size(); i++)
			{
				m.myTriangles[j][i].a.pos.x *= s;
				m.myTriangles[j][i].a.pos.y *= s;
				m.myTriangles[j][i].a.pos.z *= s;
				m.myTriangles[j][i].b.pos.x *= s;
				m.myTriangles[j][i].b.pos.y *= s;
				m.myTriangles[j][i].b.pos.z *= s;
				m.myTriangles[j][i].c.pos.x *= s;
				m.myTriangles[j][i].c.pos.y *= s;
				m.myTriangles[j][i].c.pos.z *= s;
			}
		}
		for (int j= 0; j < m.boneVectorSize; j++)
		{
			for (int i = 0; i < m.bones[j].size(); i++)
			{
				m.bones[j][i].x *= s;
				m.bones[j][i].y *= s;
				m.bones[j][i].z *= s;

			}
		}
	
		return m;
	}
	static Mesh TranslateMesh(Mesh &m, TJMatrix & n)
	{
		for (int j = 0; j < m.myTriangles.size(); j++)
		{
			for (int i = 0; i < m.myTriangles[j].size(); i++)
			{
				m.myTriangles[j][i].a = Vector_Matrix_Multiply(m.myTriangles[j][i].a, n);
				m.myTriangles[j][i].b = Vector_Matrix_Multiply(m.myTriangles[j][i].b, n);
				m.myTriangles[j][i].c = Vector_Matrix_Multiply(m.myTriangles[j][i].c, n);
			}
		}
		for (int j = 0; j < m.boneVectorSize; j++)
		{
			for (int i = 0; i < m.bones[j].size(); i++)
			{
				TJVertex temp;
				temp.pos.x = m.bones[j][i].x;
				temp.pos.y = m.bones[j][i].y;
				temp.pos.z = m.bones[j][i].z;
				temp.pos.w = m.bones[j][i].w;

				temp = Vector_Matrix_Multiply(temp, n);

				m.bones[j][i].x = temp.pos.x;
				m.bones[j][i].y = temp.pos.y;
				m.bones[j][i].z = temp.pos.z;
				m.bones[j][i].w = temp.pos.w;
			}
		}
		return m;
	}
	static Mesh * TranslateMesh(Mesh * m, TJMatrix * n)
	{
		for (int j = 0; j < m->myTriangles.size(); j++)
		{
			for (int i = 0; i < m->myTriangles[j].size(); i++)
			{
				m->myTriangles[j][i].a = Vector_Matrix_Multiply(&m->myTriangles[j][i].a, n);
				m->myTriangles[j][i].b = Vector_Matrix_Multiply(&m->myTriangles[j][i].b, n);
				m->myTriangles[j][i].c = Vector_Matrix_Multiply(&m->myTriangles[j][i].c, n);
			}
		}
		for (int j = 0; j < m->boneVectorSize; j++)
		{
			for (int i = 0; i < m->bones[j].size(); i++)
			{
				TJVertex temp;
				temp.pos.x = m->bones[j][i].x;
				temp.pos.y = m->bones[j][i].y;
				temp.pos.z = m->bones[j][i].z;
				temp.pos.w = m->bones[j][i].w;

				temp = Vector_Matrix_Multiply(&temp, n);

				m->bones[j][i].x = temp.pos.x;
				m->bones[j][i].y = temp.pos.y;
				m->bones[j][i].z = temp.pos.z;
				m->bones[j][i].w = temp.pos.w;
			}
		}
		return m;
	}

	static TJMatrix Matrix_Matrix_Multiply(TJMatrix &m, TJMatrix & n)
	{
		TJMatrix yo;
		yo.SetAsZero();
		yo.e11 = m.e11 * n.e11 + m.e12 * n.e21 + m.e13 * n.e31 + m.e14 * n.e41;			yo.e12 = m.e11 * n.e12 + m.e12 * n.e22 + m.e13 * n.e32 + m.e14 * n.e42;    yo.e13 = m.e11 * n.e13 + m.e12 * n.e23 + m.e13 * n.e33 + m.e14 * n.e43;		yo.e14 = m.e11 * n.e14 + m.e12 * n.e24 + m.e13 * n.e34 + m.e14 * n.e44;
		yo.e21 = m.e21 * n.e11 + m.e22 * n.e21 + m.e23 * n.e31 + m.e24 * n.e41;			yo.e22 = m.e21 * n.e12 + m.e22 * n.e22 + m.e23 * n.e32 + m.e24 * n.e42;    yo.e23 = m.e21 * n.e13 + m.e22 * n.e23 + m.e23 * n.e33 + m.e24 * n.e43;		yo.e24 = m.e21 * n.e14 + m.e22 * n.e24 + m.e23 * n.e34 + m.e24 * n.e44;
		yo.e31 = m.e31 * n.e11 + m.e32 * n.e21 + m.e33 * n.e31 + m.e34 * n.e41;			yo.e32 = m.e31 * n.e12 + m.e32 * n.e22 + m.e33 * n.e32 + m.e34 * n.e42;	yo.e33 = m.e31 * n.e13 + m.e32 * n.e23 + m.e33 * n.e33 + m.e34 * n.e43;		yo.e34 = m.e31 * n.e14 + m.e32 * n.e24 + m.e33 * n.e34 + m.e34 * n.e44;
		yo.e41 = m.e41 * n.e11 + m.e42 * n.e21 + m.e43 * n.e31 + m.e44 * n.e41;			yo.e42 = m.e41 * n.e12 + m.e42 * n.e22 + m.e43 * n.e32 + m.e44 * n.e42;	yo.e43 = m.e41 * n.e13 + m.e42 * n.e23 + m.e43 * n.e33 + m.e44 * n.e43;		yo.e44 = m.e41 * n.e14 + m.e42 * n.e24 + m.e43 * n.e34 + m.e44 * n.e44;

		return yo;
	}
	static TJMatrix * Matrix_Matrix_Multiply(TJMatrix * m, TJMatrix  * n)
	{
		TJMatrix * yo;
		yo->SetAsZero();
		yo->e11 = m->e11 * n->e11 + m->e12 * n->e21 + m->e13 * n->e31 + m->e14 * n->e41;			yo->e12 = m->e11 * n->e12 + m->e12 * n->e22 + m->e13 * n->e32 + m->e14 * n->e42; yo->e13 = m->e11 * n->e13 + m->e12 * n->e23 + m->e13 * n->e33 + m->e14 * n->e43;		yo->e14 = m->e11 * n->e14 + m->e12 * n->e24 + m->e13 * n->e34 + m->e14 * n->e44;
		yo->e21 = m->e21 * n->e11 + m->e22 * n->e21 + m->e23 * n->e31 + m->e24 * n->e41;			yo->e22 = m->e21 * n->e12 + m->e22 * n->e22 + m->e23 * n->e32 + m->e24 * n->e42; yo->e23 = m->e21 * n->e13 + m->e22 * n->e23 + m->e23 * n->e33 + m->e24 * n->e43;		yo->e24 = m->e21 * n->e14 + m->e22 * n->e24 + m->e23 * n->e34 + m->e24 * n->e44;
		yo->e31 = m->e31 * n->e11 + m->e32 * n->e21 + m->e33 * n->e31 + m->e34 * n->e41;			yo->e32 = m->e31 * n->e12 + m->e32 * n->e22 + m->e33 * n->e32 + m->e34 * n->e42;	yo->e33 = m->e31 * n->e13 + m->e32 * n->e23 + m->e33 * n->e33 + m->e34 * n->e43;		yo->e34 = m->e31 * n->e14 + m->e32 * n->e24 + m->e33 * n->e34 + m->e34 * n->e44;
		yo->e41 = m->e41 * n->e11 + m->e42 * n->e21 + m->e43 * n->e31 + m->e44 * n->e41;			yo->e42 = m->e41 * n->e12 + m->e42 * n->e22 + m->e43 * n->e32 + m->e44 * n->e42;	yo->e43 = m->e41 * n->e13 + m->e42 * n->e23 + m->e43 * n->e33 + m->e44 * n->e43;		yo->e44 = m->e41 * n->e14 + m->e42 * n->e24 + m->e43 * n->e34 + m->e44 * n->e44;

		return yo;
	}
	static TJVertex Vector_Matrix_Multiply(TJVertex &v, TJMatrix &m)
	{
		// TODO LAB 2: Replace with your implementation.
		TJVertex temp;
		temp.color = v.color;
		//	temp.translation = v.translation;
		//	temp.rotation = temp.rotation;
		//	temp.scale = v.scale;
			temp.u = v.u;
			temp.v = v.v;
			temp.normal = v.normal;
		temp.pos.x = m.e11 *v.pos.x + m.e21 * v.pos.y + m.e31 * v.pos.z + m.e41 * v.pos.w;
		temp.pos.y = m.e12 *v.pos.x + m.e22 * v.pos.y + m.e32 * v.pos.z + m.e42 * v.pos.w;
		temp.pos.z = m.e13 *v.pos.x + m.e23 * v.pos.y + m.e33 * v.pos.z + m.e43 * v.pos.w;
		temp.pos.w = m.e14 *v.pos.x + m.e24 * v.pos.y + m.e34 * v.pos.z + m.e44 * v.pos.w;
		return temp;
	}
	static TJVertex  Vector_Matrix_Multiply(TJVertex * v, TJMatrix * m)
	{
		// TODO LAB 2: Replace with your implementation.
		TJVertex  temp;
		temp.color = v->color;
		//	temp.translation = v->translation;
		//	temp.rotation = temp.rotation;
		//	temp.scale = v->scale;
		temp.u = v->u;
		temp.v = v->v;
		temp.normal = v->normal;
		temp.pos.x = m->e11 *v->pos.x + m->e21 * v->pos.y + m->e31 * v->pos.z + m->e41 * v->pos.w;
		temp.pos.y = m->e12 *v->pos.x + m->e22 * v->pos.y + m->e32 * v->pos.z + m->e42 * v->pos.w;
		temp.pos.z = m->e13 *v->pos.x + m->e23 * v->pos.y + m->e33 * v->pos.z + m->e43 * v->pos.w;
		temp.pos.w = m->e14 *v->pos.x + m->e24 * v->pos.y + m->e34 * v->pos.z + m->e44 * v->pos.w;
		return  temp;
	}
	static Point Point_Matrix_Multiply(Point &v, TJMatrix & m)
	{
		// TODO LAB 2: Replace with your implementation.
		Point temp;
		temp.x = m.e11 *v.x + m.e21 * v.y + m.e31 * v.z + m.e41 * v.w;
		temp.y = m.e12 *v.x + m.e22 * v.y + m.e32 * v.z + m.e42 * v.w;
		temp.z = m.e13 *v.x + m.e23 * v.y + m.e33 * v.z + m.e43 * v.w;
		temp.w = m.e14 *v.x + m.e24 * v.y + m.e34 * v.z + m.e44 * v.w;
		return temp;
	}

	float Matrix_Determinant(TJMatrix &m)
	{
		float det = m.e11 * (m.e22 * (m.e33 * m.e44 - m.e43 * m.e34) - m.e23 *(m.e32 * m.e44 - m.e42 * m.e34) + m.e24 * (m.e32 * m.e43 - m.e42 * m.e33)) - m.e12 * (m.e21 * (m.e33 * m.e44 - m.e43 * m.e34) - m.e23 *(m.e31 * m.e44 - m.e41 * m.e34) + m.e24 * (m.e31 * m.e43 - m.e41 * m.e33)) + m.e13 * (m.e21 * (m.e32 * m.e44 - m.e42 * m.e34) - m.e22 *(m.e31 * m.e44 - m.e41 * m.e34) + m.e24 * (m.e31 * m.e42 - m.e41 * m.e32)) - m.e14 * (m.e21 * (m.e32 * m.e43 - m.e42 * m.e33) - m.e22 *(m.e31 * m.e43 - m.e41 * m.e33) + m.e23 * (m.e31 * m.e42 - m.e41 * m.e32));
		return det;
	}
	float Matrix_Determinant(float e_11, float e_12, float e_13, float e_21, float e_22, float e_23, float e_31, float e_32, float e_33)
	{
		float det = e_11 * (e_22 * e_33 - e_32 * e_23) - e_12 *(e_21 * e_33 - e_31 * e_23) + e_13 * (e_21 * e_32 - e_31 * e_22);
		return det;
	}

	static DirectX::XMMATRIX TJM2Dir(TJMatrix & m)
	{
		DirectX::XMMATRIX returnMatrix;
		DirectX::XMFLOAT4X4 temp4by4;
		temp4by4._11 = m.e11;
		temp4by4._21 = m.e21;
		temp4by4._31 = m.e31;
		temp4by4._41 = m.e41;

		temp4by4._12 = m.e12;
		temp4by4._22 = m.e22;
		temp4by4._32 = m.e32;
		temp4by4._42 = m.e42;

		temp4by4._13 = m.e13;
		temp4by4._23 = m.e23;
		temp4by4._33 = m.e33;
		temp4by4._43 = m.e43;

		temp4by4._14 = m.e14;
		temp4by4._24 = m.e24;
		temp4by4._34 = m.e34;
		temp4by4._44 = m.e44;

		returnMatrix = DirectX::XMLoadFloat4x4(&temp4by4);
		return returnMatrix;
	}


};


struct color4f
{
	float r;
	float g;
	float b;
	float a;
};
struct  VERTEX
{
	float x, y, z, w;
	color4f color;
	float nx, ny, nz;
	float u, v;
	
};
struct TJMVPBuffer
{
	DirectX::XMFLOAT4X4 dirCam;
	DirectX::XMFLOAT4X4 dirView;
	DirectX::XMFLOAT4X4 dirProj;
};
struct keyFrame
{
	double time;
	std::vector<TJMatrix> frameMatrix;
};
struct AnimationClip
{
	double duration;
	std::vector<keyFrame> frames;
};

