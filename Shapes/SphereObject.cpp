#include "SphereObject.h"


SphereObject::SphereObject(void)
{
	initializeModel(0.5f, 64, 64, L"");
}

SphereObject::SphereObject(WCHAR* filename)
{

	initializeModel(0.5f, 24, 24, filename);
}

SphereObject::SphereObject(float radius, int numDiv, int numBands, WCHAR* filename)
{
	initializeModel(radius, numDiv, numBands, filename);
}

SphereObject::~SphereObject(void)
{
	m_model = 0;
}

void SphereObject::shutdown()
{
	GameObject::shutdown();
}

void SphereObject::initializeModel(float radius, int numDiv, int numBands, WCHAR* filename)
{
	//keep number of faces in a reasonable range
	if(numDiv < 3) numDiv = 3;
	if(numBands < 3) numBands = 3;
	numDiv++;
	m_numVert = (numDiv) * numBands;
	m_numInds = 6 * (numDiv) * numBands;

	m_textureVerts = new TextureVertex[m_numVert];
	m_indices = new WORD [m_numInds];

	//define the triangle pairs that make up each face
	int count = 0;
	for (int i = 0; i < numBands; i++)
	{
		float yPos = radius - (i * 2*radius/ (numBands-1));
		float rad = sqrt(radius*radius - yPos*yPos);
		for (int j = 0; j < numDiv; j++)
		{
			float angle = j * XM_2PI / (numDiv-1); 
			m_textureVerts[count].position = XMFLOAT4(rad*cos(angle), yPos, rad*sin(angle), 1);
			m_textureVerts[count].normal = XMFLOAT4(rad*cos(angle)/0.5f, yPos/0.5f, rad*sin(angle)/0.5f, 0);
			m_textureVerts[count++].texcoord = XMFLOAT2((float)j/numDiv, (float)i/(numBands-1));
		}
	}

	count = 0;
	//define indices for the entire sphere
	for (int i = 0; i < numBands; i++)
	{
		for (int j = 0; j < numDiv; j++)
		{
			m_indices[count++] = WORD(i*numDiv + j);
			m_indices[count++] = WORD(i*numDiv + (j+1)%numDiv);
			m_indices[count++] = WORD((i+1)*numDiv + (j+1)%numDiv);

			m_indices[count++] = WORD(i*numDiv + j);
			m_indices[count++] = WORD((i+1)*numDiv + (j+1)%numDiv);
			m_indices[count++] = WORD((i+1)*numDiv + j);
		}
	}

	//create Model class for rendring pipeline
	m_model = new Model(m_textureVerts, filename, m_numVert, m_indices, m_numInds, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void SphereObject::advance()
{
	rotateX(0.01f);
	rotateY(0.005f);
	rotateZ(0.007f);
}
