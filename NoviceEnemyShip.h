#include "Shapes\CubeObject.h"
#include "Shapes\PrismObject.h"
#include "Enemy.h"
#include <vector>

const float NOVICE_SHIP_MOVEMENT = 0.15f;

class NoviceEnemyShip : public Enemy
{
private:
	CubeObject* body;
	CubeObject* cannon;
	PrismObject* wing1;
	PrismObject* wing2;


public:
	NoviceEnemyShip(void);
	~NoviceEnemyShip(void);

	virtual void translate(float, float, float);
	virtual void rotateX(float);
	virtual void rotateY(float);
	virtual void rotateZ(float);

	XMFLOAT3 getPosition();
	bool update(XMFLOAT3 pos, ID3D11Device* device);
	void fireLaser(XMFLOAT3 pos, ID3D11Device* device);
};
