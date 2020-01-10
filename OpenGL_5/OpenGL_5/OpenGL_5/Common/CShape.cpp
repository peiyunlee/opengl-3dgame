#include "CShape.h"

CShape::CShape()
{
	m_bTRSUpdated = m_bViewUpdated = m_bProjUpdated = false;

	// ambient 預設為 0, diffuse, specular 的顏色都是灰色 0.5
	// Ka = 0 係數 , kd = 0.8 , ks = 0.2
	m_Material.ambient = vec4(vec3(0));
	m_Material.diffuse = vec4(vec3(0.5f));
	m_Material.specular = vec4(vec3(0.5f));
	m_Material.ka = 0; m_Material.kd = 0.8f; m_Material.ks = 0.2f;
	m_Material.shininess = 2.0f;

	m_iMode = FLAT_SHADING;
	// 預設為 RGBA 為 (0.5,0.5,0.5,1.0) , 由這個灰階顏色來代表的物件顏色
	m_fColor[0] = 0.5f; m_fColor[1] = 0.5f; m_fColor[2] = 0.5f; m_fColor[3] = 1.0f;

	m_iLighting = 1;		// 預設接受燈光的照明
	m_iTexLayer = 1;		// 預設有一張 Diffuse 貼圖

	m_fElapsedTime = 0;		// 預設時間從 0 開始

	m_pPoints = nullptr; 	m_pNormals = nullptr; 	m_pColors = nullptr; 	m_pTex1 = nullptr;
#if MULTITEXTURE >= LIGHT_MAP
	m_pTex2 = nullptr;	
#endif
#if MULTITEXTURE >= NORMAL_MAP
	m_pTex3 = nullptr;
	m_pTangentV = nullptr;
#endif
	 
}

CShape::~CShape()
{
	if( m_pPoints  != NULL ) delete [] m_pPoints;  
	if( m_pNormals != NULL ) delete	[] m_pNormals;
	if( m_pColors  != NULL ) delete	[] m_pColors;
	if (m_pTex1 != NULL) delete m_pTex1;
#if MULTITEXTURE >= LIGHT_MAP
	if (m_pTex2 != NULL) delete m_pTex2;		// Example 3
#endif
#if MULTITEXTURE  >= NORMAL_MAP
	if (m_pTex3 != NULL) delete m_pTex3;		// Example 4
	if (m_pTangentV != NULL) delete[] m_pTangentV;
#endif

	if( m_pVXshader != NULL ) delete [] m_pVXshader;
	if( m_pFSshader != NULL ) delete [] m_pFSshader;
}

void CShape::CreateBufferObject()
{
    glGenVertexArrays( 1, &m_uiVao );
    glBindVertexArray( m_uiVao );

    // Create and initialize a buffer object
    glGenBuffers( 1, &m_uiBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );

#if MULTITEXTURE < DIFFUSE_MAP
	glBufferData(GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3)+sizeof(vec4))*m_iNumVtx, NULL, GL_STATIC_DRAW);
#elif MULTITEXTURE <= DIFFUSE_MAP
    glBufferData( GL_ARRAY_BUFFER, (sizeof(vec4) + sizeof(vec3) + sizeof(vec4) + sizeof(vec2))*m_iNumVtx, NULL, GL_STATIC_DRAW );
	// (sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx + sizeof(vec4)*m_iNumVtx <- vertices, normal and color
	// (sizeof(vec4) + sizeof(vec3) + sizeof(vec4) + sizeof(vec2))*m_iNumVtx <- vertices, normal, color and texture coordinates
#elif MULTITEXTURE <= (LIGHT_MAP | DIFFUSE_MAP)
	glBufferData( GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3)+sizeof(vec4)+sizeof(vec2)*2)*m_iNumVtx, NULL, GL_STATIC_DRAW); // 包含兩張貼圖
#elif MULTITEXTURE <= (LIGHT_MAP|DIFFUSE_MAP|NORMAL_MAP)
	glBufferData(GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3)+sizeof(vec4)+sizeof(vec2)* 3 + sizeof(vec3))*m_iNumVtx, NULL, GL_STATIC_DRAW); // 包含三張貼圖與 tangent
#endif

	// vertices
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec4)*m_iNumVtx, m_pPoints );  
	// vertices' normal
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx, sizeof(vec3)*m_iNumVtx, m_pNormals );
	// vertcies' Color
	glBufferSubData( GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3))*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors ); 
