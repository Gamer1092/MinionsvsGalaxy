#include "Asteroid.h"


Asteroid::Asteroid(void)
{
	body = new SphereObject(L"Textures/moonTexture.dds");
	destroyed = false;
}


Asteroid::~Asteroid(void)
{
	if (body)
		delete body;
	body = 0;	
}

GameObject* Asteroid::getModel() { return body; }

XMFLOAT3 Asteroid::getPos() { return XMFLOAT3(pos.x,pos.y,pos.z); }
XMFLOAT4 Asteroid::getOri() { return ori; }
XMFLOAT4 Asteroid::getL()   { return L; }
XMFLOAT4 Asteroid::getVel() { return vel; }
float Asteroid::getRadius() { return 1.0f; }

void Asteroid::setPos(XMFLOAT4 newPos)
{ 
	pos = newPos; 
	body->translate(pos.x, pos.y, pos.z);
}
void Asteroid::setOri(XMFLOAT4 newOri) 
{ 
	ori = newOri;
	body->rotateX(ori.x);
	body->rotateY(ori.y);
	body->rotateZ(ori.z);
}
void Asteroid::setL(XMFLOAT4 newL) { L = newL; }
void Asteroid::setVel(XMFLOAT4 newVel) { vel = newVel; }

void Asteroid::advance()
{
	body->rotateX(L.x);
	body->rotateY(L.y);
	body->rotateZ(L.z);
}

bool Asteroid::isDestroyed() { return destroyed; }

void Asteroid::destroy() { destroyed = true; }

