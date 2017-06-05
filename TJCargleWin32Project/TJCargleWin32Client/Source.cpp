#include "TJDLL.h"
#include "window.h"
#include <Windows.h>
#include <iostream>
using namespace std;
int main(void)
{
	double a = 3;
	double b = 4;
	TJDEV5LIB::Functions functionExpert;
	cout << "a + b = " <<
		functionExpert.Add(a, b) << endl;


	//system("pause");

	fsgd::window::properties main_window_props{ 1280u, 720u, false };
	fsgd::window main_window;

	fsgd::window::create(&main_window, main_window_props);
	TJMatrix translationMatrix, tempRotationMatrix, tempWorldMatrix;
	tempWorldMatrix.SetAsIdentiy();
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
		main_window.update();
	}
	main_window.destroy();

	return 0;
}