#include "CCamera.h"

CCamera* CCamera::_pCamera = nullptr;

CCamera::CCamera() {
	_type = Type::PERSPECTIVE;
	_bViewDirty = true;
	_bProjectionDirty = true;
}

void CCamera::destroyInstance()
{
	CC_SAFE_DELETE(_pCamera);
}

CCamera::~CCamera()
{

}

// 預設是以透視投影，view 在 (0,0,0) 看向 -Z 方向
// fov = 60度
CCamera* CCamera::create()
{
	if (_pCamera == nullptr) {
		_pCamera = new (std::nothrow) CCamera;
		_pCamera->initDefault();
	}
	return _pCamera;
}

void  CCamera::initDefault()
{
	//// 設定預設鏡頭的位置
	//_viewPosition = vec4(0, 0, 0, 1.0f);
	//_lookAt = vec4(0, 0, -1.0f, 1.0f);
	//_upVector = vec4(0, 1.0f, 0, 1.0f);
	//// 產生 View Matrix
	//_view = LookAt(_viewPosition, _lookAt, _upVector);

	//// 以常用的大小開啟透視投影作為預設的鏡頭投影方式
	//_projection = Perspective(60.0, 1.0f, 1.0, 1000.0);
	//_bViewDirty = true;
	//_bProjectionDirty = true;
	//_viewProjection = _projection * _view;	// 可以先不用計算

		moveMentSpeed = 0.25f;

	// 設定預設鏡頭的位置
	_viewPosition = vec4(0, 0, 0, 1.0f);
	_lookAt = vec4(0, 0, -1.0f, 1.0f);
	_upVector = vec4(0, 1.0f, 0, 1.0f);
	// 產生 View Matrix
	_view = LookAt(_viewPosition, _lookAt, _upVector);

	// 以常用的大小開啟透視投影作為預設的鏡頭投影方式
	_projection = Perspective(60.0, 1.0f, 1.0, 1000.0);
	_bViewDirty = true;
	_bProjectionDirty = true;
	_viewProjection = _projection * _view;	// 可以先不用計算
}

CCamera* CCamera::getInstance()
{
	if ( _pCamera == nullptr ){
		_pCamera = new (std::nothrow) CCamera;
		_pCamera->initDefault();
	}
	return _pCamera;
}

void CCamera::updatePerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	_projection = Perspective(60.0, 1.0f, 1.0, 1000.0);
	_bProjectionDirty = true;
	_type = Type::PERSPECTIVE;
	_viewProjection = _projection * _view; // 同時更新  viewProjection matrix (可以先不用計算)
}

void CCamera::updateOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
	_projection = Ortho(left, right, bottom, top, nearPlane, farPlane);
	_bProjectionDirty = true;
	_type = Type::ORTHOGRAPHIC;
	_viewProjection = _projection * _view; // 同時更新  viewProjection matrix (可以先不用計算)
}

void CCamera::updateViewPosition(vec4 &vp)
{
	// 產生 View Matrix
	_viewPosition = vp;
	_view = LookAt(_viewPosition, _lookAt, _upVector);
	_bViewDirty = true;
	_viewProjection = _projection * _view; // 可以先不用計算
}
void CCamera::updateLookAt(vec4 &at)
{
	// 產生 View Matrix
	_lookAt = at;
	_view = LookAt(_viewPosition, _lookAt, _upVector);
	_bViewDirty = true;
	_viewProjection = _projection * _view;  // 可以先不用計算
}

void CCamera::updateViewLookAt(vec4 &vp, vec4 &at)
{
	// 產生 View Matrix
	_viewPosition = vp;
	_lookAt = at;
	_view = LookAt(_viewPosition, _lookAt, _upVector);
	_bViewDirty = true;
	_viewProjection = _projection * _view; // 可以先不用計算
}

const mat4& CCamera::getProjectionMatrix(bool &bProj) const
{
	bProj = _bProjectionDirty;
	if (_bProjectionDirty) _bProjectionDirty = false; // 重新取得 就設定成 false
	return _projection;
}
const mat4& CCamera::getViewMatrix(bool &bView) const
{
	bView = _bViewDirty;
	if (_bViewDirty) _bViewDirty = false; // 重新取得 就設定成 false
	return _view;
}

const mat4& CCamera::getViewProjectionMatrix() const
{
	return _viewProjection;
}


const vec4& CCamera::getViewPosition() const	// 取的 VP 矩陣
{
	return _viewPosition;
}

CCamera::Type CCamera::getProjectionType()
{
	return _type;
}




