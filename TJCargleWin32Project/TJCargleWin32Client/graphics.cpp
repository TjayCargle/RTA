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
