#pragma once
#include "GameObject.h"

class ParticlesObject : public GameObject
{
protected:
	int			numOrigins;
	int			numColors;
	XMFLOAT3*	origins;
	float*		phases;
	float*		colors;

	int		    currNumInst;

	ExplosionTextureVertex*	m_explosionTextureVerts;

public:
	ParticlesObject(void);
	~ParticlesObject(void);

	virtual XMFLOAT3 getOrigin(int);
	virtual int getNumInst();
	virtual float getPhase(int);
	virtual float getColor(int);

	virtual void addInstance(XMFLOAT3 orig, float phaz, float col);
};

