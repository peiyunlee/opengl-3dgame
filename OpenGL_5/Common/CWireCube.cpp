#include "CWireCube.h"

CWireCube::CWireCube()
{

	m_iNumVtx = WIRECUBE_NUM;
	m_pPoints = NULL; m_pNormals = NULL; m_pTex1 = NULL;

	m_pPoints  = new vec4[m_iNumVtx];
	m_pNormals = new vec3[m_iNumVtx];
	m_pColors  = new vec4[m_iNumVtx]; 
	m_pTex1     = new vec2[m_iNumVtx];
#if MULTITEXTURE >= LIGHT_MAP
	m_pTex2 = new vec2[m_iNumVtx];  // 產生 light map 所需的貼圖座標
#endif
#if MULTITEXTURE >= NORMAL_MAP
	m_pTex3 = new vec2[m_iNumVtx];	// 產生 normal map 所需的貼圖座標
	m_pTangentV = new vec3[m_iNumVtx];
#endif

    m_vertices[0] = point4( -0.5, -0.5,  0.5, 1.0 );
    m_vertices[1] = point4( -0.5,  0.5,  0.5, 1.0 );
    m_vertices[2] = point4(  0.5,  0.5,  0.5, 1.0 );
    m_vertices[3] = point4(  0.5, -0.5,  0.5, 1.0 );
    m_vertices[4] = point4( -0.5, -0.5, -0.5, 1.0 );
    m_vertices[5] = point4( -0.5,  0.5, -0.5, 1.0 );
    m_vertices[6] = point4(  0.5,  0.5, -0.5, 1.0 );
	m_vertices[7] = point4(  0.5, -0.5, -0.5, 1.0 );

	m_iIndex = 0;
	// generate 12 triangles: 36 vertices and 36 colors
    Quad( 1, 0, 3, 2 );
    Quad( 2, 3, 7, 6 );
    Quad( 3, 0, 4, 7 );
    Quad( 6, 5, 1, 2 );
    Quad( 4, 5, 6, 7 );
    Quad( 5, 4, 0, 1 );

	// 預設將所有的面都設定成灰色
	for( int i = 0 ; i < m_iNumVtx ; i++ ) m_pColors[i] = vec4(-1.0f,-1.0f,-1.0f,1.0f);

#ifdef PERVERTEX_LIGHTING
	SetShaderName("vsPerVtxLighting.glsl", "fsPerVtxLighting.glsl");
#else
	SetShaderName("vsPerPixelLighting.glsl", "fsPerPixelLighting.glsl");
#endif  

	// Create and initialize a buffer object ，將此部分的設定移入 SetShader 中
	// CreateBufferObject();

	// 設定材質
	SetMaterials(vec4(0), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	SetKaKdKsShini(0, 0.8f, 0.2f, 1);
}

void CWireCube::Quad( int a, int b, int c, int d )
{
    // Initialize temporary vectors along the quad's edge to
    //   compute its face normal 
    vec4 u = m_vertices[b] - m_vertices[a];
    vec4 v = m_vertices[c] - m_vertices[b];
    vec3 normal = normalize( cross(u, v) );

    m_pNormals[m_iIndex] = normal; m_pPoints[m_iIndex] = m_vertices[a]; m_pTex1[m_iIndex] = vec2(0,0);m_iIndex++;
    m_pNormals[m_iIndex] = normal; m_pPoints[m_iIndex] = m_vertices[b]; m_pTex1[m_iIndex] = vec2(1,0);m_iIndex++;
    m_pNormals[m_iIndex] = normal; m_pPoints[m_iIndex] = m_vertices[c]; m_pTex1[m_iIndex] = vec2(1,1);m_iIndex++;
    m_pNormals[m_iIndex] = normal; m_pPoints[m_iIndex] = m_vertices[d]; m_pTex1[m_iIndex] = vec2(0,1);m_iIndex++;
}


void CWireCube::RenderWithFlatShading(const LightSource &lights)
{
	// 以每一個面的三個頂點計算其重心，以該重心作為顏色計算的點頂
	// 根據 Phong lighting model 計算相對應的顏色，並將顏色儲存到此三個點頂
	// 因為每一個平面的頂點的 Normal 都相同，所以此處並沒有計算此三個頂點的平均 Normal

	vec4 vCentroidP;
	for( int i = 0 ; i < m_iNumVtx ; i += 4 ) {
		// 計算三角形的重心
		vCentroidP = (m_pPoints[i] + m_pPoints[i+1] + m_pPoints[i+2] + m_pPoints[i+3])/4.0f;
		m_pColors[i] = m_pColors[i + 1] = m_pColors[i + 2] = m_pColors[i + 3] = PhongReflectionModel(vCentroidP, m_pNormals[i], lights);
	}
	glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx+sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors ); // vertcies' Color
}


