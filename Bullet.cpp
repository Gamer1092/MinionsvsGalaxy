#include "Bullet.h"


Bullet::Bullet(void)
{
	XMFLOAT4 cannonColor[6];
	XMFLOAT4 cannonGreen = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	for(int i = 0; i < 6; i++)
		cannonColor[i] = cannonGreen;
	body = new CubeObject(0.2f, 0.2f, 1.0f, true, cannonColor);
	speed = 0.6f;
	frames = 600;
}


Bullet::~Bullet(void)
{
}

GameObject* Bullet::getModel() { return body; }

int Bullet::getFrames() { return frames; }

void Bullet::decFrames()
{
	frames--;
	if (frames < 0)
		frames = 0;
}

XMFLOAT3 Bullet::getPos() { return body->getPos(); }

void Bullet::advance()
{
	body->translate(speed*direction.x, speed*direction.y, speed*direction.z);
}

void Bullet::setDir(XMFLOAT3 dir) { direction = dir; }
