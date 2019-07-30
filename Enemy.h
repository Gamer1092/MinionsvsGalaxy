#pragma once
#include <vector>
#include "GameObject.h"
#include "Bullet.h"
#include <vector>
#include <algorithm>

using namespace std;

class Enemy
{
protected:
	int enemyHealth;
	int enemyArmor;
	bool attacking;
	bool dead;
	int exp;
	float radius;
	int shootingFrames;
	vector<GameObject*> objs;
	vector<Bullet*> bullets;

public:
	Enemy(void);
	virtual ~Enemy(void);

	bool isDead();
	bool isHostile();
	vector<GameObject*> getModels();
	vector<Bullet*> getBullets();

	virtual void translate(float, float, float)=0;
	virtual void rotateX(float)=0;
	virtual void rotateY(float)=0;
	virtual void rotateZ(float)=0;

	virtual XMFLOAT3 getPosition()=0;
	float getRadius();
	bool damage(int dmg);
	int getExp();
	virtual bool update(XMFLOAT3 pos, ID3D11Device* device)=0;
};

