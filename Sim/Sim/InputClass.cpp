#include "inputclass.h"


InputClass::InputClass()
{
}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}

void InputClass::Initialise()
{
	int i;

	//initialise all keys to not pressed
	for (i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}
	return;
}

void InputClass::KeyDown(unsigned int input)
{
	//if pressed, mark key as true in array
	m_keys[input] = true;
	return;
}

void InputClass::KeyUp(unsigned int input)
{
	//if released, mark key as false in array
	m_keys[input] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}