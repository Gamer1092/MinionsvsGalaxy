#pragma once
#include "Shapes/SphereObject.h"
#include <vector>
using namespace std;

const float Bomb_TURN_SPEED = 0.025f;
const float BOMB_REP = 210.0f;
const float BOMB_SPEED = 0.419f;

class Bomb
{
private:
	SphereObject* bomb;
	XMFLOAT3 bombPos;
	XMFLOAT3 direction;
	bool exploded;
	bool pressed;
	bool firing;
	int counter;

public:
	Bomb(void);
	~Bomb(void);

	GameObject* getModel();
	void spawnBomb(XMFLOAT3, XMFLOAT3);
	void setFiring(bool);
	bool isFiring();
	XMFLOAT3 getPos();
	void advance();
	bool hasExploded();
	void setExploded(bool);
	void setPressed(bool);
	void resetBomb();
};

