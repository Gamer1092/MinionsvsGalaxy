#pragma once
#include "../GameObject.h"

class CubeObject : public GameObject
{
public:
	CubeObject(void);
	CubeObject(bool);
	CubeObject(float xLength, float yLength, float zLength, bool sixSided, XMFLOAT4* pColors = 0);
	virtual ~CubeObject(void);
	virtual void shutdown();
	virtual void advance();

private:
	virtual void initializeModel(float xLength, float yLength, float zLength, bool sixSided, XMFLOAT4* pColors);
	void initSixSided(float xLength, float yLength, float zLength, XMFLOAT4* pColors);
	void initReg(float xLength, float yLength, float zLength, XMFLOAT4* pColors);
};

