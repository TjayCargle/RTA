#include "graphics.h"

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
