#include "Font.h"

Font::Font(wstring textIn, float xpos, float ypos, XMFLOAT4 col)
{
	text = textIn;
	position.x = xpos;
	position.y = ypos;
	color = col;
}

Font::~Font(void)
{
}

wstring Font::getText() {return text;}
XMFLOAT2 Font::getPosition() {return position;}
XMFLOAT4 Font::getColor() { return color; }

void Font::setText(wstring t) {text = t;}
void Font::setPosition(float x, float y) {position.x = x; position.y = y;}