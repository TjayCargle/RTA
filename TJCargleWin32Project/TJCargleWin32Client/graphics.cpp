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
std::vector<Mesh> GetMeshVertex()
{
	return myMeshes;
}
void AddMeshToVertexList(Mesh someMesh)
{
	myMeshes.push_back(someMesh);
	ID3D11Buffer * someBuffer = nullptr;
	ID3D11Buffer * someOtherBuffer = nullptr;
	ID3D11Buffer * boneBuffer = nullptr;
	ID3D11Buffer * jointBuffer = nullptr;
	otherVertexBuffer.push_back(someBuffer);
	otherIndexBuffer.push_back(someOtherBuffer);
	boneVertexBuffer.push_back(boneBuffer);
	jointVertexBuffer.push_back(jointBuffer);
}
void RemoveMeshFromVertexList(std::string name2Remove)
{
	for (int i = myMeshes.size() - 1; i >= 0; i--)
	{
		if (myMeshes[i].name == name2Remove)
		{
		
				myMeshes.erase(myMeshes.begin() + i);
		
				otherVertexBuffer.erase(otherVertexBuffer.begin() + i);	
		
				otherIndexBuffer.erase(otherIndexBuffer.begin() + i);

				boneVertexBuffer.erase(boneVertexBuffer.begin() + i);

				jointVertexBuffer.erase(jointVertexBuffer.begin() + i);
			
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

			otherVertexBuffer.erase(otherVertexBuffer.begin() + i);

			otherIndexBuffer.erase(otherIndexBuffer.begin() + i);

			boneVertexBuffer.erase(boneVertexBuffer.begin() + i);

			jointVertexBuffer.erase(jointVertexBuffer.begin() + i);

			break;
		}
	}
}
void RemoveAllMeshFromVertexList()
{
	while (jointVertexBuffer.size() > 0)
	{
		myMeshes.erase(myMeshes.begin());

		otherVertexBuffer.erase(otherVertexBuffer.begin());

		otherIndexBuffer.erase(otherIndexBuffer.begin());

		boneVertexBuffer.erase(boneVertexBuffer.begin());

		jointVertexBuffer.erase(jointVertexBuffer.begin());
	}
}
void SetCamera(TJMatrix m)
{
	camera = m;
}

void SetView(TJMatrix m)
{
	view = m;
}

void SetProj(TJMatrix m)
{
	proj = m;
}
