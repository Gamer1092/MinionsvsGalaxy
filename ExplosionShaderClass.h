#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "VertexTypes.h"
#include "GameObject.h"
#include "Camera.h"

class ExplosionShaderClass
{
private:
	ID3D11VertexShader*      m_pVertexShader;
	ID3D11GeometryShader*	 m_pGeometryShader;
	ID3D11PixelShader*       m_pPixelShader;
	ID3D11InputLayout*       m_pVertexLayout;
	ID3D11Buffer*			 m_pConstantBuffer;
	ID3D11SamplerState*		 m_pSampler;
	Camera*					 m_pCamera;

	void initialize(ID3D11Device* device);
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

public:
	ExplosionShaderClass(ID3D11Device* device, Camera* cam);
	~ExplosionShaderClass(void);

	void setParameters(ID3D11DeviceContext* deviceContext, GameObject* p_object, XMFLOAT2 t, XMFLOAT3 origin);
	void render(ID3D11DeviceContext* deviceContext, int vertCount);
};

