#pragma once

#include "../../config/config.hpp"
#include "../../offsets/offsets.hpp"
#include "../../gui/overlay.hpp"
#include "../../util/functions.hpp"

inline namespace ESP {
	void Main(C_CSPlayerPawn C_CSPlayerPawn_, view_matrix_t viewMatrix, CCSPlayerController CCSPlayerController_, CGameSceneNode CGameSceneNode_);
	void BoundingBox(Vector3 origin, view_matrix_t viewMatrix, uintptr_t boneArray);
	void SkeletonESP(view_matrix_t viewMatrix, uintptr_t boneArray);
}