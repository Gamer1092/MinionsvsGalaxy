#pragma once
#include "../GameObject.h"

class PrismObject : public GameObject
{
public:
	PrismObject(void);
	PrismObject(XMFLOAT4);
	PrismObject(float, float, int, XMFLOAT4 pColors);
	virtual ~PrismObject(void);
	virtual void shutdown();
	virtual void advance();

private:
	virtual void initializeModel(float length, float radius, int numDiv, int numBands, XMFLOAT4 pColors);
};

