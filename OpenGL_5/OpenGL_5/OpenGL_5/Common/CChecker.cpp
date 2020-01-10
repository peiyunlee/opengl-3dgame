#include "CChecker.h"



CChecker::~CChecker()
{
	delete [] m_pSquares;
	delete [] m_pfSquaresT;
}

CChecker::CChecker(int iSize, float fYPos)
{
	// 建立實體
	m_iGridSize = iSize;
	m_pSquares = new CQuad[m_iGridSize*m_iGridSize];
	m_pfSquaresT = new float[m_iGridSize*m_iGridSize*3];
	m_fYPos = fYPos;
	m_iMode = FLAT_SHADING;
}

void CChecker::SetShader()
{
	//// 產生每一個 Square 的位置與座標
	mat4 mxT;
	int idx = 0; // 
	vec4 vT, vColor;
	for( int i = 0 ; i < m_iGridSize ; i++ ) {
		for( int j = 0 ; j < m_iGridSize ; j++ ) {
			m_pfSquaresT[idx*3+2] = -m_iGridSize/2 + i + 0.5f;
			m_pfSquaresT[idx*3+0] = -m_iGridSize/2 + j + 0.5f;
			m_pfSquaresT[idx*3+1] = m_fYPos;
			if( i + j == 0 ) { // 第一個， 需要指定 Shader
				m_pSquares[idx].SetShader();
				m_uiShaderHandle = m_pSquares[idx].GetShaderHandle();
				vColor.x = 0.3f; vColor.y = 0.6f; vColor.z = 0.6f; vColor.w = 1.0f;
				m_pSquares[idx].SetColor(vColor);
			}
			else { // 其他的 
				m_pSquares[idx].SetShader(m_uiShaderHandle);
				if( (i+j)%2 ) {
					vColor.x = 0.2f; vColor.y = 0.2f; vColor.z = 0.2f; vColor.w = 1.0f;
					m_pSquares[idx].SetColor(vColor);
				}
				else {
					vColor.x = 0.3f; vColor.y = 0.6f; vColor.z = 0.6f; vColor.w = 1.0f;
					m_pSquares[idx].SetColor(vColor);
				}
			}
			mxT = Translate(m_pfSquaresT[idx*3+0],m_pfSquaresT[idx*3+1],m_pfSquaresT[idx*3+2]);	
			m_pSquares[idx].SetTRSMatrix(mxT);
			m_pSquares[idx].SetShadingMode(m_iMode);
			idx++;
		}
	}
}

void CChecker::SetShaderName(const char *vxShader, const char *fsShader)
{
	int idx = 0; 
	for (int i = 0; i < m_iGridSize; i++) {
		for (int j = 0; j < m_iGridSize; j++) {
			m_pSquares[idx].SetShaderName(vxShader, fsShader);
			idx++;
		}
	}
}

void CChecker::SetProjectionMatrix(mat4 &mat)
{
	for( int i = 0 ; i < m_iGridSize*m_iGridSize ; i++ )  {
		m_pSquares[i].SetProjectionMatrix(mat);
	}
}

void CChecker::SetTRSMatrix(mat4 &mat)
{
	mat4 mxT;
	// 必須加上每一個方格本身的位移
	for( int i = 0 ; i < m_iGridSize*m_iGridSize ; i++ )  {
		mxT = Translate(m_pfSquaresT[i*3+0],m_pfSquaresT[i*3+1],m_pfSquaresT[i*3+2]);	
		m_pSquares[i].SetTRSMatrix(mat*mxT);
	}
}

void CChecker::SetTextureLayer(int texlayer)
{
	for (int i = 0; i < m_iGridSize*m_iGridSize; i++)  {
		m_pSquares[i].SetTextureLayer(texlayer);
	}
}

void CChecker::Draw()
{
	m_pSquares[0].Draw();
	for( int i = 1 ; i < m_iGridSize*m_iGridSize ; i++ )  {
		m_pSquares[i].DrawW();
	}
}

void CChecker::SetViewMatrix(mat4 &mat)
{
	for( int i = 0 ; i < m_iGridSize*m_iGridSize ; i++ )  {
		m_pSquares[i].SetViewMatrix(mat);
	}
}

void CChecker::Update(float dt, const LightSource &lights)
{
	for( int i = 0 ; i < m_iGridSize*m_iGridSize ; i++ )  {
		m_pSquares[i].Update(dt, lights);
	}
}

void CChecker::SetMaterials(color4 ambient, color4 diffuse, color4 specular)
{
	for( int i = 0 ; i < m_iGridSize*m_iGridSize ; i++ )  {
		m_pSquares[i].SetMaterials(ambient, diffuse, specular);
	}
}
void CChecker::SetKaKdKsShini(float ka, float kd, float ks, float shininess) // ka kd ks shininess
{
	for( int i = 0 ; i < m_iGridSize*m_iGridSize ; i++ )  {
		m_pSquares[i].SetKaKdKsShini(ka, kd, ks, shininess);
	}
}