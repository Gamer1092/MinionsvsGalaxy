#pragma once
#include "../GameObject.h"

class SphereObject : public GameObject
{
public:
	SphereObject(void);
	SphereObject(WCHAR*);
	SphereObject(float, int, int, WCHAR*);
	virtual ~SphereObject(void);
	virtual void shutdown();
	virtual void advance();

private:
	virtual void initializeModel(float radius, int numDiv, int numBands, WCHAR* filename);
};

