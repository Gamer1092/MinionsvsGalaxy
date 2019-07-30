#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "VertexTypes.h"
#include "GameObject.h"
#include "Camera.h"


class ColorNormShaderClass
{
private:
	ID3D11VertexShader*      m_pVertexShader;
	ID3D11PixelShader*       m_pPixelShader;
	ID3D11InputLayout*       m_pVertexLayout;
	ID3D11Buffer*			 m_pConstantBuffer;
	Camera*					 m_pCamera;
	int						 m_numIndices;

	void initialize(ID3D11Device* device);
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

public:
	ColorNormShaderClass(ID3D11Device* device, Camera* cam);
	~ColorNormShaderClass(void);

	void setParameters(ID3D11DeviceContext* deviceContext, GameObject* p_object);
	void render(ID3D11DeviceContext* deviceContext);
};

