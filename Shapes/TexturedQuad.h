#pragma once
#include "../GameObject.h"
#include <vector>
using namespace std;

class TexturedQuad : public GameObject
{
public:
	TexturedQuad(void);
	TexturedQuad(float xLength, float yLength, WCHAR* filename, bool hasLighting);
	virtual ~TexturedQuad(void);
	virtual void shutdown();
	virtual void advance();
	vector<GameObject*> objs;

private:
	virtual void initializeModel(float xLength, float yLength, WCHAR* filename, bool hasLighting);
};

