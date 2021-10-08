#pragma once
#include <Windows.h>


class viewmatrix
{
public:
	float* operator[ ](int index)
	{
		return this->data[index];
	}

private:
	float data[4][4];
};

class EngineBase
{
public:

	union
	{

	};
	// return  *reinterpret_cast<viewmatrix*>(*reinterpret_cast<DWORD*>(this + 0x594F4C) + 0x2d4);
	viewmatrix GetViewMatrix()
	{
		return *(viewmatrix*)(*(DWORD*)(this + 0x594F4C) + 0x2D4);
	}
};

