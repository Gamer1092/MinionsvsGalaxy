#include "Skybox.h"

Skybox::Skybox(Camera* cam)
{
	offset = 150.0f;
	camera = cam;

	quads[0] = new TexturedQuad(2*offset,2*offset,L"Textures/spaceTexture.dds", false);
	quads[1] = new TexturedQuad(2*offset,2*offset,L"Textures/spaceTexture.dds", false);
	quads[2] = new TexturedQuad(2*offset,2*offset,L"Textures/spaceTexture.dds", false);
	quads[3] = new TexturedQuad(2*offset,2*offset,L"Textures/spaceTexture.dds", false);
	quads[4] = new TexturedQuad(2*offset,2*offset,L"Textures/spaceTexture.dds", false);
	quads[5] = new TexturedQuad(2*offset,2*offset,L"Textures/spaceTexture.dds", false);

	quads[0]->translate(0.0f, 0.0f, offset);
	quads[1]->translate(0.0f, 0.0f, -offset);
	quads[2]->translate(offset, 0.0f, 0.0f);
	quads[3]->translate(-offset, 0.0f, 0.0f);
	quads[4]->translate(0.0f, offset, 0.0f);
	quads[5]->translate(0.0f, -offset, 0.0f);
	

	quads[1]->rotateY(XM_PI);
	quads[2]->rotateY(XM_PIDIV2);
	quads[3]->rotateY(-XM_PIDIV2);
	quads[4]->rotateX(-XM_PIDIV2);
	quads[5]->rotateX(XM_PIDIV2);

	for (int i = 0; i < 6; i++)
		objs.push_back(quads[i]);
}


Skybox::~Skybox(void)
{
}

vector<GameObject*> Skybox::getModels() { return objs; }

void Skybox::strafeLeft()
{
	XMFLOAT3 fwd = camera->getFwd();
	XMFLOAT3 up = camera->getUp();
	XMVECTOR side = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&up), XMLoadFloat3(&fwd)));
	XMFLOAT3 move;
	XMStoreFloat3(&move, side);
	for (int i = 0; i < 6; i++)
		quads[i]->translate(-CAMERA_STRAFE_SPEED * move.x, -CAMERA_STRAFE_SPEED * move.y, -CAMERA_STRAFE_SPEED * move.z);
}

void Skybox::strafeRight()
{
	XMFLOAT3 fwd = camera->getFwd();
	XMFLOAT3 up = camera->getUp();
	XMVECTOR side = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&up), XMLoadFloat3(&fwd)));
	XMFLOAT3 move;
	XMStoreFloat3(&move, side);
	for (int i = 0; i < 6; i++)
		quads[i]->translate(CAMERA_STRAFE_SPEED * move.x, CAMERA_STRAFE_SPEED * move.y, CAMERA_STRAFE_SPEED * move.z);
}

void Skybox::moveForward()
{
	XMFLOAT3 fwd = camera->getFwd();
	for (int i = 0; i < 6; i++)
		quads[i]->translate(CAMERA_DOLLY_SPEED * fwd.x, CAMERA_DOLLY_SPEED * fwd.y, CAMERA_DOLLY_SPEED * fwd.z);
}

void Skybox::moveBackward()
{
	XMFLOAT3 fwd = camera->getFwd();
	for (int i = 0; i < 6; i++)
		quads[i]->translate(-CAMERA_DOLLY_SPEED * fwd.x, -CAMERA_DOLLY_SPEED * fwd.y, -CAMERA_DOLLY_SPEED * fwd.z);
}

void Skybox::changeTexture(ID3D11Device* device, WCHAR* tex)
{
	for (int i = 0; i < 6; i++)
		quads[i]->getModel()->initTexture(device, tex);
}
