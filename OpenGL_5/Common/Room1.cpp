#include "Room.h"

Room1::Room1(float x,float y,float z,point4 eye) {
	g_bAutoRotating = false;

	g_fElapsedTime = 0;
	g_fLightRadius = 6;
	g_fLightTheta = 0;

	g_fLightR = 1.0f;
	g_fLightG = 1.0f;
	g_fLightB = 1.0f;

	rGet = gGet = bGet = false;

	LightGenerator(x, y, z, 1);
	ObjectGenerator(x,y,z,eye);
	DoorGenerator(x, y, z,2);
	TextureGenerator(5);
}

Room1::~Room1() {
	//if (g_pCube != NULL) delete g_pCube;
	//if (g_pSphere != NULL) delete g_pSphere;

	if (g_pCat != NULL) delete g_pCat;
	if (g_pDeer != NULL) delete g_pDeer;
	if (g_pWolf != NULL) delete g_pWolf;
	if (g_pRat != NULL) delete g_pRat;
	if (g_pBtn1 != NULL) delete g_pBtn1;
	if (g_pBtn2 != NULL) delete g_pBtn2;
	if (g_pBtn3 != NULL) delete g_pBtn3;

	//if (g_uiFTexID != NULL) delete[] g_uiFTexID;
}

void Room1::LightGenerator(float px, float py, float pz, int count) {

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
		point4(px+0.0f, py+5.0f, pz+0.0f, 1.0f),   // position
		point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
		vec3(px + 0.0f, py + 0.0f, pz + 0.0f),			  //spotDirection
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

	//g_Light[0] =
	//{
	//	0,
	//	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	//	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	//	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	//	point4(g_fLightRadius, g_fLightRadius + 6.0, 0.0f, 1.0f),   // position
	//	vec3(0.0f, 0.0f, 0.0f),	  //spotTarget
	//	vec3(0.0f, 0.0f, 0.0f),  //spotDirection
	//	1.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	//	30.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	//	0.0f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	//	1,
	//};
	//g_Light[1] =
	//{
	//	1,
	//	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	//	color4(1, 0, 0.0, 1.0f), // diffuse
	//	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	//	point4(-2.0f, 5.0f, -2.0f, 1.0f),   // position
	//	vec3(-6.0f, 3.0f, -6.0f),			  //spotTarget
	//	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	//	1.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	//	60.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	//	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	//	1,
	//};
	//g_Light[2] =
	//{
	//	1,
	//	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	//	color4(0, 1.0, 0, 1.0f), // diffuse
	//	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	//	point4(4.0f, 3.0f, -4.0f, 1.0f),   // position
	//	vec3(10.0f, -1.0f, -10.0f),  //spotTarget
	//	vec3(0.0f, 0.0f, 0.0f),  //spotDirection
	//	1.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	//	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	//	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	//	1,
	//};
	//g_Light[3] =
	//{
	//	1,
	//	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	//	color4(0, 0, 1.0, 1.0f), // diffuse
	//	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	//	point4(-2.0f, 5.0f, 2.0f, 1.0f),   // position
	//	vec3(-10.0f, -1.0f, 10.0f),  //spotTarget
	//	vec3(0.0f, 0.0f, 0.0f),	  //spotDirection
	//	1.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	//	60.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	//	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	//	1,
	//};

	////代表光的物體
	//for (int i = 0; i < lightCount; i++)
	//{
	//	// 設定 代表 Light 的 WireSphere
	//	g_pLight[i].SetDefault(0.25f, 6, 3);
	//	g_pLight[i].SetShader();
	//	mxT = Translate(g_Light[i].position);
	//	g_pLight[i].SetTRSMatrix(mxT);
	//	g_pLight[i].SetColor(g_Light[i].diffuse);
	//	//g_pLight[i]->SetLightingDisable();
	//}


	//for (int i = 0; i < LIGHTCOUNT; i++)
	//{
	//	g_Light[i].UpdateDirection();
	//	g_LightLine[i].SetDefault(g_Light[i].position, g_Light[i].spotTarget, vec4(1, 0, 0, 1));
	//	g_LightLine[i].SetShader();
	//}

}

