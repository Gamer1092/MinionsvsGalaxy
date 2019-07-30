#include "Shapes\SphereObject.h"
#include "Shapes\CubeObject.h"
#include "Laser.h"
#include "Enemy.h"
#include <vector>

using namespace std;

const float ELITE_SHIP_MOVEMENT = 0.35f;

class EliteEnemyShip : public Enemy
{
private:
	SphereObject* body;
	SphereObject* body2;
	CubeObject* cannon1;
	CubeObject* cannon2;
	CubeObject* cannon3;
	CubeObject* cannon4;

public:
	EliteEnemyShip(void);
	~EliteEnemyShip(void);

	virtual void translate(float, float, float);
	virtual void rotateX(float);
	virtual void rotateY(float);
	virtual void rotateZ(float);

	XMFLOAT3 getPosition();
	void advance();
	bool update(XMFLOAT3 pos, ID3D11Device* device);
	void fireLaser(XMFLOAT3 pos, ID3D11Device* device);
};