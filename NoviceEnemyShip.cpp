#include "NoviceEnemyShip.h"

NoviceEnemyShip::NoviceEnemyShip(void)
{
	XMFLOAT4 shipColor[6];
	XMFLOAT4 shipWhite = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	for(int i = 0; i < 6; i++)
		shipColor[i] = shipWhite;
	body = new CubeObject(1.0f, 0.76f, 1.5f, true, shipColor);

	XMFLOAT4 cannonColor[6];
	XMFLOAT4 cannonRed = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	for(int i = 0; i < 6; i++)
		cannonColor[i] = cannonRed;
	cannon = new CubeObject(0.2f, 0.2f, 1.0f, true, cannonColor);
	cannon->translate(0.0f, -0.48f, -0.75f);
	cannon->setParentMatrix(body->getWorldMatrix());

	XMFLOAT4 wingBlue = XMFLOAT4(0.2f, 0.2f, 0.7f, 1.0f);
	wing1 = new PrismObject(0.1f, 0.75, 3, wingBlue);
	wing1->rotateY(XM_PI);
	wing1->translate(-0.9f, -0.1f, 0.0f);
	wing1->setParentMatrix(body->getWorldMatrix());

	wing2 = new PrismObject(0.1f, 0.75f, 3, wingBlue);
	wing2->translate(0.9f, -0.1f, 0.0f);
	wing2->setParentMatrix(body->getWorldMatrix());

	objs.push_back(body);
	objs.push_back(cannon);
	objs.push_back(wing1);
	objs.push_back(wing2);

	enemyHealth = 25;
	exp = 15;
	shootingFrames = 0;
}

NoviceEnemyShip::~NoviceEnemyShip(void)
{
	if(body)
		delete body;
	if(cannon)
		delete cannon;
	if(wing1)
		delete wing1;
	if(wing2)
		delete wing2;
	body = 0;
	cannon = 0;
	wing1 = 0;
	wing2 = 0;
}

void NoviceEnemyShip::translate(float dx, float dy, float dz)
{
	body->translate(dx, dy, dz);
}

void NoviceEnemyShip::rotateX(float angle)
{
	body->rotateX(angle);
}

void NoviceEnemyShip::rotateY(float angle)
{
	body->rotateY(angle);
}

void NoviceEnemyShip::rotateZ(float angle)
{
	body->rotateZ(angle);
}

XMFLOAT3 NoviceEnemyShip::getPosition()
{
	XMFLOAT4 pos;
	XMFLOAT4 orig = XMFLOAT4(0,0,0,1);
	XMFLOAT4X4 mat = *(body->getWorldMatrix());
	XMStoreFloat4(&pos, XMVector4Transform(XMLoadFloat4(&orig), XMLoadFloat4x4(&mat)));
	return (XMFLOAT3(pos.x, pos.y, pos.z));
}

bool NoviceEnemyShip::update(XMFLOAT3 pos, ID3D11Device* device)
{
	bool ret = false;

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
			shootingFrames = 200;
			ret = true;
		}
		else
			shootingFrames--;
	}
	return ret;
}

void NoviceEnemyShip::fireLaser(XMFLOAT3 pos, ID3D11Device* device)
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