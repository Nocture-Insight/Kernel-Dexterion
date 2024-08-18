#pragma once

#include "../../lib/json/jsonOps.hpp"
#include "../util/functions.hpp"
#include "../util/Vectors.h"
#include "../util/Memory.hpp"

struct C_UTL_VECTOR
{
	DWORD_PTR count = 0;
	DWORD_PTR data = 0;
};

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

	bool load();
};


inline namespace offsets {
	inline nlohmann::json clientDLL;

	bool load();
};

inline nlohmann::json buttons;

bool loadJson();

class CCSPlayerController {
public:
	uintptr_t entityList;
	uintptr_t baseAddy;
	int id;

	CCSPlayerController(uintptr_t base) {
		baseAddy = base;
		entityList = Driver.Read<uintptr_t>(base + offsets::clientDLL["dwEntityList"]);
	}

	uintptr_t listEntry;
	uintptr_t getListEntry();

	uintptr_t value;
	uintptr_t getController();

	int pawnHealth;
	int getPawnHealth();

	bool alive;
	bool isAlive();

	bool spectating;
	bool isSpectating(bool localPlayer);

	uintptr_t spectatorTarget;
	uintptr_t getSpectating();

	uintptr_t pawnTeam;
	uintptr_t getPawnTeam();

	uintptr_t pawnNameAddress;
	std::string pawnName;
	std::string getPawnName();

	// C_CSPlayerPawn
	std::uint32_t C_CSPlayerPawn_;
	std::uint32_t getC_CSPlayerPawn();
};

class CBasePlayerController {
public:
	uintptr_t controller;

	uint32_t iDesiredFov;
	uint32_t getDesiredFov();

	uint64_t steamId;
	uint64_t getSteamId();
};


class C_CSPlayerPawn {
public:
	std::uint32_t value;
	uintptr_t entityList;

	C_CSPlayerPawn(uintptr_t base) {
		entityList = Driver.Read<uintptr_t>(base + offsets::clientDLL["dwEntityList"]);
	}

	uintptr_t listEntry;
	uintptr_t getListEntry();

	uintptr_t playerPawn;
	uintptr_t getPlayerPawn();
	uintptr_t getPlayerPawnByCrossHairID(int crossHairEntity);

	Vector3 origin;
	Vector3 getOrigin();

	Vector3 cameraPos;
	Vector3 getCameraPos();

	Vector3 viewAngles;
	Vector3 getViewAngles();

	Vector3 position;
	Vector3 getPosition();

	Vector3 eyePos;
	Vector3 getEyePos();

	int pawnHealth;
	int getPawnHealth();

	uintptr_t pawnTeam;
	uintptr_t getPawnTeam();

	uint64_t C_CSWeaponBase;
	uint16_t weaponID;
	std::string weaponName;
	uint16_t getWeaponID();
	std::string getWeaponName();

	int spotted;
	int getEntitySpotted();

	int owner;
	int getOwner();

	// CGameSceneNode
	uintptr_t CGameSceneNode;
	uintptr_t getCGameSceneNode();

};

class CGameSceneNode {
public:
	uintptr_t value;

	uintptr_t boneArray;
	uintptr_t getBoneArray();

	Vector3 origin;
	Vector3 getOrigin();
};

class C_C4 {
public:
	uintptr_t c4;
	uintptr_t base;

	C_C4(uintptr_t baseAddy) {
		base = baseAddy;
		c4 = Driver.Read<uintptr_t>(Driver.Read<uintptr_t>(baseAddy + offsets::clientDLL["dwPlantedC4"]));
	}

	bool planted;
	bool isPlanted();

	bool m_bCarrier;
	bool isCarrier(int index);

	uintptr_t m_pCarrier;
	uintptr_t getCarrier();

	int site;
	int getPlantedSite();

	bool isDefusing;
	bool isBeingDefused();

	long defuseTime;
	long getDefuseTime();

	uintptr_t scene;
	uintptr_t getCGameSceneNode();
};

class LocalPlayer {
public:
	uintptr_t localPlayer;
	uintptr_t base;

	LocalPlayer(uintptr_t baseAddy) {
		base = baseAddy;
		localPlayer = Driver.Read<uintptr_t>(base + offsets::clientDLL["dwLocalPlayerController"]);
	}

	uintptr_t playerPawn;
	uintptr_t getPlayerPawn();

	uintptr_t playerController;
	uintptr_t getPlayerController();

	uintptr_t team;
	uintptr_t getTeam();

	Vector3 cameraPos;
	Vector3 getCameraPos();

	Vector3 origin;
	Vector3 getOrigin();

	Vector3 eyepos;
	Vector3 getEyePos();

	Vector3 viewAngles;
	Vector3 getViewAngles();

	Vector3 position;
	Vector3 getPosition();

	int flags;
	int getFlags();

	C_UTL_VECTOR aimPunchCache;
	C_UTL_VECTOR getAimPunchCache();

	Vector2 aimPunchAngle;
	Vector2 getAimPunchAngle();

	int shotsFired;
	int getShotsFired();

	int spotted;
	int getEntitySpotted();

	bool isScoped;
	bool getIsScoped();
};


// This is unrelated to all the other classes, but this is the most convenient way to share functions between features
class SharedFunctions {
public:
	static bool spottedCheck(C_CSPlayerPawn C_CSPlayerPawn, LocalPlayer localPlayer);
	static bool inGame(DWORD_PTR base);
};

namespace Shared {
	uint64_t steamId;
}