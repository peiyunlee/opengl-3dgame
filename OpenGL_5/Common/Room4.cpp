#include "Room.h"

Room4::Room4(float x, float y, float z, point4 eye) {
	g_bAutoRotating = true;

	g_fElapsedTime = 0;
	g_fLightRadius = 2.0f;
	g_fLightTheta = 0;

	g_fLightR = 1.0f;
	g_fLightG = 1.0f;
	g_fLightB = 1.0f;

	roomPosX = x;
	roomPosY = y;
	roomPosZ = z;

	LightGenerator(x, y, z, 1);
	ObjectGenerator(x, y, z, eye);
	DoorGenerator(x, y, z, 3);
	TextureGenerator(11);
}

Room4::~Room4() {
	if (g_pSphere != NULL) delete g_pSphere;
	if (g_pEarth != NULL) delete g_pEarth;
	if (g_pSun != NULL) delete g_pSun;
	if (g_pMoon != NULL) delete g_pMoon;
}

void Room4::LightGenerator(float px, float py, float pz,int count) {

	mat4 mxT;
	lightCount = count;
	g_Light = new LightSource[lightCount];
	g_pLight = new CWireSphere[lightCount];
	g_LightLine = new CLineSegment[lightCount];

	g_Light[0] =
	{
		color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
		color4(g_fLightR, 0.0, 0.0, 1.0f), // diffuse
		color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
		point4(px+0.0f, py+8.0f, pz+0.0f, 1.0f),   // position
		point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
		vec3(px+0.0f, py+0.0f, pz+0.0f),			  //spotDirection
		2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
		180.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
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

void Room4::ObjectGenerator(float px, float py, float pz, point4 eye) {
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
	g_TopWall->SetMaterials(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_TopWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_TopWall->SetShadingMode(GOURAUD_SHADING);
	g_TopWall->SetShader();

	vT.x = px + -10.0f; vT.y = py + 10.0f; vT.z = pz + 0;
	mxT = Translate(vT);
	g_LeftWall = new CQuad;
	g_LeftWall->SetColor(vec4(0.6f));
	g_LeftWall->SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_LeftWall->SetMaterials(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_LeftWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_LeftWall->SetShadingMode(GOURAUD_SHADING);
	g_LeftWall->SetShader();

	vT.x = px + 10.0f; vT.y = py + 10.0f; vT.z = pz + 0;
	mxT = Translate(vT);
	g_RightWall = new CQuad;
	g_RightWall->SetMaterials(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_RightWall->SetColor(vec4(0.6f));
	g_RightWall->SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(90.0f)*Scale(20.0f, 1, 20.0f));
	g_RightWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_RightWall->SetShadingMode(GOURAUD_SHADING);
	g_RightWall->SetShader();

	vT.x = px + 0.0f; vT.y = py + 10.0f; vT.z = pz + 10.0f;
	mxT = Translate(vT);
	g_FrontWall = new CQuad;
	g_FrontWall->SetMaterials(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_FrontWall->SetColor(vec4(0.6f));
	g_FrontWall->SetTRSMatrix(mxT*RotateZ(180.0f)*RotateX(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_FrontWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_FrontWall->SetShadingMode(GOURAUD_SHADING);
	g_FrontWall->SetShader();

	vT.x = px + 0.0f; vT.y = py + 10.0f; vT.z = pz + -10.0f;
	mxT = Translate(vT);
	g_BackWall = new CQuad;
	g_BackWall->SetMaterials(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_BackWall->SetColor(vec4(0.6f));
	g_BackWall->SetTRSMatrix(mxT*RotateX(90.0f)*Scale(20.0f, 1, 20.0f));
	g_BackWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_BackWall->SetShadingMode(GOURAUD_SHADING);
	g_BackWall->SetShader();



	// For Reflecting Sphere
	g_pSphere = new CSolidSphere(1.0f, 24, 12);
	g_pSphere->SetTextureLayer(DIFFUSE_MAP);  // 使用 
	g_pSphere->SetCubeMapTexName(1);
	g_pSphere->SetViewPosition(eye);
	g_pSphere->SetShaderName("vsCubeMapping.glsl", "fsCubeMapping.glsl");
	g_pSphere->SetShader();
	vT.x = px + 0.0f; vT.y = py + 2.0f; vT.z = pz + 0.0f;
	mxT = Translate(vT);
	mxT._m[0][0] = 1.5f; mxT._m[1][1] = 1.5f; mxT._m[2][2] = 1.5f;
	g_pSphere->SetTRSMatrix(mxT*RotateX(0.0f));
	mxEmT = mxT;
	g_pSphere->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pSphere->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pSphere->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_pSphere->SetColor(vec4(0.9f, 0.9f, 0.9f, 1.0f));

	g_pSun =  new CSolidSphere(2, 12, 8);
	sunRevTimer = 0;
	g_pSun->SetTextureLayer(DIFFUSE_MAP);
	g_pSun->SetShader();
	vT.x = px + 0.0; vT.y = py + 8.0; vT.z = pz +0.0;
	mxT = Translate(vT)* Scale(0.5f, 0.5f, 0.5f);
	mxSunT = mxT;
	g_pSun->SetTRSMatrix(mxT);
	g_pSun->SetShadingMode(GOURAUD_SHADING);
	g_pSun->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pSun->SetKaKdKsShini(0.15f, 0.9f, 0.1f, 2);

	g_pEarth = new CSolidSphere(1, 12, 16);
	earthRevTimer = 0;earthRotTimer = 0;
	g_pEarth->SetTextureLayer(DIFFUSE_MAP);
	g_pEarth->SetShader();
	vT.x += 0.0; vT.y -=7.5f; vT.z +=15.0f;
	mxT = Translate(vT)* Scale(1.5f, 1.5f, 1.5f);
	mxEarthT = mxT;
	g_pEarth->SetTRSMatrix(mxSunT*mxEarthT);
	g_pEarth->SetShadingMode(GOURAUD_SHADING);
	g_pEarth->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pEarth->SetKaKdKsShini(0.15f, 0.9f, 0.1f, 2);

	g_pMoon = new CSolidSphere(0.5, 12, 8);
	moonRevTimer = 0;moonRotTimer = 0;
	g_pMoon->SetTextureLayer(DIFFUSE_MAP);
	g_pMoon->SetShader();
	vT.x += 0.0; vT.y -= 0.5f; vT.z += 3.0f;
	mxT = Translate(vT)* Scale(0.5f, 0.5f, 0.5f);
	mxMoonT = mxT;
	g_pMoon->SetTRSMatrix(mxSunT*mxEarthT*mxMoonT);
	g_pMoon->SetShadingMode(GOURAUD_SHADING);
	g_pMoon->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pMoon->SetKaKdKsShini(0.15f, 0.9f, 0.1f, 2);

}


void Room4::SetProjectionMatrix(mat4 mpx) {

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

	g_pSphere->SetProjectionMatrix(mpx);
	g_pEarth->SetProjectionMatrix(mpx);
	g_pSun->SetProjectionMatrix(mpx);
	g_pMoon->SetProjectionMatrix(mpx);
}

void Room4::TextureGenerator(int count) {

	g_uiFTexID = new GLuint[count];
	auto texturepool = CTexturePool::create();

	g_uiFTexID[0] = texturepool->AddTexture("texture/mine/OuterSky//NY.png");
	g_uiFTexID[1] = texturepool->AddTexture("texture/mine/door.png");
	g_uiFTexID[2] = texturepool->AddTexture("texture/mine/OuterSky/PY.png");
	g_uiFTexID[3] = texturepool->AddTexture("texture/mine/OuterSky/NX.png");
	g_uiFTexID[4] = texturepool->AddTexture("texture/mine/OuterSky/PX.png");
	g_uiFTexID[5] = texturepool->AddTexture("texture/mine/OuterSky/NZ.png");
	g_uiFTexID[6] = texturepool->AddTexture("texture/mine/OuterSky/PZ.png");
	g_uiFTexID[7] = texturepool->AddTexture("texture/mine/metal.png");
	g_uiFTexID[8] = texturepool->AddTexture("texture/earth.png");
	g_uiFTexID[9] = texturepool->AddTexture("texture/mine/Sun.png");
	g_uiFTexID[10] = texturepool->AddTexture("texture/mine/moon.png");
	g_uiSphereCubeMap = CubeMap_load_SOIL();
}

void Room4::Draw(vec4 cameraPos) {

	glEnable(GL_BLEND);  // 設定2D Texure Mapping 有作用
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < lightCount; i++)
	{
		g_pLight[i].Draw();
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[0]);
	g_BottomWall->Draw(); // 與 Diffuse Map 結合

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[3]); // 與 Diffuse Map 結合
	g_LeftWall->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[4]); // 與 Diffuse Map 結合
	g_RightWall->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[5]); // 與 Diffuse Map 結合
	g_FrontWall->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[6]); // 與 Diffuse Map 結合
	g_BackWall->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[2]); // 與 Diffuse Map 結合
	g_TopWall->Draw();
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[7]); // 與 Diffuse Map 結合
	glActiveTexture(GL_TEXTURE1); // select active texture 1
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_uiSphereCubeMap); // 與 Light Map 結合
	g_pSphere->Draw();
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[8]);
	g_pEarth->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[9]);
	g_pSun->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[10]);
	g_pMoon->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[1]); // 與 Diffuse Map 結合
	for (int i = 0; i < doorCount; i++)
	{
		g_pDoor[i].Draw();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Room4::UpdateLightPosition(float dt)
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

void Room4::SetViewMatrix(mat4 mvx, vec4 cameraViewPosition) {
	for (int i = 0; i < lightCount; i++)
	{
		g_pLight[i].SetViewMatrix(mvx);
	}

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

	g_pSphere->SetViewMatrix(mvx);
	g_pEarth->SetViewMatrix(mvx);
	g_pSun->SetViewMatrix(mvx);
	g_pMoon->SetViewMatrix(mvx);
}

void Room4::Update(float delta) {

	if (g_bAutoRotating) { // Part 2 : 重新計算 Light 的位置
		//UpdateLightPosition(delta);
		PlanetUpdate(delta);
	}

	g_pSphere->Update(delta, g_Light[0]);
	g_pEarth->Update(delta, g_Light[0]);
	g_pSun->Update(delta);
	g_pMoon->Update(delta, g_Light[0]);

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

}

void Room4::DoorGenerator(float px, float py, float pz, int count) {
	doorCount = count;
	g_pDoor = new CQuad[doorCount];
	mat4 mxT;
	vec4 vT;

	//door to room1
	vT.x = px +0.0f; vT.y = py + 5.0f; vT.z = pz + 9.999f;
	mxT = Translate(vT);
	g_pDoor[0].SetShadingMode(GOURAUD_SHADING);
	g_pDoor[0].SetShader();
	g_pDoor[0].SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(180.0f)*Scale(6, 1, 8));
	g_pDoor[0].SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor[0].SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[0].SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//door to room3
	vT.x = px + 9.999f; vT.y = py + 5.0f; vT.z = pz + 0.0f;
	mxT = Translate(vT);
	g_pDoor[1].SetShadingMode(GOURAUD_SHADING);
	g_pDoor[1].SetShader();
	g_pDoor[1].SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(90.0f)*Scale(6, 1, 8));
	g_pDoor[1].SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor[1].SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[1].SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//door to room5
	vT.x = px + 0.0f; vT.y = py + 5.0f; vT.z = pz -9.999f;
	mxT = Translate(vT);
	g_pDoor[2].SetShadingMode(GOURAUD_SHADING);
	g_pDoor[2].SetShader();
	g_pDoor[2].SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(0.0f)*Scale(6, 1, 8));
	g_pDoor[2].SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor[2].SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[2].SetKaKdKsShini(0, 0.8f, 0.5f, 1);
}


