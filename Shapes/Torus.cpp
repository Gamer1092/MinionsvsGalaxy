#include "Torus.h"


Torus::Torus(void)
{
	isColored = false;
	initializeModel();
}

Torus::Torus(XMFLOAT4 col)
{
	isColored = true;
	color = col;
	initializeModel();
}

Torus::~Torus(void)
{
	m_model = 0;
}

void Torus::initializeModel()
{
	int circle = 30;
	float theta, phi;
	XMFLOAT3 ccen;
	float trad = 0.6f;
	float crad = 0.2f;
	int loop;
	int segment = 30;
	loop = 3 * segment;
	int m_numVert = circle*loop;
	int m_numInds = circle*loop * 6;
	ColorNormVertex thisone;
	XMFLOAT3 thisnor;
	m_colorNormVerts = new ColorNormVertex[m_numVert];
	m_indices = new WORD[m_numInds];

	// torus:
	for (int i = 0; i < loop; i++)
	{
		theta = 2*i*3.1416f/loop; // large loop
		//crad = 0.3 + 0.08*sin(7*theta); // vary small circle radius, 7 lobes
		ccen = XMFLOAT3(trad*cos(theta),trad*sin(theta),0); // centre of this small circle

		for (int j = 0; j < circle; j++) // small circle
		{
				
			phi = 2*j*3.1416f / circle; // from 0 to 2PI
				
			// normal direction
			thisnor = XMFLOAT3(cos(theta)*sin(phi), sin(theta)*sin(phi), cos(phi));
			// position + color of this vertex
			thisone.position = XMFLOAT3(ccen.x+thisnor.x*crad, ccen.y+thisnor.y*crad, ccen.z+thisnor.z*crad);
			thisone.normal = XMFLOAT3(thisnor.x, thisnor.y, thisnor.z);
			thisone.color = isColored ? color : XMFLOAT4(i / (segment + 0.01f), j / (circle + 0.01f), 0.05f, 1.0f);
			m_colorNormVerts[i*circle + j] = thisone; // add to vertex array
		}
	}

	int count = 0;
	for (int i = 0; i < loop; i++)
	{
		for (int j = 0; j < circle; j++)
		{
			// two triangles per quad
			
			m_indices[count++] = WORD((i*circle + j));	
			m_indices[count++] = WORD(i*circle + ((j + 1) % circle));
			m_indices[count++] = WORD(((i + 1) % loop)*circle + j);
		
			m_indices[count++] = WORD(i*circle + ((j + 1) % circle));
			m_indices[count++] = WORD(((i + 1) % loop)*circle + ((j + 1) % circle));
			m_indices[count++] = WORD(((i + 1) % loop)*circle + j);
		}
	}

	//create Model class for rendring pipeline
	m_model = new Model(m_colorNormVerts, m_numVert, m_indices, m_numInds, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Torus::shutdown()
{
	GameObject::shutdown();
}

void Torus::advance()
{
	rotateY(0.01f);
	rotateZ(-0.007f);
}
