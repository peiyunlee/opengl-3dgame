// 
// Cubic mapping (Environment Mapping)
// 
// 執行前的準備工作
// 關閉 CShape.h 中的 #define PERVERTEX_LIGHTING，使用 PERPIXEL_LIGHTING 才會有作用
// 設定 #define MULTITEXTURE  (DIFFUSE_MAP|LIGHT_MAP|NORMAL_MAP)
// 開啟 #define CUBIC_MAP 1

#include "header/Angel.h"
#include "Common/CQuad.h"
#include "Common/CSolidCube.h"
#include "Common/CSolidSphere.h"
#include "Common/CWireSphere.h"
#include "Common/CWireCube.h"
#include "Common/CChecker.h"
#include "Common/CCamera.h"
#include "Common/CTexturePool.h"
#include "png_loader.h"
//#include "Common/ModelPool.h"
#include "Common/C2DSprite.h"
#include "Common/Room.h"

#define SPACE_KEY 32
#define SCREEN_SIZE 800
#define HALF_SIZE SCREEN_SIZE /2 
#define VP_HALFWIDTH  20.0f
#define VP_HALFHEIGHT 20.0f
#define GRID_SIZE 20 // must be an even number

#define RED_BUTTON   0
#define GREEN_BUTTON 1
#define BLUE_BUTTON  2
#define WHITE_BUTTON 3

bool rgbWork[3] = { false };	//判斷RGB得到
bool isRGBBtnDown[3] = { false };	//判斷RGB按下
bool isBtnGet[3] = { false };	//判斷得到按鈕
bool isBtnDown[4] = { false };	//判斷按鈕按下
bool canShoot = false;

// For Model View and Projection Matrix
mat4 g_mxModelView(1.0f);
mat4 g_mxProjection;

// For View Point
GLfloat g_fRadius = 80.0;
GLfloat g_fTheta = 45.0f*DegreesToRadians;
GLfloat g_fPhi = 45.0f*DegreesToRadians;

point4  g_vEye(g_fRadius*sin(g_fTheta)*cos(g_fPhi), g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), 1.0);
point4  g_vAt(0.0, 0.0, 0.0, 1.0);
vec4    g_vUp(0.0, 1.0, 0.0, 0.0);

// 2D 介面所需要的相關變數
C2DSprite *g_p2DBtn[4];
C2DSprite *shoot;
mat4  g_2DView = mat4(1, 0, 0, 0
	, 0, 1, 0, 0
	, 0, 0, 1, 0
	, 0, 0, -1, 1);;
mat4  g_2DProj;

Room *room1,*room2, *room3, *room4, *room5, *room6, *room7, *room8, *room9;;

// 函式的原型宣告
extern void IdleProcess();
void UIGenerator();
void UIAction(vec2 pt);
void GameActionSystem();
void Shoot();

int playerState;
int UpdatePlayerState();

bool leftbtndown = false;
float beforeX = 0;

enum PLAYERSTATE
{
	OUTSIDE=0,
	ROOM1=1,
	ROOM2,
	ROOM3,
	ROOM4,
	ROOM5,
	ROOM6,
};

