#ifndef CSHAPE_H
#define CSHAPE_H

#define  _CRT_SECURE_NO_WARNINGS 1
#include "../Header/Angel.h"
#include "../SOIL/SOIL.h"
#include "TypeDefine.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

#define FLAT_SHADING    0
#define GOURAUD_SHADING 1

// GPU 的計算必須傳更多的參數進入 Shader

//#define PERVERTEX_LIGHTING
// 設定上面為註解就是啟動 PERPIXEL_LIGHTING

// 以下為開啟多層次貼圖
//#define MULTITEXTURE  NONE_MAP
//#define MULTITEXTURE  DIFFUSE_MAP  
//#define MULTITEXTURE  (DIFFUSE_MAP|LIGHT_MAP)  // For Example3LM
#define MULTITEXTURE  (DIFFUSE_MAP|LIGHT_MAP|NORMAL_MAP)// For Example 4 ~~

// 開啟以下的定義代表使用 Environment  Mapping
// 上面開啟 #define MULTITEXTURE  DIFFUSE_MAP  
// 使用 Environment Mapping 必須設定使用  vsCubeMapping.glsl 與 fsCubeMapping.glsl
// 目前並沒有在 Cubic Mapping 中納入 NORMAL_MAP 與 LIGHT_MAP
#define CUBIC_MAP 1		// 這個只用在 Example 5

// 當模型有執行 non-uniform scale 的操作時，必須透過計算反矩陣來得到正確的 Normal 方向
// 開啟以下的定義即可，目前 CPU 計算的有提供
// GPU 的部分則是設定成註解

// #define GENERAL_CASE 1 



#define Type_3DMax 'M'
#define Type_Blender 'B'
#define Type_Cinema_4D 'C'


class CShape 
{
protected:
	vec4 *m_pPoints;
	vec3 *m_pNormals;
	vec4 *m_pColors;
	vec2 *m_pTex1;

#if MULTITEXTURE >= LIGHT_MAP
	vec2 *m_pTex2;	// 新增第二張貼圖 for example 3
#endif
#if MULTITEXTURE >= NORMAL_MAP
	vec2 *m_pTex3;		// 新增第三張貼圖 for example 4
	vec3 *m_pTangentV;	// 新增 tangent vector for each vertex
#endif

	int  m_iNumVtx;

	GLfloat m_fColor[4]; // Object's color
	// For shaders' name
	char *m_pVXshader, *m_pFSshader;

	// For VAO
	GLuint m_uiVao;

	// For Shader
	GLuint  m_uiModelView, m_uiProjection, m_uiColor;
	GLuint  m_uiProgram;
	GLuint  m_uiBuffer;

	point4  m_vLightInView;	 // 光源在世界座標的位置
	GLuint  m_uiLightInView;	 // 光源在 shader 的位置
	GLuint  m_uiAmbient;		 // light's ambient  與 Object's ambient  與 ka 的乘積
	GLuint  m_uiDiffuse;		 // light's diffuse  與 Object's diffuse  與 kd 的乘積
	GLuint  m_uiSpecular;	 // light's specular 與 Object's specular 與 ks 的乘積
	GLuint  m_uiShininess;
	GLuint  m_uiLighting;	// 光源的個數
	GLuint  m_uiTexLayer;	// 貼圖的層次，預設就是一層 diffuse

	GLuint  m_isAlpha;	// 透明
	int isAlpha; //0false;1true


	// 將該物件目前為止出現的時間傳入，如果需要控制物件隨著時間改變，可以用這個變數
	GLuint  m_uiElapsedTime;	// 該物件出現到目前為止經過的時間
	GLfloat m_fElapsedTime;


#ifdef CUBIC_MAP
	GLuint  m_uiTRS;			// TRSMatrix 傳入 Pixel Shader 的位置
	GLuint  m_uiViewPos;		// ViewPoint 傳入 Pixel Shader 的位置
	point4  m_v4Eye;			// Camera 的位置

	GLuint  m_uiCubeMap;     // pixel shader 中的 CubeMap Texture Name 的位置
	GLuint  m_uiCubeMapTexName;  // 傳入 pixel shader 中的 Cube map 的貼圖編號， 
#endif

	LightSource m_Light1;

	color4 m_AmbientProduct;
	color4 m_DiffuseProduct;
	color4 m_SpecularProduct;
	int    m_iLighting;		// 設定是否要打燈
	int    m_iTexLayer;		// 設定貼圖的層次，0 表示沒有貼圖

	// For Matrices
	mat4    m_mxView, m_mxProjection, m_mxTRS;
	mat4    m_mxMVFinal;
	mat3    m_mxMV3X3Final;		// 使用在計算 物體旋轉後的新 Normal
	mat3		m_mxITMV;
	bool    m_bProjUpdated, m_bViewUpdated, m_bTRSUpdated;

	// For materials
	Material m_Material;

	// For Shading Mode
	int		m_iMode;	 // 0: Flat shading, 1: Gouraud shading, 0 for default

	void	CreateBufferObject();
	void	DrawingSetShader();
	void	DrawingWithoutSetShader();

public:
	CShape();
	virtual ~CShape();
	virtual void Draw() = 0;
	virtual void DrawW() = 0; // Drawing without setting shaders
	virtual void Update(float dt, const LightSource &lights) = 0;
	virtual void Update(float dt) = 0; // 不計算光源的照明

	void SetShaderName(const char vxShader[], const char fsShader[]);
	void SetShader(GLuint uiShaderHandle = MAX_UNSIGNED_INT);
	void SetColor(vec4 vColor);
	void SetViewMatrix(mat4 &mat);
	void SetProjectionMatrix(mat4 &mat);
	void SetTRSMatrix(mat4 &mat);
	mat4 GetTRSMatrix();
	void SetTextureLayer(int texlayer);

	// For setting materials 
	void SetMaterials(color4 ambient, color4 diffuse, color4 specular);
	void SetKaKdKsShini(float ka, float kd, float ks, float shininess); // ka kd ks shininess

	// For Lighting Calculation
	void SetShadingMode(int iMode) {m_iMode = iMode;}
	vec4 PhongReflectionModel(vec4 vPoint, vec3 vNormal, const LightSource &lights);


	void SetShader_2DUI(GLuint uiShaderHandle = MAX_UNSIGNED_INT);
	void CreateBufferObject_2DUI();
	void DrawingSetShader_2DUI();


	// For controlling texture mapping
	void SetMirror(bool uAxis, bool vAxis); // U軸 與 V軸 是否要鏡射
	void SetTiling(float uTiling, float vTiling);  // 對 U軸 與 V軸 進行拼貼的縮放
	void SetLightMapTiling(float uTiling, float vTiling);  // 對 LightMap U軸 與 V軸 進行拼貼的縮放

	void SetLightingDisable() {m_iLighting = 0;}

	// For Cube Map
#ifdef CUBIC_MAP
	void SetCubeMapTexName(GLuint uiTexName) {
		m_uiCubeMapTexName = uiTexName;
	}
	void SetViewPosition(point4 vEye) {
			m_v4Eye = vEye;
	}
#endif
};

#endif
