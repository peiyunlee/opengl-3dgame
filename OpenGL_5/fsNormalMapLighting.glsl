#version 130
// iTexLayer 的設定與判斷方式，假設此處可以支援到六張貼圖

#define NONE_MAP    0
#define DIFFUSE_MAP 1
#define LIGHT_MAP   2
#define NORMAL_MAP  4

//#define LIGHTMAP_COLOR

in vec3 v3L;
in vec3 v3E;

in vec2 DiffuseMapUV;   // 輸入 Diffuse Map 貼圖座標
in vec2 LightMapUV;     // 輸入 Light Map 貼圖座標
in vec2 NormalMapUV;  // 輸入 Normal Map 貼圖座標

uniform int iTexLayer;

// 以下為新增的部分
uniform vec4  LightInView;        // Light's position in View Space
uniform vec4  AmbientProduct;  // light's ambient  與 Object's ambient  與 ka 的乘積
uniform vec4  DiffuseProduct;  // light's diffuse  與 Object's diffuse  與 kd 的乘積
uniform vec4  SpecularProduct; // light's specular 與 Object's specular 與 ks 的乘積
uniform float fShininess;
uniform int   iLighting;
uniform vec4  vObjectColor;    // 代表物件的單一顏色

// For Texture Sampler
uniform sampler2D diffuMap; // 貼圖的參數設定
uniform sampler2D lightMap; // 貼圖的參數設定
uniform sampler2D normalMap; // 貼圖的參數設定

void main()
{
	// 先宣告 diffuse 與 specular
    vec4 diffuse = vec4(0.0,0.0,0.0,1.0);
	vec4 specular = vec4(0.0,0.0,0.0,1.0);
	vec4 LightingColor = vec4(0.0,0.0,0.0,1.0);
	
	if( iLighting != 1 ) {
		if( iTexLayer == NONE_MAP ) gl_FragColor = vObjectColor;
		else gl_FragColor = vObjectColor * texture2D(diffuMap, DiffuseMapUV); 
	}
	else {	
		// 1. 計算 Ambient color : Ia = AmbientProduct = Ka * Material.ambient * La = 
		vec4 ambient = AmbientProduct; // m_sMaterial.ka * m_sMaterial.ambient * vLightI;

		// 單位從 normal map 中所取得的 Normal Dir
		vec3 vN;
		vN = normalize(2.0f*texture2D(normalMap, NormalMapUV).xyz - 1.0f);

		// 2. 單位化傳入的 Light Dir
		vec3 vL = normalize(v3L); // normalize light vector

		// 5. 計算 L dot N
		float fLdotN = vL.x*vN.x + vL.y*vN.y + vL.z*vN.z;

		if( fLdotN >= 0 ) { // 該點被光源照到才需要計算
			// Diffuse Color : Id = Kd * Material.diffuse * Ld * (L dot N)
			diffuse = fLdotN * DiffuseProduct; 

			// Specular color
			// Method 1: Phone Model
			// 計算 View Vector
			// 單位化傳入的 fE , View Direction
			vec3 vV = normalize(v3E);

			//計算 Light 的 反射角 vRefL = 2 * fLdotN * vN - L
			// 同時利用 normalize 轉成單位向量
			vec3 vRefL = normalize(2.0f * (fLdotN) * vN - vL);

			//   計算  vReflectedL dot View
			float RdotV = vRefL.x*vV.x + vRefL.y*vV.y + vRefL.z*vV.z;

			// Specular Color : Is = Ks * Material.specular * Ls * (R dot V)^Shininess;
			if( RdotV > 0 ) specular = SpecularProduct * pow(RdotV, fShininess); 
		}

		LightingColor = ambient + diffuse + specular;  // 計算顏色 ambient + diffuse + specular;
		LightingColor.w = DiffuseProduct.w;	// 設定為傳入材質的 alpha,	DiffuseProduct.w
		// LightingColor.w = 1.0;	// 設定 alpha 為 1.0

		if( iTexLayer == NONE_MAP ) gl_FragColor = LightingColor;
		else if( iTexLayer == DIFFUSE_MAP || iTexLayer == (DIFFUSE_MAP|NORMAL_MAP) ) gl_FragColor = LightingColor * texture2D(diffuMap, DiffuseMapUV);
		else if( iTexLayer == ( DIFFUSE_MAP | LIGHT_MAP) ) {
			gl_FragColor =(LightingColor * texture2D(diffuMap, DiffuseMapUV) + texture2D(diffuMap, DiffuseMapUV) * texture2D(lightMap, LightMapUV));
		}
	}
}
