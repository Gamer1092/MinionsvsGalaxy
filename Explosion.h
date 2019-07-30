#pragma once
#include "ParticlesObject.h"
#include <random>
using namespace std;

class Explosion : public ParticlesObject
{
public:
	Explosion(WCHAR*);
	~Explosion(void);
	virtual void shutdown();
	virtual void advance();
	
	void update(float);

private:
	int numParticles;
	virtual void initializeModel(WCHAR*);
	void randomizePosition(int);
};

