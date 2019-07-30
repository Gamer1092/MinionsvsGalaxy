#include "Laser.h"

Laser::Laser(void)
{
	XMFLOAT4 laserColors[6];
	XMFLOAT4 laserRed = XMFLOAT4(0.75f,0.0f,0.0f,1.0f);
	for (int i = 0; i < 6; i++)
		laserColors[i] = laserRed;
	beam = new CubeObject(0.02f, 0.02f, 100.0f, true, laserColors);
	beam->translate(-0.3f,-0.5f,50.0f);
	beam2 = new CubeObject(0.02f, 0.02f, 100.0f, true, laserColors);
	beam2->translate(0.3f,-0.5f,50.0f);
	XMFLOAT4 targetColors[6];
	XMFLOAT4 targetYellow = XMFLOAT4(0.75f,0.75f,0.0f,1.0f);
	for (int i = 0; i < 6; i++)
		targetColors[i] = targetYellow;
	target = new CubeObject(0.06f, 0.06f, 0.06f, true, targetColors);
	target->translate(0.0f,0.0f,5.0f);
	firing = false;
	vertAngle = 0.0f;
	horzAngle = 0.0f;
	objs.push_back(beam);
	objs.push_back(beam2);
	objs.push_back(target);
	parentMatrix = new XMFLOAT4X4;
}

Laser::~Laser(void)
{
	if(beam) delete beam; beam = 0;
	if(target) delete target; target = 0;
}

GameObject* Laser::getModel() { return beam; }
GameObject* Laser::getModel2() { return beam2; }
GameObject* Laser::getTargetModel() { return target; }

void Laser::setParentMatrix(XMFLOAT4X4* mat) 
{ 
	parentMatrix = mat;
	beam->setParentMatrix(mat); 
	beam2->setParentMatrix(mat);
	target->setParentMatrix(mat);
}

void Laser::setParentMatrix(XMFLOAT4 ori, XMFLOAT3 pos)
{
	XMStoreFloat4x4(parentMatrix, XMMatrixIdentity() * XMMatrixRotationQuaternion(XMLoadFloat4(&ori)) * XMMatrixTranslation(pos.x, pos.y, pos.z));
	beam->setParentMatrix(parentMatrix); 
	beam2->setParentMatrix(parentMatrix);
	target->setParentMatrix(parentMatrix);
}

void Laser::orbitX(int factor) 
{ 
	float angle = factor * LASER_TURN_SPEED;
	if (abs(vertAngle + angle) < 0.6f)
	{
		vertAngle += angle;
		beam->orbitX(angle, XMFLOAT3(0.0f, 0.0f, -50.0f)); 
		beam2->orbitX(angle, XMFLOAT3(0.0f, 0.0f, -50.0f));
		target->orbitX(angle, XMFLOAT3(0.0f, 0.0f, -5.0f));
	}
}

void Laser::orbitY(int factor) 
{ 
	float angle = factor * LASER_TURN_SPEED;
	if (abs(horzAngle + angle) < 0.8f)
	{
		horzAngle += angle;
		beam->orbitY(angle, XMFLOAT3(0.0f, 0.0f, -50.0f));
		beam2->orbitY(angle, XMFLOAT3(0.0f, 0.0f, -50.0f)); 
		target->orbitY(angle, XMFLOAT3(0.0f, 0.0f, -5.0f)); 
	}
}

bool Laser::isFiring() { return firing; }

void Laser::toggle() { firing = !firing; }

vector<GameObject*> Laser::getModels() { return objs; }