void Room4::RotateBillboard(float g_fPhi) {

}


void Room4::TurnObj() {

}

void Room4::PlanetUpdate(float dt) {
	mat4 mxSunRevy, mxEarthRevy, mxMoonRevy, mxY;
	mat4 mxEarth, mxSun, mxMoon;
	sunRevTimer += dt * 10.0f; // 太陽公轉，每秒轉 10 度

	mxSunRevy = RotateY(sunRevTimer); // 太陽公轉
	mxSun = mxSunRevy*mxSunT;
	mxSun = mxSunRevy;
	mxSun._m[0].w = mxSunT._m[0].w;
	mxSun._m[1].w = mxSunT._m[1].w;
	mxSun._m[2].w = mxSunT._m[2].w;
	g_pSun->SetTRSMatrix(mxSun); // 公轉，先位移再旋轉
	g_pSphere->SetTRSMatrix(mxEmT*RotateX(sunRevTimer));

	earthRevTimer += dt * 20.0f;		// 地球公轉，每秒轉 5 度 
	earthRotTimer += dt * 40.0f;		// 地球自轉，每秒轉 4 度 
	mxY = RotateX(earthRotTimer);		// 地球自轉
	mxEarthRevy = RotateY(earthRevTimer);

	//直接設定位移量，省去矩陣相乘的計算;
	mxEarth = mxEarthRevy*mxEarthT;
	mxEarth = mxEarthRevy;
	mxEarth._m[0].w = mxEarthT._m[0].w;
	mxEarth._m[1].w = mxEarthT._m[1].w;
	mxEarth._m[2].w = mxEarthT._m[2].w;
	g_pEarth->SetTRSMatrix(mxSun*mxEarth*mxY);

	moonRevTimer += dt * 40.0f;	// 月球公轉，每秒轉 10 度 
	moonRotTimer += dt * 60.0f;  // 月球自轉，每秒轉 6 度 
	mxY = RotateZ(moonRotTimer);	// 月球自轉
	mxMoonRevy = RotateY(moonRevTimer);

	// 直接設定位移量，省去矩陣相乘的計算;
	mxMoon = mxMoonRevy * mxMoonT;
	mxMoon = mxMoonRevy;
	mxMoon._m[0].w = mxMoonT._m[0].w;
	mxMoon._m[1].w = mxMoonT._m[1].w;
	mxMoon._m[2].w = mxMoonT._m[2].w;
	g_pMoon->SetTRSMatrix(mxSun*mxEarth*mxMoon*mxY);

}


void Room4::ChangeLevel(int tolevel, bool &rBtnGet, bool &gBtnGet, bool &bBtnGet) {
	switch (tolevel) {
	case 1:
		roomState = DONE;
		g_bAutoRotating = false;
		g_Light[0].diffuse.x = 1.0;
		g_Light[0].diffuse.y = 1.0;
		g_Light[0].diffuse.z = 1.0;
		break;
	default:
		break;
	}
}