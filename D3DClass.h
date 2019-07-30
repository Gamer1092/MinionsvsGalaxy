#pragma once

#include "GameObject.h"
#include "ParticlesObject.h"
#include "Camera.h"
#include "ColorShaderClass.h"
#include "ColorNormShaderClass.h"
#include "TextureShaderClass.h"
#include "FlatTextureShaderClass.h"
#include "ExplosionShaderClass.h"
#include "Sprite.h"
#include "Font.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include <vector>
using std::vector;

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 mLightPos;
	XMFLOAT4 mLightPos2;
	XMFLOAT4 mEyePos;
};
static_assert((sizeof(ConstantBuffer) % 16) == 0, "Constant Buffer size must be 16-byte aligned");

class D3DClass
{
private:
	D3D_DRIVER_TYPE          g_driverType;
	D3D_FEATURE_LEVEL        g_featureLevel;
	ID3D11Device*            g_pd3dDevice;
	ID3D11DeviceContext*     g_pImmediateContext;
	IDXGISwapChain*          g_pSwapChain;
	ID3D11RenderTargetView*  g_pRenderTargetView;
	ID3D11Texture2D*		 g_depthStencilBuffer;
	ID3D11DepthStencilState* g_depthStencilState;
	ID3D11DepthStencilView*  g_depthStencilView;
	ID3D11RasterizerState*   g_rasterState;
	ID3D11BlendState*		 g_blendState;
	ColorShaderClass*		 g_colorShaders;
	ColorNormShaderClass*	 g_colorNormShaders;
	TextureShaderClass*		 g_textureShaders;
	FlatTextureShaderClass*	 g_flatTextureShaders;
	ExplosionShaderClass*	 g_explosionShaders;
	SpriteBatch*			 g_spriteBatch;
	SpriteFont*				 g_spriteFont;
	Camera*					 g_pCamera;
	boolean					 g_vsync;

	HRESULT CompileShaderFromFile(WCHAR*, LPCSTR, LPCSTR, ID3DBlob**);

public:
	D3DClass(Camera*);
	~D3DClass(void);
	void shutdown();

	HRESULT initialize(int, int, HWND);
	ID3D11DeviceContext* getDeviceContext();
	ID3D11Device* getDevice();
	void beginScene();
	void endScene();
	void renderVertices(GameObject*);
	void renderSpritesAndFonts(vector<Sprite*>* sprites, vector<Font*>* fonts);
	void renderParticles(ParticlesObject*, XMFLOAT2 t, XMFLOAT3 origin);

	IDXGISwapChain* getSwapChain();
	ID3D11RenderTargetView* getRTV();
};