#if  MULTITEXTURE >= DIFFUSE_MAP
	// Diffuse Map's Texture Coordinates
	glBufferSubData( GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3)+sizeof(vec4))*m_iNumVtx, sizeof(vec2)*m_iNumVtx, m_pTex1 );  //第一張貼圖
#endif
#if MULTITEXTURE >= LIGHT_MAP
	// Light Map's Texture Coordinates
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3)+sizeof(vec4)+sizeof(vec2))*m_iNumVtx, sizeof(vec2)*m_iNumVtx, m_pTex2); // 第二張貼圖
#endif
#if MULTITEXTURE >= NORMAL_MAP
	// Normal Map's Texture Coordinates
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3)+sizeof(vec4)+sizeof(vec2)*2)*m_iNumVtx, sizeof(vec2)*m_iNumVtx, m_pTex3); // 第三張貼圖
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3)+sizeof(vec4)+sizeof(vec2)*3)*m_iNumVtx, sizeof(vec3)*m_iNumVtx, m_pTangentV); // 第三張貼圖
	// Vertex's Tangent
#endif
}

void CShape::SetShader(GLuint uiShaderHandle)
{
	// 改放置這裡, 方便每一個物件的設定
	CreateBufferObject();

    // Load shaders and use the resulting shader program
	if( uiShaderHandle == MAX_UNSIGNED_INT) m_uiProgram = InitShader( m_pVXshader, m_pFSshader );
	else m_uiProgram = uiShaderHandle;

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( m_uiProgram, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    GLuint vNormal = glGetAttribLocation( m_uiProgram, "vNormal" ); 
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*m_iNumVtx) );

    GLuint vColorVtx = glGetAttribLocation( m_uiProgram, "vVtxColor" );  // vertices' color 
    glEnableVertexAttribArray( vColorVtx );
    glVertexAttribPointer( vColorVtx, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(vec4)+sizeof(vec3))*m_iNumVtx));

	GLuint vDifMapCoord = glGetAttribLocation( m_uiProgram, "vDiffuseMapCoord" );  // vertices' texture coordinates
	glEnableVertexAttribArray( vDifMapCoord );
	glVertexAttribPointer( vDifMapCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(vec4)+sizeof(vec3)+sizeof(vec4))*m_iNumVtx));
	glUniform1i(glGetUniformLocation(m_uiProgram, "diffuMap"), 0);

#if MULTITEXTURE >= LIGHT_MAP
	// For Light Map
	GLuint vLightMapCoord = glGetAttribLocation(m_uiProgram, "vLightMapCoord");  // Light maps' texture coordinates， 必須新增到 Shader 中
	glEnableVertexAttribArray(vLightMapCoord);
	glVertexAttribPointer(vLightMapCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(vec4)+sizeof(vec3)+sizeof(vec4)+sizeof(vec2))*m_iNumVtx));
	glUniform1i(glGetUniformLocation(m_uiProgram, "lightMap"), 1);
#endif
#if MULTITEXTURE >= NORMAL_MAP
	// For Normal Map
	GLuint vNormalMapCoord = glGetAttribLocation(m_uiProgram, "vNormalMapCoord");  // Light maps' texture coordinates， 必須新增到 Shader 中
	glEnableVertexAttribArray(vNormalMapCoord);
	glVertexAttribPointer(vNormalMapCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(vec4)+sizeof(vec3)+sizeof(vec4)+sizeof(vec2)*2)*m_iNumVtx));
	glUniform1i(glGetUniformLocation(m_uiProgram, "normalMap"), 2);

	GLuint vTangentVec = glGetAttribLocation(m_uiProgram, "vTangentV");  // vertices' color 
	glEnableVertexAttribArray(vTangentVec);
	glVertexAttribPointer(vTangentVec, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(vec4)+sizeof(vec3)+sizeof(vec4)+sizeof(vec2)* 3)*m_iNumVtx));

