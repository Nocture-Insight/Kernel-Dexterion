#include "entry.hpp"


void mainLoop(bool state, MemoryManagement::moduleData client) {
	CCSPlayerController		CCSPlayerController(client.base);
	CBasePlayerController	CBasePlayerController;
	C_CSPlayerPawn			C_CSPlayerPawn(client.base);
	CGameSceneNode			CGameSceneNode;
	LocalPlayer				LocalPlayer(client.base);
	C_C4					C_C4(client.base);

	view_matrix_t viewMatrix = Driver.Read<view_matrix_t>(client.base + offsets::clientDLL["dwViewMatrix"]);
	Vector3 baseViewAngles = Driver.Read<Vector3>(client.base + offsets::clientDLL["dwViewAngles"]);
	DWORD_PTR baseViewAnglesAddy = client.base + offsets::clientDLL["dwViewAngles"];
	uintptr_t entityList = Driver.Read<uintptr_t>(client.base + offsets::clientDLL["dwEntityList"]);

	for (int i = 0; i < 16; i++) {
		CCSPlayerController.id = i;
		CCSPlayerController.getListEntry();
		CCSPlayerController.getController();
		CCSPlayerController.getPawnName();

		LocalPlayer.getPlayerController();
		LocalPlayer.getPlayerPawn();

		if (strcmp(CCSPlayerController.pawnName.c_str(), "DemoRecorder") == 0 || LocalPlayer.getTeam() == CCSPlayerController.getPawnTeam()) continue;

		C_CSPlayerPawn.value = CCSPlayerController.getC_CSPlayerPawn();
		C_CSPlayerPawn.getListEntry();
		C_CSPlayerPawn.getPlayerPawn();
		C_CSPlayerPawn.getPawnHealth();

		if ((C_CSPlayerPawn.pawnHealth <= 0 || C_CSPlayerPawn.pawnHealth > 100) || LocalPlayer.playerPawn == C_CSPlayerPawn.playerPawn) continue;

		CGameSceneNode.value = C_CSPlayerPawn.getCGameSceneNode();

		if (espConf.state) {
			C_CSPlayerPawn.getOrigin();
			CGameSceneNode.getBoneArray();

			ESP::Main(C_CSPlayerPawn, viewMatrix, CCSPlayerController, CGameSceneNode);
		}
	}
}