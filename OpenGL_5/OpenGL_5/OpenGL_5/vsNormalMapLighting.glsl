// Phong reflection model
#version 150
in vec4 vPosition;	// Vertex Position
in vec3 vNormal;    // Vertex Normal
in vec4 vVtxColor;  // Vertex Color
in vec2 vDiffuseMapCoord;
in vec2 vLightMapCoord;
in vec2 vNormalMapCoord;
in vec3 vTangentV;

out vec3 v3N; // 輸出 Normal 在 Tangent Space 座標下的方向
out vec3 v3L; // 輸出 Light Direction 在 Tangent Space 座標下的方向
out vec3 v3E; // 輸出 View Direction 在 Tangent Space 座標下的方向
out vec2 DiffuseMapUV;  // 輸出貼圖座標
out vec2 LightMapUV;    // 輸出貼圖座標
out vec2 NormalMapUV;  // 輸出貼圖座標

uniform mat4  ModelView;   // Model View Matrix
uniform mat4  Projection;  // Projection Matrix
uniform vec4  LightInView; // Light's position in View Space

void main()
{
	// Vertex 轉換到 World Space
	vec4 vPosInView = ModelView * vPosition;

	// Building the matrix Eye Space -> Tangent Space
	// 以下兩行用於計算對物件進行 non-uniform scale 時，物件 normal 的修正計算
	mat3 ITModelView = transpose(inverse(mat3(ModelView))); 
	//	vec3 vN = normalize(ITModelView * vNormal); 

    // 計算 vertex normal 與 vTangent 轉換到正確的 object local space 上
	vec3 vn = normalize(ITModelView * vNormal);
	vec3 vt = normalize(ITModelView * vTangentV);
	vec3 vb = cross(vn, vt);

	// 將 Light Vector 轉換到 Tangent Space
	vec3 tmpV = vec3(LightInView.xyz - vPosInView.xyz);
	v3L.x = dot(tmpV, vt);
	v3L.y = dot(tmpV, vb);
	v3L.z = dot(tmpV, vn);

	// 將 Eye Vector 轉換到 Tangent Space
	tmpV = -vPosInView.xyz;
	v3E.x = dot(tmpV, vt);
	v3E.y = dot(tmpV, vb);
	v3E.z = dot(tmpV, vn);

	gl_Position = Projection * vPosInView;
	DiffuseMapUV = vDiffuseMapCoord;
	LightMapUV  = vLightMapCoord;
	NormalMapUV = vNormalMapCoord;
}
