#include "Shapes\Torus.h"
#include "Shapes\CubeObject.h"
#include "Laser.h"
#include "Enemy.h"
#include <vector>

using namespace std;

const float WARRIOR_SHIP_MOVEMENT = 0.25f;

class WarriorEnemyShip : public Enemy
{
private:
	Torus* body;
	Torus* body2;
	CubeObject* cannon1;
	CubeObject* cannon2;


public:
	WarriorEnemyShip(void);
	~WarriorEnemyShip(void);

	virtual void translate(float, float, float);
	virtual void rotateX(float);
	virtual void rotateY(float);
	virtual void rotateZ(float);

	XMFLOAT3 getPosition();
	void advance();
	bool update(XMFLOAT3 pos, ID3D11Device* device);
	void fireLaser(XMFLOAT3 pos, ID3D11Device* device);
};