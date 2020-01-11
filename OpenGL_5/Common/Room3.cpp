#include "Room.h"

Room3::Room3(float x, float y, float z, point4 eye) {
	g_bAutoRotating = false;

	g_fElapsedTime = 0;
	g_fLightRadius = 6;
	g_fLightTheta = 0;

	g_fLightR = 0.85f;
	g_fLightG = 0.85f;
	g_fLightB = 0.85f;

	roomPosX = x;
	roomPosZ = y;

	LightGenerator(x, y, z, 1);
	ObjectGenerator(x, y, z, eye);
	DoorGenerator(x, y, z, 2);
	TextureGenerator(10);
}

Room3::~Room3() {
	if (g_pSpiderFly != NULL) delete g_pSpiderFly;
	if (g_pG != NULL) delete g_pG;
	if (g_pSpiderDown != NULL) delete g_pSpiderDown;
}

void Room3::LightGenerator(float px, float py, float pz,int count) {

	mat4 mxT;
	lightCount = count;
	g_Light = new LightSource[lightCount];
	g_pLight = new CWireSphere[lightCount];
	g_LightLine = new CLineSegment[lightCount];

	g_Light[0] =
	{
		color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
		color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
		color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
		point4(px+0.0f, py+6.0f, pz+0.0f, 1.0f),   // position
		point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
		vec3(px+0.0f, py+0.0f, pz+0.0f),			  //spotDirection
		2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
		45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
		1.0f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
		1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
		0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
		0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
	};

	for (int i = 0; i < lightCount; i++) {
		// 設定 代表 Light 的 WireSphere
		g_pLight[i].SetDefault(0.25f, 6, 3);
		g_pLight[i].SetLightingDisable();
		g_pLight[i].SetTextureLayer(NONE_MAP);	// 沒有貼圖
		g_pLight[i].SetShader();
		mxT = Translate(g_Light[i].position);
		g_pLight[i].SetTRSMatrix(mxT);
		g_pLight[i].SetColor(g_Light[i].diffuse);
	}
}

void Room3::ObjectGenerator(float px, float py, float pz, point4 eye) {
	mat4 mxT;
	vec4 vT;

	vT.x = px + 0.0f; vT.y = py + 0.0f; vT.z = pz + 0;
	mxT = Translate(vT);
	g_BottomWall = new CQuad;
#ifdef MULTITEXTURE
	g_BottomWall->SetTextureLayer(DIFFUSE_MAP);
#endif
	g_BottomWall->SetShader();
	g_BottomWall->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_BottomWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_BottomWall->SetColor(vec4(0.6f));
	g_BottomWall->SetTRSMatrix(mxT*Scale(20.0f, 1, 20.0f));
	//g_BottomWall->SetTiling(1, 1);
	g_BottomWall->SetShadingMode(GOURAUD_SHADING);

	vT.x = px + 0.0f; vT.y = py + 20.0f; vT.z = pz + 0;
	mxT = Translate(vT);
	g_TopWall = new CQuad;
	g_TopWall->SetColor(vec4(0.6f));
	g_TopWall->SetTRSMatrix(mxT*RotateZ(180.0f)*Scale(20.0f, 1, 20.0f));
	g_TopWall->SetMaterials(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 1), vec4(0.5f, 0.5f, 0.5f, 1.0f));
	g_TopWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_TopWall->SetShadingMode(GOURAUD_SHADING);
	g_TopWall->SetShader();

	vT.x = px + -10.0f; vT.y = py + 10.0f; vT.z = pz + 0;
	mxT = Translate(vT);
	g_LeftWall = new CQuad;
	g_LeftWall->SetColor(vec4(0.6f));
	g_LeftWall->SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_LeftWall->SetMaterials(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 1), vec4(0.5f, 0.5f, 0.5f, 1.0f));
	g_LeftWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_LeftWall->SetShadingMode(GOURAUD_SHADING);
	g_LeftWall->SetShader();

	vT.x = px + 10.0f; vT.y = py + 10.0f; vT.z = pz + 0;
	mxT = Translate(vT);
	g_RightWall = new CQuad;
	g_RightWall->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85, 0.85f, 0.85, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_RightWall->SetColor(vec4(0.6f));
	g_RightWall->SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(90.0f)*Scale(20.0f, 1, 20.0f));
	g_RightWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_RightWall->SetShadingMode(GOURAUD_SHADING);
	g_RightWall->SetShader();

	vT.x = px + 0.0f; vT.y = py + 10.0f; vT.z = pz + 10.0f;
	mxT = Translate(vT);
	g_FrontWall = new CQuad;
	g_FrontWall->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85, 0.85f, 0.85, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_FrontWall->SetColor(vec4(0.6f));
	g_FrontWall->SetTRSMatrix(mxT*RotateZ(180.0f)*RotateX(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_FrontWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_FrontWall->SetShadingMode(GOURAUD_SHADING);
	g_FrontWall->SetShader();

	vT.x = px + 0.0f; vT.y = py + 10.0f; vT.z = pz + -10.0f;
	mxT = Translate(vT);
	g_BackWall = new CQuad;
	g_BackWall->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85, 0.85f, 0.85, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_BackWall->SetColor(vec4(0.6f));
	g_BackWall->SetTRSMatrix(mxT*RotateX(90.0f)*Scale(20.0f, 1, 20.0f));
	g_BackWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_BackWall->SetShadingMode(GOURAUD_SHADING);
	g_BackWall->SetShader();

	//spiderfly
	vT.x = px + 1.0f; vT.y = py +8.0f; vT.z = pz + 0.05f;
	mxT = Translate(vT);
	g_pSpiderFly = new CQuad;
#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pSpiderFly->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif
	g_pSpiderFly->SetShadingMode(GOURAUD_SHADING);
	g_pSpiderFly->SetShader();
	g_pSpiderFly->SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(180.0f)*Scale(6, 1, 12));
	g_pSpiderFly->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pSpiderFly->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//G
	vT.x = px + 0.0f; vT.y = py + 10.0f; vT.z = pz + 0.0f;
	mxT = Translate(vT);
	g_pG = new CQuad;
#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pG->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif
	g_pG->SetShadingMode(GOURAUD_SHADING);
	g_pG->SetShader();
	g_pG->SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(180.0f)*Scale(4, 1, 8));
	g_pG->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pG->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//spiderdown
	vT.x = px + 5.0f; vT.y = py + 4.0f; vT.z = pz + 5.0f;
	mxT = Translate(vT);
	g_pSpiderDown = new CQuad;
