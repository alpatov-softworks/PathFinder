#pragma once
#include "CBaseEntity.h"




class ClientBase
{
public:
	union
	{
		DEFINE_MEMBER_N(int, m_ForceForward, 0xC6D47C);
		DEFINE_MEMBER_N(CBaseEntity*, LocalPlayer, 0xC3F6D0);
	};

private:

};
