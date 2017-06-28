#include "graphics.h"
#include <iterator>
#ifdef FSGD_USING_D3D11
#include "graphics_d3d11.hpp"
#endif

TJMatrix GetCamera()
{
	return camera;
}

TJMatrix GetView()
{
	return view;
}

TJMatrix GetProj()
{
	return proj;
}
std::vector<Mesh*> GetMeshVertex()
{
	return myMeshes;
}
void AddMeshToVertexList(Mesh *someMesh)
{
	myMeshes.push_back(someMesh);

	myFrameNumbers.push_back(0);
}
void RemoveMeshFromVertexList(std::string name2Remove)
{
	for (int i = myMeshes.size() - 1; i >= 0; i--)
	{
		if (myMeshes[i]->name == name2Remove)
		{
		
				myMeshes.erase(myMeshes.begin() + i);			
				break;
		}
	}
}
void RemoveMeshFromVertexList(int index)
{
	for (int i = myMeshes.size() - 1; i >= 0; i--)
	{
		if (i == index)
		{

			myMeshes.erase(myMeshes.begin() + i);

			break;
		}
	}
}
void RemoveAllMeshFromVertexList()
{
	while (myFrameNumbers.size() > 0)
	{
		myMeshes.erase(myMeshes.begin());
;

		myFrameNumbers.erase(myFrameNumbers.begin());


	
	}
}
void SetCamera(TJMatrix &m)
{
	camera = m;
}

void SetView(TJMatrix &m)
{
	view = m;
}

void SetProj(TJMatrix &m)
{
	proj = m;
}

void SetAutoBool(bool setAuto)
{
	AutoAnimate = setAuto;
}
bool GetAutoBool()
{
	return AutoAnimate;
}

void SetFrameNum(int setFrame, int index)
{
	SetAutoBool(false);
	animationFrameNum[index] = setFrame;
}
std::vector<int> GetFrameNum()
{
	return animationFrameNum;
}