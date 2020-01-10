#ifndef MODELPOOL_H
#define MODELPOOL_H

#include "../header/Angel.h"
#include "CShape.h"

class ModelPool: public CShape
{
public:
	ModelPool(const char*  path, char type = Type_3DMax);
	~ModelPool() {};

	int Num;

	void Draw();
	void DrawW();
	//GLuint AddObj(char *ObjName);
	//GLuint GetTextureID(char *ObjName) {}; // 沒有實作
	void Update(float dt, point4 vLightPos, color4 vLightI);
	void Update(float dt, const LightSource &Lights) ;
	//void Update(const LightSource *Lights, float dt) ;
	void Update(float dt) ;
protected:
	
};

#endif