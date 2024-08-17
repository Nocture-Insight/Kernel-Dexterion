#pragma once

#include "../lib/json/jsonOps.hpp"
#include "../util/functions.hpp"

namespace clientDLL {
	nlohmann::json clientDLLOffsets;

	nlohmann::json C_BaseEntity_;
	nlohmann::json C_BaseModelEntity_;
	nlohmann::json CCSPlayerController_;
	nlohmann::json CBasePlayerController_;
	nlohmann::json C_BasePlayerPawn_;
	nlohmann::json C_CSPlayerPawn_;
	nlohmann::json C_CSPlayerPawnBase_;
	nlohmann::json CBaseAnimGraph_;
	nlohmann::json C_EconItemView_;
	nlohmann::json C_AttributeContainer_;
	nlohmann::json C_EconEntity_;
	nlohmann::json CSkeletonInstance_;
	nlohmann::json CGameSceneNode_;
	nlohmann::json EntitySpottedState_t_;
	nlohmann::json C_CSGameRules_;
	nlohmann::json CCSWeaponBaseVData_;
	nlohmann::json CCSPlayerBase_CameraServices_;
	nlohmann::json C_PlantedC4_;

	bool load();
};


namespace offsets {
	nlohmann::json clientDLL;

	bool load();
};

bool loadJson();

nlohmann::json buttons;