void init( void )
{
	mat4 mxT;
	vec4 vT, vColor;
	// 產生所需之 Model View 與 Projection Matrix
	// 產生所需之 Model View 與 Projection Matrix

	//point4  eye(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);
	//point4  at(0.0f, 0.0f, 0.0f, 1.0f);
	point4  eye(g_fRadius*sin(g_fTheta)*sin(g_fPhi)-40.0f, g_fRadius*cos(g_fTheta)-50.0f, g_fRadius*sin(g_fTheta)*cos(g_fPhi)-40.0f, 1.0f);
	point4  at(0.0f, 2.0f, 0.0f, 1.0f);

	auto camera = CCamera::create();
	camera->updateViewLookAt(eye, at);
	camera->updatePerspective(60.0, (GLfloat)SCREEN_SIZE / (GLfloat)SCREEN_SIZE, 1.0, 1000.0);
	
	UIGenerator();

	room1 = new Room1(0.0f,0.0f,0.0f,eye);
	room2 = new Room2(20.001f, 0.0f, 0.0f, eye);
	room3 = new Room3(20.001f, 0.0f, -20.001f, eye);
	room4 = new Room4(0.0f, 0.0f, -20.001f, eye);
	room5 = new Room5(0.0f, 0.0f, -40.002f , eye);
	room6 = new Room6(20.001f, 0.0f, -40.002f, eye);
	room7 = new Room7(40.002f, 0.0f, -60.003f, eye);
	room8 = new Room7(40.002f, 20.001f, -40.002f, eye);
	room9 = new Room7(20.001f, 20.001f, -60.003f, eye);

	// 因為本範例不會動到 Projection Matrix 所以在這裡設定一次即可
	// 就不寫在 OnFrameMove 中每次都 Check
	bool bPDirty;
	mat4 mpx = camera->getProjectionMatrix(bPDirty);

	room1->SetProjectionMatrix(mpx);
	room2->SetProjectionMatrix(mpx);
	room3->SetProjectionMatrix(mpx);
	room4->SetProjectionMatrix(mpx);
	room5->SetProjectionMatrix(mpx);
	room6->SetProjectionMatrix(mpx);
	room7->SetProjectionMatrix(mpx);
	room8->SetProjectionMatrix(mpx);
	room9->SetProjectionMatrix(mpx);
}

void GL_Display( void )
{
	auto camera = CCamera::getInstance();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window

	glEnable(GL_BLEND);  // 設定2D Texure Mapping 有作用
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	room9->Draw(camera->getViewPosition());
	room8->Draw(camera->getViewPosition());
	room7->Draw(camera->getViewPosition());
	room6->Draw(camera->getViewPosition());
	room1->Draw(camera->getViewPosition());
	room2->Draw(camera->getViewPosition());
	room5->Draw(camera->getViewPosition());
	room4->Draw(camera->getViewPosition());
	room3->Draw(camera->getViewPosition());

	g_p2DBtn[WHITE_BUTTON]->Draw();
	if(isBtnGet[0])
		g_p2DBtn[RED_BUTTON]->Draw();
	if (isBtnGet[1])
		g_p2DBtn[GREEN_BUTTON]->Draw();
	if (isBtnGet[2])
		g_p2DBtn[BLUE_BUTTON]->Draw();

	if(canShoot && playerState == ROOM2)
		shoot->Draw();

	glDisable(GL_BLEND);	// 關閉 Blending
	glutSwapBuffers();	// 交換 Frame Buffer
}

void onFrameMove(float delta)
{
	mat4 mvx;	// view matrix & projection matrix
	bool bVDirty;	// view 與 projection matrix 是否需要更新給物件
	auto camera = CCamera::getInstance();
	mvx = camera->getViewMatrix(bVDirty);
	if (bVDirty) {
		room1->SetViewMatrix(mvx, camera->getViewPosition());
		room2->SetViewMatrix(mvx, camera->getViewPosition());
		room3->SetViewMatrix(mvx, camera->getViewPosition());
		room4->SetViewMatrix(mvx, camera->getViewPosition());
		room5->SetViewMatrix(mvx, camera->getViewPosition());
		room6->SetViewMatrix(mvx, camera->getViewPosition());
		room7->SetViewMatrix(mvx, camera->getViewPosition());
		room8->SetViewMatrix(mvx, camera->getViewPosition());
		room9->SetViewMatrix(mvx, camera->getViewPosition());
	}

	playerState = UpdatePlayerState();
	room1->Update(delta);
	room2->Update(delta);
	room3->Update(delta);
	room4->Update(delta);
	room5->Update(delta);
	room6->Update(delta);
	room7->Update(delta);
	room8->Update(delta);
	room9->Update(delta);

	Shoot();
	GL_Display();
}

