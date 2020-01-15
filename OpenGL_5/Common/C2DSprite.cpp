#include "C2DSprite.h"

C2DSprite::C2DSprite()
{
	isAlpha = false;
	m_iNumVtx = QUAD_NUM;
	iNumVtx = m_iNumVtx;
	m_pPoints = NULL; m_pNormals = NULL; m_pTex1 = NULL;

	m_pPoints = new vec4[m_iNumVtx];
	m_pNormals = new vec3[m_iNumVtx];

	// 此處改成正面為面向正 Z 方向 
	m_pPoints[0] = vec4(-0.5f, -0.5f, 0.0f, 1.0f);
	m_pPoints[1] = vec4(0.5f, -0.5f, 0.0f, 1.0f);
	m_pPoints[2] = vec4(0.5f, 0.5f, 0.0f, 1.0f);
	m_pPoints[3] = vec4(-0.5f, -0.5f, 0.0f, 1.0f);
	m_pPoints[4] = vec4(0.5f, 0.5f, 0.0f, 1.0f);
	m_pPoints[5] = vec4(-0.5f, 0.5f, 0.0f, 1.0f);

	m_pNormals[0] = vec3(0, 0, 1.0f);  // Normal Vector 的 W 為 0
	m_pNormals[1] = vec3(0, 0, 1.0f);
	m_pNormals[2] = vec3(0, 0, 1.0f);
	m_pNormals[3] = vec3(0, 0, 1.0f);
	m_pNormals[4] = vec3(0, 0, 1.0f);
	m_pNormals[5] = vec3(0, 0, 1.0f);

	// Set shader's name
	SetShaderName("vsVtxNormal.glsl", "fsVtxNormal.glsl");

	// Create and initialize a buffer object 
	//CreateBufferObject_2DUI();

	// 預設為 -1.0f, 由 Normal 決定顏色
	m_fColor[0] = -1.0f; m_fColor[1] = -1.0f; m_fColor[2] = -1.0f; m_fColor[3] = 1;

	// Sprite 的左下與右上角的 local 座標
	m_OriginalLf = vec4(-0.5f, -0.5f, 0.0f, 1.0f);
	m_OriginalTR = vec4(0.5f, 0.5f, 0.0f, 1.0f);

	m_bPushed = false;

}

