#include "Explosion.h"

Explosion::Explosion(WCHAR* filename)
{
	numParticles = 7000;
	numOrigins = 6;
	numColors = 4;
	origins = new XMFLOAT3[numOrigins];
	phases = new float[numOrigins];
	colors = new float[numOrigins];

	initializeModel(filename);
}


Explosion::~Explosion(void)
{
	m_model = 0;
}

void Explosion::initializeModel(WCHAR* filename)
{
	int m_numVert = numParticles;
	int m_numInds = 0;
	ExplosionTextureVertex thisone;
	XMFLOAT3 thisnor;
	m_explosionTextureVerts = new ExplosionTextureVertex[m_numVert];
	m_indices = 0;

	float trad = 0.9f;
	float crad = 0.3f;
	float spray;
	float maxspray = 1.5f;
	XMFLOAT3 wander;
	// random number generator:
	random_device rd;
	mt19937 lotto(rd());

	uniform_real_distribution<> distro(0, 1);
	float u, v, w;

	for (int i = 0; i < numParticles; i++)
	{	
		u = static_cast<float>(distro(lotto));
		v = static_cast<float>(distro(lotto));
		w = static_cast<float>(distro(lotto));

		float theta = u * 2 * 3.1416f;
		float phi = acos(2*v - 1.0f);
		spray = maxspray*pow(w, 1.0f/3.0f);

		// normal direction
		thisnor = XMFLOAT3(spray*cos(theta)*sin(phi), spray*sin(theta)*sin(phi), spray*cos(phi));
			
		// position + color of this vertex, now added normal and texcoord

		thisone.position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		thisone.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		thisone.normal = XMFLOAT4(thisnor.x, thisnor.y, thisnor.z, 0.0f);
		thisone.texcoord = XMFLOAT2(i/(float)numParticles,0.0f); //id

		m_explosionTextureVerts[i] = thisone; // add to vertex buffer
	}

	for (int i = 0; i < numOrigins; i++)
	{
		float v = static_cast<float>(distro(lotto));
		phases[i] = 5 * v;
	}

	//create Model class for rendring pipeline
	m_model = new Model(m_explosionTextureVerts, filename, m_numVert, m_indices, m_numInds, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void Explosion::shutdown()
{
	GameObject::shutdown();
}

void Explosion::advance()
{
}

void Explosion::randomizePosition(int index)
{
	random_device rd;
	mt19937 lotto(rd());
	uniform_real_distribution<> distro(0, 1);

	float x = static_cast<float>(distro(lotto));
	float y = static_cast<float>(distro(lotto));
	float z = static_cast<float>(distro(lotto));
	float w = static_cast<float>(distro(lotto));

	x = 5*(2*x - 1.0f);
	y = 2*(2*y - 1.0f) + 1.0f;
	z = 2*z - 1.0f;

	origins[index] = XMFLOAT3(x,y,z);
	colors[index] = floor(numColors * w);
}

void Explosion::update(float timer)
{
	for (int i = 0; i < currNumInst; i++)
	{
		float time = timer + phases[i];
		if (time > 5.0f)
			time -= 5.0f;
		if (time < 0.018f)
		{
			for (int i = 0; i < currNumInst-1; i++)
			{
				origins[i] = origins[i+1];
				phases[i] = phases[i+1];
				colors[i] = colors[i+1];
			}
			currNumInst--;
		}
	}
}