void Win_Keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
	case  SPACE_KEY:
		//外掛
		room4->g_bAutoRotating = !room4->g_bAutoRotating;
		break;
	case 82: // R key
		if (rgbWork[0] && playerState == ROOM1)
			isRGBBtnDown[0] = true;
		break;
	case 114: // r key
		if (rgbWork[0] && playerState == ROOM1)
			isRGBBtnDown[0] = true;
		break;
	case 71: // G key
		if (rgbWork[1] && playerState == ROOM1)
			isRGBBtnDown[1] = true;
		break;
	case 103: // g key
		if (rgbWork[1] && playerState == ROOM1)
			isRGBBtnDown[1] = true;
		break;
	case 66: // B key
		if (rgbWork[2] && playerState == ROOM1)
			isRGBBtnDown[2] = true;
		break;
	case 98: // b key
		if (rgbWork[2] && playerState == ROOM1)
			isRGBBtnDown[2] = true;
		break;

    case 033:
		glutIdleFunc( NULL );
		CCamera::getInstance()->destroyInstance();
		CTexturePool::getInstance()->destroyInstance();

		for (int i = 0; i < 4; i++)
		{
			delete g_p2DBtn[i];
		}
		delete shoot;

		delete room1;
		delete room2;
		delete room3;
		delete room4;
		delete room5;
		delete room6;
		delete room7;
		delete room8;
		delete room9;

        exit( EXIT_SUCCESS );
        break;
    }
}

void Win_KeyboardUp(unsigned char key, int x, int y)
{
	switch (key) {
	case 82: // R key
		if (rgbWork[0] && playerState == ROOM1)
			isRGBBtnDown[0] = false;
		break;
	case 114: // r key
		if (rgbWork[0] && playerState == ROOM1)
			isRGBBtnDown[0] = false;
		break;
	case 71: // G key
		if (rgbWork[1] && playerState == ROOM1)
			isRGBBtnDown[1] = false;
		break;
	case 103: // g key
		if (rgbWork[1] && playerState == ROOM1)
			isRGBBtnDown[1] = false;
		break;
	case 66: // B key
		if (rgbWork[2] && playerState == ROOM1)
			isRGBBtnDown[2] = false;
		break;
	case 98: // b key
		if (rgbWork[2] && playerState == ROOM1)
			isRGBBtnDown[2] = false;
		break;

		break;
	}
}

inline void ScreenToUICoordinate(int x, int y, vec2 &pt)
{
	pt.x = 2.0f*(float)x / SCREEN_SIZE - 1.0f;
	pt.y = 2.0f*(float)(SCREEN_SIZE - y) / SCREEN_SIZE - 1.0f;
}

void Win_Mouse(int button, int state, int x, int y) {
	vec2 pt;
	switch(button) {
		case GLUT_LEFT_BUTTON:   // 目前按下的是滑鼠左鍵
			if (state == GLUT_DOWN) {
				ScreenToUICoordinate(x, y, pt);
				UIAction(pt);
				leftbtndown = true;
				beforeX = x;

				GameActionSystem();
			}
			else if(state == GLUT_UP){
				leftbtndown = false;
				ScreenToUICoordinate(x, y, pt);
				UIAction(pt);
			}
			break;
		case GLUT_MIDDLE_BUTTON:  // 目前按下的是滑鼠中鍵 ，換成 Y 軸
			//if ( state == GLUT_DOWN ) ; 
			break;
		case GLUT_RIGHT_BUTTON:   // 目前按下的是滑鼠右鍵
			//if ( state == GLUT_DOWN ) ;
			break;
		default:
			break;
	} 
}

