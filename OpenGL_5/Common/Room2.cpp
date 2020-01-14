#include "Room.h"

Room2::Room2(float x, float y, float z, point4 eye) {
	g_bAutoRotating = false;

	g_fElapsedTime = 0;
	g_fLightRadius = 6;
	g_fLightTheta = 0;

	g_fLightR = 0.85f;
	g_fLightG = 0.85f;
	g_fLightB = 0.85f;

	roomPosX = x;
	roomPosY = y;
	roomPosZ = z;

	timer1 = 0;
	timer2 = 0;

	LightGenerator(x, y, z, 1);
	ObjectGenerator(x, y, z, eye);
	DoorGenerator(x, y, z, 2);
	TextureGenerator(7);
}

Room2::~Room2() {
	if (g_pCar1 != NULL) delete g_pCar1;
	if (g_pCar2 != NULL) delete g_pCar2;
}

void Room2::LightGenerator(float px, float py, float pz,int count) {

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
		point4(px+0.0f, py+10.0f, pz+0.0f, 1.0f),   // position
		point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
		vec3(px+0.0f, py+0.0f, pz+0.0f),			  //spotDirection
		2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
		45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
		0.0f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
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

void Room2::ObjectGenerator(float px, float py, float pz, point4 eye) {
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
	g_TopWall->SetTextureLayer(0);
	g_TopWall->SetColor(vec4(0.6f));
	g_TopWall->SetTRSMatrix(mxT*RotateZ(180.0f)*Scale(20.0f, 1, 20.0f));
	g_TopWall->SetMaterials(vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(0.56f, 0.86f, 0.99f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
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
	g_LeftWall->SetTiling(1, 1);
	g_LeftWall->SetShader();

	vT.x = px + 10.0f; vT.y = py + 10.0f; vT.z = pz + 0;
	mxT = Translate(vT);
	g_RightWall = new CQuad;
	g_RightWall->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85, 0.85f, 0.85, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_RightWall->SetColor(vec4(0.6f));
	g_RightWall->SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(90.0f)*Scale(20.0f, 1, 20.0f));
	g_RightWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_RightWall->SetShadingMode(GOURAUD_SHADING);
	g_RightWall->SetTiling(1, 1);
	g_RightWall->SetShader();

	vT.x = px + 0.0f; vT.y = py + 10.0f; vT.z = pz + 10.0f;
	mxT = Translate(vT);
	g_FrontWall = new CQuad;
	g_FrontWall->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85, 0.85f, 0.85, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_FrontWall->SetColor(vec4(0.6f));
	g_FrontWall->SetTRSMatrix(mxT*RotateZ(180.0f)*RotateX(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_FrontWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_FrontWall->SetShadingMode(GOURAUD_SHADING);
	g_FrontWall->SetTiling(1, 1);
	g_FrontWall->SetShader();

	vT.x = px + 0.0f; vT.y = py + 10.0f; vT.z = pz + -10.0f;
	mxT = Translate(vT);
	g_BackWall = new CQuad;
	g_BackWall->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85, 0.85f, 0.85, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_BackWall->SetColor(vec4(0.6f));
	g_BackWall->SetTRSMatrix(mxT*RotateX(90.0f)*Scale(20.0f, 1, 20.0f));
	g_BackWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_BackWall->SetShadingMode(GOURAUD_SHADING);
	g_BackWall->SetTiling(1, 1);
	g_BackWall->SetShader();

	vT.x = px - 2.0f; vT.y = py + 0.5; vT.z = pz - 0.0f;
	mxT = Translate(vT);
	g_pCar1 = new ModelPool("Model/car1.obj", Type_3DMax);
	g_pCar1->SetTextureLayer(DIFFUSE_MAP);
	carInitPos1 = RotateY(0.0f)*mxT*Scale(0.025f, 0.025f, 0.025f);
	g_pCar1->SetTRSMatrix(carInitPos1);
	g_pCar1->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85, 0.85f, 0.85, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pCar1->SetKaKdKsShini(0.15f, 0.8f, 0.2f, 2);

	vT.x = px + 2.0f; vT.y = py + 0.5; vT.z = pz - 0.0f;
	mxT = Translate(vT);
	g_pCar2 = new ModelPool("Model/car1.obj", Type_3DMax);
	g_pCar2->SetTextureLayer(DIFFUSE_MAP);
	carInitPos2 = mxT*Scale(0.025f, 0.025f, 0.025f)*RotateY(180.0f);
	g_pCar2->SetTRSMatrix(carInitPos2);
	g_pCar2->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85, 0.85f, 0.85, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pCar2->SetKaKdKsShini(0.15f, 0.8f, 0.2f, 2);

	//vT.x = px - 10.0f; vT.y = py + 0.5; vT.z = pz -5.0f;
	//mxT = Translate(vT);
	//g_pCar3 = new ModelPool("Model/car1.obj", Type_3DMax);
	//g_pCar3->SetTextureLayer(DIFFUSE_MAP);
	//carInitPos3 = RotateY(90.0f)*mxT*Scale(0.025f, 0.025f, 0.025f);
	//g_pCar3->SetTRSMatrix(carInitPos3);
	//g_pCar3->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85, 0.85f, 0.85, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//g_pCar3->SetKaKdKsShini(0.15f, 0.8f, 0.2f, 2);
}

void Room2::SetProjectionMatrix(mat4 mpx) {

	for (int i = 0; i < lightCount; i++)
	{
		g_pLight[i].SetProjectionMatrix(mpx);
	}

	
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

	g_pCar1->SetProjectionMatrix(mpx);
	g_pCar2->SetProjectionMatrix(mpx);
}

void Room2::TextureGenerator(int count) {

	g_uiFTexID = new GLuint[count];
	auto texturepool = CTexturePool::create();

	g_uiFTexID[0] = texturepool->AddTexture("texture/mine/road2.png");
	g_uiFTexID[1] = texturepool->AddTexture("texture/mine/door.png");
	g_uiFTexID[2] = texturepool->AddTexture("texture/mine/street2.png");
	g_uiFTexID[3] = texturepool->AddTexture("texture/mine/street2noB.png");
	g_uiFTexID[4] = texturepool->AddTexture("texture/mine/street1.png");
	g_uiFTexID[5] = texturepool->AddTexture("texture/mine/Car1.png");
	g_uiFTexID[6] = texturepool->AddTexture("texture/mine/Car2.png");
}

void Room2::Draw(vec4 cameraPos) {
	for (int i = 0; i < lightCount; i++)
	{
		g_pLight[i].Draw();
	}


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[0]);
	g_BottomWall->Draw(); // 與 Diffuse Map 結合


	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[4]); // 與 Diffuse Map 結合
	g_RightWall->Draw();
	g_LeftWall->Draw();

	if (roomState == DONE) {
		glActiveTexture(GL_TEXTURE0); // select active texture 0
		glBindTexture(GL_TEXTURE_2D, g_uiFTexID[3]); // 與 Diffuse Map 結合
		g_BackWall->Draw();
	}

	else {
		glActiveTexture(GL_TEXTURE0); // select active texture 0
		glBindTexture(GL_TEXTURE_2D, g_uiFTexID[2]); // 與 Diffuse Map 結合
		g_BackWall->Draw();
	}
	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[2]); // 與 Diffuse Map 結合
	g_FrontWall->Draw();
	glBindTexture(GL_TEXTURE_2D, 0);

	g_TopWall->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[1]); // 與 Diffuse Map 結合
	for (int i = 0; i < doorCount; i++)
	{
		g_pDoor[i].Draw();
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[5]);
	g_pCar1->Draw(); // 與 Diffuse Map 結合
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[6]);
	g_pCar2->Draw(); // 與 Diffuse Map 結合
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Room2::UpdateLightPosition(float dt)
{
	mat4 mxT;
	// 每秒繞 Y 軸轉 90 度
	g_fElapsedTime += dt;
	g_fLightTheta = g_fElapsedTime*(float)M_PI_2;
	if (g_fLightTheta >= (float)M_PI*2.0f) {
		g_fLightTheta -= (float)M_PI*2.0f;
		g_fElapsedTime -= 4.0f;
	}

	g_Light[0].position.x = roomPosX + g_fLightRadius * cosf(g_fLightTheta);
	g_Light[0].position.z = roomPosZ + g_fLightRadius * sinf(g_fLightTheta);
	mxT = Translate(g_Light[0].position);
	g_pLight[0].SetTRSMatrix(mxT);
	//g_LightLine[0].UpdatePosition(g_Light[0].position, g_Light[0].spotTarget);

}

