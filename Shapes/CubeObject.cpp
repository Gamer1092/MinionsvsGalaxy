#include "CubeObject.h"


CubeObject::CubeObject()
{
	initializeModel(1.0f, 1.0f, 1.0f, 0, false);
}

CubeObject::CubeObject(bool sixSided)
{
	initializeModel(1.0f, 1.0f, 1.0f, sixSided, 0);
}

CubeObject::CubeObject(float xLength, float yLength, float zLength, bool sixSided, XMFLOAT4* pColors)
{
	initializeModel(xLength, yLength, zLength, sixSided, pColors);
}

CubeObject::~CubeObject(void)
{
	m_model = 0;
}

void CubeObject::shutdown()
{
	GameObject::shutdown();	
}

void CubeObject::initializeModel(float xLength, float yLength, float zLength, bool sixSided, XMFLOAT4* pColors)
{
	if (sixSided)
		initSixSided(xLength, yLength, zLength, pColors);
	else
		initReg(xLength, yLength, zLength, pColors);
}

void CubeObject::initSixSided(float xLength, float yLength, float zLength, XMFLOAT4* pColors)
{
	int m_numVert = 24;
	int m_numInds = 36;

	m_colorVerts = new ColorVertex[m_numVert];
	m_indices = new WORD[m_numInds];
	XMFLOAT4 color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);  //default color
	
	//Front face
	color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);  //default color
	if (pColors) color = pColors[0];
	m_colorVerts[0].position = XMFLOAT3(-xLength/2, -yLength/2, -zLength/2); m_colorVerts[0].color = color;
	m_colorVerts[1].position = XMFLOAT3(-xLength/2,  yLength/2, -zLength/2); m_colorVerts[1].color = color;
	m_colorVerts[2].position = XMFLOAT3( xLength/2, -yLength/2, -zLength/2); m_colorVerts[2].color = color;
	m_colorVerts[3].position = XMFLOAT3( xLength/2,  yLength/2, -zLength/2); m_colorVerts[3].color = color;

	//Back face
	color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);  //default color
	if (pColors) color = pColors[1];
	m_colorVerts[4].position = XMFLOAT3(-xLength/2, -yLength/2,  zLength/2); m_colorVerts[4].color = color;
	m_colorVerts[5].position = XMFLOAT3(-xLength/2,  yLength/2,  zLength/2); m_colorVerts[5].color = color;
	m_colorVerts[6].position = XMFLOAT3( xLength/2, -yLength/2,  zLength/2); m_colorVerts[6].color = color;
	m_colorVerts[7].position = XMFLOAT3( xLength/2,  yLength/2,  zLength/2); m_colorVerts[7].color = color;

	//Right face
	color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);  //default color
	if (pColors) color = pColors[2];
	m_colorVerts[8].position = XMFLOAT3( xLength/2,  yLength/2, -zLength/2); m_colorVerts[8].color = color;
	m_colorVerts[9].position = XMFLOAT3( xLength/2, -yLength/2, -zLength/2); m_colorVerts[9].color = color;
	m_colorVerts[10].position = XMFLOAT3( xLength/2,  yLength/2,  zLength/2); m_colorVerts[10].color = color;
	m_colorVerts[11].position = XMFLOAT3( xLength/2, -yLength/2,  zLength/2); m_colorVerts[11].color = color;

	//Left face
	color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);  //default color
	if (pColors) color = pColors[3];
	m_colorVerts[12].position = XMFLOAT3(-xLength/2,  yLength/2, -zLength/2); m_colorVerts[12].color = color;
	m_colorVerts[13].position = XMFLOAT3(-xLength/2, -yLength/2, -zLength/2); m_colorVerts[13].color = color;
	m_colorVerts[14].position = XMFLOAT3(-xLength/2,  yLength/2,  zLength/2); m_colorVerts[14].color = color;
	m_colorVerts[15].position = XMFLOAT3(-xLength/2, -yLength/2,  zLength/2); m_colorVerts[15].color = color;

	//Top face
	color = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);  //default color
	if (pColors) color = pColors[4];
	m_colorVerts[16].position = XMFLOAT3(-xLength/2,  yLength/2, -zLength/2); m_colorVerts[16].color = color;
	m_colorVerts[17].position = XMFLOAT3( xLength/2,  yLength/2, -zLength/2); m_colorVerts[17].color = color;
	m_colorVerts[18].position = XMFLOAT3(-xLength/2,  yLength/2,  zLength/2); m_colorVerts[18].color = color;
	m_colorVerts[19].position = XMFLOAT3( xLength/2,  yLength/2,  zLength/2); m_colorVerts[19].color = color;

	//Bottom face
	color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);  //default color
	if (pColors) color = pColors[5];
	m_colorVerts[20].position = XMFLOAT3(-xLength/2, -yLength/2, -zLength/2); m_colorVerts[20].color = color;
	m_colorVerts[21].position = XMFLOAT3( xLength/2, -yLength/2, -zLength/2); m_colorVerts[21].color = color;
	m_colorVerts[22].position = XMFLOAT3(-xLength/2, -yLength/2,  zLength/2); m_colorVerts[22].color = color;
	m_colorVerts[23].position = XMFLOAT3( xLength/2, -yLength/2,  zLength/2); m_colorVerts[23].color = color;

	//Indicies
	//Front
	m_indices[0] = 0; m_indices[1] = 1; m_indices[2] = 2;
	m_indices[3] = 1; m_indices[4] = 3; m_indices[5] = 2;

	//Right
	m_indices[6] = 9; m_indices[7] = 8; m_indices[8] = 11;
	m_indices[9] = 8; m_indices[10] = 10; m_indices[11] = 11;

	//Left
	m_indices[12] = 12; m_indices[13] = 13; m_indices[14] = 15;
	m_indices[15] = 12; m_indices[16] = 15; m_indices[17] = 14;

	//Top
	m_indices[18] = 17; m_indices[19] = 16; m_indices[20] = 18;
	m_indices[21] = 17; m_indices[22] = 18; m_indices[23] = 19;

	//Bottom
	m_indices[24] = 20; m_indices[25] = 23; m_indices[26] = 22;
	m_indices[27] = 20; m_indices[28] = 21; m_indices[29] = 23;

	//Back
	m_indices[30] = 7; m_indices[31] = 5; m_indices[32] = 4;
	m_indices[33] = 7; m_indices[34] = 4; m_indices[35] = 6;

	//create Model class for rendring pipeline
	m_model = new Model(m_colorVerts, m_numVert, m_indices, m_numInds, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CubeObject::initReg(float xLength, float yLength, float zLength, XMFLOAT4* pColors)
{
	int m_numVert = 8;
	int m_numInds = 36;

	m_colorVerts = new ColorVertex[m_numVert];
	m_indices = new WORD[m_numInds];
	XMFLOAT4 color;
	
	//Front top left
	color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);  //default color
	if (pColors) color = pColors[0];
	m_colorVerts[0].position = XMFLOAT3(-xLength/2, yLength/2, -zLength/2); m_colorVerts[0].color = color;

	//Front top right
	color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);  //default color
	if (pColors) color = pColors[1];
	m_colorVerts[1].position = XMFLOAT3( xLength/2,  yLength/2, -zLength/2); m_colorVerts[1].color = color;

	//Front bottom right
	color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);  //default color
	if (pColors) color = pColors[2];
	m_colorVerts[2].position = XMFLOAT3( xLength/2, -yLength/2, -zLength/2); m_colorVerts[2].color = color;

	//Front bottom left
	color = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);  //default color
	if (pColors) color = pColors[3];
	m_colorVerts[3].position = XMFLOAT3(-xLength/2, -yLength/2, -zLength/2); m_colorVerts[3].color = color;

	//Back top left
	color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);  //default color
	if (pColors) color = pColors[4];
	m_colorVerts[4].position = XMFLOAT3(-xLength/2,  yLength/2, zLength/2); m_colorVerts[4].color = color;

	//Back top right
	color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);  //default color
	if (pColors) color = pColors[5];
	m_colorVerts[5].position = XMFLOAT3( xLength/2,  yLength/2, zLength/2); m_colorVerts[5].color = color;

	//Back bottom right
	color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);  //default color
	if (pColors) color = pColors[0];
	m_colorVerts[6].position = XMFLOAT3( xLength/2, -yLength/2, zLength/2); m_colorVerts[6].color = color;

	//Back bottom left
	color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);  //default color
	if (pColors) color = pColors[1];
	m_colorVerts[7].position = XMFLOAT3(-xLength/2, -yLength/2, zLength/2); m_colorVerts[7].color = color;

	//Indicies
	//Front
	m_indices[0] = 0; m_indices[1] = 1; m_indices[2] = 2;
	m_indices[3] = 0; m_indices[4] = 2; m_indices[5] = 3;

	//Right
	m_indices[6] = 1; m_indices[7] = 5; m_indices[8] = 6;
	m_indices[9] = 1; m_indices[10] = 6; m_indices[11] = 2;

	//Left
	m_indices[12] = 4; m_indices[13] = 0; m_indices[14] = 3;
	m_indices[15] = 4; m_indices[16] = 3; m_indices[17] = 7;

	//Top
	m_indices[18] = 4; m_indices[19] = 5; m_indices[20] = 1;
	m_indices[21] = 4; m_indices[22] = 1; m_indices[23] = 0;

	//Bottom
	m_indices[24] = 3; m_indices[25] = 2; m_indices[26] = 6;
	m_indices[27] = 3; m_indices[28] = 6; m_indices[29] = 7;

	//Back
	m_indices[30] = 5; m_indices[31] = 4; m_indices[32] = 7;
	m_indices[33] = 5; m_indices[34] = 7; m_indices[35] = 6;

	//create Model class for rendring pipeline
	m_model = new Model(m_colorVerts, m_numVert, m_indices, m_numInds, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CubeObject::advance()
{
	rotateX(-0.008f);
	rotateY(0.005f);
}