void Room1::ObjectGenerator(float px, float py, float pz, point4 eye) {
	mat4 mxT;
	vec4 vT;



	vT.x = px+0.0f; vT.y = py+0.0f; vT.z = pz+0;
	mxT = Translate(vT);
	g_BottomWall = new CQuad;
#ifdef MULTITEXTURE
	g_BottomWall->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif
	g_BottomWall->SetShader();
	g_BottomWall->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_BottomWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_BottomWall->SetColor(vec4(0.6f));
	g_BottomWall->SetTRSMatrix(mxT*Scale(20.0f, 1, 20.0f));
	g_BottomWall->SetTiling(5,5);
	g_BottomWall->SetShadingMode(GOURAUD_SHADING);

	vT.x = px+0.0f; vT.y = py+20.0f; vT.z = pz+0;
	mxT = Translate(vT);
	g_TopWall = new CQuad;
	g_TopWall->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_TopWall->SetColor(vec4(0.6f));
	g_TopWall->SetTRSMatrix(mxT*RotateZ(180.0f)*Scale(20.0f, 1, 20.0f));
	g_TopWall->SetMaterials(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 1), vec4(0.5f, 0.5f, 0.5f, 1.0f));
	g_TopWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_TopWall->SetShadingMode(GOURAUD_SHADING);
	g_TopWall->SetTiling(5, 5);
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

	vT.x = px + 7.0; vT.y = py + 0.5; vT.z = pz + -6.0;
	mxT = Translate(vT);
	g_pCat = new ModelPool("Model/cat.obj", Type_3DMax);
	g_pCat->SetTextureLayer(0);
	g_pCat->SetTRSMatrix(mxT*RotateY(225.0f)*Scale(0.004f, 0.004f, 0.004f));
	g_pCat->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.5, 0.5, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pCat->SetKaKdKsShini(0.15f, 0.8f, 0.2f, 2);


	vT.x = px + -6.0; vT.y = py + 0.5; vT.z = pz + -6.0;
	mxT = Translate(vT);
	g_pDeer = new ModelPool("Model/deer.obj", Type_3DMax);
	g_pDeer->SetTextureLayer(0);
	g_pDeer->SetTRSMatrix(mxT*RotateY(315.0f)*Scale(0.004f, 0.004f, 0.004f));
	g_pDeer->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.5, 0.5, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDeer->SetKaKdKsShini(0.15f, 0.8f, 0.2f, 2);

	vT.x = px + -6.0; vT.y = py + 0.5; vT.z = pz + 6.0;
	mxT = Translate(vT);
	g_pWolf = new ModelPool("Model/wolf.obj", Type_3DMax);
	g_pWolf->SetTextureLayer(0);
	g_pWolf->SetTRSMatrix(mxT*RotateY(45.0f)*Scale(0.007f, 0.007f, 0.007f));
	g_pWolf->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.5, 0.5, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pWolf->SetKaKdKsShini(0.15f, 0.8f, 0.2f, 2);

	vT.x = px + 0.0; vT.y = py + 0.5; vT.z = pz + -0.0;
	mxT = Translate(vT);
	g_pRat = new ModelPool("Model/rat.obj", Type_3DMax);
	g_pRat->SetTextureLayer(0);
	g_pRat->SetTRSMatrix(mxT*RotateY(45.0f)*Scale(0.015f, 0.015f, 0.015f));
	g_pRat->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.5, 0.5, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pRat->SetKaKdKsShini(0.15f, 0.8f, 0.2f, 2);

	vT.x = px + 5.0; vT.y = py + 0.5; vT.z = pz + -5.0;
	mxT = Translate(vT);
	g_pBtn1 = new ModelPool("Model/btn.obj", Type_3DMax);
	g_pBtn1->SetTextureLayer(0);
	g_pBtn1->SetTRSMatrix(mxT*RotateY(45.0f)*Scale(0.05f, 0.05f, 0.05f));
	g_pBtn1->SetMaterials(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(1.0, 1.0, 1.0f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pBtn1->SetKaKdKsShini(0.15f, 0.8f, 0.2f, 2);

	vT.x = px + 7.0; vT.y = py + 0.5; vT.z = pz + -4.0;
	mxT = Translate(vT);
	g_pBtn2 = new ModelPool("Model/btn.obj", Type_3DMax);
	g_pBtn2->SetTextureLayer(0);
	g_pBtn2->SetTRSMatrix(mxT*RotateY(45.0f)*Scale(0.05f, 0.05f, 0.05f));
	g_pBtn2->SetMaterials(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(1.0, 1.0, 1.0f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pBtn2->SetKaKdKsShini(0.15f, 0.8f, 0.2f, 2);

	vT.x = px + 5.0; vT.y = py + 0.5; vT.z = pz + -7.0;
	mxT = Translate(vT);
	g_pBtn3 = new ModelPool("Model/btn.obj", Type_3DMax);
	g_pBtn3->SetTextureLayer(0);
	g_pBtn3->SetTRSMatrix(mxT*RotateY(45.0f)*Scale(0.05f, 0.05f, 0.05f));
	g_pBtn3->SetMaterials(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(1.0, 1.0, 1.0f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pBtn3->SetKaKdKsShini(0.15f, 0.8f, 0.2f, 2);

}

void Room1::SetProjectionMatrix(mat4 mpx) {

	for (int i = 0; i < lightCount; i++)
	{
		g_pLight[i].SetProjectionMatrix(mpx);
	}

	g_pCat->SetProjectionMatrix(mpx);
	g_pDeer->SetProjectionMatrix(mpx);
	g_pWolf->SetProjectionMatrix(mpx);
	g_pRat->SetProjectionMatrix(mpx);
	g_pBtn1->SetProjectionMatrix(mpx);
	g_pBtn2->SetProjectionMatrix(mpx);
	g_pBtn3->SetProjectionMatrix(mpx);

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

void Room1::TextureGenerator(int count) {

	g_uiFTexID = new GLuint[count];
	auto texturepool = CTexturePool::create();

	g_uiFTexID[0] = texturepool->AddTexture("texture/mine/forestfloor.png");
	g_uiFTexID[1] = texturepool->AddTexture("texture/mine/door.png");
	g_uiFTexID[3] = texturepool->AddTexture("texture/metal.png");
	g_uiFTexID[4] = texturepool->AddTexture("texture/Masonry.Brick.normal.png");
	g_uiFTexID[5] = texturepool->AddTexture("texture/mine/forest2.png");
#ifdef MULTITEXTURE
	//g_uiFTexID[2] = texturepool->AddTexture("texture/lightMap1.png");
	g_uiFTexID[2] = texturepool->AddTexture("texture/mine/forestfloorspecular.png");
#endif
}

void Room1::Draw(vec4 cameraPos) {
	for (int i = 0; i < lightCount; i++)
	{
		g_pLight[i].Draw();
	}


	g_pCat->Draw();
	g_pDeer->Draw();
	g_pWolf->Draw();
	if (roomState == LEVEL0) {
		g_pRat->Draw();
	}
	else if (roomState == LEVEL1) {
		if(!rGet)
			g_pBtn1->Draw();
		if (!gGet)
			g_pBtn2->Draw();
		if (!bGet)
			g_pBtn3->Draw();
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[2]);
	g_BottomWall->Draw(); // 與 Diffuse Map 結合
	g_TopWall->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[5]); // 與 Diffuse Map 結合
	g_LeftWall->Draw();
	g_RightWall->Draw();
	g_FrontWall->Draw();
	g_BackWall->Draw();
	glBindTexture(GL_TEXTURE_2D, 0);



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
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Room1::UpdateLightPosition(float dt)
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

void Room1::SetViewMatrix(mat4 mvx, vec4 cameraViewPosition) {

	for (int i = 0; i < lightCount; i++)
	{
		g_pLight[i].SetViewMatrix(mvx);
	}

	g_pCat->SetViewMatrix(mvx);
	g_pDeer->SetViewMatrix(mvx);
	g_pWolf->SetViewMatrix(mvx);
	g_pRat->SetViewMatrix(mvx);
	g_pBtn1->SetViewMatrix(mvx);
	g_pBtn2->SetViewMatrix(mvx);
	g_pBtn3->SetViewMatrix(mvx);

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

void Room1::Update(float delta) {

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
	g_pCat->Update(delta, g_Light[0]);
	g_pDeer->Update(delta, g_Light[0]);
	g_pWolf->Update(delta, g_Light[0]);
	g_pRat->Update(delta, g_Light[0]);
	g_pBtn1->Update(delta, g_Light[0]);
	g_pBtn2->Update(delta, g_Light[0]);
	g_pBtn3->Update(delta, g_Light[0]);

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

void Room1::DoorGenerator(float px, float py, float pz,int count) {
	doorCount = count;
	g_pDoor = new CQuad[doorCount];
	mat4 mxT;
	vec4 vT;

	//door to room2
	vT.x = px + 9.999f; vT.y = py + 5.0f; vT.z = pz + 0.0f;
	mxT = Translate(vT);
	g_pDoor[0].SetShadingMode(GOURAUD_SHADING);
	g_pDoor[0].SetShader();
	g_pDoor[0].SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(90.0f)*Scale(6, 1, 8));
	g_pDoor[0].SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor[0].SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[0].SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//door to room4
	vT.x = px + 0.0f; vT.y = py + 5.0f; vT.z = pz - 9.999f;
	mxT = Translate(vT);
	g_pDoor[1].SetShadingMode(GOURAUD_SHADING);
	g_pDoor[1].SetShader();
	g_pDoor[1].SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(0.0f)*Scale(6, 1, 8));
	g_pDoor[1].SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor[1].SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[1].SetKaKdKsShini(0, 0.8f, 0.5f, 1);
}


void Room1::RotateBillboard(float g_fPhi) {

}

void Room1::TurnObj() {

}

void Room1::ChangeLevel(int tolevel, bool &rget, bool &gget, bool &bget) {
	mat4 mxT,mxObj;
	vec4 vT;
	switch (tolevel) {
	case 1:
		roomState = LEVEL1;
		mxObj = g_pCat->GetTRSMatrix();
		vT.x = -7.0; vT.y = 0.5; vT.z = 6.0;
		mxT = Translate(vT);
		g_pCat->SetTRSMatrix(mxT*mxObj);
		//老鼠消失
		//按鈕出現
		break;
	case 2:	//判斷燈光可否拿到按鈕
		if (g_Light[0].diffuse.x >= 1.0 && g_Light[0].diffuse.y <= 0.0 && g_Light[0].diffuse.z >= 1.0) {
			rGet = true;
			rget = rGet;
		}
		else if (g_Light[0].diffuse.x >= 1.0 && g_Light[0].diffuse.y >= 1.0 && g_Light[0].diffuse.z <= 0.0) {
			gGet = true;
			gget = gGet;
		}
		else if (g_Light[0].diffuse.x <= 0.0 && g_Light[0].diffuse.y >= 1.0 && g_Light[0].diffuse.z >= 1.0) {
			bGet = true;
			bget = bGet;
		}
		if (rget && gget && bget)
			roomState = LEVEL2;	//全部按鈕都拿到
		//room1->g_pLight[0].SetColor(room1->g_Light[0].diffuse);
		break;
	default:
		break;
	}
}