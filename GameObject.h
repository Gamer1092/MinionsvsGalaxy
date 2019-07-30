#pragma once

#include "Model.h"

class GameObject
{
protected:
	Model*		 m_model;
	XMFLOAT4X4	 m_worldMatrix;
	XMFLOAT4X4*  m_parentMatrix;
	XMFLOAT4X4	 m_scaleMatrix;
	XMFLOAT4X4	 m_rotationMatrix;
	XMFLOAT4X4	 m_orbitMatrix;
	XMFLOAT4X4	 m_translationMatrix;
	ColorVertex* m_colorVerts;
	ColorNormVertex* m_colorNormVerts;
	TextureVertex*	m_textureVerts;
	FlatTextureVertex*	m_flatTextureVerts;
	int          m_numVert;
	int          m_numInds;
	WORD*        m_indices;


public:
	GameObject(void);
	virtual ~GameObject(void);
	virtual void shutdown();

	virtual Model* getModel();
	virtual XMFLOAT4X4* getWorldMatrix();
	virtual void setParentMatrix(XMFLOAT4X4* matrix);
	bool isColorObject();
	bool isColorNormObject();
	bool isTextureObject();
	bool isFlatTextureObject();

	//Transformations
	virtual void advance() = 0;

	virtual void scale(float xFactor, float yFactor, float zFactor);
	virtual void rotateX(float theta);
	virtual void rotateY(float theta);
	virtual void rotateZ(float theta);
	virtual void orbitX(float theta, XMFLOAT3 pos);
	virtual void orbitY(float theta, XMFLOAT3 pos);
	virtual void orbitZ(float theta, XMFLOAT3 pos);
	virtual void translate(float dx, float dy, float dz);

	XMFLOAT3 getPos();
};