void Win_SpecialKeyboard(int key, int x, int y) {

	switch(key) {
		case GLUT_KEY_LEFT:		// 目前按下的是向左方向鍵

			break;
		case GLUT_KEY_RIGHT:	// 目前按下的是向右方向鍵

			break;
		case GLUT_KEY_UP:	// 目前按下的是向上方向鍵
			if (isRGBBtnDown[0] || isRGBBtnDown[1] || isRGBBtnDown[2]) {
				if (isRGBBtnDown[0]) {
					if (room1->g_fLightR <= 1.0f) room1->g_fLightR += 0.05f;
					room1->g_Light[0].diffuse.x = room1->g_fLightR;
					//room1->g_pLight[0].SetColor(room1->g_Light[0].diffuse);
				}
				if (isRGBBtnDown[1]) {
					if (room1->g_fLightG <= 1.0f) room1->g_fLightG += 0.05f;
					room1->g_Light[0].diffuse.y = room1->g_fLightG;
					//room1->g_pLight[0].SetColor(room1->g_Light[0].diffuse);
				}
				if (isRGBBtnDown[2]) {
					if (room1->g_fLightB <= 1.0f) room1->g_fLightB += 0.05f;
					room1->g_Light[0].diffuse.z = room1->g_fLightB;
					//room1->g_pLight[0].SetColor(room1->g_Light[0].diffuse);
				}
			}
			else {
				if(room5->roomState == room5->DONE)
					CCamera::getInstance()->moveForward(playerState, true);
				else
					CCamera::getInstance()->moveForward(playerState, false);
			}
			break;
		case GLUT_KEY_DOWN:	// 目前按下的是向下方向鍵
			if (isRGBBtnDown[0] || isRGBBtnDown[1] || isRGBBtnDown[2]) {
				if (isRGBBtnDown[0]) {
					if (room1->g_fLightR >= 0.0f) room1->g_fLightR -= 0.05f;
					room1->g_Light[0].diffuse.x = room1->g_fLightR;
					//room1->g_pLight[0].SetColor(room1->g_Light[0].diffuse);
				}
				if (isRGBBtnDown[1]) {
					if (room1->g_fLightG >= 0.0f) room1->g_fLightG -= 0.05f;
					room1->g_Light[0].diffuse.y = room1->g_fLightG;
					//room1->g_pLight[0].SetColor(room1->g_Light[0].diffuse);
				}
				if (isRGBBtnDown[2]) {
					if (room1->g_fLightB >= 0.0f) room1->g_fLightB -= 0.05f;
					room1->g_Light[0].diffuse.z = room1->g_fLightB;
					//room1->g_pLight[0].SetColor(room1->g_Light[0].diffuse);
				}
			}
			else {
				if (room5->roomState == room5->DONE)
					CCamera::getInstance()->moveBackward(playerState, true);
				else
					CCamera::getInstance()->moveBackward(playerState, false);
			}
			break;
		default:
			break;
	}
}

void Win_PassiveMotion(int x, int y) {
	//g_fPhi = (float)-M_PI*(x - HALF_SIZE) / (HALF_SIZE);  // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
	//g_fTheta = (float)M_PI*(float)y / SCREEN_SIZE;;
	//point4  eye(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);
	//point4  at(0.0f, 0.0f, 0.0f, 1.0f);
	//auto camera = CCamera::getInstance();
	//camera->updateViewLookAt(eye, at);

	//g_fPhi = (float)-M_PI*(x - HALF_SIZE) / (HALF_SIZE);   // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
	//g_fTheta = (float)-M_PI*y / (SCREEN_SIZE);

	//point4  at(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);

	//CCamera::getInstance()->updateLookAt(at);

	////room3->RotateBillboard(g_fPhi);
	////Print(g_fPhi);

	if (x <= 450 || x > 450 && y <= 710) {
		if (leftbtndown) {
			g_fPhi += (float)-M_PI*((x - beforeX) / (HALF_SIZE));   // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
			g_fTheta = (float)-M_PI*(y) / (SCREEN_SIZE);

			point4  at(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);

			CCamera::getInstance()->updateLookAt(at);

			beforeX = x;
			//Print(at);
		}
	}
}

void Win_MouseMotion(int x, int y) {
	//g_fPhi = (float)-M_PI*(x - HALF_SIZE) / (HALF_SIZE); // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
	//g_fTheta = (float)M_PI*(float)y / SCREEN_SIZE;
	//point4  eye(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);
	//point4  at(0.0f, 0.0f, 0.0f, 1.0f);
	//auto camera = CCamera::getInstance();
	//camera->updateViewLookAt(eye, at);

	//g_fPhi = (float)-M_PI*(x - HALF_SIZE) / (HALF_SIZE);   // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
	//g_fTheta = (float)-M_PI*y / (SCREEN_SIZE);

	//point4  at(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);
	//CCamera::getInstance()->updateLookAt(at);

	////room3->RotateBillboard(g_fPhi);
	if (x <= 450 || x > 450 && y <= 710) {
		if (leftbtndown) {
			g_fPhi += (float)-M_PI*((x - beforeX) / (HALF_SIZE));   // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
			g_fTheta = (float)-M_PI*(y) / (SCREEN_SIZE);

			point4  at(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);

			//room3->RotateBillboard(g_fPhi);
			beforeX = x;

			CCamera::getInstance()->updateLookAt(at);
		}
	}
}

void GL_Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black background
	glEnable(GL_DEPTH_TEST);
}

