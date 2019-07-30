#include "WarriorEnemyShip.h"

WarriorEnemyShip::WarriorEnemyShip(void)
{
	body = new Torus(XMFLOAT4(0.1, 0.4, 0.1, 1.0));

	body2 = new Torus(XMFLOAT4(0.4, 0.9, 0.4, 1.0));
	body2->setParentMatrix(body->getWorldMatrix());
	body2->scale(0.8f, 0.8f, 0.8f);
	
	XMFLOAT4 cannonColor[6];
	XMFLOAT4 cannonRed = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	for(int i = 0; i < 6; i++)
		cannonColor[i] = cannonRed;
	cannon1 = new CubeObject(0.2f, 0.2f, 1.0f, true, cannonColor);
	cannon1->translate(0.85f, 0.0f, -0.4f);
	cannon1->setParentMatrix(body->getWorldMatrix());

	cannon2 = new CubeObject(0.2f, 0.2f, 1.0f, true, cannonColor);
	cannon2->translate(-0.85f, 0.0f, -0.4f);
	cannon2->setParentMatrix(body->getWorldMatrix());

	objs.push_back(body);
	objs.push_back(body2);
	objs.push_back(cannon1);
	objs.push_back(cannon2);

	enemyHealth = 50;
	exp = 50;
	shootingFrames = 0;
}

WarriorEnemyShip::~WarriorEnemyShip(void)
{
	if(body)
		delete body;
	if(cannon1)
		delete cannon1;
	if(cannon2)
		delete cannon2;
	body = 0;
	cannon1 = 0;
	cannon2 = 0;

}

void WarriorEnemyShip::translate(float dx, float dy, float dz)
{
	body->translate(dx, dy, dz);
}

void WarriorEnemyShip::rotateX(float angle)
{
	body->rotateX(angle);
}

void WarriorEnemyShip::rotateY(float angle)
{
	body->rotateY(angle);
}

void WarriorEnemyShip::rotateZ(float angle)
{
	body->rotateZ(angle);
}

XMFLOAT3 WarriorEnemyShip::getPosition()
{
	XMFLOAT4 pos;
	XMFLOAT4 orig = XMFLOAT4(0,0,0,1);
	XMFLOAT4X4 mat = *(body->getWorldMatrix());
	XMStoreFloat4(&pos, XMVector4Transform(XMLoadFloat4(&orig), XMLoadFloat4x4(&mat)));
	return (XMFLOAT3(pos.x, pos.y, pos.z));
}

void WarriorEnemyShip::advance()
{
	body2->rotateX(0.07f);
}

bool WarriorEnemyShip::update(XMFLOAT3 pos, ID3D11Device* device)
{
	bool ret = false;
	advance();

	for (unsigned int i = 0; i < bullets.size(); i++)
	{
		Bullet* b = bullets.at(i);
		b->advance();
		if (b->getFrames() == 0)
		{
			bullets.erase(remove(bullets.begin(), bullets.end(), b), bullets.end());
			delete b;
		}
	}

	XMFLOAT3 bpos = body->getPos();
	if ((bpos.x-pos.x)*(bpos.x-pos.x) + (bpos.y-pos.y)*(bpos.y-pos.y) + (bpos.z-pos.z)*(bpos.z-pos.z) < 2500)
	{
		if (shootingFrames == 0)
		{
			fireLaser(pos, device);
			shootingFrames = 130;
			ret = true;
		}
		else
			shootingFrames--;
	}
	return ret;
}

void WarriorEnemyShip::fireLaser(XMFLOAT3 pos, ID3D11Device* device)
{
	Bullet* b = new Bullet();
	GameObject* obj = b->getModel();
	obj->getModel()->initialize(device);
	XMFLOAT3 shipPos = body->getPos();
	obj->translate(shipPos.x, shipPos.y, shipPos.z);
	XMFLOAT3 dir = XMFLOAT3(pos.x-shipPos.x, pos.y-shipPos.y, pos.z-shipPos.z);
	XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&dir)));
	b->setDir(dir);
	bullets.push_back(b);
}

