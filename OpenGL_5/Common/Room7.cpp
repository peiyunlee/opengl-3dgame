#include "Room.h"

Room7::Room7(float x, float y, float z, point4 eye) {
	g_bAutoRotating = false;

	g_fElapsedTime = 0;
	g_fLightRadius = 6;
	g_fLightTheta = 0;

	g_fLightR = 0.85f;
	g_fLightG = 0.85f;
	g_fLightB = 0.85f;

	LightGenerator(x, y, z, 1);
	ObjectGenerator(x, y, z, eye);
	DoorGenerator(x, y, z, 0);
	TextureGenerator(1);
}

Room7::~Room7() {
}

void Room7::LightGenerator(float px, float py, float pz,int count) {

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
		point4(px-2.0f, py+4.0f, pz+0.0f, 1.0f),   // position
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

void Room7::ObjectGenerator(float px, float py, float pz, point4 eye) {
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
	g_TopWall->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_TopWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_TopWall->SetShadingMode(GOURAUD_SHADING);
	g_TopWall->SetShader();

	vT.x = px + 10.0f; vT.y = py + 10.0f; vT.z = pz + 0;
	mxT = Translate(vT);
	g_RightWall = new CQuad;
	g_RightWall->SetColor(vec4(0.6f));
	g_RightWall->SetTRSMatrix(mxT*RotateY(0.0f)*RotateX(90.0f)*RotateZ(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_RightWall->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_RightWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_RightWall->SetShadingMode(GOURAUD_SHADING);
	g_RightWall->SetShader();


	vT.x = px + -10.0f; vT.y = py + 10.0f; vT.z = pz + 0;
	mxT = Translate(vT);
	g_LeftWall = new CQuad;
	g_LeftWall->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_LeftWall->SetColor(vec4(0.6f));
	g_LeftWall->SetTRSMatrix(mxT*RotateY(180.0f)*RotateX(90.0f)*RotateZ(90.0f)*Scale(20.0f, 1, 20.0f));
	g_LeftWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_LeftWall->SetShadingMode(GOURAUD_SHADING);
	g_LeftWall->SetShader();

	vT.x = px + 0.0f; vT.y = py + 10.0f; vT.z = pz + 10.0f;
	mxT = Translate(vT);
	g_FrontWall = new CQuad;
	g_FrontWall->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_FrontWall->SetColor(vec4(0.6f));
	g_FrontWall->SetTRSMatrix(mxT*RotateY(0.0f)*RotateZ(180.0f)*RotateX(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_FrontWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_FrontWall->SetShadingMode(GOURAUD_SHADING);
	g_FrontWall->SetShader();

	vT.x = px + 0.0f; vT.y = py + 10.0f; vT.z = pz + -10.0f;
	mxT = Translate(vT);
	g_BackWall = new CQuad;
	g_BackWall->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_BackWall->SetColor(vec4(0.6f));
	g_BackWall->SetTRSMatrix(mxT*RotateX(90.0f)*Scale(20.0f, 1, 20.0f));
	g_BackWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_BackWall->SetShadingMode(GOURAUD_SHADING);
	g_BackWall->SetShader();
}

void Room7::SetProjectionMatrix(mat4 mpx) {

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
}

void Room7::TextureGenerator(int count) {

	g_uiFTexID = new GLuint[count];
	auto texturepool = CTexturePool::create();

	g_uiFTexID[0] = texturepool->AddTexture("texture/mine/floor6.png");
}

void Room7::Draw(vec4 cameraPos) {

	for (int i = 0; i < lightCount; i++)
	{
		g_pLight[i].Draw();
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[0]);
	g_BottomWall->Draw(); // 與 Diffuse Map 結合

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[0]); // 與 Diffuse Map 結合
	g_LeftWall->Draw();
	g_FrontWall->Draw();
	g_BackWall->Draw();
	g_TopWall->Draw();
	g_RightWall->Draw();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Room7::UpdateLightPosition(float dt)
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

void Room7::SetViewMatrix(mat4 mvx, vec4 cameraViewPosition) {

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
}

void Room7::Update(float delta) {

	if (g_bAutoRotating) { // Part 2 : 重新計算 Light 的位置
		UpdateLightPosition(delta);
	}

	for (int i = 0; i < lightCount; i++)
	{
		g_pLight[i].Update(delta);
	}

	//// 如果需要重新計算時，在這邊計算每一個物件的顏色

	g_BottomWall->Update(delta, g_Light[0]);
	g_TopWall->Update(delta, g_Light[0]);
	g_LeftWall->Update(delta, g_Light[0]);
	g_RightWall->Update(delta, g_Light[0]);
	g_FrontWall->Update(delta, g_Light[0]);
	g_BackWall->Update(delta, g_Light[0]);
}

void Room7::DoorGenerator(float px, float py, float pz, int count) {
	doorCount = count;
}


void Room7::RotateBillboard(float g_fPhi) {

}


void Room7::TurnObj() {

}


void Room7::ChangeLevel(int tolevel, bool &rBtnGet, bool &gBtnGet, bool &bBtnGet) {
}