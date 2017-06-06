#include "TJDLL.h"
#include "window.h"
#include <Windows.h>
#include <iostream>
using namespace std;
VERTEX CPUSideVertexBuffer[1024];
int main(void)
{
	double a = 3;
	double b = 4;
	TJDEV5LIB::Functions functionExpert;
	cout << "a + b = " <<
		functionExpert.Add(a, b) << endl;
	functionExpert.SetupFbxManager();
	functionExpert.setFbxIORoot();
	functionExpert.setupFbxImporter("../Teddy_Idle.fbx");
	functionExpert.importFbxscene("testScene");
	Mesh testMesh;
	testMesh = functionExpert.getMeshFromFbx();

	functionExpert.setupFbxImporter("../Mage_Idle.fbx");
	functionExpert.importFbxscene("testScene");

	Mesh testMesh2;
	testMesh2 = functionExpert.getMeshFromFbx();

	

	testMesh = TJMatrix::ScaleMesh(testMesh, 0.003f);

	TJMatrix teddyTrans = TJMatrix::CreateTranslationMatrix(-0.3, 0, 0);

	testMesh = TJMatrix::TranslateMesh(testMesh, teddyTrans);

	testMesh2 = TJMatrix::ScaleMesh(testMesh2, 0.09f);

	TJMatrix mageTrans = TJMatrix::CreateTranslationMatrix(0.3, 0, 0);
	testMesh2 = TJMatrix::TranslateMesh(testMesh2, mageTrans);


	//system("pause");

	fsgd::window::properties main_window_props{ 1280u, 720u, false };
	fsgd::window main_window;

	fsgd::window::create(&main_window, main_window_props);
	TJMatrix translationMatrix, tempRotationMatrix, tempWorldMatrix;
	tempWorldMatrix.SetAsIdentiy();

	AddMeshToVertexList(testMesh);
	AddMeshToVertexList(testMesh2);

	
	while (main_window.pump_events())
	{
		if (GetAsyncKeyState(VK_UP) & 0x0001)
		{
			translationMatrix.SetAsTranslation(0, -0.05, 0);
			tempWorldMatrix = TJMatrix::Matrix_Matrix_Multiply(translationMatrix, tempWorldMatrix);
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x0001)
		{
			translationMatrix.SetAsTranslation(0, 0.05, 0);
			tempWorldMatrix = TJMatrix::Matrix_Matrix_Multiply(translationMatrix, tempWorldMatrix);
		}

		if (GetAsyncKeyState(VK_LEFT) & 0x0001)
		{
			translationMatrix.SetAsTranslation(0.05, 0, 0);
			tempWorldMatrix = TJMatrix::Matrix_Matrix_Multiply(translationMatrix, tempWorldMatrix);
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
		{
			translationMatrix.SetAsTranslation(-0.05, 0, 0);
			tempWorldMatrix = TJMatrix::Matrix_Matrix_Multiply(translationMatrix, tempWorldMatrix);
		}
		if (GetAsyncKeyState(VK_RSHIFT) & 0x0001)
		{
			translationMatrix.SetAsTranslation(0, 0, -0.05);
			tempWorldMatrix = TJMatrix::Matrix_Matrix_Multiply(translationMatrix, tempWorldMatrix);
		}
		if (GetAsyncKeyState(VK_LSHIFT) & 0x0001)
		{
			translationMatrix.SetAsTranslation(0, 0, 0.05);
			tempWorldMatrix = TJMatrix::Matrix_Matrix_Multiply(translationMatrix, tempWorldMatrix);
		}
		SetCamera(tempWorldMatrix);

		if (GetAsyncKeyState(VK_LCONTROL) )
		{
			
			tempWorldMatrix = TJMatrix::Matrix_Matrix_Multiply(TJMatrix::CreateRoatation_X(-10), tempWorldMatrix);
		}
		if (GetAsyncKeyState(VK_RCONTROL))
		{

			tempWorldMatrix = TJMatrix::Matrix_Matrix_Multiply(TJMatrix::CreateRoatation_X(10), tempWorldMatrix);
		}
		if (GetAsyncKeyState(VK_LBUTTON))
		{

			tempWorldMatrix = TJMatrix::Matrix_Matrix_Multiply(TJMatrix::CreateRoatation_Y(-10), tempWorldMatrix);
		}
		if (GetAsyncKeyState(VK_RBUTTON))
		{

			tempWorldMatrix = TJMatrix::Matrix_Matrix_Multiply(TJMatrix::CreateRoatation_Y(10), tempWorldMatrix);
		}



		main_window.update();
	}
	main_window.destroy();
	functionExpert.ReleaseFBXPointers();
	return 0;
}