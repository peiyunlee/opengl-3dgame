#pragma once

#include "../header/Angel.h"

#include "ModelPool.h"
#include "CQuad.h"
#include "CWireSphere.h"
#include "CLineSegment.h"
#include "CTexturePool.h"

#include "CSolidCube.h"
#include "CSolidSphere.h"
#include "../png_loader.h"

class Room
{
private:
public:
	Room();
	~Room();

	int lightCount;
	int doorCount;
	float roomPosX,roomPosZ;

	bool g_bAutoRotating;

	float g_fLightR;
	float g_fLightG;
	float g_fLightB;

	float g_fElapsedTime;
	float g_fLightRadius;
	float g_fLightTheta;

	virtual void SetProjectionMatrix(mat4 mpx) = 0;
	virtual void SetViewMatrix(mat4 mvx,vec4 cameraViewPosition) = 0;
	virtual void Draw(vec4 cameraPos) = 0;
	virtual void Update(float delta) = 0;

	LightSource *g_Light;

	CWireSphere *g_pLight;
	CLineSegment *g_LightLine;

	// Texture 
	int g_iTexWidth, g_iTexHeight;
	GLuint *g_uiFTexID; // 三個物件分別給不同的貼圖

	virtual void RotateBillboard(float g_fPhi)=0;
protected:
	CQuad *g_LeftWall, *g_RightWall;
	CQuad *g_FrontWall, *g_BackWall;
	CQuad *g_BottomWall, *g_TopWall;

	CQuad *g_pDoor;

	virtual void LightGenerator(float x, float y, float z, int count) = 0;
	virtual void ObjectGenerator(float x, float y, float z, point4 eye) = 0;
	virtual void DoorGenerator(float x, float y, float z, int count) = 0;

};


class Room1 :public Room
{
private:

	// For Objects
	//CQuad		  *g_pFloor;
	//CSolidCube    *g_pCube;
	//CSolidSphere  *g_pSphere;



	ModelPool *g_pCat;
	ModelPool *g_pDeer;
	ModelPool *g_pRat;
	ModelPool *g_pWolf;

	// Texture 
	GLuint g_uiSphereCubeMap; // for Cubic Texture

public:
	Room1(float px, float py, float pz, point4 eye);
	~Room1();

	void SetProjectionMatrix(mat4 mpx);
	void SetViewMatrix(mat4 mvx, vec4 cameraViewPosition);
	void Draw(vec4 cameraPos);
	void Update(float delta);
	void RotateBillboard(float g_fPhi);
protected:
	void LightGenerator(float x, float y, float z, int count);
	void ObjectGenerator(float x, float y, float z, point4 eye);
	void TextureGenerator(int count);
	void UpdateLightPosition(float dt);
	void DoorGenerator(float px, float py, float pz, int count);
};

class Room2 :public Room
{
private:

	// For Objects
	//CQuad		  *g_pFloor;
	//CSolidCube    *g_pCube;
	//CSolidSphere  *g_pSphere;

	ModelPool *g_pCat;
	ModelPool *g_pDeer;
	ModelPool *g_pRat;
	ModelPool *g_pWolf;

	// Texture 
	GLuint g_uiSphereCubeMap; // for Cubic Texture

public:
	Room2(float px, float py, float pz, point4 eye);
	~Room2();

	void SetProjectionMatrix(mat4 mpx);
	void SetViewMatrix(mat4 mvx, vec4 cameraViewPosition);
	void Draw(vec4 cameraPos);
	void Update(float delta);
	void RotateBillboard(float g_fPhi);
protected:
	void LightGenerator(float x, float y, float z, int count);
	void ObjectGenerator(float x, float y, float z, point4 eye);
	void TextureGenerator(int count);
	void UpdateLightPosition(float dt);
	void DoorGenerator(float px, float py, float pz, int count);
};

class Room3 :public Room
{
private:

	// For Objects
	//CQuad		  *g_pFloor;
	//CSolidCube    *g_pCube;
	//CSolidSphere  *g_pSphere;

