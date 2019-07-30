#include "TexturedQuad.h"


TexturedQuad::TexturedQuad()
{
	initializeModel(1.0f, 1.0f, L"", false);
}

TexturedQuad::TexturedQuad(float xLength, float yLength, WCHAR* filename, bool hasLighting)
{
	initializeModel(xLength, yLength, filename, hasLighting);
}

TexturedQuad::~TexturedQuad(void)
{
	m_model = 0;
}

void TexturedQuad::shutdown()
{
	GameObject::shutdown();	
}

void TexturedQuad::initializeModel(float xLength, float yLength, WCHAR* filename, bool hasLighting)
{
	int m_numVert = 4;
	int m_numInds = 6;

	//Indicies
	m_indices = new WORD[m_numInds];

	//Front
	m_indices[0] = 0; m_indices[1] = 1; m_indices[2] = 2;
	m_indices[3] = 0; m_indices[4] = 2; m_indices[5] = 3;

	if (hasLighting)
	{
		m_textureVerts = new TextureVertex[m_numVert];
	
		//Vertices
		m_textureVerts[0].position = XMFLOAT4(-xLength/2,  yLength/2, 0, 1); 
		m_textureVerts[0].texcoord = XMFLOAT2(0.0f, 0.0f);
		m_textureVerts[0].normal = XMFLOAT4(0.0f, 0.0f, -1.0f, 0);

		m_textureVerts[1].position = XMFLOAT4( xLength/2,  yLength/2, 0, 1); 
		m_textureVerts[1].texcoord = XMFLOAT2(1.0f, 0.0f);
		m_textureVerts[1].normal = XMFLOAT4(0.0f, 0.0f, -1.0f, 0);

		m_textureVerts[2].position = XMFLOAT4( xLength/2, -yLength/2, 0, 1); 
		m_textureVerts[2].texcoord = XMFLOAT2(1.0f, 1.0f);
		m_textureVerts[2].normal = XMFLOAT4(0.0f, 0.0f, -1.0f, 0);

		m_textureVerts[3].position = XMFLOAT4(-xLength/2, -yLength/2, 0, 1); 
		m_textureVerts[3].texcoord = XMFLOAT2(0.0f, 1.0f);
		m_textureVerts[3].normal = XMFLOAT4(0.0f, 0.0f, -1.0f, 0);

		//create Model class for rendring pipeline
		m_model = new Model(m_textureVerts, filename, m_numVert, m_indices, m_numInds, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	else
	{
		m_flatTextureVerts = new FlatTextureVertex[m_numVert];
	
		//Vertices
		m_flatTextureVerts[0].position = XMFLOAT3(-xLength/2,  yLength/2, 0); 
		m_flatTextureVerts[0].texcoord = XMFLOAT2(0.0f, 0.0f);

		m_flatTextureVerts[1].position = XMFLOAT3( xLength/2,  yLength/2, 0); 
		m_flatTextureVerts[1].texcoord = XMFLOAT2(1.0f, 0.0f);

		m_flatTextureVerts[2].position = XMFLOAT3( xLength/2, -yLength/2, 0); 
		m_flatTextureVerts[2].texcoord = XMFLOAT2(1.0f, 1.0f);

		m_flatTextureVerts[3].position = XMFLOAT3(-xLength/2, -yLength/2, 0); 
		m_flatTextureVerts[3].texcoord = XMFLOAT2(0.0f, 1.0f);

		//create Model class for rendring pipeline
		m_model = new Model(m_flatTextureVerts, filename, m_numVert, m_indices, m_numInds, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}

void TexturedQuad::advance()
{
	rotateY(0.01f);
}	