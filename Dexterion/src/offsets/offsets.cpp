#include "offsets.hpp"
#pragma warning(disable: 4244)

inline bool clientDLL::load() {
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

inline bool offsets::load()
{
	clientDLL = json::readFromJsonFile(utils::getExePath(), json::offsetFile);

	if (clientDLL == 0)
		return 0;

	clientDLL = clientDLL["client.dll"];

	return 1;
}

bool loadJson()
{
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

uintptr_t CCSPlayerController::getListEntry() {
	listEntry = Driver.Read<uintptr_t>(entityList + (0x8 * (id & 0x7FFF) >> 9) + 0x10);
	return listEntry;
}

uintptr_t CCSPlayerController::getController() {
	value = Driver.Read<uintptr_t>(listEntry + 0x78 * (id & 0x1FF));
	return value;
}

int CCSPlayerController::getPawnHealth() {
	pawnHealth = Driver.Read<int>(value + clientDLL::CCSPlayerController_["m_iPawnHealth"]);
	return pawnHealth;
}

bool CCSPlayerController::isAlive()
{
	alive = Driver.Read<int>(value + clientDLL::CCSPlayerController_["m_iPawnHealth"]) > 0 && Driver.Read<int>(value + clientDLL::CCSPlayerController_["m_iPawnHealth"]) < 2000;
	return alive;
}

uintptr_t getAddressBase(uintptr_t entityList, uintptr_t playerPawn) {
	uintptr_t listEntrySecond = Driver.Read<uintptr_t>(entityList + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
	return listEntrySecond == 0
		? 0
		: Driver.Read<uintptr_t>(listEntrySecond + 120 * (playerPawn & 0x1FF));
}

bool CCSPlayerController::isSpectating(bool localPlayer)
{
	uintptr_t LocalPlayer = Driver.Read<uintptr_t>(baseAddy + offsets::clientDLL["dwLocalPlayerController"]);
	uintptr_t localPlayerPawn = Driver.Read<uintptr_t>(LocalPlayer + clientDLL::clientDLLOffsets["CBasePlayerController"]["fields"]["m_hPawn"]);
	uintptr_t list_entry2 = Driver.Read<uintptr_t>(entityList + 0x8 * ((localPlayerPawn & 0x7FFF) >> 9) + 16);
	if (!list_entry2)
		return false;

	const uintptr_t CSlocalPlayerPawn = Driver.Read<uintptr_t>(list_entry2 + 120 * (localPlayerPawn & 0x1FF));

	if (localPlayer)
		return this->getSpectating() == CSlocalPlayerPawn;
	return this->getSpectating() != 0;
}

uintptr_t CCSPlayerController::getSpectating()
{
	uint32_t spectatorPawn = Driver.Read<uint32_t>(value + clientDLL::clientDLLOffsets["CBasePlayerController"]["fields"]["m_hPawn"]);
	uintptr_t pawn = getAddressBase(entityList, spectatorPawn);

	uintptr_t obs = Driver.Read<uintptr_t>(pawn + clientDLL::clientDLLOffsets["C_BasePlayerPawn"]["fields"]["m_pObserverServices"]);
	uint64_t oTarget = Driver.Read<uint64_t>(obs + clientDLL::clientDLLOffsets["CPlayer_ObserverServices"]["fields"]["m_hObserverTarget"]);
	uintptr_t handle = getAddressBase(entityList, oTarget);

	if (obs)
		return spectatorTarget = handle;
	return spectatorTarget = 0;
}

std::uint32_t CCSPlayerController::getC_CSPlayerPawn() {
	C_CSPlayerPawn_ = Driver.Read<std::uint32_t>(value + clientDLL::CCSPlayerController_["m_hPlayerPawn"]);
	return C_CSPlayerPawn_;
}

uintptr_t CCSPlayerController::getPawnTeam() {
	pawnTeam = Driver.Read<uintptr_t>(value + clientDLL::C_BaseEntity_["m_iTeamNum"]);
	return pawnTeam;
}

std::string CCSPlayerController::getPawnName() {
	pawnNameAddress = Driver.Read<uintptr_t>(value + clientDLL::CCSPlayerController_["m_sSanitizedPlayerName"]);
	if (pawnNameAddress) {
		char buf[MAX_PATH] = {};
		Driver.ReadRaw(pawnNameAddress, buf, MAX_PATH);
		pawnName = std::string(buf);
	}
	else {
		pawnName = "Unknown";
	}
	return pawnName;
}



uintptr_t C_CSPlayerPawn::getListEntry() {
	listEntry = Driver.Read<uintptr_t>(entityList + 0x8 * ((value & 0x7FFF) >> 9) + 0x10);
	return listEntry;
}

uintptr_t C_CSPlayerPawn::getPlayerPawn() {
	playerPawn = Driver.Read<uintptr_t>(listEntry + 0x78 * (value & 0x1FF));
	return playerPawn;
}

uintptr_t C_CSPlayerPawn::getPlayerPawnByCrossHairID(int crossHairEntity) {
	uintptr_t crosshairEntityEntry = Driver.Read<uintptr_t>(entityList + 0x8 * (crossHairEntity >> 9) + 0x10);
	playerPawn = Driver.Read<uintptr_t>(crosshairEntityEntry + 0x78 * (crossHairEntity & 0x1FF));
	return playerPawn;
}

Vector3 C_CSPlayerPawn::getOrigin() {
	origin = Driver.Read<Vector3>(playerPawn + clientDLL::C_BasePlayerPawn_["m_vOldOrigin"]);
	return origin;
}

Vector3 C_CSPlayerPawn::getCameraPos() {
	cameraPos = Driver.Read<Vector3>(playerPawn + clientDLL::C_CSPlayerPawnBase_["m_vecLastClipCameraPos"]);
	return cameraPos;
}

Vector3 C_CSPlayerPawn::getEyePos() {
	eyePos = Driver.Read<Vector3>(playerPawn + clientDLL::C_BasePlayerPawn_["m_vOldOrigin"]) +
		Driver.Read<Vector3>(playerPawn + clientDLL::C_BaseModelEntity_["m_vecViewOffset"]);
	return eyePos;
}

uintptr_t C_CSPlayerPawn::getCGameSceneNode() {
	CGameSceneNode = Driver.Read<uintptr_t>(playerPawn + clientDLL::C_BaseEntity_["m_pGameSceneNode"]);
	return CGameSceneNode;
}

Vector3 C_CSPlayerPawn::getViewAngles() {
	viewAngles = Driver.Read<Vector3>(playerPawn + clientDLL::C_CSPlayerPawnBase_["m_angEyeAngles"]);
	return viewAngles;
}

Vector3 C_CSPlayerPawn::getPosition() {
	position = Driver.Read<Vector3>(playerPawn + clientDLL::CBaseAnimGraph_["m_vLastSlopeCheckPos"]);
	return position;
}

uint16_t C_CSPlayerPawn::getWeaponID() {
	C_CSWeaponBase = Driver.Read<uint64_t>(playerPawn + clientDLL::C_CSPlayerPawnBase_["m_pClippingWeapon"]);
	weaponID = Driver.Read<uint16_t>(C_CSWeaponBase + clientDLL::C_EconItemView_["m_iItemDefinitionIndex"] + clientDLL::C_AttributeContainer_["m_Item"] + clientDLL::C_EconEntity_["m_AttributeManager"]);
	return weaponID;
}

std::string C_CSPlayerPawn::getWeaponName() {
	C_CSWeaponBase = Driver.Read<uint64_t>(playerPawn + clientDLL::C_CSPlayerPawnBase_["m_pClippingWeapon"]);
	uint64_t weaponData = Driver.Read<uint64_t>(C_CSWeaponBase + clientDLL::C_BaseEntity_["m_nSubclassID"] + 0x8);
	uint64_t weaponNameAddress = Driver.Read<uint64_t>(weaponData + clientDLL::CCSWeaponBaseVData_["m_szName"]);

	if (!weaponNameAddress) {
		weaponName = "NULL";
	}
	else {
		char buf[MAX_PATH] = {};
		Driver.ReadRaw(weaponNameAddress, buf, MAX_PATH);
		weaponName = std::string(buf);
		if (weaponName.compare(0, 7, "weapon_") == 0) {
			weaponName = weaponName.substr(7, weaponName.length());
		}
	}
	return weaponName;
}

int C_CSPlayerPawn::getPawnHealth() {
	pawnHealth = Driver.Read<int>(playerPawn + clientDLL::C_BaseEntity_["m_iHealth"]);
	return pawnHealth;
}

uintptr_t C_CSPlayerPawn::getPawnTeam() {
	pawnTeam = Driver.Read<uintptr_t>(value + clientDLL::C_BaseEntity_["m_iTeamNum"]);
	return pawnTeam;
}

int C_CSPlayerPawn::getEntitySpotted() {
	spotted = Driver.Read<DWORD_PTR>(playerPawn + clientDLL::C_CSPlayerPawn_["m_entitySpottedState"] + clientDLL::EntitySpottedState_t_["m_bSpottedByMask"]);
	return spotted;
}

int C_CSPlayerPawn::getOwner() {
	owner = Driver.Read<DWORD_PTR>(playerPawn + clientDLL::C_BaseEntity_["m_hOwnerEntity"]);
	return owner;
}




uintptr_t LocalPlayer::getPlayerPawn() {
	playerPawn = Driver.Read<uintptr_t>(base + offsets::clientDLL["dwLocalPlayerPawn"]);
	return playerPawn;
}

uintptr_t LocalPlayer::getPlayerController() {
	playerController = Driver.Read<uintptr_t>(base + offsets::clientDLL["dwLocalPlayerController"]);
	return playerController;
}

uintptr_t LocalPlayer::getTeam() {
	team = Driver.Read<uintptr_t>(localPlayer + clientDLL::C_BaseEntity_["m_iTeamNum"]);
	return team;
}

Vector3 LocalPlayer::getCameraPos() {
	cameraPos = Driver.Read<Vector3>(playerPawn + clientDLL::C_CSPlayerPawnBase_["m_vecLastClipCameraPos"]);
	return cameraPos;
}

Vector3 LocalPlayer::getViewAngles() {
	viewAngles = Driver.Read<Vector3>(playerPawn + clientDLL::C_BasePlayerPawn_["v_angle"]);
	return viewAngles;
}

Vector3 LocalPlayer::getPosition() {
	position = Driver.Read<Vector3>(playerPawn + clientDLL::CBaseAnimGraph_["m_vLastSlopeCheckPos"]);
	return position;
}

Vector3 LocalPlayer::getOrigin() {
	origin = Driver.Read<Vector3>(playerPawn + clientDLL::C_BasePlayerPawn_["m_vOldOrigin"]);
	return origin;
}

Vector3 LocalPlayer::getEyePos() {
	eyepos = this->getOrigin() + Driver.Read<Vector3>(playerPawn + clientDLL::C_BaseModelEntity_["m_vecViewOffset"]);
	return eyepos;
}

int LocalPlayer::getFlags() {
	flags = Driver.Read<int>(playerPawn + clientDLL::C_BaseEntity_["m_fFlags"]);
	return flags;
}

C_UTL_VECTOR LocalPlayer::getAimPunchCache() {
	aimPunchCache = Driver.Read<C_UTL_VECTOR>(playerPawn + clientDLL::C_CSPlayerPawn_["m_aimPunchCache"]);
	return aimPunchCache;
}

Vector2 LocalPlayer::getAimPunchAngle() {
	aimPunchAngle = Driver.Read<Vector2>(playerPawn + clientDLL::C_CSPlayerPawn_["m_aimPunchAngle"]);
	return aimPunchAngle;
}

int LocalPlayer::getShotsFired() {
	shotsFired = Driver.Read<int>(playerPawn + clientDLL::C_CSPlayerPawn_["m_iShotsFired"]);
	return shotsFired;
}

int LocalPlayer::getEntitySpotted() {
	spotted = Driver.Read<DWORD_PTR>(playerPawn + clientDLL::C_CSPlayerPawn_["m_entitySpottedState"] + clientDLL::EntitySpottedState_t_["m_bSpottedByMask"]);
	return spotted;
}

bool LocalPlayer::getIsScoped() {
	isScoped = Driver.Read<DWORD_PTR>(playerPawn + clientDLL::C_CSPlayerPawn_["m_bIsScoped"]);
	return isScoped;
}



uintptr_t CGameSceneNode::getBoneArray() {
	boneArray = Driver.Read<uintptr_t>(value + clientDLL::CSkeletonInstance_["m_modelState"] + 0x80);
	return boneArray;
}

Vector3 CGameSceneNode::getOrigin() {
	origin = Driver.Read<Vector3>(value + clientDLL::CGameSceneNode_["m_vecAbsOrigin"]);
	return origin;
}



bool SharedFunctions::spottedCheck(C_CSPlayerPawn C_CSPlayerPawn, LocalPlayer localPlayer) {
	if (C_CSPlayerPawn.getEntitySpotted() & (1 << (localPlayer.playerPawn)) || localPlayer.getEntitySpotted() & (1 << (C_CSPlayerPawn.playerPawn))) return 1;
	return 0;
}

bool SharedFunctions::inGame(DWORD_PTR base) {
	uintptr_t gameRules = Driver.Read<uintptr_t>(base + offsets::clientDLL["dwGameRules"]);

	bool warmup = Driver.Read<bool>(gameRules + clientDLL::C_CSGameRules_["m_bWarmupPeriod"]);
	bool match = Driver.Read<bool>(gameRules + clientDLL::C_CSGameRules_["m_bHasMatchStarted"]);
	bool freeze = Driver.Read<bool>(gameRules + clientDLL::C_CSGameRules_["m_bFreezePeriod"]);

	return match;
}



bool C_C4::isPlanted() {
	planted = Driver.Read<bool>(base + offsets::clientDLL["dwPlantedC4"] - 0x8);
	return planted;
}

bool C_C4::isCarrier(int index) {
	m_bCarrier = false;
	return m_bCarrier;
}

uintptr_t C_C4::getCarrier() {
	m_pCarrier = 0x0;
	return m_pCarrier;
}

int C_C4::getPlantedSite()
{
	site = Driver.Read<int>(c4 + clientDLL::C_PlantedC4_["m_nBombSite"]);
	return site;
}

bool C_C4::isBeingDefused()
{
	isDefusing = Driver.Read<bool>(c4 + clientDLL::C_PlantedC4_["m_bBeingDefused"]);
	return isDefusing;
}

long C_C4::getDefuseTime()
{
	defuseTime = Driver.Read<long>(c4 + clientDLL::C_PlantedC4_["m_flDefuseCountDown"]);
	return defuseTime;
}

uintptr_t C_C4::getCGameSceneNode() {
	scene = Driver.Read<uintptr_t>(c4 + clientDLL::C_BaseEntity_["m_pGameSceneNode"]);
	return scene;
}

uint32_t CBasePlayerController::getDesiredFov()
{
	iDesiredFov = Driver.Read<uint32_t>(controller + clientDLL::CBasePlayerController_["m_iDesiredFov"]);
	return iDesiredFov;
}

uint64_t CBasePlayerController::getSteamId()
{
	steamId = Driver.Read<uint64_t>(controller + clientDLL::CBasePlayerController_["m_steamID"]);
	return steamId;
}