#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pSpiderDown->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif
	g_pSpiderDown->SetShadingMode(GOURAUD_SHADING);
	g_pSpiderDown->SetShader();
	g_pSpiderDown->SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(180.0f)*Scale(6, 1, 8));
	g_pSpiderDown->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pSpiderDown->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
}

void Room3::SetProjectionMatrix(mat4 mpx) {

	for (int i = 0; i < lightCount; i++)
	{
		g_pLight[i].SetProjectionMatrix(mpx);
	}

	g_pSpiderFly->SetProjectionMatrix(mpx);
	g_pG->SetProjectionMatrix(mpx);
	g_pSpiderDown->SetProjectionMatrix(mpx);

	//for (int i = 0; i < LIGHTCOUNT; i++)
	//{
	//	g_pLight[i].SetProjectionMatrix(mpx);
	//	g_LightLine[i].SetProjectionMatrix(mpx);
	//}

	g_BottomWall->SetProjectionMatrix(mpx);
	g_TopWall->SetProjectionMatrix(mpx);
	g_LeftWall->SetProjectionMatrix(mpx);
	g_RightWall->SetProjectionMatrix(mpx);
	g_FrontWall->SetProjectionMatrix(mpx);
	g_BackWall->SetProjectionMatrix(mpx);

	for (int i = 0; i < doorCount; i++)
	{
		g_pDoor[i].SetProjectionMatrix(mpx);
	}
}

void Room3::TextureGenerator(int count) {

	g_uiFTexID = new GLuint[count];
	auto texturepool = CTexturePool::create();

	g_uiFTexID[0] = texturepool->AddTexture("texture/mine/spiderbg.png");
	g_uiFTexID[1] = texturepool->AddTexture("texture/mine/door.png");
	g_uiFTexID[2] = texturepool->AddTexture("texture/mine/spiderfly.png");
	g_uiFTexID[3] = texturepool->AddTexture("texture/mine/G.png");
	g_uiFTexID[4] = texturepool->AddTexture("texture/mine/spiderdown.png");
//#ifdef MULTITEXTURE
//	//g_uiFTexID[2] = texturepool->AddTexture("texture/lightMap1.png");
//	g_uiFTexID[2] = texturepool->AddTexture("texture/mine/forestfloorspecular.png");
//#endif
//	g_uiSphereCubeMap = CubeMap_load_SOIL();
}

void Room3::Draw(vec4 cameraPos) {

	glEnable(GL_BLEND);  // 設定2D Texure Mapping 有作用
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < lightCount; i++)
	{
		g_pLight[i].Draw();
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[0]);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, g_uiFTexID[2]);
	g_BottomWall->Draw(); // 與 Diffuse Map 結合
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[0]); // 與 Diffuse Map 結合
	g_LeftWall->Draw();
	g_RightWall->Draw();
	g_FrontWall->Draw();
	g_BackWall->Draw();
	glBindTexture(GL_TEXTURE_2D, 0);

	g_TopWall->Draw();


	//for (int i = 0; i < LIGHTCOUNT; i++)
	//{
	//	g_pLight[i].Draw();
	//	g_LightLine[i].Draw();
	//}

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[1]); // 與 Diffuse Map 結合
	for (int i = 0; i < doorCount; i++)
	{
		g_pDoor[i].Draw();
	}


	glDepthMask(GL_FALSE);

	if (cameraPos.z > roomPosX){

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_uiFTexID[3]);
		g_pG->Draw();
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_uiFTexID[2]);
		g_pSpiderFly->Draw();
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_uiFTexID[4]);
		g_pSpiderDown->Draw();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else {

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_uiFTexID[4]);
		g_pSpiderDown->Draw();
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_uiFTexID[2]);
		g_pSpiderFly->Draw();
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_uiFTexID[3]);
		g_pG->Draw();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glDisable(GL_BLEND);	// 關閉 Blending
	glDepthMask(GL_TRUE);	// 開啟對 Z-Buffer 的寫入操作
}