void Room2::SetViewMatrix(mat4 mvx, vec4 cameraViewPosition) {
	for (int i = 0; i < lightCount; i++)
	{
		g_pLight[i].SetViewMatrix(mvx);
	}

	g_pCar1->SetViewMatrix(mvx);
	g_pCar2->SetViewMatrix(mvx);

	g_BottomWall->SetViewMatrix(mvx);
	g_TopWall->SetViewMatrix(mvx);
	g_LeftWall->SetViewMatrix(mvx);
	g_RightWall->SetViewMatrix(mvx);
	g_FrontWall->SetViewMatrix(mvx);
	g_BackWall->SetViewMatrix(mvx);

	for (int i = 0; i < doorCount; i++)
	{
		g_pDoor[i].SetViewMatrix(mvx);
	}
}

void Room2::Update(float delta) {

	if (g_bAutoRotating) { // Part 2 : 重新計算 Light 的位置
		UpdateLightPosition(delta);
	}

	for (int i = 0; i < lightCount; i++)
	{
		g_pLight[i].Update(delta);
	}

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

	g_pCar1->Update(delta, g_Light[0]);
	g_pCar2->Update(delta, g_Light[0]);

	CarMove(delta);

}

void Room2::DoorGenerator(float px, float py, float pz, int count) {
	doorCount = count;
	g_pDoor = new CQuad[doorCount];
	mat4 mxT;
	vec4 vT;

	//door to room1
	vT.x = px - 9.999f; vT.y = py + 5.0f; vT.z = pz + 0.0f;
	mxT = Translate(vT);
	g_pDoor[0].SetShadingMode(GOURAUD_SHADING);
	g_pDoor[0].SetShader();
	g_pDoor[0].SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(-90.0f)*Scale(6, 1, 8));
	g_pDoor[0].SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor[0].SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[0].SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//door to room3
	vT.x = px +0.0f; vT.y = py + 5.0f; vT.z = pz - 9.999f;
	mxT = Translate(vT);
	g_pDoor[1].SetShadingMode(GOURAUD_SHADING);
	g_pDoor[1].SetShader();
	g_pDoor[1].SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(0.0f)*Scale(6, 1, 8));
	g_pDoor[1].SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor[1].SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[1].SetKaKdKsShini(0, 0.8f, 0.5f, 1);
}


void Room2::RotateBillboard(float g_fPhi) {

}

void Room2::TurnObj() {

}

float carSpeed = -3.0f;
void Room2::CarMove(float dt) {
	mat4 mxT1, mxT2;
	vec4 vT=0.0f;
	timer1 += dt;

	vT.x = carSpeed*timer1; vT.y = 0.0; vT.z = 0.0;
	mxT1 = Translate(vT);
	g_pCar1->SetTRSMatrix(mxT1*carInitPos1);

	vT.x = -carSpeed*timer1; vT.y = 0.0; vT.z = 0.0;
	mxT2 = Translate(vT);
	g_pCar2->SetTRSMatrix(mxT2*carInitPos2);
	if (timer1 >= 4.0f) {
		timer1 = 0;
		carSpeed = -carSpeed;
		carInitPos1 = mxT1*carInitPos1;
		carInitPos2 = mxT2*carInitPos2;
	}
}


void Room2::ChangeLevel(int tolevel, bool &rBtnGet, bool &gBtnGet, bool &bBtnGet) {
	switch (tolevel) {
	case 1:
		roomState = DONE;
		//換圖
		break;
	default:
		break;
	}
}