void CCamera::moveForward(int playerState) {
	vec4 viewDirection(_lookAt.x - _viewPosition.x, 0.0f, _lookAt.z - _viewPosition.z, 0.0f);
	vec4 viewPosition = _viewPosition;
	vec4 lookAt = _lookAt;
	bool result=true;

	viewDirection = normalize(viewDirection);
	viewPosition += moveMentSpeed * viewDirection;
	lookAt += moveMentSpeed * viewDirection;
	//updateViewLookAt(viewPosition, lookAt);
	result = MoveCheck(playerState, viewPosition);
	if(result){
		updateViewLookAt(viewPosition, lookAt);
	}

	//Print(viewPosition);
}

void CCamera::moveBackward(int playerState) {
	vec4 viewDirection(_lookAt.x - _viewPosition.x, 0.0f, _lookAt.z - _viewPosition.z, 0.0f);
	vec4 viewPosition = _viewPosition;
	vec4 lookAt = _lookAt;
	bool result = true;

	viewDirection = normalize(viewDirection);
	viewPosition += -moveMentSpeed * viewDirection;
	lookAt += -moveMentSpeed * viewDirection;
	//updateViewLookAt(viewPosition, lookAt);
	result = MoveCheck(playerState, viewPosition);
	if (result) {
		updateViewLookAt(viewPosition, lookAt);
	}
}

bool CCamera::MoveCheck(int playerState,vec4 viewPos) {
	//return true;
	switch (playerState) {
	case 0:
		return false;
	case 1:	
		//door to room2
		if (viewPos.x >= 8.5f && (viewPos.z<1.5f && viewPos.z>-2.0f))
			return true;
		//door to room4
		else if (viewPos.z <= -8.0f && (viewPos.x<1.5f && viewPos.x>-2.0f))
			return true;
		//room collider
		else if (viewPos.z > -8.0f && viewPos.z < 8.0f && viewPos.x > -8.5f && viewPos.x < 8.5f)
			return true;
		else
			return false;
		break;
	case 2:	
		//door to room1
		if (viewPos.x <= 11.0f && (viewPos.z<1.5f && viewPos.z>-2.0f))
			return true;
		//door to room3
		else if (viewPos.z <= -8.0f && (viewPos.x>18.0f && viewPos.x<21.5f))
			return true;
		//room collider
		else if (viewPos.z > -8.0f && viewPos.z < 8.0f && viewPos.x > 11.0f && viewPos.x < 27.0f)
			return true;
		else
			return false;
		break;
	case 3:	
		//door to room2
		if (viewPos.z >= -11.0f && (viewPos.x>18.0f && viewPos.x<21.5f))
			return true;
		//door to room4
		else if (viewPos.x <= 11.0f && (viewPos.z<-18.5f && viewPos.z>-22.0f))
			return true;
		//room collider
		else if (viewPos.z < -11.0f && viewPos.z > -27.0f && viewPos.x > 11.0f && viewPos.x < 27.0f)
			return true;
		else
			return false;
		break;
	case 4:	
		//door to room1
		 if (viewPos.z >= -11.0f && (viewPos.x<1.5f && viewPos.x>-2.0f))
			return true;
		//door to room3
		else if (viewPos.x >= 8.5f && (viewPos.z<-18.5f && viewPos.z>-22.0f))
			 return true;
		//door to room5
		else if (viewPos.z <= -27.0f && (viewPos.x<1.5f && viewPos.x>-2.0f))
			return true;
		 //room collider
		else if (viewPos.z < -11.0f && viewPos.z > -27.0f && viewPos.x > -8.5f && viewPos.x < 8.5f)
			return true;
		else
			return false;
		break;
	case 5:	
		//door to room4
		if (viewPos.z >= -30.0f && (viewPos.x<1.5f && viewPos.x>-2.0f))
			return true;
		//door to room6
		else if (viewPos.x >= 8.5f && (viewPos.z<-38.5f && viewPos.z>-42.0f))
			return true;
		//room collider
		else if (viewPos.z < -30.0f && viewPos.z > -46.0f && viewPos.x > -8.5f && viewPos.x < 8.5f)
			return true;
		else
			return false;
		break;
	case 6:	
		//door to room5
		if (viewPos.x <= 11.0f && (viewPos.z<-38.5f && viewPos.z>-42.0f))
			return true;
		//room collider
		else if (viewPos.z < -30.0f && viewPos.z > -46.0f && viewPos.x > 11.0f && viewPos.x < 27.0f)
			return true;
		else
			return false;
		break;
	default:
			return false;
		break;
	}
}