#include "esp.hpp"

void ESP::Main(C_CSPlayerPawn C_CSPlayerPawn_, view_matrix_t viewMatrix, CCSPlayerController CCSPlayerController_, CGameSceneNode CGameSceneNode_) {
	if (!Gui::IsMenuOpen())
		if (!utils::IsGameWindowActive()) return;

	if (espConf.boundBox) ESP::BoundingBox(C_CSPlayerPawn_.origin, viewMatrix, CGameSceneNode_.boneArray);
	if (espConf.skeleton) ESP::SkeletonESP(viewMatrix, CGameSceneNode_.boneArray);
}

void ESP::BoundingBox(Vector3 origin, view_matrix_t viewMatrix, uintptr_t boneArray) {
	if (origin.IsZero()) return;

	Vector3 originalPTS = origin.worldToScreen(viewMatrix);
	std::cout << originalPTS.z << std::endl;
	if (originalPTS.z < 2250.f) return;

	Vector3 headPos = Driver.Read<Vector3>(boneArray + bones::head * 32);
	headPos.z = headPos.z + 15.f;
	Vector3 headPTS = headPos.worldToScreen(viewMatrix);

	float y = originalPTS.y - headPTS.y;
	float x = (y * espConf.width) / 10.f;

	ImColor boxColor = utils::Float4ToImColor(espConf.spottedColours);

	ImGui::GetBackgroundDrawList()->AddRect({ headPTS.x - x, headPTS.y }, { headPTS.x + x, originalPTS.y }, utils::Float4ToImColor(espConf.cornerColours), 0.f, ImDrawFlags_None, 1.f);
	ImGui::GetBackgroundDrawList()->AddRectFilled({ headPTS.x - x, headPTS.y }, { headPTS.x + x, originalPTS.y }, boxColor, 0.f, ImDrawFlags_None);
}

void ESP::SkeletonESP(view_matrix_t viewMatrix, uintptr_t boneArray) {
	ImColor SkeletonColor = utils::Float4ToImColor(espConf.skeletonColours);

	for (int i = 0; i < sizeof(boneConnections) / sizeof(boneConnections[0]); i++) {
		int bone1 = boneConnections[i].bone1;
		int bone2 = boneConnections[i].bone2;

		Vector3 VectorBone1 = Driver.Read<Vector3>(boneArray + bone1 * 32);
		Vector3 VectorBone2 = Driver.Read<Vector3>(boneArray + bone2 * 32);

		Vector3 b1 = VectorBone1.worldToScreen(viewMatrix);
		Vector3 b2 = VectorBone2.worldToScreen(viewMatrix);

		if (b1.z < 0.1f || b2.z < 0.1f) return;

		ImGui::GetBackgroundDrawList()->AddLine({ b1.x, b1.y }, { b2.x, b2.y }, SkeletonColor, 1.5f);
	}
}