C2DSprite::C2DSprite(int i)
{
	isAlpha = false;
	m_iNumVtx = 66;
	iNumVtx = m_iNumVtx;
	m_pPoints = NULL; m_pNormals = NULL; m_pTex1 = NULL;

	m_pPoints = new vec4[m_iNumVtx];
	m_pNormals = new vec3[m_iNumVtx];

	float b = 1.0f;
	float a = b - 0.02f;
	for (int i = 0; i < 42; i++)
	{
		if (i % 2 == 0) {
			m_pPoints[i].x = 1.05f* cosf(M_PI*2.0f*(float)(i / 2) / 20.0f);
			m_pPoints[i].y = 1.05f* sinf(M_PI*2.0f*(float)(i / 2) / 20.0f);
		}
		else {
			m_pPoints[i].x = 1.0f* cosf(M_PI*2.0f*(float)(i / 2) / 20.0f);
			m_pPoints[i].y = 1.0f* sinf(M_PI*2.0f*(float)(i / 2) / 20.0f);
		}
		m_pPoints[i].z = 0.0f;
		m_pPoints[i].w = 1.0f;
		m_pNormals[i] = vec3(1.0f, 0, 0.0f);
	}

	m_pPoints[42] = vec4(-0.025f, 0.5f, 0.0f, 1.0f);
	m_pPoints[43] = vec4(-0.025f, 1.5f, 0.0f, 1.0f);
	m_pPoints[44] = vec4(0.025f, 0.5f, 0.0f, 1.0f);
	m_pPoints[45] = vec4(0.025f, 1.5f, 0.0f, 1.0f);
	m_pNormals[42] = vec3(1.0f, 0, 0.0f);
	m_pNormals[43] = vec3(1.0f, 0, 0.0f);
	m_pNormals[44] = vec3(1.0f, 0, 0.0f);
	m_pNormals[45] = vec3(1.0f, 0, 0.0f);

	m_pPoints[46] = vec4(-0.025f, -0.5f, 0.0f, 1.0f);
	m_pPoints[47] = vec4(-0.025f, -1.5f, 0.0f, 1.0f);
	m_pPoints[48] = vec4(0.025f, -0.5f, 0.0f, 1.0f);
	m_pPoints[49] = vec4(0.025f, -1.5f, 0.0f, 1.0f);
	m_pNormals[46] = vec3(1.0f, 0, 0.0f);
	m_pNormals[47] = vec3(1.0f, 0, 0.0f);
	m_pNormals[48] = vec3(1.0f, 0, 0.0f);
	m_pNormals[49] = vec3(1.0f, 0, 0.0f);

	m_pPoints[50] = vec4(0.5f, -0.025f, 0.0f, 1.0f);
	m_pPoints[51] = vec4(0.5f, 0.025f, 0.0f, 1.0f);
	m_pPoints[52] = vec4(1.5f, -0.025f, 0.0f, 1.0f);
	m_pPoints[53] = vec4(1.5f, 0.025f, 0.0f, 1.0f);
	m_pNormals[50] = vec3(1.0f, 0, 0.0f);
	m_pNormals[51] = vec3(1.0f, 0, 0.0f);
	m_pNormals[52] = vec3(1.0f, 0, 0.0f);
	m_pNormals[53] = vec3(1.0f, 0, 0.0f);

	m_pPoints[54] = vec4(-0.5f, -0.025f, 0.0f, 1.0f);
	m_pPoints[55] = vec4(-0.5f, 0.025f, 0.0f, 1.0f);
	m_pPoints[56] = vec4(-1.5f, -0.025f, 0.0f, 1.0f);
	m_pPoints[57] = vec4(-1.5f, 0.025f, 0.0f, 1.0f);
	m_pNormals[54] = vec3(1.0f, 0, 0.0f);
	m_pNormals[55] = vec3(1.0f, 0, 0.0f);
	m_pNormals[56] = vec3(1.0f, 0, 0.0f);
	m_pNormals[57] = vec3(1.0f, 0, 0.0f);

	m_pPoints[58] = vec4(-0.025f, -0.25f, 0.0f, 1.0f);
	m_pPoints[59] = vec4(-0.025f, 0.25f, 0.0f, 1.0f);
	m_pPoints[60] = vec4(0.025f, -0.25f, 0.0f, 1.0f);
	m_pPoints[61] = vec4(0.025f, 0.25f, 0.0f, 1.0f);
	m_pNormals[58] = vec3(1.0f, 0, 0.0f);
	m_pNormals[59] = vec3(1.0f, 0, 0.0f);
	m_pNormals[60] = vec3(1.0f, 0, 0.0f);
	m_pNormals[61] = vec3(1.0f, 0, 0.0f);

	m_pPoints[62] = vec4(-0.25f, -0.025f, 0.0f, 1.0f);
	m_pPoints[63] = vec4(-0.25f, 0.025f, 0.0f, 1.0f);
	m_pPoints[64] = vec4(0.25f, -0.025f, 0.0f, 1.0f);
	m_pPoints[65] = vec4(0.25f, 0.025f, 0.0f, 1.0f);
	m_pNormals[62] = vec3(1.0f, 0, 0.0f);
	m_pNormals[63] = vec3(1.0f, 0, 0.0f);
	m_pNormals[64] = vec3(1.0f, 0, 0.0f);
	m_pNormals[65] = vec3(1.0f, 0, 0.0f);


	// Set shader's name
	SetShaderName("vsVtxNormal.glsl", "fsVtxNormal.glsl");

	// Create and initialize a buffer object 
	//CreateBufferObject_2DUI();

	// 預設為 -1.0f, 由 Normal 決定顏色
	m_fColor[0] = -1.0f; m_fColor[1] = -1.0f; m_fColor[2] = -1.0f; m_fColor[3] = 1;

	// Sprite 的左下與右上角的 local 座標
	m_OriginalLf = vec4(-0.5f, -0.5f, 0.0f, 1.0f);
	m_OriginalTR = vec4(0.5f, 0.5f, 0.0f, 1.0f);

	m_bPushed = false;

}

C2DSprite::~C2DSprite()
{

}

void C2DSprite::Update(float dt, point4 vLightPos, color4 vLightI)
{

}

void C2DSprite::Update(float dt, const LightSource &Lights)
{

}

void C2DSprite::Update(float dt)
{
}

void C2DSprite::SetTRSMatrix(mat4 &mat)
{
	m_mxTRS = mat;
	m_bTRSUpdated = true;
	// 計算 Sprite 在世界座標的左下角與右上角的位置
	m_Lf = m_mxTRS*m_OriginalLf;
	m_TR = m_mxTRS*m_OriginalTR;
}

// 傳入的點已經是對應到 (-1,-1) (1,1) 之間的 2D 座標
bool C2DSprite::OnTouches(const vec2 &tp)
{
	if (m_Lf.x <= tp.x && tp.x <= m_TR.x && m_Lf.y <= tp.y && tp.y <= m_TR.y) // 該按鈕被按到
	{
		if (!m_bPushed) {
			// 變成灰階
			SetColor(vec4(0.5, 0.5, 0.5, 1.0f));
			m_bPushed = true;
		}
		else {
			m_bPushed = false;
			SetColor(m_DefaultColor);
		}
		return true;		// 傳回有點到按鈕
	}
	else return false;	// 傳回沒有點到按鈕
}

void C2DSprite::SetDefaultColor(vec4 vColor)
{
	m_DefaultColor = vColor;
	SetColor(vColor);
}

void C2DSprite::Draw()
{
	DrawingSetShader_2DUI();
	if(iNumVtx == QUAD_NUM)
		glDrawArrays(GL_TRIANGLES, 0, iNumVtx);
	else {
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 42);
		glDrawArrays(GL_TRIANGLE_STRIP, 42, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 46, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 50, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 54, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 58, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 62, 4);
	}
}

void C2DSprite::DrawW()
{
	DrawingWithoutSetShader();
	glDrawArrays(GL_TRIANGLES, 0, iNumVtx);


}
//void C2DSprite::Draw_UITexture()
//{
//	DrawingSetShader_2DUI_Texture();
//	glDrawArrays(GL_TRIANGLES, 0, QUAD_NUM);
//}