void Room3::UpdateLightPosition(float dt)
{
	mat4 mxT;
	// 每秒繞 Y 軸轉 90 度
	g_fElapsedTime += dt;
	g_fLightTheta = g_fElapsedTime*(float)M_PI_2;
	if (g_fLightTheta >= (float)M_PI*2.0f) {
		g_fLightTheta -= (float)M_PI*2.0f;
		g_fElapsedTime -= 4.0f;
	}

	g_Light[0].position.x = g_fLightRadius * cosf(g_fLightTheta);
	g_Light[0].position.z = g_fLightRadius * sinf(g_fLightTheta);
	mxT = Translate(g_Light[0].position);
	g_pLight[0].SetTRSMatrix(mxT);
	//g_LightLine[0].UpdatePosition(g_Light[0].position, g_Light[0].spotTarget);

}

void Room3::SetViewMatrix(mat4 mvx, vec4 cameraViewPosition) {

	for (int i = 0; i < lightCount; i++)
	{
		g_pLight[i].SetViewMatrix(mvx);
	}

	g_pSpiderFly->SetViewMatrix(mvx);
	g_pG->SetViewMatrix(mvx);
	g_pSpiderDown->SetViewMatrix(mvx);

	g_BottomWall->SetViewMatrix(mvx);
	g_TopWall->SetViewMatrix(mvx);
	g_LeftWall->SetViewMatrix(mvx);
	g_RightWall->SetViewMatrix(mvx);
	g_FrontWall->SetViewMatrix(mvx);
	g_BackWall->SetViewMatrix(mvx);
	//for (int i = 0; i < LIGHTCOUNT; i++)
	//{
	//	g_pLight[i].SetViewMatrix(mvx);
	//	g_LightLine[i].SetViewMatrix(mvx);

	//}

	for (int i = 0; i < doorCount; i++)
	{
		g_pDoor[i].SetViewMatrix(mvx);
	}
}

void Room3::Update(float delta) {

	if (g_bAutoRotating) { // Part 2 : 重新計算 Light 的位置
		UpdateLightPosition(delta);
	}

	for (int i = 0; i < lightCount; i++)
	{
		g_pLight[i].Update(delta);
	}

	//for (int i = 0; i < LIGHTCOUNT; i++)
	//{
	//	g_pLight[i].Update(delta);
	//	g_LightLine[i].UpdatePosition(g_Light[i].position, g_Light[i].spotTarget);
	//}

	//// 如果需要重新計算時，在這邊計算每一個物件的顏色
	g_pSpiderFly->Update(delta, g_Light[0]);
	g_pG->Update(delta, g_Light[0]);
	g_pSpiderDown->Update(delta, g_Light[0]);

	g_BottomWall->Update(delta, g_Light[0]);
	g_TopWall->Update(delta, g_Light[0]);
	g_LeftWall->Update(delta, g_Light[0]);
	g_RightWall->Update(delta, g_Light[0]);
	g_FrontWall->Update(delta, g_Light[0]);
	g_BackWall->Update(delta, g_Light[0]);

	for (int i = 0; i < doorCount; i++)
	{
		g_pDoor[i].Update(delta, g_Light[0]);
	}
}

void Room3::DoorGenerator(float px, float py, float pz, int count) {
	doorCount = count;
	g_pDoor = new CQuad[doorCount];
	mat4 mxT;
	vec4 vT;

	//door to room2
	vT.x = px +0.0f; vT.y = py + 5.0f; vT.z = pz + 9.999f;
	mxT = Translate(vT);
	g_pDoor[0].SetShadingMode(GOURAUD_SHADING);
	g_pDoor[0].SetShader();
	g_pDoor[0].SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(180.0f)*Scale(6, 1, 8));
	g_pDoor[0].SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor[0].SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[0].SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//door to room4
	vT.x = px - 9.999f; vT.y = py + 5.0f; vT.z = pz + 0.0f;
	mxT = Translate(vT);
	g_pDoor[1].SetShadingMode(GOURAUD_SHADING);
	g_pDoor[1].SetShader();
	g_pDoor[1].SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(-90.0f)*Scale(6, 1, 8));
	g_pDoor[1].SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor[1].SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[1].SetKaKdKsShini(0, 0.8f, 0.5f, 1);
}


void Room3::RotateBillboard(float g_fPhi) {
	mat4 mxT;
	mxT = g_pG->GetTRSMatrix();
	float ry;

	ry = (g_fPhi - (-3))*60.0f;

	g_pG->SetTRSMatrix(mxT*RotateY(ry));
}