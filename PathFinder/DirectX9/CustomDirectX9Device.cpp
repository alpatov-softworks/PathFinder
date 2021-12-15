#include "CustomDirectX9Device.h"


ImVec2 CustomDirect3D9Device::GetWindowSize()
{
	D3DVIEWPORT9 view_port;
	this->GetViewport(&view_port);

	return ImVec2(view_port.Width, view_port.Height);
}

ImVec3 CustomDirect3D9Device::WorldToScreen(ImVec3 pos)
{
	auto dwViewmatrix = reinterpret_cast<EngineBase*>(GetModuleHandle("engine.dll"))->view_matrix;
	ImVec2 window_size = this->GetWindowSize();

	float _x = dwViewmatrix[0][0] * pos.x + dwViewmatrix[0][1] * pos.y + dwViewmatrix[0][2] * pos.z + dwViewmatrix[0][3];
	float _y = dwViewmatrix[1][0] * pos.x + dwViewmatrix[1][1] * pos.y + dwViewmatrix[1][2] * pos.z + dwViewmatrix[1][3];
	float _z = dwViewmatrix[2][0] * pos.x + dwViewmatrix[2][1] * pos.y + dwViewmatrix[2][2] * pos.z + dwViewmatrix[2][3];
	float w  = dwViewmatrix[3][0] * pos.x + dwViewmatrix[3][1] * pos.y + dwViewmatrix[3][2] * pos.z + dwViewmatrix[3][3];


	float x = (window_size.x / 2 * (_x / w)) + ((_x / w) + window_size.x / 2);

	float y = -(window_size.y / 2 * (_y / w)) + ((_y / w) + window_size.y / 2);

	return ImVec3(x, y, w);
}