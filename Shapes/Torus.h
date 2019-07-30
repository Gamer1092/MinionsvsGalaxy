#pragma once
#pragma once
#include "../GameObject.h"
#include <DirectXMath.h>

class Torus: public GameObject
{
public:
	Torus(void);
	Torus(XMFLOAT4 color);
	~Torus(void);
	virtual void shutdown();
	virtual void advance();

private:
	XMFLOAT4 color;
	bool isColored;
	virtual void initializeModel();
};

