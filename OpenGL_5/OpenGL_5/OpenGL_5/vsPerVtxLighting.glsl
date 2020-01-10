// Phong reflection model
// #define BLINN_PHONG
#version 130
in  vec4  vPosition;	// Vertex Position
in  vec3  vNormal;    // Vertex Normal
in  vec4  vVtxColor;  // Vertex Color
in  vec2  vDiffuseMapCoord;
in  vec2  vLightMapCoord;

out vec4  vColor;         // 輸出的顏色
out vec2  DiffuseMapUV;   // 輸出貼圖座標
out vec2  LightMapUV;     // 輸出貼圖座標

uniform mat4  ModelView;   // Model View Matrix
uniform mat4  Projection;  // Projection Matrix
uniform vec4  vObjectColor;    // 代表物件的單一顏色

// 上面的都與  vsLighting_CPU.glsl 相同
// 以下為新增的部分
uniform vec4  LightInView;     // Light's position in View Space
uniform vec4  AmbientProduct;  // light's ambient  與 Object's ambient  與 ka 的乘積
uniform vec4  DiffuseProduct;  // light's diffuse  與 Object's diffuse  與 kd 的乘積
uniform vec4  SpecularProduct; // light's specular 與 Object's specular 與 ks 的乘積
uniform float fShininess;
uniform int   iLighting;


void main()
{
	// 先宣告 diffuse 與 specular
    vec4 diffuse = vec4(0.0,0.0,0.0,1.0);
	vec4 specular = vec4(0.0,0.0,0.0,1.0);

	if( iLighting != 1 ) {
		gl_Position = Projection * ModelView * vPosition;	// 計算 vPosition 投影後的位置
		vColor = vObjectColor;
	}
	else {	
		// 1. 將點頂轉到鏡頭座標系統，必須以 m_mxMVFinal 計算在世界座標的絕對位置
		vec4 vPosInView = ModelView * vPosition;  

		// 2. 將面的 Normal 轉到鏡頭座標系統，並轉成單位向量
		// 最後一個補上 0 ，不需要位移量，否則出來的 Normal 會不對
		// 同時利用 normalize 轉成單位向量

// 以下兩行用於計算對物件進行 non-uniform scale 時，物件 normal 的修正計算
//		mat3 ITModelView = transpose(inverse(mat3(ModelView)); 
//		vec3 vN = normalize(ITModelView * vNormal); 

		vec3 vN = normalize(ModelView * vec4(vNormal, 0.0)).xyz; 

		// 3. 計算 Ambient color : Ia = AmbientProduct = Ka * Material.ambient * La = 
		vec4 ambient = AmbientProduct; // m_sMaterial.ka * m_sMaterial.ambient * vLightI;

		// 4. 傳入的 Light 的位置已經在鏡頭座標
		vec3 vL = normalize(vec3(LightInView.xyz - vPosInView.xyz)); // normalize light vector

		// 5. 計算 L dot N
		float fLdotN = vL.x*vN.x + vL.y*vN.y + vL.z*vN.z;
		if( fLdotN >= 0 ) { // 該點被光源照到才需要計算
			// Diffuse Color : Id = Kd * Material.diffuse * Ld * (L dot N)
			diffuse = fLdotN * DiffuseProduct; 

			// Specular color
			// Method 1: Phone Model
			// 計算 View Vector
			// 目前已經以鏡頭座標為基礎, 所以 View 的位置就是 (0,0,0), 所以點位置的負向就是 View Dir
			vec3 vV = -normalize(vPosInView.xyz);

#ifndef BLINN_PHONG
			//計算 Light 的 反射角 vRefL = 2 * fLdotN * vN - L
			// 同時利用 normalize 轉成單位向量
			vec3 vRefL = normalize(2.0f * (fLdotN) * vN - vL);

			//   計算  vReflectedL dot View
			float RdotV = vRefL.x*vV.x + vRefL.y*vV.y + vRefL.z*vV.z;

			// Specular Color : Is = Ks * Material.specular * Ls * (R dot V)^Shininess;
			if( RdotV > 0 ) specular = SpecularProduct * pow(RdotV, fShininess); 
#else

			// Blinn-Phong Reflection Model
			vec3 vH = normalize( vL + vV ); 
			float HdotN = vH.x*vN.x + vH.y*vN.y + vH.z*vN.z; //   計算  H dot N
			if( HdotN > 0 ) specular = SpecularProduct * pow(HdotN, fShininess); 
#endif
		}

		gl_Position = Projection * vPosInView;	// 計算 vPosition 投影後的位置
		vColor = ambient + diffuse + specular;  // 計算顏色 ambient + diffuse + specular;
		vColor.w = DiffuseProduct.w;	 // 統一使用模型材質 diffuse 的 alpha 值
		// vColor = vec4((ambient + diffuse + specular).xyz, 1.0);
	}
	DiffuseMapUV = vDiffuseMapCoord;
	LightMapUV  = vLightMapCoord;
}
