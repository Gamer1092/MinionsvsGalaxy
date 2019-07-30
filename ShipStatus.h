#pragma once

#define NUM_DEBRIS 500
#define NUM_HEALTH 25
#define NUM_ARMOR 25
#include <sstream>
#include <vector>
#include <DirectXMath.h>
#include "Font.h"
#include "Sprite.h"
#include "Health.h"
#include "Armor.h"
#include "GameObject.h"
#include "Sprite.h"

using namespace std;
using namespace DirectX;

class ShipStatus{
public:

	ShipStatus(ID3D11Device*);
	~ShipStatus(void);
	void Shutdown();
	void setGameOver(bool);
	bool takeDamage(int);
    bool gainExperience(int);
	bool isGameOver();
	void levelUp();
	void die();
	bool display();
	void setShow(bool);
	void update();
    void updateIcons(int, WCHAR*);
    int getLife();
	vector<Font*>* getFonts();
	vector<Sprite*>* getSprites();

private:
	int lives;
	int score;
	int maxHealth; 
	int health;
	int maxExperience;
	int currentExperience;
	int armor;
	int maxArmor;
	int level;
	int hitFrames;
	bool gameover;
	bool leveledUp;
	bool show;
	vector<Font*> m_fonts;
	vector<Sprite*> m_sprites;
	ID3D11Device* sStatusDevice;
};