#include "PrismObject.h"


PrismObject::PrismObject(void)
{
	initializeModel(1.0f, 1.0f, 24, 0, XMFLOAT4(1,1,1,1));
}

PrismObject::PrismObject(XMFLOAT4 col)
{
	initializeModel(1.0f, 1.0f, 24, 0, col);
}

PrismObject::PrismObject(float length, float radius, int numDiv, XMFLOAT4 col)
{
	initializeModel(length, radius, numDiv, 2, col);
}

PrismObject::~PrismObject(void)
{
	m_model = 0;
}

void PrismObject::shutdown()
{
	GameObject::shutdown();
}

void PrismObject::initializeModel(float length, float radius, int numDiv, int numBands, XMFLOAT4 col)
{
	//keep number of faces in a reasonable range
	if(numDiv < 3) numDiv = 3;
//	if(numDiv > 24) numDiv = 24;
	
	//changing the sign of angle will affect whether the inside or outside of the prism
	//is visible
	float angle = -2 * XM_PI / numDiv; //slice angle of each face

 	XMFLOAT3* topVertices = new XMFLOAT3[numDiv + 1];
 	XMFLOAT3* bottomVertices = new XMFLOAT3[numDiv + 1];

    XMFLOAT3 v0top(radius, length/2, 0); 
    XMFLOAT3 v0bottom(radius, -length/2, 0);

    XMFLOAT3 topCenter(0, length/2, 0);
    XMFLOAT3 bottomCenter(0, -length/2, 0);

    topVertices[0] = v0top;
    topVertices[numDiv] = v0top;
    bottomVertices[0] = v0bottom;
    bottomVertices[numDiv] = v0bottom;

    //define the vertices around the top and bottom of prism

    XMFLOAT4X4 rotationMatrix;
    for (int i = 1; i < numDiv; i++)
    {
		XMStoreFloat4x4(&rotationMatrix, XMMatrixRotationY(angle * i));
	    XMStoreFloat3(&topVertices[i], XMVector3Transform(XMLoadFloat3(&v0top), XMLoadFloat4x4(&rotationMatrix)));
	    XMStoreFloat3(&bottomVertices[i], XMVector3Transform(XMLoadFloat3(&v0bottom), XMLoadFloat4x4(&rotationMatrix)));
		int g = 0;
    }

	m_numVert = numDiv * 12;
	m_numInds = m_numVert;

	m_colorVerts = new ColorVertex[m_numVert];
	m_indices = new WORD [m_numInds];

	//define the triangle pairs that make up each face
    for (int i = 0, j = 0; i < numDiv; i++, j++)
    {
        //face vertices -in clockwise render order
		m_colorVerts[12*i+0].position = topVertices[i];		  m_colorVerts[12*i+0].color = col;	
	    m_colorVerts[12*i+1].position = topVertices[i+1];	  m_colorVerts[12*i+1].color = col;
	    m_colorVerts[12*i+2].position = bottomVertices[i];    m_colorVerts[12*i+2].color = col;

		m_colorVerts[12*i+3].position = bottomVertices[i];    m_colorVerts[12*i+3].color = col;	
	    m_colorVerts[12*i+4].position = topVertices[i+1];	  m_colorVerts[12*i+4].color = col;
	    m_colorVerts[12*i+5].position = bottomVertices[i+1];  m_colorVerts[12*i+5].color = col;

		//top slice triangle
		m_colorVerts[12*i+6].position = topVertices[i];		  m_colorVerts[12*i+6].color = col;	
	    m_colorVerts[12*i+7].position = topCenter;			  m_colorVerts[12*i+7].color = col;
	    m_colorVerts[12*i+8].position = topVertices[i+1];	  m_colorVerts[12*i+8].color = col;

		//bottom slice triangle
		m_colorVerts[12*i+9].position = bottomCenter;		  m_colorVerts[12*i+9].color = col;	
	    m_colorVerts[12*i+10].position = bottomVertices[i];   m_colorVerts[12*i+10].color = col;
	    m_colorVerts[12*i+11].position = bottomVertices[i+1]; m_colorVerts[12*i+11].color = col;
     }

	//release memory for temporary arrays
    delete [] topVertices;
	delete [] bottomVertices;

	// Load the index array with data.
	// Two triangles per face. The directions are consistent
	// With back-face culling in a left-hand co-ordinate system.
	for(int i = 0; i < m_numInds; i++)
	     m_indices[i] = i;  // map vertices directly to indices

	//create Model class for rendring pipeline
	m_model = new Model(m_colorVerts, m_numVert, m_indices, m_numInds, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void PrismObject::advance()
{
}
