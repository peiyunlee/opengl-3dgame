#include "CSolidTorus.h"
// Torus equation
// x(theta, phi) = (R + r cos(phi)) cos(theta)
// y(theta, phi) = (R + r cos(phi)) sin(theta) 
// z(theta, phi) = r sin(phi)
// 0 <= thtea, phi <= 2*PI
// R is the distance from the center of the tube to the center of the torus,
// r is the radius of the tube.

CSolidTorus::CSolidTorus(const GLfloat fRadius, const int iSlices,const  int iStacks)
{
    GLfloat drho = (GLfloat)(3.141592653589) / (GLfloat) iStacks;  
    GLfloat dtheta = 2.0f * (GLfloat)(3.141592653589) / (GLfloat) iSlices;  
    GLfloat ds = 1.0f / (GLfloat) iSlices;  
    GLfloat dt = 1.0f / (GLfloat) iStacks;  
    GLfloat t = 1.0f;      
    GLfloat s = 0.0f;  
    GLint i, j;     // Looping variables  
	int idx = 0; // 儲存 vertex 順序的索引值

	m_fRadius = fRadius;
	m_iSlices = iSlices;
	m_iStacks = iStacks;
	m_iNumVtx = iStacks*(2*(iSlices+1));

	m_pPoints = NULL; m_pNormals = NULL; m_pTex1 = NULL;

	m_pPoints  = new vec4[m_iNumVtx];
	m_pNormals = new vec3[m_iNumVtx];
	m_pColors  = new vec4[m_iNumVtx]; 
	m_pTex1     = new vec2[m_iNumVtx];


	for (i = 0; i < iStacks; i++ ) {  
		GLfloat rho = (GLfloat)i * drho;  
		GLfloat srho = (GLfloat)(sin(rho));  
		GLfloat crho = (GLfloat)(cos(rho));  
		GLfloat srhodrho = (GLfloat)(sin(rho + drho));  
		GLfloat crhodrho = (GLfloat)(cos(rho + drho));  
		
		// Many sources of OpenGL sphere drawing code uses a triangle fan  
		// for the caps of the sphere. This however introduces texturing   
		// artifacts at the poles on some OpenGL implementations  
		s = 0.0f;  
		for ( j = 0; j <= iSlices; j++) {  
            GLfloat theta = (j == iSlices) ? 0.0f : j * dtheta;  
            GLfloat stheta = (GLfloat)(-sin(theta));  
            GLfloat ctheta = (GLfloat)(cos(theta));  
  
            GLfloat x = stheta * srho;  
            GLfloat y = ctheta * srho;  
            GLfloat z = crho;  
              
			m_pPoints[idx].x = x * m_fRadius;
			m_pPoints[idx].y = y * m_fRadius;
			m_pPoints[idx].z = z * m_fRadius;
			m_pPoints[idx].w = 1;

			m_pNormals[idx].x = x;
			m_pNormals[idx].y = y;
			m_pNormals[idx].z = z;

			m_pTex1[idx].x = s;
			m_pTex1[idx].y = t; // 設定貼圖座標
			idx++;

            x = stheta * srhodrho;  
            y = ctheta * srhodrho;  
            z = crhodrho;

			m_pPoints[idx].x = x * m_fRadius;
			m_pPoints[idx].y = y * m_fRadius;
			m_pPoints[idx].z = z * m_fRadius;
			m_pPoints[idx].w = 1;

			m_pNormals[idx].x = x;
			m_pNormals[idx].y = y;
			m_pNormals[idx].z = z;

			m_pTex1[idx].x = s;
			m_pTex1[idx].y = t - dt; // 設定貼圖座標
			idx++;
			s += ds; 
		}  
		t -= dt;  
	}  

	// 預設將所有的面都設定成灰色
	for( int i = 0 ; i < m_iNumVtx ; i++ ) m_pColors[i] = vec4(-1.0f,-1.0f,-1.0f,1.0f);

#ifdef LIGHTING_WITHCPU
	// Default Set shader's name
	SetShaderName("vsLighting_CPU.glsl", "fsLighting_CPU.glsl");
#else // lighting with GPU
#ifdef PERVERTEX_LIGHTING
	SetShaderName("vsLighting_GPU.glsl", "fsLighting_GPU.glsl");
#else
	SetShaderName("vsPerPixelLighting.glsl", "fsPerPixelLighting.glsl");
#endif
#endif  

	// Create and initialize a buffer object ，將此部分的設定移入 SetShader 中
	// CreateBufferObject();

	// 設定材質
	SetMaterials(vec4(0), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	SetKaKdKsShini(0, 0.8f, 0.2f, 1);
}

// 回家自己寫
void CSolidTorus::RenderWithFlatShading(vec4 vLightPos, color4 vLightI)
{


}

void CSolidTorus::RenderWithGouraudShading(vec4 vLightPos, color4 vLightI)
{
	// Method 1 : 對每一個 Vertex 都計算顏色
	 for (int i = 0; i < m_iStacks; i++ ) {  
		 for( int j = 0 ; j < 2*(m_iSlices+1) ; j++ ) {
			m_pColors[i*2*(m_iSlices+1)+j] = PhongReflectionModel(m_pPoints[i*2*(m_iSlices+1)+j], m_pNormals[i*2*(m_iSlices+1)+j], vLightPos, vLightI);
		 }
	 } 

	// Method 2 : 重疊的 Vertex 使用前一次計算的顏色
	 //先計算第一個 Stack 的顏色
	//for( int j = 0 ; j < 2*(m_iSlices+1) ; j++ ) {
	//	m_pColors[j] = PhongReflectionModel(m_pPoints[j], m_pNormals[j], vLightPos, vLightI);
	//}
	//// 後續 Stack 的 vertex 顏色，編號偶數(含 0) 使用前一個 stack 編號+1的 顏色
	//// 編號奇數就必須計算顏色
	//// 每一個 Slices 最後兩個 vertex 於開頭前兩個 vertex 重疊，所以使用該兩個 vertex 的顏色
	//for (int i = 1; i < m_iStacks; i++ ) {  
	//	for( int j = 0 ; j < 2*(m_iSlices+1) - 2 ; j++ ) {
	//		if( j%2 ) m_pColors[i*2*(m_iSlices+1)+j] = PhongReflectionModel(m_pPoints[i*2*(m_iSlices+1)+j], m_pNormals[i*2*(m_iSlices+1)+j], vLightPos, vLightI);
	//		else m_pColors[i*2*(m_iSlices+1)+j] =  m_pColors[(i-1)*2*(m_iSlices+1)+j+1];		
	//	}
	//	m_pColors[(i+1)*2*(m_iSlices+1)-2] = m_pColors[i*2*(m_iSlices+1)];
	//	m_pColors[(i+1)*2*(m_iSlices+1)-1] = m_pColors[i*2*(m_iSlices+1)+1];
	//}

	glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx+sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors ); // vertcies' Color
}

