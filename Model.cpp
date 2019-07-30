#include "Model.h"

Model::Model(void)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_indices = 0;
	m_colorVerts = 0;
	m_textureVerts = 0;
	m_flatTextureVerts = 0;
	m_explosionTextureVerts = 0;
	m_textureFileName = 0;
}

Model::Model(ColorVertex* vertices, int num_verts, WORD* inds, int num_inds, D3D_PRIMITIVE_TOPOLOGY topology)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_texture = 0;
	m_textureView = 0;
	m_textureFileName = 0;
	m_colorVerts = vertices;
	m_textureVerts = 0;
	m_colorNormVerts = 0;
	m_flatTextureVerts = 0;
	m_explosionTextureVerts = 0;
	m_numVert = num_verts;
	m_indices = inds;
	m_numInds = num_inds;
	m_primTop = topology;
}

Model::Model(TextureVertex* vertices, WCHAR* filename, int num_verts, WORD* inds, int num_inds, D3D_PRIMITIVE_TOPOLOGY topology)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_texture = 0;
	m_textureView = 0;
	m_textureFileName = filename;
	m_colorVerts = 0;
	m_textureVerts = vertices;
	m_colorNormVerts = 0;
	m_flatTextureVerts = 0;
	m_explosionTextureVerts = 0;
	m_numVert = num_verts;
	m_indices = inds;
	m_numInds = num_inds;
	m_primTop = topology;
}

Model::Model(ColorNormVertex* vertices, int num_verts, WORD* inds, int num_inds, D3D_PRIMITIVE_TOPOLOGY topology)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_texture = 0;
	m_textureView = 0;
	m_textureFileName = 0;
	m_colorVerts = 0;
	m_textureVerts = 0;
	m_colorNormVerts = vertices;
	m_flatTextureVerts = 0;
	m_explosionTextureVerts = 0;
	m_numVert = num_verts;
	m_indices = inds;
	m_numInds = num_inds;
	m_primTop = topology;
}

Model::Model(FlatTextureVertex* vertices, WCHAR* filename, int num_verts, WORD* inds, int num_inds, D3D_PRIMITIVE_TOPOLOGY topology)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_texture = 0;
	m_textureView = 0;
	m_textureFileName = filename;
	m_colorVerts = 0;
	m_textureVerts = 0;
	m_colorNormVerts = 0;
	m_flatTextureVerts = vertices;
	m_explosionTextureVerts = 0;
	m_numVert = num_verts;
	m_indices = inds;
	m_numInds = num_inds;
	m_primTop = topology;
}

Model::Model(ExplosionTextureVertex* vertices, WCHAR* filename, int num_verts, WORD* inds, int num_inds, D3D_PRIMITIVE_TOPOLOGY topology)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_texture = 0;
	m_textureView = 0;
	m_textureFileName = filename;
	m_colorVerts = 0;
	m_textureVerts = 0;
	m_colorNormVerts = 0;
	m_flatTextureVerts = 0;
	m_explosionTextureVerts = vertices;
	m_numVert = num_verts;
	m_indices = inds;
	m_numInds = num_inds;
	m_primTop = topology;
}


Model::~Model(void)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_colorVerts = 0;
	m_textureVerts = 0;
	m_colorNormVerts = 0;
	m_flatTextureVerts = 0;
	m_explosionTextureVerts = 0;
	m_indices = 0;
	m_texture = 0;
	m_textureView = 0;
}

void Model::shutdown()
{
	if (m_indexBuffer)	m_indexBuffer->Release();
	if (m_vertexBuffer)	m_vertexBuffer->Release();
	if (m_textureView)  m_textureView->Release();
	if (m_texture) m_texture->Release();
}

HRESULT Model::initialize(ID3D11Device* device)
{
	HRESULT hr;

	int vertSize = 0;
	if (m_colorVerts)
		vertSize = sizeof(ColorVertex);
	else if (m_textureVerts)
	{
		vertSize = sizeof(TextureVertex);
		initTexture(device, m_textureFileName);
	}
	else if (m_colorNormVerts)
		vertSize = sizeof(ColorNormVertex);
	else if (m_flatTextureVerts)
	{
		vertSize = sizeof(FlatTextureVertex);
		initTexture(device, m_textureFileName);
	}
	else if (m_explosionTextureVerts)
	{
		vertSize = sizeof(ExplosionTextureVertex);
		initTexture(device, m_textureFileName);
	}

	// Create Vertex Buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = vertSize * m_numVert;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	if (m_colorVerts)
		InitData.pSysMem = m_colorVerts;
	else if (m_textureVerts)
		InitData.pSysMem = m_textureVerts;
	else if (m_colorNormVerts)
		InitData.pSysMem = m_colorNormVerts;
	else if (m_flatTextureVerts)
		InitData.pSysMem = m_flatTextureVerts;
	else if (m_explosionTextureVerts)
		InitData.pSysMem = m_explosionTextureVerts;
    hr = device->CreateBuffer(&bd, &InitData, &m_vertexBuffer);
    if (FAILED(hr))
        return hr;

	if(m_numInds > 0)
	{
		// Create Index Buffer
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * m_numInds;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		InitData.pSysMem = m_indices;
		hr = device->CreateBuffer(&bd, &InitData, &m_indexBuffer);
		if (FAILED(hr))
			return hr;
	}

	return true;
}

HRESULT Model::initTexture(ID3D11Device* device, WCHAR* filename)
{
	HRESULT rc;
	rc = CreateDDSTextureFromFile(device, filename, &m_texture, &m_textureView, NULL);
	return rc;
}

void Model::render(ID3D11DeviceContext* context)
{
	unsigned int stride;
	unsigned int offset = 0;

	if (m_colorVerts)
		stride = sizeof(ColorVertex);
	else if (m_textureVerts)
		stride = sizeof(TextureVertex);
	else if (m_colorNormVerts)
		stride = sizeof(ColorNormVertex);
	else if (m_flatTextureVerts)
		stride = sizeof(FlatTextureVertex);
	else if (m_explosionTextureVerts)
		stride = sizeof(ExplosionTextureVertex);

    // Set vertex buffer
    context->IASetVertexBuffers( 0, 1, &m_vertexBuffer, &stride, &offset );

	// Set Index Buffer
	if(m_numInds > 0)
		context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // Set primitive topology
	context->IASetPrimitiveTopology(m_primTop);
}

int Model::getIndexCount() { return m_numInds; }

int Model::getVertexCount() { return m_numVert; }

ColorVertex* Model::getColorVerts() { return m_colorVerts; }

TextureVertex* Model::getTextureVerts() { return m_textureVerts; }

ColorNormVertex* Model::getColorNormVerts() { return m_colorNormVerts; }

FlatTextureVertex* Model::getFlatTextureVerts() { return m_flatTextureVerts; }

WORD* Model::getIndicies() { return m_indices; }

ID3D11ShaderResourceView* Model::getTexShadResView() { return m_textureView; }