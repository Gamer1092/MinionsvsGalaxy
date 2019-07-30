#pragma once
#include "GameObject.h"
#include "Shapes/CubeObject.h"

class Bullet
{
private:
	CubeObject* body;
	XMFLOAT3 direction;
	float speed;
	int frames;

public:
	Bullet();
	~Bullet(void);

	GameObject* getModel();
	void advance();
	int getFrames();
	void decFrames();
	XMFLOAT3 getPos();
	void setDir(XMFLOAT3);
};

