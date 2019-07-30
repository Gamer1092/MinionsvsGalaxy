#pragma once

const int ascii_A = 65;
const int ascii_B = 66;
const int ascii_C = 67;
const int ascii_D = 68;
const int ascii_E = 69;
const int ascii_F = 70;
const int ascii_I = 73;
const int ascii_J = 74;
const int ascii_K = 75;
const int ascii_L = 76;
const int ascii_P = 80;
const int ascii_Q = 81;
const int ascii_R = 82;
const int ascii_S = 83;
const int ascii_W = 87;
const int ascii_X = 88;
const int ascii_Y = 89;
const int ascii_Z = 90;
const int ascii_1 = 49;
const int ascii_2 = 50;
const int ascii_3 = 51;
const int ascii_4 = 52;
const int ascii_5 = 53;
const int ascii_6 = 54;

class InputClass
{
private:
	bool m_keys[256];

public:
	InputClass();
	~InputClass();

	void resetInput();

	void keyDown(unsigned int);
	void keyUp(unsigned int);
	bool isKeyDown(unsigned int);
};

