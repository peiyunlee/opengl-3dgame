#ifndef TYPEDEFINE_H
#define TYPEDEFINE_H
#include "../Header/Angel.h"

#define NONE_MAP    0x0
#define DIFFUSE_MAP 0x1
#define LIGHT_MAP   0x2
#define NORMAL_MAP  0x4

// 
// bit 2 : Normal Map
// iTexLayer 的設定與判斷方式，假設此處可以支援到六張貼圖
// bit 0 : diffuce Map
// bit 1 : Light Map
// bit 2 : 
// 
//
//


typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

typedef struct MaterialParameters {
	color4 ambient;		// ambient 顏色
	color4 diffuse;		
	color4 specular;
	color4 emission;
	float  ka, kd, ks;	// 簡化計算, 假設 RGB 三個顏色上的反射係數皆相同
	float  shininess;
} Material;

typedef struct LightSourceParameters  
{  
    color4 ambient;  
    color4 diffuse;  
    color4 specular;  
    point4 position;  
    point4 halfVector;  
    vec3 spotDirection; 
    float spotExponent;  
    float spotCutoff; // (range: [0.0,90.0], 180.0)  
    float spotCosCutoff; // (range: [1.0,0.0],-1.0)  
    float constantAttenuation;  
    float linearAttenuation;  
    float quadraticAttenuation;  
} LightSource;


#endif