#pragma once

#include "../../lib/json/jsonOps.hpp"
#include "../util/functions.hpp"

inline namespace clientDLL {
	inline nlohmann::json clientDLLOffsets;

	inline nlohmann::json C_BaseEntity_;
	inline nlohmann::json C_BaseModelEntity_;
	inline nlohmann::json CCSPlayerController_;
	inline nlohmann::json CBasePlayerController_;
	inline nlohmann::json C_BasePlayerPawn_;
	inline nlohmann::json C_CSPlayerPawn_;
	inline nlohmann::json C_CSPlayerPawnBase_;
	inline nlohmann::json CBaseAnimGraph_;
	inline nlohmann::json C_EconItemView_;
	inline nlohmann::json C_AttributeContainer_;
	inline nlohmann::json C_EconEntity_;
	inline nlohmann::json CSkeletonInstance_;
	inline nlohmann::json CGameSceneNode_;
	inline nlohmann::json EntitySpottedState_t_;
	inline nlohmann::json C_CSGameRules_;
	inline nlohmann::json CCSWeaponBaseVData_;
	inline nlohmann::json CCSPlayerBase_CameraServices_;
	inline nlohmann::json C_PlantedC4_;

	inline bool load();
};


inline namespace offsets {
	inline nlohmann::json clientDLL;

	inline bool load();
};

bool loadJson();

inline nlohmann::json buttons;