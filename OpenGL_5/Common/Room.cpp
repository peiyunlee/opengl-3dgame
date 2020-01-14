#include "Room.h"

Room::Room() {
	roomState = LEVEL0;
	g_bAutoRotating = false;

	g_fElapsedTime = 0;
	g_fLightRadius = 5;
	g_fLightTheta = 0;
}

Room::~Room() {
	if (g_Light != NULL) delete[] g_Light;
	if (g_pLight != NULL) delete[] g_pLight;
	if (g_LightLine != NULL) delete[] g_LightLine;

	if (g_uiFTexID != NULL) delete[] g_uiFTexID;

	if (g_LeftWall != NULL) delete g_LeftWall;
	if (g_RightWall != NULL) delete g_RightWall;
	if (g_FrontWall != NULL) delete g_FrontWall;
	if (g_BackWall != NULL) delete g_BackWall;
	if (g_TopWall != NULL) delete g_TopWall;
	if (g_BottomWall != NULL) delete g_BottomWall;

	if (g_pDoor != NULL) delete[] g_pDoor;
}