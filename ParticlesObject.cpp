#include "ParticlesObject.h"


ParticlesObject::ParticlesObject(void)
{
	currNumInst = 0;
}

ParticlesObject::~ParticlesObject(void)
{
}

XMFLOAT3 ParticlesObject::getOrigin(int n) { return origins[n]; }

int ParticlesObject::getNumInst() { return currNumInst; }

float ParticlesObject::getPhase(int n) { return phases[n]; }

float ParticlesObject::getColor(int n) { return colors[n]; }

void ParticlesObject::addInstance(XMFLOAT3 orig, float phaz, float col)
{
	if (currNumInst < numOrigins)
	{
		origins[currNumInst] = orig;
		phases[currNumInst] = phaz;
		colors[currNumInst] = col;
		currNumInst++;
	}
}