#endif
	m_uiModelView = glGetUniformLocation( m_uiProgram, "ModelView" );
	glUniformMatrix4fv( m_uiModelView, 1, GL_TRUE, m_mxMVFinal );

    m_uiProjection = glGetUniformLocation( m_uiProgram, "Projection" );
	glUniformMatrix4fv( m_uiProjection, 1, GL_TRUE, m_mxProjection );

    m_uiColor = glGetUniformLocation( m_uiProgram, "vObjectColor" );
	glUniform4fv(m_uiColor, 1, m_fColor ); 

	m_uiLightInView = glGetUniformLocation( m_uiProgram, "LightInView" );
	glUniform4fv(m_uiLightInView, 1, m_vLightInView); 

	m_uiAmbient = glGetUniformLocation( m_uiProgram, "AmbientProduct" );
	glUniform4fv(m_uiAmbient, 1, m_AmbientProduct); 

	m_uiDiffuse = glGetUniformLocation( m_uiProgram, "DiffuseProduct" );
	glUniform4fv(m_uiDiffuse, 1, m_DiffuseProduct); 

	m_uiSpecular = glGetUniformLocation( m_uiProgram, "SpecularProduct" );
	glUniform4fv(m_uiSpecular, 1, m_SpecularProduct); 

	m_uiShininess = glGetUniformLocation( m_uiProgram, "fShininess" );
	glUniform1f(m_uiShininess, m_Material.shininess); 

	m_uiLighting = glGetUniformLocation( m_uiProgram, "iLighting" );
	glUniform1i(m_uiLighting, m_iLighting); // 光源的個數

	m_uiTexLayer = glGetUniformLocation(m_uiProgram, "iTexLayer");
	glUniform1i(m_uiTexLayer, m_iTexLayer);	// 貼圖的個數，預設為 1，直接傳入 pixel shader

	m_uiElapsedTime = glGetUniformLocation(m_uiProgram, "fElapsedTime");
	glUniform1f(m_uiElapsedTime, m_fElapsedTime);	// 貼圖的個數，預設為 1，直接傳入 pixel shader

#ifdef CUBIC_MAP
	// For Cube Map
	m_uiCubeMap = glGetUniformLocation(m_uiProgram, "cubeMap");
	glUniform1i(m_uiCubeMap, m_uiCubeMapTexName);

	m_uiViewPos = glGetUniformLocation(m_uiProgram, "ViewPos");
	glUniform4fv(m_uiViewPos, 1, m_v4Eye);

	m_uiTRS = glGetUniformLocation(m_uiProgram, "TRSMatrix");
	glUniformMatrix4fv(m_uiTRS, 1, GL_TRUE, m_mxTRS);
#endif

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void CShape::DrawingSetShader()
{
	glUseProgram( m_uiProgram );
	glBindVertexArray( m_uiVao );
	glUniformMatrix4fv( m_uiModelView, 1, GL_TRUE, m_mxMVFinal );

	if( m_bProjUpdated ) {
		glUniformMatrix4fv( m_uiProjection, 1, GL_TRUE, m_mxProjection );
		m_bProjUpdated = false;
	}

	// 將資訊更新到 Shader 中
	glUniform4fv(m_uiColor, 1, m_fColor ); 
	glUniform4fv(m_uiLightInView, 1, m_vLightInView); 
	glUniform4fv(m_uiAmbient, 1, m_AmbientProduct); 
	glUniform4fv(m_uiDiffuse, 1, m_DiffuseProduct); 
	glUniform4fv(m_uiSpecular, 1, m_SpecularProduct); 
	glUniform1f(m_uiShininess, m_Material.shininess); 
	glUniform1i(m_uiLighting, m_iLighting);
	glUniform1i(m_uiTexLayer, m_iTexLayer);

	glUniform1f(m_uiElapsedTime, m_fElapsedTime);

#ifdef CUBIC_MAP
	glUniform4fv(m_uiViewPos, 1, m_v4Eye);
	glUniformMatrix4fv(m_uiTRS, 1, GL_TRUE, m_mxTRS);
#endif
}

