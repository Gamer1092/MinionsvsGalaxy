#include "Camera.h"

Camera::Camera(int width, int height)
{
	position =		XMFLOAT3(0.0f, 0.0f, -5.0f);
	direction =		XMFLOAT3(0.0f, 0.0f, 1.0f);
	upDirection =	XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMStoreFloat4(&orientation, XMQuaternionRotationRollPitchYaw(0, 0, 0));
	screenWidth = width;
	screenHeight = height;
	screenAspectRatio = (float)screenWidth / screenHeight;
	fieldOfView = NOMINAL_FOV_Y;
}

Camera::~Camera(void)
{
}

void Camera::updateMatrices()
{
	// rebuild direction vectors with current camera quaternion
	XMStoreFloat3(&direction, XMVector3Normalize(XMVector4Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMMatrixRotationQuaternion(XMLoadFloat4(&orientation)))));
	XMStoreFloat3(&upDirection, XMVector3Normalize(XMVector4Transform(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMMatrixRotationQuaternion(XMLoadFloat4(&orientation)))));

	//Create a view matrix based on direction camera is looking
	XMStoreFloat4x4(&m_viewMatrix, XMMatrixLookToLH(XMLoadFloat3(&position), XMLoadFloat3(&direction), XMLoadFloat3(&upDirection)));

	//Create the projection matrix for 3D rendering.
	XMStoreFloat4x4(&m_projectionMatrix, XMMatrixPerspectiveFovLH(NOMINAL_FOV_Y, screenAspectRatio, 0.01f, 300.0f));//fieldOfView, screenAspectRatio, CAMERA_SCREEN_NEAR, CAMERA_SCREEN_FAR));
}

XMFLOAT4X4 Camera::getViewMatrix() { return m_viewMatrix; }

XMFLOAT4X4 Camera::getProjectionMatrix() { return m_projectionMatrix; }

void Camera::setPosition(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z);
}
void Camera::setDirection(float x, float y, float z)
{
	XMFLOAT3 newDirection = XMFLOAT3(x, y, z);
	XMStoreFloat3(&direction, XMVector3Normalize(XMLoadFloat3(&newDirection)));
}
void Camera::setUpDirection(float x, float y, float z)
{
	XMFLOAT3 newUpDirection = XMFLOAT3(x, y, z);
	XMStoreFloat3(&upDirection, XMVector3Normalize(XMLoadFloat3(&newUpDirection)));
}

XMFLOAT3 Camera::getPos() { return position; }

XMFLOAT4 Camera::getOri() { return orientation; }

XMFLOAT3 Camera::getFwd() { return direction; }

XMFLOAT3 Camera::getUp() { return upDirection; }

void Camera::moveForward()
{
	position.x += direction.x * CAMERA_DOLLY_SPEED;
	position.y += direction.y * CAMERA_DOLLY_SPEED;
	position.z += direction.z * CAMERA_DOLLY_SPEED;
}

void Camera::moveForward(float delta)
{
	position.x += delta;
}

void Camera::moveBackward()
{
	position.x -= direction.x * CAMERA_DOLLY_SPEED;
	position.y -= direction.y * CAMERA_DOLLY_SPEED;
	position.z -= direction.z * CAMERA_DOLLY_SPEED;
}

void Camera::craneUp()
{
	position.x += upDirection.x * CAMERA_CRANE_SPEED;
	position.y += upDirection.y * CAMERA_CRANE_SPEED;
	position.z += upDirection.z * CAMERA_CRANE_SPEED;
}

void Camera::craneDown()
{
	position.x -= upDirection.x * CAMERA_CRANE_SPEED;
	position.y -= upDirection.y * CAMERA_CRANE_SPEED;
	position.z -= upDirection.z * CAMERA_CRANE_SPEED;
}

void Camera::strafeLeft()
{
	XMVECTOR sideWaysVector = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&upDirection), XMLoadFloat3(&direction)));

	position.x -= XMVectorGetX(sideWaysVector) * CAMERA_STRAFE_SPEED;
	position.y -= XMVectorGetY(sideWaysVector) * CAMERA_STRAFE_SPEED;
	position.z -= XMVectorGetZ(sideWaysVector) * CAMERA_STRAFE_SPEED;
}

void Camera::strafeRight()
{
	XMVECTOR sideWaysVector = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&upDirection), XMLoadFloat3(&direction)));

	position.x += XMVectorGetX(sideWaysVector) * CAMERA_STRAFE_SPEED;
	position.y += XMVectorGetY(sideWaysVector) * CAMERA_STRAFE_SPEED;
	position.z += XMVectorGetZ(sideWaysVector) * CAMERA_STRAFE_SPEED;
} 

void Camera::tiltDown()
{
	XMVECTOR sideWaysVector = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&upDirection), XMLoadFloat3(&direction)));
	XMVECTOR tiltRotationQuaternion = XMQuaternionRotationAxis(sideWaysVector, XM_PIDIV4/100*CAMERA_TILT_SPEED);

	XMStoreFloat4(&orientation, XMQuaternionMultiply(XMLoadFloat4(&orientation),tiltRotationQuaternion));
} 

void Camera::tiltUp()
{
	XMVECTOR sideWaysVector = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&upDirection), XMLoadFloat3(&direction)));
	XMVECTOR tiltRotationQuaternion = XMQuaternionRotationAxis(sideWaysVector, -XM_PIDIV4/100*CAMERA_TILT_SPEED);

	XMStoreFloat4(&orientation, XMQuaternionMultiply(XMLoadFloat4(&orientation),tiltRotationQuaternion));
} 

void Camera::panLeft()
{
	XMVECTOR panRotationQuaternion = XMQuaternionRotationAxis(XMLoadFloat3(&upDirection), -XM_PIDIV4/100*CAMERA_PAN_SPEED);
	XMStoreFloat4(&orientation, XMQuaternionMultiply(XMLoadFloat4(&orientation),panRotationQuaternion));
} 

void Camera::panRight()
{
	XMVECTOR panRotationQuaternion = XMQuaternionRotationAxis(XMLoadFloat3(&upDirection), XM_PIDIV4/100*CAMERA_PAN_SPEED);
	XMStoreFloat4(&orientation, XMQuaternionMultiply(XMLoadFloat4(&orientation),panRotationQuaternion));
}

void Camera::rollLeft()
{
	XMVECTOR rollRotationQuaternion = XMQuaternionRotationAxis(XMLoadFloat3(&direction), XM_PIDIV4/100*CAMERA_ROLL_SPEED);
	XMStoreFloat4(&orientation, XMQuaternionMultiply(XMLoadFloat4(&orientation),rollRotationQuaternion));
}

void Camera::rollRight()
{
	XMVECTOR rollRotationQuaternion = XMQuaternionRotationAxis(XMLoadFloat3(&direction), -XM_PIDIV4/100*CAMERA_ROLL_SPEED);
	XMStoreFloat4(&orientation, XMQuaternionMultiply(XMLoadFloat4(&orientation),rollRotationQuaternion));
}

void Camera::zoomIn()
{
	if(fieldOfView * CAMERA_ZOOM_IN_FACTOR >= MIN_CAMERA_FOV)
	fieldOfView = fieldOfView * CAMERA_ZOOM_IN_FACTOR;
}

void Camera::zoomOut()
{
    if( fieldOfView * CAMERA_ZOOM_OUT_FACTOR <= MAX_CAMERA_FOV)
	   fieldOfView = fieldOfView * CAMERA_ZOOM_OUT_FACTOR;
} 

void Camera::resetZoom()
{
	fieldOfView = NOMINAL_FOV_Y;
}
