#pragma once

#include <DirectXMath.h>
#include <string>

using std::wstring;
using namespace DirectX;

class Font
{
public:
	Font(wstring text, float xpos=0.0f, float ypos=0.0f, XMFLOAT4 color=XMFLOAT4(1,1,1,1));
	~Font(void);
	void setText(wstring);
	void setPosition(float, float);
	wstring getText();
	XMFLOAT2 getPosition();
	XMFLOAT4 getColor();

private:
	wstring text;
	XMFLOAT2 position;
	XMFLOAT4 color;
};