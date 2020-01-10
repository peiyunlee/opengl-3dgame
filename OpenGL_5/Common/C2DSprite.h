#ifndef C2DSPRITE_H
#define C2DSPRITE_H

#include "../header/Angel.h"
#include "CShape.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

#define QUAD_NUM 6		// 2 faces, 2 triangles/face 

class C2DSprite : public CShape
{
public:
	C2DSprite();
	~C2DSprite();

	vec4  m_OriginalLf, m_OriginalTR;  // sprite 在local 座標的左上與右下兩個頂點座標
	vec4  m_Lf, m_TR;  // sprite 在世界座標的左上與右下兩個頂點座標
	vec4  m_DefaultColor;
	bool  m_bPushed;

	void Update(float dt, point4 vLightPos, color4 vLightI);
	void Update(float dt, const LightSource &Lights);
	void Update(const LightSource *, float) {};
	void Update(float dt); // 不計算光源的照明

	void SetTRSMatrix(mat4 &mat);
	void SetDefaultColor(vec4 vColor);
	bool getButtonStatus() { return m_bPushed; }
	bool OnTouches(const vec2 &tp);

	GLuint GetShaderHandle() { return m_uiProgram; }

	void Draw();
	void DrawW();
};




#endif