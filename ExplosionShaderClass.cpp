#include "ExplosionShaderClass.h"

ExplosionShaderClass::ExplosionShaderClass(ID3D11Device* device, Camera* cam)
{
	m_pVertexShader = 0;
	m_pGeometryShader = 0;
	m_pPixelShader = 0;
	m_pVertexLayout = 0;
	m_pConstantBuffer = 0;
	m_pSampler = 0;
	m_pCamera = cam;
	initialize(device);
}

ExplosionShaderClass::~ExplosionShaderClass(void)
{
	if (m_pVertexShader) m_pVertexShader->Release();
	if (m_pGeometryShader) m_pGeometryShader->Release();
	if (m_pPixelShader) m_pPixelShader->Release();
	if (m_pVertexLayout) m_pVertexLayout->Release();
	if (m_pConstantBuffer) m_pConstantBuffer->Release();
	if (m_pSampler) m_pSampler->Release();
	m_pVertexShader = 0;
	m_pPixelShader = 0;
	m_pVertexLayout = 0;
	m_pConstantBuffer = 0;
	m_pSampler = 0;
	m_pCamera = 0;
}

void ExplosionShaderClass::initialize(ID3D11Device* device)
{
	// Compile the vertex shader
	HRESULT hr;
    ID3DBlob* pVSBlob = nullptr;
    hr = CompileShaderFromFile(L"ExplosionShaders_VS.hlsl", "main", "vs_4_0", &pVSBlob);
    if(FAILED(hr))
        MessageBox(nullptr, L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);

	// Create the vertex shader
	hr = device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader);
	if(FAILED(hr))
		pVSBlob->Release();

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
	UINT numElements = ARRAYSIZE(layout);

    // Create the input layout
	hr = device->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pVertexLayout);
	pVSBlob->Release();

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
    hr = CompileShaderFromFile(L"ExplosionShaders_PS.hlsl", "main", "ps_4_0", &pPSBlob);
    if(FAILED(hr))
        MessageBox(nullptr, L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);

	// Create the pixel shader
	hr = device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);
	pPSBlob->Release();

	// Compile the geometry shader
	ID3DBlob* pGSBlob = nullptr;
    hr = CompileShaderFromFile(L"GeoShaders_GS.hlsl", "main", "gs_4_0", &pGSBlob);
    if(FAILED(hr))
        MessageBox(nullptr, L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);

    // Create the geometry shader
	hr = device->CreateGeometryShader(pGSBlob->GetBufferPointer(), pGSBlob->GetBufferSize(), nullptr, &m_pGeometryShader);
	pGSBlob->Release();

	// Init Constant Buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ExplosionTextureConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = device->CreateBuffer(&bd, nullptr, &m_pConstantBuffer);

	// Init Texture Sampler State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = device->CreateSamplerState(&sampDesc, &m_pSampler);
}

void ExplosionShaderClass::setParameters(ID3D11DeviceContext* deviceContext, GameObject* p_object, XMFLOAT2 t, XMFLOAT3 origin)
{
	//get updated object world matrix
	XMFLOAT4X4 g_World = *(p_object->getWorldMatrix());
	XMFLOAT4X4 g_View = m_pCamera->getViewMatrix();
	XMFLOAT4X4 g_Projection = m_pCamera->getProjectionMatrix();

	// Update constant buffer
	ExplosionTextureConstBuffer cb;
	cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&g_World));
	cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&g_View));
	cb.mProjection = XMMatrixTranspose(XMLoadFloat4x4(&g_Projection));
	cb.mTimer = XMFLOAT4(t.x, t.y, 0.0f, 0.0f);
	XMFLOAT3 up = m_pCamera->getUp();
	XMFLOAT3 dir = m_pCamera->getFwd();
	XMVECTOR side = XMVector3Cross(XMLoadFloat3(&up), XMLoadFloat3(&dir));
	XMVector3Normalize(side);
	cb.mUp = XMFLOAT4(up.x, up.y, up.z, 0);
	XMFLOAT3 side3;
	XMStoreFloat3(&side3, side);
	cb.mSide = XMFLOAT4(side3.x, side3.y, side3.z, 0);
	cb.mOrigin = XMFLOAT4(origin.x, origin.y, origin.z, 1.0f);

	// Set contant buffer
	deviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);

	// Send the textures to the Pixel Shader
	ID3D11ShaderResourceView* texView = p_object->getModel()->getTexShadResView();
	deviceContext->PSSetShaderResources(0, 1, &texView);
}

void ExplosionShaderClass::render(ID3D11DeviceContext* deviceContext, int vertCount)
{
	// Set the input layout
	deviceContext->IASetInputLayout(m_pVertexLayout);

	// Set Shader properties
	deviceContext->VSSetShader(m_pVertexShader, nullptr, 0 );
	deviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	deviceContext->PSSetShader(m_pPixelShader, nullptr, 0 );
	deviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	deviceContext->PSSetSamplers(0, 1, &m_pSampler);
	deviceContext->GSSetShader(m_pGeometryShader, nullptr, 0 );
	deviceContext->GSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	// Render triangles
	deviceContext->Draw(vertCount, 0);
}

HRESULT ExplosionShaderClass::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;

    // Disable optimizations to further improve shader debugging
    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ID3DBlob* pErrorBlob = nullptr;
    hr = D3DCompileFromFile( szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob );
    if( FAILED(hr) )
    {
        if( pErrorBlob )
        {
            OutputDebugStringA( reinterpret_cast<const char*>( pErrorBlob->GetBufferPointer() ) );
            pErrorBlob->Release();
        }
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}