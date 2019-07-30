#include "Bomb.h"

Bomb::Bomb(void)
{
	bomb = new SphereObject(L"Textures/bomb.dds");
	bomb->scale(3.0f, 3.0f, 3.0f);
	bombPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	direction = XMFLOAT3(0.0, 0.0, 0.0);
	
	firing = false;
	pressed = false;
	exploded = false;
	counter = 0;
}

Bomb::~Bomb(void)
{
	if(bomb)
		delete bomb;
	bomb = 0;
}

GameObject* Bomb::getModel() { return bomb; }

void Bomb::spawnBomb(XMFLOAT3 pos, XMFLOAT3 dir)
{
	bombPos = XMFLOAT3(pos.x + 4*dir.x, pos.y + 4*dir.y, pos.z + 4*dir.z);
	bomb->translate(bombPos.x, bombPos.y, bombPos.z);
	direction = dir;
}

void Bomb::advance()
{
	counter++;

	bombPos.x += direction.x * BOMB_SPEED;
	bombPos.y += direction.y * BOMB_SPEED;
	bombPos.z += direction.z * BOMB_SPEED;
	bomb->translate(direction.x * BOMB_SPEED, direction.y * BOMB_SPEED, direction.z * BOMB_SPEED);
	if (BOMB_REP < counter)
		exploded = true;
}

void Bomb::resetBomb()
{
	firing = false;
	exploded = false;
	counter = 0;

	bomb->translate(-bombPos.x, -bombPos.y, -bombPos.z);
	bombPos.x = 0.0f;
	bombPos.y = 0.0f;
	bombPos.z = 0.0f;
}

bool Bomb::isFiring() { return firing; }

XMFLOAT3 Bomb::getPos() { return bombPos; }

void Bomb::setExploded(bool state){ exploded = state; }

void Bomb::setPressed(bool state){ pressed = state; }

bool Bomb::hasExploded(){ return exploded; }

void Bomb::setFiring(bool firingState) { firing = firingState; }