int main( int argc, char **argv )
{
    
	glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( SCREEN_SIZE, SCREEN_SIZE );

	// If you use freeglut the two lines of code can be added to your application 
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );

    glutCreateWindow("Compositing and Blending");

	// The glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), 
	// which ensures that all extensions with valid entry points will be exposed.
	glewExperimental = GL_TRUE; 
    glewInit();  

    init();

	glutMouseFunc(Win_Mouse);
	glutMotionFunc(Win_MouseMotion);
	glutPassiveMotionFunc(Win_PassiveMotion);  
    glutKeyboardFunc( Win_Keyboard );	// 處理 ASCI 按鍵如 A、a、ESC 鍵...等等
	glutSpecialFunc( Win_SpecialKeyboard);	// 處理 NON-ASCI 按鍵如 F1、Home、方向鍵...等等
	glutKeyboardUpFunc(Win_KeyboardUp);	// 處理 NON-ASCI 按鍵如 F1、Home、方向鍵...等等
    glutDisplayFunc( GL_Display );
	glutReshapeFunc( GL_Reshape );
	glutIdleFunc( IdleProcess );
	
    glutMainLoop();
    return 0;
}


void UIGenerator() {
	// 以下為利用平行投影產生 2D 的介面
	// 範圍在 X/Y 平面的  -1 到 1 之間，介面都放在 Z = 0 
	mat4 mxT2D, mxS2D;
	vec4 vColor2D = vec4(0, 0, 0, 1);

	//白色按鈕
	g_p2DBtn[0] = new C2DSprite; g_p2DBtn[0]->SetShader_2DUI();
	vColor2D.x = 1; vColor2D.y = 0; vColor2D.z = 1; g_p2DBtn[0]->SetDefaultColor(vColor2D);
	mxS2D = Scale(0.1f, 0.1f, 1.0f);
	mxT2D = Translate(0.2f, -0.85f, 0);
	g_p2DBtn[0]->SetTRSMatrix(mxT2D*mxS2D);
	g_p2DBtn[0]->SetViewMatrix(g_2DView);
	g_p2DBtn[0]->SetViewMatrix(g_2DProj);

	//藍色按鈕
	g_p2DBtn[1] = new C2DSprite; g_p2DBtn[1]->SetShader_2DUI();
	vColor2D.x = 1; vColor2D.y = 1; vColor2D.z = 0; g_p2DBtn[1]->SetDefaultColor(vColor2D);
	mxT2D = Translate(0.4f, -0.85f, 0);
	g_p2DBtn[1]->SetTRSMatrix(mxT2D*mxS2D);
	g_p2DBtn[1]->SetViewMatrix(g_2DView);
	g_p2DBtn[1]->SetViewMatrix(g_2DProj);

	//綠色按鈕
	g_p2DBtn[2] = new C2DSprite; g_p2DBtn[2]->SetShader_2DUI();
	vColor2D.x = 0; vColor2D.y = 1; vColor2D.z = 1; g_p2DBtn[2]->SetDefaultColor(vColor2D);
	mxT2D = Translate(0.6f, -0.85f, 0);
	g_p2DBtn[2]->SetTRSMatrix(mxT2D*mxS2D);
	g_p2DBtn[2]->SetViewMatrix(g_2DView);
	g_p2DBtn[2]->SetViewMatrix(g_2DProj);

	//紅色按鈕
	g_p2DBtn[3] = new C2DSprite; g_p2DBtn[3]->SetShader_2DUI();
	vColor2D.x = 0.85; vColor2D.y = 0.85; vColor2D.z = 0.85; g_p2DBtn[3]->SetDefaultColor(vColor2D);
	mxT2D = Translate(0.8f, -0.85f, 0);
	g_p2DBtn[3]->SetTRSMatrix(mxT2D*mxS2D);
	g_p2DBtn[3]->SetViewMatrix(g_2DView);
	g_p2DBtn[3]->SetViewMatrix(g_2DProj);


	shoot = new C2DSprite(1);
	//vColor2D.x = 0.81; vColor2D.y = 0.81; vColor2D.z = 0.81; vColor2D.w = 0.5; 
	//shoot->SetDefaultColor(vColor2D);
	shoot->SetShader_2DUI();
	mxT2D = Translate(0.0f, 0.0f, 0);
	shoot->SetTRSMatrix(mxT2D*mxS2D);
	shoot->SetViewMatrix(g_2DView);
	shoot->SetViewMatrix(g_2DProj);
}

