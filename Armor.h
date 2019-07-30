#include "Shapes/CubeObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class Armor
{
private:
	XMFLOAT4 pos;
	XMFLOAT4 ori;
	XMFLOAT4 L;
	XMFLOAT4 vel;
	CubeObject* body;

	bool collected;

public:
	Armor(void);
	~Armor(void);

	GameObject* getModel();

	XMFLOAT3 getPos();
	XMFLOAT4 getOri();
	XMFLOAT4 getL();
	XMFLOAT4 getVel();
	float    getRadius();

	void setPos(XMFLOAT4);
	void setOri(XMFLOAT4);
	void setL(XMFLOAT4);
	void setVel(XMFLOAT4);

	void advance();
	bool isCollected();
	void collect();
	void translate(float, float, float);
};