void CWireCube::RenderWithGouraudShading(const LightSource &lights)
{
	for( int i = 0 ; i < m_iNumVtx ; i += 4 ) {
		m_pColors[i] = PhongReflectionModel(m_pPoints[i], m_pNormals[i], lights);
		m_pColors[i + 1] = PhongReflectionModel(m_pPoints[i + 1], m_pNormals[i + 1], lights);
		m_pColors[i + 2] = PhongReflectionModel(m_pPoints[i + 2], m_pNormals[i + 2], lights);
		m_pColors[i + 3] = PhongReflectionModel(m_pPoints[i + 3], m_pNormals[i + 3], lights);
	}
	glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx+sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors ); // vertcies' Color
}


// 此處所給的 vLightPos 必須是世界座標的確定絕對位置
void CWireCube::Update(float dt, const LightSource &lights)
{
#ifdef LIGHTING_WITHCPU
	if( m_bViewUpdated || m_bTRSUpdated  ) { // Model View 的相關矩陣內容有更動
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxMV3X3Final = mat3(
			m_mxMVFinal._m[0].x,  m_mxMVFinal._m[1].x, m_mxMVFinal._m[2].x,
			m_mxMVFinal._m[0].y,  m_mxMVFinal._m[1].y, m_mxMVFinal._m[2].y,
			m_mxMVFinal._m[0].z,  m_mxMVFinal._m[1].z, m_mxMVFinal._m[2].z);
#ifdef GENERAL_CASE
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal); 
#endif
		m_bViewUpdated = m_bTRSUpdated = false;
	}
	if (m_iMode == FLAT_SHADING) RenderWithFlatShading(lights);
	else RenderWithGouraudShading(lights);

#else // Lighting With GPU
	if (m_bViewUpdated || m_bTRSUpdated) {
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_bViewUpdated = m_bTRSUpdated = false;
	}
	m_vLightInView = m_mxView * lights.position;		// 將 Light 轉換到鏡頭座標再傳入
	// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
	m_AmbientProduct = m_Material.ka * m_Material.ambient  * lights.ambient;
	m_AmbientProduct.w = m_Material.ambient.w;
	m_DiffuseProduct = m_Material.kd * m_Material.diffuse  * lights.diffuse;
	m_DiffuseProduct.w = m_Material.diffuse.w;
	m_SpecularProduct = m_Material.ks * m_Material.specular * lights.specular;
	m_SpecularProduct.w = m_Material.specular.w;
#endif

}


void CWireCube::Update(float dt)
{
	if (m_bViewUpdated || m_bTRSUpdated) { // Model View 的相關矩陣內容有更動
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal); 
		m_bViewUpdated = m_bTRSUpdated = false;
	}
}


void CWireCube::Draw()
{
	DrawingSetShader();
	glDrawArrays( GL_LINE_LOOP,  0, 4 );
	glDrawArrays( GL_LINE_LOOP,  4, 4 );
	glDrawArrays( GL_LINE_LOOP,  8, 4 );
	glDrawArrays( GL_LINE_LOOP, 12, 4 );
	glDrawArrays( GL_LINE_LOOP, 16, 4 );
	glDrawArrays( GL_LINE_LOOP, 20, 4 );
}


void CWireCube::DrawW()
{
	DrawingWithoutSetShader();
	glDrawArrays( GL_LINE_LOOP,  0, 4 );
	glDrawArrays( GL_LINE_LOOP,  4, 4 );
	glDrawArrays( GL_LINE_LOOP,  8, 4 );
	glDrawArrays( GL_LINE_LOOP, 12, 4 );
	glDrawArrays( GL_LINE_LOOP, 16, 4 );
	glDrawArrays( GL_LINE_LOOP, 20, 4 );
}


