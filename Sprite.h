#pragma once

#include <directxmath.h>
#include <d3d11.h>
#include "SpriteBatch.h"
#include "DDSTextureLoader.h"

using namespace DirectX;

class Sprite
{
private:
	XMFLOAT2					m_position;
	RECT						m_sourceFrame;
	ID3D11ShaderResourceView*	m_texture;
	float						m_scaleFactor;
	ID3D11Device*               m_device;

public:
	Sprite(WCHAR* filename, int x, int y, float scale, ID3D11Device* device);
	~Sprite();

	XMFLOAT2 getPosition();
	RECT* getFrame();
	ID3D11ShaderResourceView* getTexture();
	float getScaleFactor();
	void update(WCHAR*);
};