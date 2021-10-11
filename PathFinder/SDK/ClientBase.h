#pragma once
#include "CBaseEntity.h"




class ClientBase
{
public:
	union
	{
		DEFINE_MEMBER_N(int, m_ForceForward, 0xC6D40C);
		DEFINE_MEMBER_N(CBaseEntity*, LocalPlayer, 0xC3F650);
	};


private:

};
