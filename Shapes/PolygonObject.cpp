#include "PolygonObject.h"


PolygonObject::PolygonObject(void)
{
	initializeModel(3.0f, 24, 0);
}


PolygonObject::~PolygonObject(void)
{
	m_model = 0;
}

void PolygonObject::shutdown()
{
	GameObject::shutdown();
}

void PolygonObject::initializeModel(float radius, int numDiv, XMFLOAT4* pColors)
{
	XMFLOAT3 centerVertex = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 v0 = XMFLOAT3(radius, 0.0f, 0.0f);
	float angle = -2 * XM_PI / numDiv;

	m_numVert = numDiv + 1;
	m_numInds = 3 * numDiv;

	m_colorVerts = new ColorVertex[m_numVert];
	m_indices = new WORD[m_numInds];

	XMFLOAT4 color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	m_colorVerts[m_numVert-1].position = centerVertex; m_colorVerts[0].color = color;

	XMFLOAT4X4 rotationMatrix;
	for (int i = 0; i < m_numVert; i++)
	{
		XMStoreFloat4x4(&rotationMatrix, XMMatrixRotationZ(angle * i));
	    XMStoreFloat3(&m_colorVerts[i].position, XMVector3Transform(XMLoadFloat3(&v0), XMLoadFloat4x4(&rotationMatrix)));
		m_colorVerts[i].color = color;
	}

	for (int i = 0, j = 0; i < numDiv; i++, j++)
	{
		m_indices[3*i] = m_numVert-1;
		m_indices[3*i+1] = j;
		m_indices[3*i+2] = j+1;
	}
	m_indices[m_numInds-1] = 0;

	m_model = new Model(m_colorVerts, m_numVert, m_indices, m_numInds, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void PolygonObject::advance()
{

}
