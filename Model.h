#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include "VertexTypes.h"
#include "DDSTextureLoader.h"
using namespace DirectX;

class Model
{
private:
	D3D_PRIMITIVE_TOPOLOGY	m_primTop;
	ID3D11Buffer*			m_vertexBuffer;
	ID3D11Buffer*			m_indexBuffer;
	WCHAR*					m_textureFileName;
	ID3D11Resource*			m_texture;
	ID3D11ShaderResourceView* m_textureView;
	int						m_numVert;
	int						m_numInds;
	WORD*					m_indices;
	ColorVertex*			m_colorVerts;
	TextureVertex*			m_textureVerts;
	ColorNormVertex*		m_colorNormVerts;
	FlatTextureVertex*		m_flatTextureVerts;
	ExplosionTextureVertex*	m_explosionTextureVerts;

public:
	Model(void);
	Model(ColorVertex*, int, WORD*, int, D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Model(TextureVertex*, WCHAR*, int, WORD*, int, D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Model(ColorNormVertex* vertices, int num_verts, WORD* inds, int num_inds, D3D_PRIMITIVE_TOPOLOGY topology);
	Model(FlatTextureVertex*, WCHAR*, int, WORD*, int, D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Model(ExplosionTextureVertex*, WCHAR*, int, WORD*, int, D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	~Model(void);
	void shutdown();

	int getIndexCount();
	int getVertexCount();
	ColorVertex* getColorVerts();
	TextureVertex* getTextureVerts();
	ColorNormVertex* getColorNormVerts();
	FlatTextureVertex* getFlatTextureVerts();
	ExplosionTextureVertex* getExplosionTextureVerts();

	WORD* getIndicies();
	HRESULT initialize(ID3D11Device*);
	HRESULT initTexture(ID3D11Device*, WCHAR*);
	ID3D11ShaderResourceView* getTexShadResView();
	void render(ID3D11DeviceContext*);
};

