#pragma once
#include "Shapes/CubeObject.h"
#include <vector>
using namespace std;

const float LASER_TURN_SPEED = 0.025f;

class Laser
{
private:
	CubeObject* beam;
	CubeObject* beam2;
	CubeObject* target;
	bool firing;
	float vertAngle;
	float horzAngle;
	vector<GameObject*> objs;
	XMFLOAT4X4* parentMatrix;

public:
	Laser(void);
	~Laser(void);

	GameObject* getModel();
	GameObject* getModel2();
	GameObject* getTargetModel();
	void setParentMatrix(XMFLOAT4X4*);
	void setParentMatrix(XMFLOAT4, XMFLOAT3);
	void orbitX(int);
	void orbitY(int);
	void toggle();
	bool isFiring();
	vector<GameObject*> getModels();
};

