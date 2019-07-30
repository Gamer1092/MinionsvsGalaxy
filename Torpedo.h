#pragma once
#include "Shapes/CubeObject.h"
#include "Shapes/PrismObject.h"
#include <vector>
using namespace std;

const float TORPEDO_TURN_SPEED = 0.025f;

class Torpedo
{
private:
	CubeObject* body;
	PrismObject* wing;
	PrismObject* wing2;
	CubeObject* target;
	bool firing;
	float vertAngle;
	float horzAngle;
	vector<GameObject*> objs;
	XMFLOAT4X4* parentMatrix;

public:
	Torpedo(void);
	~Torpedo(void);

	GameObject* getModel();
	GameObject* getModel2();
	GameObject* getModel3();
	GameObject* getTargetModel();
	void setParentMatrix(XMFLOAT4X4*);
	void setParentMatrix(XMFLOAT4, XMFLOAT3);
	void orbitX(int);
	void orbitY(int);
	void toggle();
	bool isFiring();
	vector<GameObject*> getModels();
};