void UIAction(vec2 pt) {
	if (isBtnGet[0]) {
		if (g_p2DBtn[RED_BUTTON]->OnTouches(pt)) {
			if (g_p2DBtn[0]->getButtonStatus()) {
				//room1->g_Light[1].isLighting = false;
				//if(room1->roomState == room1->DONE)
				isBtnDown[RED_BUTTON] = true;
			}
		}
	}
	if (isBtnGet[1]) {
		if (g_p2DBtn[GREEN_BUTTON]->OnTouches(pt)) {
			if (g_p2DBtn[1]->getButtonStatus()) {
				//if (room1->roomState == room1->DONE)
				isBtnDown[GREEN_BUTTON] = true;
			}
			else {
			}
		}
	}
	if (isBtnGet[2]) {
		if (g_p2DBtn[BLUE_BUTTON]->OnTouches(pt)) {
			if (g_p2DBtn[2]->getButtonStatus()) {
				//room1->g_Light[3].isLighting = false;
				//if (room1->roomState == room1->DONE)
				isBtnDown[BLUE_BUTTON] = true;
			}
			else {
			}
		}
	}
	if (g_p2DBtn[WHITE_BUTTON]->OnTouches(pt)) {
		if (g_p2DBtn[3]->getButtonStatus()) {
			//room1->g_Light[0].isLighting = false;
			isBtnDown[WHITE_BUTTON] = true;
		}
		else {
		}
	}
}

int UpdatePlayerState() {
	vec4 cameravp = CCamera::getInstance()->_viewPosition;

	if (cameravp.z > -9.5f && cameravp.z < 9.5f && cameravp.x < 9.5f && cameravp.x > -9.5f) {
		return PLAYERSTATE::ROOM1;
	}
	else if (cameravp.z > -9.5f && cameravp.z < 9.5f  && cameravp.x >= 9.5f && cameravp.x < 27.0f) {
		return PLAYERSTATE::ROOM2;
	}
	else if (cameravp.z <= -9.5f && cameravp.z > -29.5f  &&  cameravp.x < 9.5f && cameravp.x > -9.5f) {
		return PLAYERSTATE::ROOM4;
	}
	else if (cameravp.z <= -9.5f && cameravp.z > -29.5f  && cameravp.x >= 9.5f && cameravp.x < 27.0f) {
		if (cameravp.z <= -11.5f && cameravp.z > -27.5f  && cameravp.x >= 11.5f && cameravp.x < 25.0f)
			room3->g_bAutoRotating = true;
		else
			room3->g_bAutoRotating = false;
		return PLAYERSTATE::ROOM3;
	}
	else if (cameravp.z <= -29.5f && cameravp.z > -49.5f  &&  cameravp.x < 9.5f && cameravp.x > -9.5f) {
		return PLAYERSTATE::ROOM5;
	}
	else if (cameravp.z <= -29.5f && cameravp.z > -49.5f  && cameravp.x >= 9.5f && cameravp.x < 27.0f) {
		return PLAYERSTATE::ROOM6;
	}
	return PLAYERSTATE::OUTSIDE;
}

