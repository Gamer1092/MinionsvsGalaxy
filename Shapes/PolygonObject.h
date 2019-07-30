#pragma once
#include "../GameObject.h"

class PolygonObject : public GameObject
{
public:
	PolygonObject(void);
	~PolygonObject(void);
	virtual void shutdown();
	virtual void advance();

private:
	virtual void initializeModel(float radius, int numDiv, XMFLOAT4* pColors);
};

