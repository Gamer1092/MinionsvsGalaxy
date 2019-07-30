#include "Torpedo.h"

Torpedo::Torpedo(void)
{
	XMFLOAT4 torpedoColors[6];
	XMFLOAT4 torpedoBlue = XMFLOAT4(0.0f, 0.0f, 0.75f, 1.0f);
	for(int i = 0; i < 6; i++)
		torpedoColors[i] = torpedoBlue;
	body = new CubeObject(0.2f, 0.2f, 100.0f, true, torpedoColors);
	body->translate(0.0f, -0.5f, 50.0f);
	wing = new PrismObject(0.2f, 0.2f, 100.0f, torpedoColors[0]);
	wing->translate(-0.15f, -0.5f, 50.0f);
	wing2 = new PrismObject(0.02f, 0.02f, 100.0f, torpedoColors[0]);
	wing2->translate(0.15f, -0.5f, 50.0f);
	XMFLOAT4 targetColors[6];
	XMFLOAT4 targetYellow = XMFLOAT4(0.75f,0.75f,0.0f,1.0f);
	for (int i = 0; i < 6; i++)
		targetColors[i] = targetYellow;
	target = new CubeObject(0.06f, 0.06f, 0.06f, true, targetColors);
	target->translate(0.0f,0.0f,5.0f);
	firing = false;
	vertAngle = 0.0f;
	horzAngle = 0.0f;
	objs.push_back(body);
	objs.push_back(wing);
	objs.push_back(wing2);
	objs.push_back(target);
	parentMatrix = new XMFLOAT4X4;
}

Torpedo::~Torpedo(void)
{
	if(body)
		delete body;
	if(wing)
		delete wing;
	if(wing2)
		delete wing2;
	body = 0;
	wing = 0;
	wing2 = 0;
}

GameObject* Torpedo::getModel() { return body; }
GameObject* Torpedo::getModel2() { return wing; }
GameObject* Torpedo::getModel3() { return wing2; }
GameObject* Torpedo::getTargetModel() { return target; }

void Torpedo::setParentMatrix(XMFLOAT4X4* mat)
{
	parentMatrix = mat;
	body->setParentMatrix(mat);
	wing->setParentMatrix(mat);
	wing2->setParentMatrix(mat);
	target->setParentMatrix(mat);
}

void Torpedo::setParentMatrix(XMFLOAT4 ori, XMFLOAT3 pos)
{
	XMStoreFloat4x4(parentMatrix, XMMatrixIdentity() * XMMatrixRotationQuaternion(XMLoadFloat4(&ori)) * XMMatrixTranslation(pos.x, pos.y, pos.z));
	body->setParentMatrix(parentMatrix); 
	wing->setParentMatrix(parentMatrix);
	wing2->setParentMatrix(parentMatrix);
	target->setParentMatrix(parentMatrix);
}

void Torpedo::orbitX(int factor)
{
	float angle = factor * TORPEDO_TURN_SPEED;
	if (abs(vertAngle + angle) < 0.6f)
	{
		vertAngle += angle;
		body->orbitX(angle, XMFLOAT3(0.0f, 0.0f, -50.0f)); 
		wing->orbitX(angle, XMFLOAT3(0.0f, 0.0f, -50.0f));
		wing2->orbitX(angle, XMFLOAT3(0.0f, 0.0f, -50.0f));
		target->orbitX(angle, XMFLOAT3(0.0f, 0.0f, -5.0f));
	}
}

void Torpedo::orbitY(int factor)
{
	float angle = factor * TORPEDO_TURN_SPEED;
	if (abs(horzAngle + angle) < 0.6f)
	{
		horzAngle += angle;
		body->orbitY(angle, XMFLOAT3(0.0f, 0.0f, -50.0f)); 
		wing->orbitY(angle, XMFLOAT3(0.0f, 0.0f, -50.0f));
		wing2->orbitY(angle, XMFLOAT3(0.0f, 0.0f, -50.0f));
		target->orbitY(angle, XMFLOAT3(0.0f, 0.0f, -5.0f));
	}
}

bool Torpedo::isFiring() { return firing; }

void Torpedo::toggle() { firing = !firing; }

vector<GameObject*> Torpedo::getModels() { return objs; }