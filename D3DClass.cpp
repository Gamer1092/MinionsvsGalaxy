#ifndef UNICODE
#define UNICODE
#endif

#include "D3DClass.h"

D3DClass::D3DClass(Camera* cam)
{
	g_driverType = D3D_DRIVER_TYPE_NULL;
	g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	g_pd3dDevice = nullptr;
	g_pImmediateContext = nullptr;
	g_pSwapChain = nullptr;
	g_pRenderTargetView = nullptr;
	g_depthStencilBuffer = nullptr;
	g_depthStencilState = nullptr;
	g_depthStencilView = nullptr;
	g_rasterState = nullptr;
	g_colorShaders = nullptr;
	g_colorNormShaders = nullptr;
	g_textureShaders = nullptr;
	g_flatTextureShaders = nullptr;
	g_explosionShaders = nullptr;
	g_pCamera = cam;

	g_vsync = true;
}


D3DClass::~D3DClass(void)
{
	g_pd3dDevice = 0;
	g_pImmediateContext = 0;
	g_pSwapChain = 0;
	g_pRenderTargetView = 0;
	g_depthStencilBuffer = 0;
	g_depthStencilState = 0;
	g_depthStencilView = 0;
	g_rasterState = 0;
	g_colorShaders = 0;
	g_colorNormShaders = 0;
	g_textureShaders = 0;
	g_flatTextureShaders = 0;
	g_explosionShaders = 0;
	g_pCamera = 0;
}

void D3DClass::shutdown()
{
	if( g_pImmediateContext ) g_pImmediateContext->ClearState();

	if (g_pd3dDevice) g_pd3dDevice->Release();
	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_pRenderTargetView) g_pRenderTargetView->Release();
	if (g_depthStencilBuffer) g_depthStencilBuffer->Release();
	if (g_depthStencilState) g_depthStencilState->Release();
	if (g_depthStencilView) g_depthStencilView->Release();
	if (g_rasterState) g_rasterState->Release();

	if (g_colorShaders) delete g_colorShaders;
	if (g_colorNormShaders) delete g_colorNormShaders;
	if (g_textureShaders) delete g_textureShaders;
	if (g_flatTextureShaders) delete g_flatTextureShaders;
	if (g_explosionShaders) delete g_explosionShaders;
}

HRESULT D3DClass::initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	HRESULT hr = S_OK;

	//create D3D device
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDevice( nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
                                D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );

        if ( hr == E_INVALIDARG )
        {
            // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
            hr = D3D11CreateDevice( nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                                    D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
        }

        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
    IDXGIFactory1* dxgiFactory = nullptr;
    {
        IDXGIDevice* dxgiDevice = nullptr;
        hr = g_pd3dDevice->QueryInterface( __uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice) );
        if (SUCCEEDED(hr))
        {
            IDXGIAdapter* adapter = nullptr;
            hr = dxgiDevice->GetAdapter(&adapter);
            if (SUCCEEDED(hr))
            {
                hr = adapter->GetParent( __uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory) );
                adapter->Release();
            }
            dxgiDevice->Release();
        }
    }
    if (FAILED(hr))
        return hr;

    // Create swap chain
    // DirectX 11.0 systems
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = screenWidth;
    sd.BufferDesc.Height = screenHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    hr = dxgiFactory->CreateSwapChain(g_pd3dDevice, &sd, &g_pSwapChain);

    dxgiFactory->Release();

    if (FAILED(hr))
        return hr;
	   
	// Initialize the description of the depth buffer.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	hr = g_pd3dDevice->CreateTexture2D(&depthBufferDesc, NULL, &g_depthStencilBuffer);
	if(FAILED(hr))
		return hr;

	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	hr = g_pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &g_depthStencilState);
	if(FAILED(hr))
		return hr;

	// Set the depth stencil state.
	g_pImmediateContext->OMSetDepthStencilState(g_depthStencilState, 1);

	// Initialize the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	hr = g_pd3dDevice->CreateDepthStencilView(g_depthStencilBuffer, &depthStencilViewDesc, &g_depthStencilView);
	if(FAILED(hr))
		return hr;
		
	// Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &pBackBuffer ) );
    if( FAILED( hr ) )
        return hr;

    hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &g_pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;
	// Bind the render target view and depth stencil buffer to the output render pipeline.
    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	hr = g_pd3dDevice->CreateRasterizerState(&rasterDesc, &g_rasterState);
	if(FAILED(hr))
		return hr;

	// Now set the rasterizer state using the device context.
	g_pImmediateContext->RSSetState(g_rasterState);

	// Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)screenWidth;
    vp.Height = (FLOAT)screenHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports(1, &vp);

	// Clear the blend state description.
	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.AlphaToCoverageEnable = true;
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
    blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//0x0f;

	// Create the blend state using the description.
	hr = g_pd3dDevice->CreateBlendState(&blendStateDescription, &g_blendState);
	if(FAILED(hr))
	{
		return false;
	}
	float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	g_pImmediateContext->OMSetBlendState(g_blendState, NULL, 0xFFFFFFFF);

	// Create and initialize shaders
	g_colorShaders = new ColorShaderClass(g_pd3dDevice, g_pCamera);
	g_colorNormShaders = new ColorNormShaderClass(g_pd3dDevice, g_pCamera);
	g_textureShaders = new TextureShaderClass(g_pd3dDevice, g_pCamera);
	g_flatTextureShaders = new FlatTextureShaderClass(g_pd3dDevice, g_pCamera);
	g_explosionShaders = new ExplosionShaderClass(g_pd3dDevice, g_pCamera);

	g_spriteBatch = new SpriteBatch(g_pImmediateContext);

	g_spriteFont = new SpriteFont(g_pd3dDevice, L"gameFont.spritefont" );

	return hr;
}

