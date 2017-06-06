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

	testMesh = TJMatrix::ScaleMesh(testMesh, 0.003f);

	TJMatrix teddyTrans = TJMatrix::CreateTranslationMatrix(-0.3, 0, 0);
	testMesh = TJMatrix::TranslateMesh(testMesh, teddyTrans);
	testMesh.name = "Teddy";

	


	//system("pause");

	fsgd::window::properties main_window_props{ 1280u, 720u, false };
	fsgd::window main_window;

	fsgd::window::create(&main_window, main_window_props);
	TJMatrix translationMatrix, tempRotationMatrix, tempWorldMatrix;
	tempWorldMatrix.SetAsIdentiy();

	AddMeshToVertexList(testMesh);
	functionExpert.setupFbxImporter("../Mage_Idle.fbx");
	functionExpert.importFbxscene("testScene");

	Mesh testMesh2;

	testMesh2 = functionExpert.getMeshFromFbx();
	testMesh2.name = "Mage";

	testMesh2 = TJMatrix::ScaleMesh(testMesh2, 0.09f);

	TJMatrix mageTrans = TJMatrix::CreateTranslationMatrix(0.3, 0, 0);
	testMesh2 = TJMatrix::TranslateMesh(testMesh2, mageTrans);
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

		if (GetAsyncKeyState(VK_SPACE))
		{
			functionExpert.setupFbxImporter("../Mage_Idle.fbx");
			functionExpert.importFbxscene("testScene2");

			Mesh testMesh2;
			
			testMesh2 = functionExpert.getMeshFromFbx();
			testMesh2.name = "Mage";

			testMesh2 = TJMatrix::ScaleMesh(testMesh2, 0.09f);

			TJMatrix mageTrans = TJMatrix::CreateTranslationMatrix(0.3, 0, 0);
			testMesh2 = TJMatrix::TranslateMesh(testMesh2, mageTrans);
			AddMeshToVertexList(testMesh2);
		}

		if (GetAsyncKeyState(VK_RETURN))
		{
			functionExpert.setupFbxImporter("../Teddy_Idle.fbx");
			functionExpert.importFbxscene("testScene");

			Mesh atestMesh;

			atestMesh = functionExpert.getMeshFromFbx();
			atestMesh.name = "Teddy";

			atestMesh = TJMatrix::ScaleMesh(atestMesh, 0.003f);

			TJMatrix teddytrans = TJMatrix::CreateTranslationMatrix(-0.3, 0, 0);
			atestMesh = TJMatrix::TranslateMesh(atestMesh, teddytrans);
			AddMeshToVertexList(atestMesh);
		}
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			RemoveMeshFromVertexList("Mage");
		}
		if (GetAsyncKeyState(VK_BACK))
		{
			RemoveMeshFromVertexList(0);
		}
		main_window.update();
	}

	main_window.destroy();
	
	functionExpert.ReleaseFBXPointers();
	return 0;
}