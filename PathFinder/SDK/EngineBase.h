#pragma once
#include <Windows.h>
#include "../Utils/ImVec3.h"

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}


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
		DEFINE_MEMBER_N(viewmatrix, view_matrix, 0x59CB80);
		DEFINE_MEMBER_N(ImVec3, view_angles, 0x467474);
	};
	// return  *reinterpret_cast<viewmatrix*>(*reinterpret_cast<DWORD*>(this + 0x594F4C) + 0x2d4);
};

