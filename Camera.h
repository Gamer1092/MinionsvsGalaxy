#pragma once
#include <DirectXMath.h>
using namespace DirectX;

const float CAMERA_TILT_SPEED = 2.0f; //up and down rotation about the sideways vector direction
const float CAMERA_PAN_SPEED = 2.0f; //left and right rotation about the up vector
const float CAMERA_ROLL_SPEED = 2.0f; //left and right rotation about the camera direction vector
const float CAMERA_DOLLY_SPEED = 0.15f;; //used for forward and backward travel along the camera direction vector
const float CAMERA_STRAFE_SPEED = 0.15f; //sideways translation along the sideways vector direction
const float CAMERA_CRANE_SPEED = 0.15f; //up and down translatoin along the up vector direction
const float CAMERA_ZOOM_IN_FACTOR = 0.95f; //field of view mult. factor to zoom in
const float CAMERA_ZOOM_OUT_FACTOR = 1.05f; //field of view mult. factor to zoom out

const float	NOMINAL_FOV_Y  =  60.0f * XM_PI / 180.0f;
const float MAX_CAMERA_FOV = 170.0f * XM_PI / 180.0f;
const float MIN_CAMERA_FOV =  10.0f * XM_PI / 180.0f;

const float CAMERA_SCREEN_FAR = 1000.0f;
const float CAMERA_SCREEN_NEAR = 0.1f;

class Camera
{
private:
	int screenWidth;
	int screenHeight;
	float screenAspectRatio;
	float fieldOfView;
	XMFLOAT3 position; //position in 3-space of the camera
	XMFLOAT4 orientation;

	XMFLOAT3 direction; //direction the camera is pointing
	XMFLOAT3 upDirection;

	XMFLOAT4X4 m_viewMatrix;
	XMFLOAT4X4 m_projectionMatrix;

public:
	Camera(int, int);
	~Camera(void);

	void updateMatrices();
	XMFLOAT4X4 getViewMatrix();
	XMFLOAT4X4 getProjectionMatrix();

	void setPosition(float, float, float);
	void setDirection(float, float, float);
	void setUpDirection(float, float, float);
	XMFLOAT3 getPos();
	XMFLOAT4 getOri();
	XMFLOAT3 getFwd();
	XMFLOAT3 getUp();

	void moveForward(); //translate forward along camera direction vector
	void moveForward(float);
	void moveBackward(); //translate backwards along camera direction vector
	void strafeLeft(); //translate left along camera sideways vector
	void strafeRight(); //translate right along camera sideways vector
	void craneUp(); //translate up along camera up vector
	void craneDown(); //translate down along camera up vector
	void tiltUp(); //rotate lens, or view, upwards around camera sideways vector
	void tiltDown(); //rotate lens, or view, downwards around camera sideways vector
	void panLeft(); //rotate left around camera up vector
	void panRight(); //rotate right around cameara up vector
	void rollLeft(); //rotate left around camera direction vector
	void rollRight(); //rotate right around cameara direction vector
	void zoomIn(); //increase the effective focal length for camera lens (smaller field of view angle)
	void zoomOut(); //decrease the effectigve focal length of the camera lens (greater field of view angle)
	void resetZoom();
};

