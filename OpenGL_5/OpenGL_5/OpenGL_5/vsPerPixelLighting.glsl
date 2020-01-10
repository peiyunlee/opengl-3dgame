// Phong reflection model
#version 130
in vec4 vPosition;	// Vertex Position
in vec3 vNormal;    // Vertex Normal
in vec4 vVtxColor;  // Vertex Color
in vec2 vDiffuseMapCoord;
in vec2 vLightMapCoord;

out vec3 v3N; // 輸出 Normal 在鏡頭座標下的方向
out vec3 v3L; // 輸出 Light Direction 在鏡頭座標下的方向
out vec3 v3E; // 輸出 View Direction 在鏡頭座標下的方向
out vec2 DiffuseMapUV;  // 輸出貼圖座標
out vec2 LightMapUV;    // 輸出貼圖座標

uniform mat4  ModelView;   // Model View Matrix
uniform mat4  Projection;  // Projection Matrix
uniform vec4  LightInView; // Light's position in View Space

void main()
{
	vec4 vPosInView = ModelView * vPosition;
	// 目前已經以鏡頭座標為基礎, 所以 View 的位置就是 (0,0,0), 所以位置的富項就是 View Dir
	v3E = -vPosInView.xyz;

	// 以下兩行用於計算對物件進行 non-uniform scale 時，物件 normal 的修正計算
	//		mat3 ITModelView = transpose(inverse(mat3(ModelView)); 
	//		vec3 v3N = normalize(ITModelView * vNormal); 
	v3N = (ModelView * vec4(vNormal, 0.0)).xyz;

	v3L = vec3(LightInView.xyz - vPosInView.xyz);
	gl_Position = Projection * vPosInView;
	DiffuseMapUV = vDiffuseMapCoord;
	LightMapUV  = vLightMapCoord;
}
