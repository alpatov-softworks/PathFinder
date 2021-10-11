#pragma once
#include "../Utils/ImVec3.h"

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}


class CBaseEntity
{
public:

	union
	{
		DEFINE_MEMBER_N(int, m_iTeamNum, 0xB0);
		DEFINE_MEMBER_N(int, m_iHealth, 0xA8);
		DEFINE_MEMBER_N(int, m_fFlags, 0x037C);
		DEFINE_MEMBER_N(ImVec3, m_vecViewOffset, 0xfc);
		DEFINE_MEMBER_N(ImVec3, m_vecOrigin, 0x2BC);
		DEFINE_MEMBER_N(ImVec3, m_vecVelocity, 0x120);
		DEFINE_MEMBER_N(int, m_iMaxHealth, 0x17A0);
		DEFINE_MEMBER_N(bool, m_bDormant, 0x1AA);
		DEFINE_MEMBER_N(bool, m_bDead, 0xA5);
		DEFINE_MEMBER_N(bool, m_bBurning, 0xC92);
	};
void AimAt(ImVec3 targetpos)
	{
		ImVec3 myPos = this->m_vecOrigin + this->m_vecViewOffset;

	    float distance = sqrtf(powf(targetpos.x - myPos.x, 2) + powf(targetpos.y - myPos.y, 2) + powf(targetpos.z - myPos.z, 2));

		float pitch = -asinf((targetpos.z - myPos.z) / distance) * (180 / 3.1415926f);
		float yaw =   atan2f(targetpos.y - myPos.y, targetpos.x - myPos.x) * (180 / 3.1415926f);


		if ((-89.f <= pitch) && (pitch <= 89.f) && (-180.f <= yaw) && (yaw <= 180.f))
		{
			auto viewangles = (ImVec3*)( (DWORD)GetModuleHandle("engine.dll") + 0x465464);
			viewangles->x = pitch;
			viewangles->y = yaw;
		}
	}
};