// 此處預設使用前一個描繪物件所使用的 Shader
void CShape::DrawingWithoutSetShader()
{
	glBindVertexArray( m_uiVao );
	glUniformMatrix4fv( m_uiModelView, 1, GL_TRUE, m_mxMVFinal );

	if( m_bProjUpdated ) {
		glUniformMatrix4fv( m_uiProjection, 1, GL_TRUE, m_mxProjection );
		m_bProjUpdated = false;
	}

	glUniform4fv(m_uiColor, 1, m_fColor ); 
	glUniform4fv(m_uiLightInView, 1, m_vLightInView); 
	glUniform4fv(m_uiAmbient,  1, m_AmbientProduct); 
	glUniform4fv(m_uiDiffuse,  1, m_DiffuseProduct); 
	glUniform4fv(m_uiSpecular, 1, m_SpecularProduct); 
	glUniform1f(m_uiShininess, m_Material.shininess); 
	glUniform1i(m_uiLighting, m_iLighting); 
	glUniform1i(m_uiTexLayer, m_iTexLayer);

#ifdef CUBIC_MAP
	glUniform4fv(m_uiViewPos, 1, m_v4Eye);
	glUniformMatrix4fv(m_uiTRS, 1, GL_TRUE, m_mxTRS);
#endif
}

void CShape::SetShaderName(const char vxShader[], const char fsShader[])
{
	int len;
	len = strlen(vxShader);
	m_pVXshader = new char[len+1];
	memcpy(m_pVXshader, vxShader, len+1);

	len = strlen(fsShader);
	m_pFSshader = new char[len+1];
	memcpy(m_pFSshader, fsShader, len+1);
}

void CShape::SetViewMatrix(mat4 &mat)
{
	m_mxView = mat;
	m_bViewUpdated = true;
}

void CShape::SetTRSMatrix(mat4 &mat)
{
	m_mxTRS = mat;
	m_bTRSUpdated = true;
}

void CShape::SetProjectionMatrix(mat4 &mat)
{
	m_mxProjection = mat;
	m_bProjUpdated = true;
}

void CShape::SetColor(vec4 vColor)
{
	m_fColor[0] = vColor.x;
	m_fColor[1] = vColor.y;
	m_fColor[2] = vColor.z;
	m_fColor[3] = vColor.w;
//	glUniform4fv(m_uiColor, 1, m_fColor); 
}

void CShape::SetTextureLayer(int texlayer)
{
	m_iTexLayer = texlayer;
}

void CShape::SetMaterials(color4 ambient, color4 diffuse, color4 specular)
{
	m_Material.ambient =  ambient;
	m_Material.diffuse =  diffuse;
	m_Material.specular = specular;
}

void CShape::SetKaKdKsShini(float ka, float kd, float ks, float shininess) // ka kd ks shininess
{
	m_Material.ka = ka;
	m_Material.kd = kd;
	m_Material.ks = ks;
	m_Material.shininess = shininess;
}

