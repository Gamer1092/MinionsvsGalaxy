#include "Armor.h"

Armor::Armor(void)
{
	XMFLOAT4 armorColor[6];
	XMFLOAT4 armorSilver = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	for(int i = 0; i < 6; i++)
		armorColor[i] = armorSilver;

	body = new CubeObject(1.0f, 1.0f, 1.0f, true, armorColor);
	collected = false;
}


Armor::~Armor(void)
{
	if (body)
		delete body;
	body = 0;
	
}

GameObject* Armor::getModel() { return body; }

XMFLOAT3 Armor::getPos() { return XMFLOAT3(pos.x,pos.y,pos.z); }
XMFLOAT4 Armor::getOri() { return ori; }
XMFLOAT4 Armor::getL()   { return L; }
XMFLOAT4 Armor::getVel() { return vel; }
float Armor::getRadius() { return 1.0f; }

void Armor::setPos(XMFLOAT4 newPos)
{ 
	pos = newPos; 
	body->translate(pos.x, pos.y, pos.z);
}
void Armor::setOri(XMFLOAT4 newOri) 
{ 
	ori = newOri;
	body->rotateX(ori.x);
	body->rotateY(ori.y);
	body->rotateZ(ori.z);
}
void Armor::setL(XMFLOAT4 newL) { L = newL; }
void Armor::setVel(XMFLOAT4 newVel) { vel = newVel; }

void Armor::advance()
{
	body->rotateY(0.015f);
}

bool Armor::isCollected() { return collected; }

void Armor::collect() { collected = true; }

void Armor::translate(float dx, float dy, float dz)
{
	body->translate(dx, dy, dz);
}
