#pragma once
#include "../Utils/ImVec3.h"
#include <vector>
class CPather
{
public:
	CPather();
	void FindPath(std::vector<ImVec3> end_pos);
	void LoadPath();
};