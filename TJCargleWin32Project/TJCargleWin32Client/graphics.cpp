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
	ID3D11Buffer * someBuffer;
	ID3D11Buffer * someOtherBuffer;
	ID3D11Buffer * boneBuffer;
	otherVertexBuffer.push_back(someBuffer);
	otherIndexBuffer.push_back(someOtherBuffer);
	boneVertexBuffer.push_back(boneBuffer);
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
