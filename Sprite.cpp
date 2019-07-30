#include "Sprite.h"

Sprite::Sprite(WCHAR* filename, int x, int y, float scale, ID3D11Device* device)
{
	m_device = device;
    m_position = XMFLOAT2((float)x, (float)y);
    update(filename);
    m_scaleFactor = scale;
}

XMFLOAT2 Sprite::getPosition() { return m_position; }

RECT* Sprite::getFrame() { return &m_sourceFrame; }

ID3D11ShaderResourceView* Sprite::getTexture() { return m_texture; }

float Sprite::getScaleFactor() { return m_scaleFactor; } 

void Sprite::update(WCHAR* filename ){
	ID3D11Resource* resource;
	CreateDDSTextureFromFile(m_device, filename, &resource, &m_texture);

	auto txt = reinterpret_cast<ID3D11Texture2D*>(resource);
	D3D11_TEXTURE2D_DESC desc;
	txt->GetDesc(&desc);

	m_sourceFrame.left = 0;
	m_sourceFrame.top = 0;
	m_sourceFrame.right = desc.Width;
	m_sourceFrame.bottom =  desc.Height;
}