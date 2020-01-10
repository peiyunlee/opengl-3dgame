#version 130
// iTexLayer 的設定與判斷方式，假設此處可以支援到六張貼圖
#define NONE_MAP    0
#define DIFFUSE_MAP 1
#define LIGHT_MAP   2

#define LIGHTMAP_COLOR

in vec4 vColor;
in vec2 DiffuseMapUV;   // 輸入 Diffuse Map 貼圖座標
in vec2 LightMapUV;     // 輸入 Light Map 貼圖座標

uniform int iTexLayer;
uniform float fElapsedTime;

// For Texture Sampler
uniform sampler2D diffuMap; // 貼圖的參數設定
uniform sampler2D lightMap; // 貼圖的參數設定

void main()
{
#ifndef LIGHTMAP_COLOR
	if( iTexLayer == NONE_MAP ) gl_FragColor = vColor;
	else if( iTexLayer == DIFFUSE_MAP ) gl_FragColor = vColor * texture2D(diffuMap, DiffuseMapUV);
	else if( iTexLayer == (DIFFUSE_MAP|LIGHT_MAP)) gl_FragColor = (vColor * texture2D(diffuMap, DiffuseMapUV)) + (texture2D(diffuMap, DiffuseMapUV)* texture2D(lightMap, LightMapUV) );
#else
	
	if( iTexLayer == NONE_MAP ) gl_FragColor = vColor;
	else if( iTexLayer == DIFFUSE_MAP ) gl_FragColor = vColor * texture2D(diffuMap, DiffuseMapUV);
	else if( iTexLayer == (DIFFUSE_MAP|LIGHT_MAP) ) {
		gl_FragColor = (vColor * texture2D(diffuMap, DiffuseMapUV)) + (texture2D(diffuMap, DiffuseMapUV) * texture2D(lightMap, LightMapUV) * vec4(0.0f, 0.05f, 0.55f, 1.0));
	}
#endif
}
