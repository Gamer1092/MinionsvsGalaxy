#include "Health.h"

Health::Health(void)
{
	XMFLOAT4 healthColor[6];
	XMFLOAT4 healthRed = XMFLOAT4(0.9f, 0.2f, 0.2f, 1.0f);
	for(int i = 0; i < 6; i++)
		healthColor[i] = healthRed;

	body = new CubeObject(1.0f, 1.0f, 1.0f, true, healthColor);
	collected = false;
}


Health::~Health(void)
{
	if (body)
		delete body;
	body = 0;
	
}

GameObject* Health::getModel() { return body; }

XMFLOAT3 Health::getPos() { return XMFLOAT3(pos.x,pos.y,pos.z); }
XMFLOAT4 Health::getOri() { return ori; }
XMFLOAT4 Health::getL()   { return L; }
XMFLOAT4 Health::getVel() { return vel; }
float Health::getRadius() { return 1.0f; }

void Health::setPos(XMFLOAT4 newPos)
{ 
	pos = newPos; 
	body->translate(pos.x, pos.y, pos.z);
}
void Health::setOri(XMFLOAT4 newOri) 
{ 
	ori = newOri;
	body->rotateX(ori.x);
	body->rotateY(ori.y);
	body->rotateZ(ori.z);
}
void Health::setL(XMFLOAT4 newL) { L = newL; }
void Health::setVel(XMFLOAT4 newVel) { vel = newVel; }

void Health::advance()
{
	body->rotateX(L.x);
	body->rotateY(L.y);
	body->rotateZ(L.z);
}

bool Health::isCollected() { return collected; }

void Health::collect() { collected = true; }

void Health::translate(float dx, float dy, float dz)
{
	body->translate(dx, dy, dz);
}
