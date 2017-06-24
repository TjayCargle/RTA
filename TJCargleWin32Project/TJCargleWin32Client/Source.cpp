#include "../TJCargleWin32Project/TJDLL.h"
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
	cout << "Controls:" << endl;
	cout << "Arrow Keys -> Move Up Down Left or Right" << endl;
	cout << "RShift and LShift-> move forward or backward" << endl;
	cout << "CTRL keys -> rotate left and right" << endl;
	cout << "Right and Left mouse -> move up and down" << endl;
	cout << "Numpad 4-> Decreese Frame Number" << endl;
	cout << "Numpad 5-> Toggles  Animating to the object" << endl;
	cout << "Numpad 6-> Increase Frame Number" << endl;
	cout << "ESC -> removes the Mage" << endl;
	cout << "Backspace -> removes a Mesh" << endl;
	cout << "Spacebar -> imports a Mage" << endl;
	cout << "Enter/Return -> imports a TeddyBear" << endl;

	bool autoRun = true;
	int animationFrameNumber = 0;

	functionExpert.SetupFbxManager();
	functionExpert.setFbxIORoot();
	functionExpert.setupFbxImporter("../Teddy_Run.fbx");
	functionExpert.importFbxscene("testScene");
	Mesh * testMesh;
	testMesh = functionExpert.getMeshFromFbx();
//	testMesh = functionExpert.LoadMeshAnimationData(testMesh);
	*testMesh = TJMatrix::ScaleMesh(*testMesh, 0.0003f);

	TJMatrix teddyTrans = TJMatrix::CreateTranslationMatrix(-0.1, 0, 0);
	*testMesh = TJMatrix::TranslateMesh(*testMesh, teddyTrans);
	testMesh->name = "Teddy";

	//TJColor testColor;
	//testColor.CreateFromUint(0x800000);
	//testColor.CreateFromUint(0xFF5733);
	//testColor.CreateFromUint(0xFF00FFFF);
	//testColor.CreateFromUint(0xFF0000FF);


	//system("pause");

	fsgd::window::properties main_window_props{ 1280u, 720u, false };
	fsgd::window main_window;

	fsgd::window::create(&main_window, main_window_props);
	TJMatrix translationMatrix, tempRotationMatrix, tempWorldMatrix;
	tempWorldMatrix.SetAsIdentiy();

	AddMeshToVertexList(testMesh);
	functionExpert.setupFbxImporter("../Mage_Idle.fbx");
	functionExpert.importFbxscene("testScene");

	Mesh * testMesh2;

	testMesh2 = functionExpert.getMeshFromFbx();
	testMesh2->name = "Mage";

	*testMesh2 = TJMatrix::ScaleMesh(*testMesh2, 0.009f);

	TJMatrix mageTrans = TJMatrix::CreateTranslationMatrix(0.1, 0, 0);
	*testMesh2 = TJMatrix::TranslateMesh(*testMesh2, mageTrans);
	AddMeshToVertexList(testMesh2);

	translationMatrix.SetAsTranslation(0, 0.05, 0);
	tempWorldMatrix = TJMatrix::Matrix_Matrix_Multiply(translationMatrix, tempWorldMatrix);
	translationMatrix.SetAsTranslation(0, 0, -0.5);
	tempWorldMatrix = TJMatrix::Matrix_Matrix_Multiply(translationMatrix, tempWorldMatrix);
	while (main_window.pump_events())
	{

		if (GetAsyncKeyState(VK_UP) & 0x0001)
		{
			translationMatrix.SetAsTranslation(0, 0.005, 0);
			tempWorldMatrix = TJMatrix::Matrix_Matrix_Multiply(translationMatrix, tempWorldMatrix);
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x0001)
		{
			translationMatrix.SetAsTranslation(0, -0.005, 0);
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

			Mesh * testMesh2;
			
			testMesh2 = functionExpert.getMeshFromFbx();
			testMesh2->name = "Mage";

			*testMesh2 = TJMatrix::ScaleMesh(*testMesh2, 0.09f);

			TJMatrix mageTrans = TJMatrix::CreateTranslationMatrix(0.3, 0, 0);
			*testMesh2 = TJMatrix::TranslateMesh(*testMesh2, mageTrans);
			AddMeshToVertexList(testMesh2);
		}

		if (GetAsyncKeyState(VK_RETURN))
		{
			functionExpert.setupFbxImporter("../Teddy_Run.fbx");
			functionExpert.importFbxscene("testScene");

			Mesh * atestMesh;

			atestMesh = functionExpert.getMeshFromFbx();
			atestMesh->name = "Teddy";

			*atestMesh = TJMatrix::ScaleMesh(*atestMesh, 0.003f);

			TJMatrix teddytrans = TJMatrix::CreateTranslationMatrix(-0.3, 0, 0);
			*atestMesh = TJMatrix::TranslateMesh(*atestMesh, teddytrans);
			AddMeshToVertexList(atestMesh);
		}
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			//RemoveMeshFromVertexList("Mage");
			break;
		}
		if (GetAsyncKeyState(VK_BACK))
		{
			RemoveMeshFromVertexList(0);
		}
		animationFrameNumber = GetFrameNum();
		autoRun = GetAutoBool();
		if (GetAsyncKeyState(VK_NUMPAD4))
		{
			animationFrameNumber--;
			SetFrameNum(animationFrameNumber);
		}
		if (GetAsyncKeyState(VK_NUMPAD5))
		{
			if(autoRun)
			{
				autoRun = false;
				SetAutoBool(autoRun);
			}
			else
			{
				animationFrameNumber = 0;
				SetFrameNum(animationFrameNumber);
				autoRun = true;
				SetAutoBool(autoRun);
			}
		}
		if (GetAsyncKeyState(VK_NUMPAD6))
		{
			animationFrameNumber++;
			SetFrameNum(animationFrameNumber);
		}

		
		main_window.update();
	}

	main_window.destroy();
	
	functionExpert.ReleaseFBXPointers();
	return 0;
}