#ifndef CTEXTUREPOOL_H
#define CTEXTUREPOOL_H

#define CC_SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define CC_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)

#include "../header/Angel.h"

#define	TEXTUREPOOL_MAX 100

typedef struct structTextureElement{
	GLuint  uiTextureID;
	char   *TextureName;
	int		iTexWidth;
	int		iTexHeight;
} TextureElement;

class CTexturePool
{
	int _iNumTextures;
	TextureElement _Pool[TEXTUREPOOL_MAX];		// 預設能儲存 100 張貼圖
public:
	static CTexturePool* create();		// 建立 CTexturePool 的實體
	static CTexturePool *getInstance();	// 取得  CTexturePool 的實體

	void destroyInstance();		// 釋放取得資源
	GLuint AddTexture(char *texName);
	GLuint GetTextureID(char *texName) {}; // 沒有實作

protected:
	CTexturePool();			// 避免被誤用
	~CTexturePool();
	void initDefault(){};
	static CTexturePool* _pTexturePool;
};

#endif