// 此處所給的 vLightPos 必須是世界座標的確定絕對位置
void CSolidTorus::Update(float dt, point4 vLightPos, color4 vLightI)
{
#ifdef LIGHTING_WITHCPU
	if( m_bMVUpdated  ) { // Model View 的相關矩陣內容有更動
		m_mxMVFinal = m_mxModelView * m_mxTRS;
		m_mxMV3X3Final = mat3(
			m_mxMVFinal._m[0].x,  m_mxMVFinal._m[1].x, m_mxMVFinal._m[2].x,
			m_mxMVFinal._m[0].y,  m_mxMVFinal._m[1].y, m_mxMVFinal._m[2].y,
			m_mxMVFinal._m[0].z,  m_mxMVFinal._m[1].z, m_mxMVFinal._m[2].z);
#ifdef GENERAL_CASE
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal); 
#endif
		m_bMVUpdated = false;
	}
	if( m_iMode == FLAT_SHADING )  RenderWithGouraudShading(vLightPos, vLightI);
	else RenderWithGouraudShading(vLightPos, vLightI);

#else // Lighting With GPU
	if( m_bMVUpdated  ) {
		m_mxMVFinal = m_mxModelView * m_mxTRS;
		m_bMVUpdated = false;
	}
	m_vLightInView = m_mxModelView * vLightPos;		// 將 Light 轉換到鏡頭座標再傳入
	// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
	m_AmbientProduct  = m_Material.ka * m_Material.ambient  * vLightI;
	m_DiffuseProduct  = m_Material.kd * m_Material.diffuse  * vLightI;
	m_SpecularProduct = m_Material.ks * m_Material.specular * vLightI;
#endif
}

void CSolidTorus::Update(float dt, const structLightSource &LightSource)
{
#ifdef LIGHTING_WITHCPU
	if( m_bMVUpdated  ) { // Model View 的相關矩陣內容有更動
		m_mxMVFinal = m_mxModelView * m_mxTRS;
		m_mxMV3X3Final = mat3(
			m_mxMVFinal._m[0].x,  m_mxMVFinal._m[1].x, m_mxMVFinal._m[2].x,
			m_mxMVFinal._m[0].y,  m_mxMVFinal._m[1].y, m_mxMVFinal._m[2].y,
			m_mxMVFinal._m[0].z,  m_mxMVFinal._m[1].z, m_mxMVFinal._m[2].z);
#ifdef GENERAL_CASE
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal); 
#endif
		m_bMVUpdated = false;
	}
	if( m_iMode == FLAT_SHADING )  RenderWithGouraudShading(LightSource.position, LightSource.diffuse);
	else RenderWithGouraudShading(LightSource.position, LightSource.diffuse);

#else // Lighting With GPU
	if( m_bMVUpdated  ) {
		m_mxMVFinal = m_mxModelView * m_mxTRS;
		m_bMVUpdated = false;
	}
	m_vLightInView = m_mxModelView * LightSource.position;		// 將 Light 轉換到鏡頭座標再傳入
	// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
	m_AmbientProduct  = m_Material.ka * m_Material.ambient  * LightSource.ambient;
	m_DiffuseProduct  = m_Material.kd * m_Material.diffuse  * LightSource.diffuse;
	m_SpecularProduct = m_Material.ks * m_Material.specular * LightSource.specular;
#endif
}

void CSolidTorus::Update(float dt)
{
	if( m_bMVUpdated  ) { // Model View 的相關矩陣內容有更動
		m_mxMVFinal = m_mxModelView * m_mxTRS;
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal); 
	}
}

void CSolidTorus::Draw()
{
	DrawingSetShader();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Change to wireframe mode
	glBindTexture(GL_TEXTURE_2D, m_uiTexObject[0]); 
	for (int i = 0; i < m_iStacks; i++ ) {  
		glDrawArrays( GL_TRIANGLE_STRIP, i*(2*(m_iSlices+1)), 2*(m_iSlices+1) );
	}
	glBindTexture(GL_TEXTURE_2D, 0); 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Return to solid mode
}

void CSolidTorus::DrawW()
{
	DrawingWithoutSetShader();
	glBindTexture(GL_TEXTURE_2D, m_uiTexObject[0]); 
	for (int i = 0; i < m_iStacks; i++ ) {  
		glDrawArrays( GL_TRIANGLE_STRIP, i*(2*(m_iSlices+1)), 2*(m_iSlices+1) );
	}
	glBindTexture(GL_TEXTURE_2D, 0); 
}

CSolidTorus::~CSolidTorus()
{

}