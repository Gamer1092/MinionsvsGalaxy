#pragma once

#include "Camera.h"
#include "Shapes/TexturedQuad.h"
#include <vector>
using namespace std;

class Skybox
{
private:
	Camera* camera;
	TexturedQuad* quads[6];
	vector<GameObject*> objs;
	float offset;

public:
	Skybox(Camera* cam);
	~Skybox(void);

	vector<GameObject*> getModels();
	void strafeLeft();
	void strafeRight();
	void moveForward();
	void moveBackward();
	void changeTexture(ID3D11Device*, WCHAR*);
};