void GameActionSystem() {
	//左鍵按下

	switch (playerState)
	{
	case PLAYERSTATE::ROOM1:
		if (isBtnDown[WHITE_BUTTON] && room1->roomState < room1->DONE) {
			if (room1->roomState == room1->LEVEL0) {
				room1->ChangeLevel(1, isBtnGet[0], isBtnGet[1], isBtnGet[2]);	//貓吃老鼠按鈕出現TOLEVEL1
			}
			else if (room1->roomState == room1->LEVEL1) {	//等到三個按鈕拿到才會換LEVEL2
				room1->ChangeLevel(2, isBtnGet[0], isBtnGet[1], isBtnGet[2]);	//判斷可否拿按鈕
			}
			isBtnDown[WHITE_BUTTON] = false;
		}
		break;
	case PLAYERSTATE::ROOM2:
		//如果房間結束rgbWork[2]=true;
		if (isBtnDown[WHITE_BUTTON] && room2->roomState < room2->DONE) {
			if (room2->roomState == room2->LEVEL0 && canShoot) {
				room2->ChangeLevel(1, isBtnGet[0], isBtnGet[1], isBtnGet[2]);	//按按鈕牆壁換圖片拿到B
				rgbWork[2] = true;	//done
			}
			isBtnDown[WHITE_BUTTON] = false;
		}
		break;
	case PLAYERSTATE::ROOM3:
		//如果房間結束rgbWork[1]=true;
		if (isBtnDown[WHITE_BUTTON] && room3->roomState < room3->DONE) {
			if (room3->roomState == room3->LEVEL0) {
				room3->ChangeLevel(1, isBtnGet[0], isBtnGet[1], isBtnGet[2]);	//按按鈕牆壁換圖片拿到B
			}
			else if (room3->roomState == room3->LEVEL1) {
				room3->ChangeLevel(2, isBtnGet[0], isBtnGet[1], isBtnGet[2]);	//按按鈕牆壁換圖片拿到g
				rgbWork[1] = true;	//done
			}
			isBtnDown[WHITE_BUTTON] = false;
		}
		break;
	case PLAYERSTATE::ROOM4:
		//如果房間結束rgbWork[0]=true;
		if (isBtnDown[WHITE_BUTTON] && room4->roomState < room4->DONE) {
			if (room4->roomState == room4->LEVEL0) {
				room4->ChangeLevel(1, isBtnGet[0], isBtnGet[1], isBtnGet[2]);	//按按鈕牆壁換圖片拿到r
				rgbWork[0] = true;	//done
			}
			isBtnDown[WHITE_BUTTON] = false;
		}
		break;
	case PLAYERSTATE::ROOM5:
		if (room5->roomState < room5->DONE) {
			if (isBtnDown[0] && room5->roomState == room5->LEVEL0) {
				room5->ChangeLevel(1, isBtnGet[0], isBtnGet[1], isBtnGet[2]);	//第一塊紅色積木
				isBtnDown[RED_BUTTON] = false;
			}
			else if (isBtnDown[2] && room5->roomState == room5->LEVEL1) {
				room5->ChangeLevel(2, isBtnGet[0], isBtnGet[1], isBtnGet[2]);	//第二塊綠色積木
				isBtnDown[BLUE_BUTTON] = false;
			}
			else if (isBtnDown[1] && room5->roomState == room5->LEVEL2) {
				room5->ChangeLevel(3, isBtnGet[0], isBtnGet[1], isBtnGet[2]);	//第三塊黃色積木
				isBtnDown[GREEN_BUTTON] = false;
			}
			else if (isBtnDown[2] && room5->roomState == room5->LEVEL3) {
				room5->ChangeLevel(4, isBtnGet[0], isBtnGet[1], isBtnGet[2]);	//第四塊綠色積木
				isBtnDown[BLUE_BUTTON] = false;
			}
		}
		break;
	case PLAYERSTATE::ROOM6:
		if (isBtnDown[WHITE_BUTTON] && room6->roomState < room6->DONE) {
			if (room6->roomState == room6->LEVEL0) {
				room6->ChangeLevel(1, isBtnGet[0], isBtnGet[1], isBtnGet[2]);	//按按鈕牆壁換圖片拿到r
			}
			isBtnDown[WHITE_BUTTON] = false;
		}
		break;
	default:
		break;
	}
	isBtnDown[WHITE_BUTTON] = false;
	isBtnDown[RED_BUTTON] = false;
	isBtnDown[BLUE_BUTTON] = false;
	isBtnDown[GREEN_BUTTON] = false;
}

void Shoot() {
	vec4 pos = CCamera::getInstance()->_viewPosition;
	vec4 attest = CCamera::getInstance()->_lookAt;
	float tx, ty;
	float t;
	t = (-10.0f-pos.z) / attest.z;
	tx = pos.x + attest.x*t;
	ty = pos.y + attest.y*t;
	if (tx >= 20.5f && tx <= 22.0f && ty >= 10.5f && ty <= 12.0f && attest.y >=0 && room2->roomState == room2->LEVEL0) canShoot = true;
	else canShoot = false;
	//Print(tx);
	//Print(ty);
	//Print(attest);
}