ID3D11DeviceContext* D3DClass::getDeviceContext() { return g_pImmediateContext; }

ID3D11Device* D3DClass::getDevice() { return g_pd3dDevice; }

void D3DClass::beginScene()
{
	// Clear the back buffer 
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, Colors::MidnightBlue);

	// Clear the depth buffer.
	g_pImmediateContext->ClearDepthStencilView(g_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DClass::endScene()
{
	// Present the information rendered to the back buffer to the front buffer (the screen)
	if (g_vsync)
		g_pSwapChain->Present(1, 0);
	else
		g_pSwapChain->Present(0, 0);
}

void D3DClass::renderVertices(GameObject* p_object)
{
	if (p_object->isColorObject())
	{
		g_colorShaders->setParameters(g_pImmediateContext, p_object);
		g_colorShaders->render(g_pImmediateContext);
	}
	else if (p_object->isColorNormObject())
	{
		g_colorNormShaders->setParameters(g_pImmediateContext, p_object);
		g_colorNormShaders->render(g_pImmediateContext);
	}
	else if (p_object->isTextureObject())
	{
		g_textureShaders->setParameters(g_pImmediateContext, p_object);
		g_textureShaders->render(g_pImmediateContext);
	}
	else if (p_object->isFlatTextureObject())
	{
		g_flatTextureShaders->setParameters(g_pImmediateContext, p_object);
		g_flatTextureShaders->render(g_pImmediateContext);
	}
}

void D3DClass::renderSpritesAndFonts(vector<Sprite*>* sprites, vector<Font*>* fonts)
{
	g_spriteBatch->Begin(SpriteSortMode_Deferred, g_blendState, nullptr, g_depthStencilState, g_rasterState);

	for (unsigned int i = 0; i < sprites->size(); i++)
	{
		Sprite* s = sprites->at(i);
		g_spriteBatch->Draw(s->getTexture(), s->getPosition(), s->getFrame(), Colors::White, 0.0f, XMFLOAT2(0,0), s->getScaleFactor());
	}

	for (unsigned int i = 0; i < fonts->size(); i++)
	{
		wstring text = fonts->at(i)->getText();
		const WCHAR* textOut = text.c_str();
		g_spriteFont->DrawString(g_spriteBatch, textOut, fonts->at(i)->getPosition(), XMLoadFloat4(&(fonts->at(i)->getColor())));
	}

	g_spriteBatch->End();
}

void D3DClass::renderParticles(ParticlesObject* p_object, XMFLOAT2 t, XMFLOAT3 origin)
{
	g_explosionShaders->setParameters(g_pImmediateContext, p_object, t, origin);
	g_explosionShaders->render(g_pImmediateContext, p_object->getModel()->getVertexCount());
}

IDXGISwapChain* D3DClass::getSwapChain() { return g_pSwapChain; }

ID3D11RenderTargetView* D3DClass::getRTV() { return g_pRenderTargetView; }