// Phong Reflection Model
vec4 CShape::PhongReflectionModel(vec4 vPoint, vec3 vNormal, const LightSource &lights)
{
	vec3 vN;		// 用 vec3 來宣告是為了節省計算, 如果要讓程式寫起來更方便，可改用 vec4 來宣告
	vec4 vColor;	// 該頂點的顏色
	vec4 vPosInView, vLightInView;

	// 1. 將點頂轉到鏡頭座標系統，因為頂點會根據 TRS Matrix 移動，所以必須以 m_mxMVFinal 計算在世界座標的絕對位置
	vPosInView = m_mxMVFinal * vPoint;

	// 2. 將面的 Normal 轉到鏡頭座標系統，並轉成單位向量
	//    只有物件的 rotation 會改變 Normal 的方向，乘上物件的 Final ModelView Matrix，同時轉換到鏡頭座標
	//    m_mxMV3X3Final 在 Update 處取自  m_mxMVFinal 的左上的 3X3 部分, 不需要位移量
#ifdef GENERAL_CASE
	//	  m_mxITMV 已經在呼叫 SetModelViewMatrix(mat4 &mat) 與 SetTRSMatrix(mat4 &mat) 就同時計算
	//    此處就不用再重新計算
	vN = m_mxITMV * vNormal;
#else
	vN = m_mxMV3X3Final * vNormal;
#endif

	vN = normalize(vN);

	// 3. 計算 Ambient color :  Ia = Ka * Material.ambient * La
	vColor = m_Material.ka * m_Material.ambient * lights.diffuse;

	// 4. 計算 Light 在鏡頭座標系統的位置
	//    此處假設 Light 的位置已經在世界座標的絕對位置
	vLightInView = m_mxView * lights.position;

	vec3 vLight; // 用 vec3 來宣告是為了節省計算, 如果要讓程式寫起來更方便，可改用 vec4 來宣告
	vLight.x = vLightInView.x - vPosInView.x;
	vLight.y = vLightInView.y - vPosInView.y;
	vLight.z = vLightInView.z - vPosInView.z;
	vLight = normalize(vLight); // normalize light vector

	// 5. 計算 L dot N
	GLfloat fLdotN = vLight.x*vN.x + vLight.y*vN.y + vLight.z*vN.z;
	if (fLdotN > 0) { // 該點被光源照到才需要計算
		// Diffuse Color : Id = Kd * Material.diffuse * Ld * (L dot N)
		vColor += m_Material.kd * m_Material.diffuse * lights.diffuse * fLdotN;

		// Specular color
		// Method 1: Phone Model
		//   計算 View Vector
		vec3 vView;
		vView.x = 0 - vPosInView.x;  // 目前已經以鏡頭座標為基礎, 所以 View 的位置就是 (0,0,0)
		vView.y = 0 - vPosInView.y;
		vView.z = 0 - vPosInView.z;
		vView = normalize(vView);

		//	 計算 Light 的 反射角 vRefL
		vec3 vRefL = 2.0f * (fLdotN)* vN - vLight;

		vRefL = normalize(vRefL);
		//   計算  vReflectedL dot View
		GLfloat RdotV = vRefL.x*vView.x + vRefL.y*vView.y + vRefL.z*vView.z;

		// Specular Color : Is = Ks * Ls * (R dot V)^Shininess;
		if (RdotV > 0) vColor += m_Material.ks * m_Material.specular * lights.diffuse * powf(RdotV, m_Material.shininess);
	}

	vColor.w = 1; // Alpha 改設定成 1，預設都是不透明物體
	// Method 2: Modified Phone Model 

	// 將顏色儲存到  m_Colors 中，因為 Quad 是兩個共平面的三角面所構成, 所以設定兩個三角面都有相同的顏色
	// 也就是設定所有的頂點都是這個顏色
	return vColor;
}


// For controlling texture mapping ----------------
void CShape::SetMirror(bool uAxis, bool vAxis) // U軸 與 V軸 是否要鏡射
{
	if( uAxis )  {
		if( vAxis ) { // U V 軸同時鏡射
			for(int i = 0 ; i < m_iNumVtx ; i++ ) { // 將每一個 vertex 的貼圖座標用 1 去減
				m_pTex1[i].x -= 1.0f; m_pTex1[i].y -= 1.0f;
			}
		}
		else { // 只有 U 軸鏡射
			for(int i = 0 ; i < m_iNumVtx ; i++ ) { // 將每一個 vertex 的貼圖座標用 1 去減
				m_pTex1[i].x -= 1.0f; // x 就是 U 軸
			}
		}
	}
	else if( vAxis ) { // 只有 V 軸鏡射
		for(int i = 0 ; i < m_iNumVtx ; i++ ) { // 將每一個 vertex 的貼圖座標用 1 去減
			m_pTex1[i].y -= 1.0f; // y 為 V 軸
		}
	}
	else ;
	glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );
	glBufferSubData( GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3)+sizeof(vec4))*m_iNumVtx, sizeof(vec2)*m_iNumVtx, m_pTex1 ); // vertcies' Color

}

void CShape::SetTiling(float uTiling, float vTiling)  // 對 U軸 與 V軸 進行拼貼的縮放
{
	for(int i = 0 ; i < m_iNumVtx ; i++ ) {
		m_pTex1[i].x *= uTiling; m_pTex1[i].y *= vTiling;
	}
	glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );
	glBufferSubData( GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3)+sizeof(vec4))*m_iNumVtx, sizeof(vec2)*m_iNumVtx, m_pTex1 ); // vertcies' Color
}

void CShape::SetLightMapTiling(float uTiling, float vTiling)
{
#if MULTITEXTURE >= LIGHT_MAP
	for (int i = 0; i < m_iNumVtx; i++) {
		m_pTex2[i].x *= uTiling; m_pTex2[i].y *= vTiling;
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(vec4) + sizeof(vec3) + sizeof(vec4) + sizeof(vec2))*m_iNumVtx, sizeof(vec2)*m_iNumVtx, m_pTex2);
#endif
}
//-----------------------------------------------------------------------------------------------------------------------------