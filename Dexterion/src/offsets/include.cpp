#include "include.hpp"

bool clientDLL::load() {
	clientDLLOffsets = json::readFromJsonFile(utils::getExePath(), json::clientDLLFile);

	if (clientDLLOffsets == 0)
		return 0;

	clientDLLOffsets = clientDLLOffsets["client.dll"]["classes"];

	C_BaseEntity_ = clientDLLOffsets["C_BaseEntity"]["fields"];
	C_BaseModelEntity_ = clientDLLOffsets["C_BaseModelEntity"]["fields"];
	CCSPlayerController_ = clientDLLOffsets["CCSPlayerController"]["fields"];
	CBasePlayerController_ = clientDLLOffsets["CBasePlayerController"]["fields"];
	C_BasePlayerPawn_ = clientDLLOffsets["C_BasePlayerPawn"]["fields"];
	C_CSPlayerPawn_ = clientDLLOffsets["C_CSPlayerPawn"]["fields"];
	C_CSPlayerPawnBase_ = clientDLLOffsets["C_CSPlayerPawnBase"]["fields"];
	CBaseAnimGraph_ = clientDLLOffsets["CBaseAnimGraph"]["fields"];
	C_EconItemView_ = clientDLLOffsets["C_EconItemView"]["fields"];
	C_AttributeContainer_ = clientDLLOffsets["C_AttributeContainer"]["fields"];
	C_EconEntity_ = clientDLLOffsets["C_EconEntity"]["fields"];
	CSkeletonInstance_ = clientDLLOffsets["CSkeletonInstance"]["fields"];
	CGameSceneNode_ = clientDLLOffsets["CGameSceneNode"]["fields"];
	EntitySpottedState_t_ = clientDLLOffsets["EntitySpottedState_t"]["fields"];
	C_CSGameRules_ = clientDLLOffsets["C_CSGameRules"]["fields"];
	CCSWeaponBaseVData_ = clientDLLOffsets["CCSWeaponBaseVData"]["fields"];
	CCSPlayerBase_CameraServices_ = clientDLLOffsets["CCSPlayerBase_CameraServices"]["fields"];
	C_PlantedC4_ = clientDLLOffsets["C_PlantedC4"]["fields"];

	return 1;
}

bool offsets::load()
{
	clientDLL = json::readFromJsonFile(utils::getExePath(), json::offsetFile);

	if (clientDLL == 0)
		return 0;

	clientDLL = clientDLL["client.dll"];

	return 1;
}

bool loadJson() {
	buttons = json::readFromJsonFile(utils::getExePath(), json::buttonsFile);

	if (buttons == 0)
		return 0;

	buttons = buttons["client.dll"];

	if (!offsets::load())
		return 0;

	if (!clientDLL::load())
		return 0;

	return 1;
}