	CQuad *g_pSpiderFly, *g_pG, *g_pSpiderDown;

	// Texture 
	GLuint g_uiSphereCubeMap; // for Cubic Texture

public:
	Room3(float px, float py, float pz, point4 eye);
	~Room3();

	void SetProjectionMatrix(mat4 mpx);
	void SetViewMatrix(mat4 mvx, vec4 cameraViewPosition);
	void Draw(vec4 cameraPos);
	void Update(float delta);
	void RotateBillboard(float g_fPhi);
protected:
	void LightGenerator(float x, float y, float z, int count);
	void ObjectGenerator(float x, float y, float z, point4 eye);
	void TextureGenerator(int count);
	void UpdateLightPosition(float dt);
	void DoorGenerator(float px, float py, float pz, int count);
};

class Room4 :public Room
{
private:

	// For Objects
	//CQuad		  *g_pFloor;
	//CSolidCube    *g_pCube;
	//CSolidSphere  *g_pSphere;

	ModelPool *g_pCat;
	ModelPool *g_pDeer;
	ModelPool *g_pRat;
	ModelPool *g_pWolf;

	// Texture 
	GLuint g_uiSphereCubeMap; // for Cubic Texture

public:
	Room4(float px, float py, float pz, point4 eye);
	~Room4();

	void SetProjectionMatrix(mat4 mpx);
	void SetViewMatrix(mat4 mvx, vec4 cameraViewPosition);
	void Draw(vec4 cameraPos);
	void Update(float delta);
	void RotateBillboard(float g_fPhi);
protected:
	void LightGenerator(float x, float y, float z, int count);
	void ObjectGenerator(float x, float y, float z, point4 eye);
	void TextureGenerator(int count);
	void UpdateLightPosition(float dt);
	void DoorGenerator(float px, float py, float pz, int count);

};

class Room5 :public Room
{
private:

	// For Objects
	//CQuad		  *g_pFloor;
	//CSolidCube    *g_pCube;
	//CSolidSphere  *g_pSphere;



	ModelPool *g_pCat;
	ModelPool *g_pDeer;
	ModelPool *g_pRat;
	ModelPool *g_pWolf;

	// Texture 
	GLuint g_uiSphereCubeMap; // for Cubic Texture

public:
	Room5(float px, float py, float pz, point4 eye);
	~Room5();

	void SetProjectionMatrix(mat4 mpx);
	void SetViewMatrix(mat4 mvx, vec4 cameraViewPosition);
	void Draw(vec4 cameraPos);
	void Update(float delta);
	void RotateBillboard(float g_fPhi);
protected:
	void LightGenerator(float x, float y, float z, int count);
	void ObjectGenerator(float x, float y, float z, point4 eye);
	void TextureGenerator(int count);
	void UpdateLightPosition(float dt);
	void DoorGenerator(float px, float py, float pz, int count);
};

class Room6 :public Room
{
private:

	// For Objects
	//CQuad		  *g_pFloor;
	//CSolidCube    *g_pCube;
	//CSolidSphere  *g_pSphere;

	ModelPool *g_pCat;
	ModelPool *g_pDeer;
	ModelPool *g_pRat;
	ModelPool *g_pWolf;

	// Texture 
	GLuint g_uiSphereCubeMap; // for Cubic Texture

public:
	Room6(float px, float py, float pz, point4 eye);
	~Room6();

	void SetProjectionMatrix(mat4 mpx);
	void SetViewMatrix(mat4 mvx, vec4 cameraViewPosition);
	void Draw(vec4 cameraPos);
	void Update(float delta);
	void RotateBillboard(float g_fPhi);
protected:
	void LightGenerator(float x, float y, float z, int count);
	void ObjectGenerator(float x, float y, float z, point4 eye);
	void TextureGenerator(int count);
	void UpdateLightPosition(float dt);
	void DoorGenerator(float px, float py, float pz, int count);
};