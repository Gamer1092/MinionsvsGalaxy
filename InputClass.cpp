#include "InputClass.h"


InputClass::InputClass(void)
{
	resetInput();
}

InputClass::~InputClass(void)
{
}

void InputClass::resetInput()
{
	for (int i = 0; i < 256; i++)
		m_keys[i] = false;
}

void InputClass::keyDown(unsigned int input) { m_keys[input] = true; }

void InputClass::keyUp(unsigned int input) { m_keys[input] = false; }

bool InputClass::isKeyDown(unsigned int key) { return m_keys[key]; }