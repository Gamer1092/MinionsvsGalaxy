#include "GameObject.h"


GameObject::GameObject(void)
{
	m_model = 0;
	m_colorVerts = 0;
	m_colorNormVerts = 0;
	m_textureVerts = 0;
	m_flatTextureVerts = 0;
	m_numVert = 0;
	m_numInds = 0;
	m_indices = 0;
	m_parentMatrix = new XMFLOAT4X4();
	XMStoreFloat4x4(&m_worldMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_scaleMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_rotationMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_orbitMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_translationMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(m_parentMatrix, XMMatrixIdentity());
}

GameObject::~GameObject(void)
{
	m_model = 0;
	m_colorVerts = 0;
	m_colorNormVerts = 0;
	m_textureVerts = 0;
	m_flatTextureVerts = 0;
	m_indices = 0;
	m_parentMatrix = 0;
}

void GameObject::shutdown()
{
	if (m_model) m_model->shutdown();
	if (m_colorVerts) delete[] m_colorVerts;
	if (m_colorNormVerts) delete[] m_colorNormVerts;
	if (m_textureVerts) delete[] m_textureVerts;
	if (m_flatTextureVerts) delete[] m_flatTextureVerts;
	if (m_indices) delete[] m_indices;
}

Model* GameObject::getModel() { return m_model; }

bool GameObject::isColorObject() { return m_colorVerts ? true : false; }

bool GameObject::isColorNormObject() { return m_colorNormVerts ? true : false; }

bool GameObject::isTextureObject() { return m_textureVerts ? true : false; }

bool GameObject::isFlatTextureObject() { return m_flatTextureVerts ? true : false; }
	
XMFLOAT4X4* GameObject::getWorldMatrix()
{
	XMStoreFloat4x4(&m_worldMatrix, 
		XMLoadFloat4x4(&m_scaleMatrix) * 
		XMLoadFloat4x4(&m_rotationMatrix) *
		XMLoadFloat4x4(&m_orbitMatrix) *
		XMLoadFloat4x4(&m_translationMatrix) *
		XMLoadFloat4x4(m_parentMatrix));
	return &m_worldMatrix;
}

void GameObject::setParentMatrix(XMFLOAT4X4* matrix)
{
	m_parentMatrix = matrix; 
}

void GameObject::scale(float xFactor, float yFactor, float zFactor)
{
	XMStoreFloat4x4(&m_scaleMatrix, XMLoadFloat4x4(&m_scaleMatrix) * XMMatrixScaling(xFactor, yFactor, zFactor));
}

void GameObject::rotateX(float theta)
{
	XMStoreFloat4x4(&m_rotationMatrix, XMLoadFloat4x4(&m_rotationMatrix) * XMMatrixRotationX(theta));
}

void GameObject::rotateY(float theta)
{
	XMStoreFloat4x4(&m_rotationMatrix, XMLoadFloat4x4(&m_rotationMatrix) * XMMatrixRotationY(theta));
}

void GameObject::rotateZ(float theta)
{
	XMStoreFloat4x4(&m_rotationMatrix, XMLoadFloat4x4(&m_rotationMatrix) * XMMatrixRotationZ(theta));
}

void GameObject::orbitX(float theta, XMFLOAT3 pos)
{
	XMStoreFloat4x4(&m_orbitMatrix, XMLoadFloat4x4(&m_orbitMatrix) * 
		XMMatrixTranslation(-pos.x, -pos.y, -pos.z) * 
		XMMatrixRotationX(theta) * 
		XMMatrixTranslation(pos.x, pos.y, pos.z));
}

void GameObject::orbitY(float theta, XMFLOAT3 pos)
{
	XMStoreFloat4x4(&m_orbitMatrix, XMLoadFloat4x4(&m_orbitMatrix) * 
		XMMatrixTranslation(-pos.x, -pos.y, -pos.z) * 
		XMMatrixRotationY(theta) * 
		XMMatrixTranslation(pos.x, pos.y, pos.z));
}

void GameObject::orbitZ(float theta, XMFLOAT3 pos)
{
	XMStoreFloat4x4(&m_orbitMatrix, XMLoadFloat4x4(&m_orbitMatrix) * 
		XMMatrixTranslation(-pos.x, -pos.y, -pos.z) * 
		XMMatrixRotationZ(theta) * 
		XMMatrixTranslation(pos.x, pos.y, pos.z));
}

void GameObject::translate(float dx, float dy, float dz)
{
	XMStoreFloat4x4(&m_translationMatrix, XMLoadFloat4x4(&m_translationMatrix) * XMMatrixTranslation(dx, dy, dz));
}

XMFLOAT3 GameObject::getPos()
{
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, XMVector3Transform(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), XMLoadFloat4x4(&m_translationMatrix) * XMLoadFloat4x4(m_parentMatrix)));
	return pos;
}