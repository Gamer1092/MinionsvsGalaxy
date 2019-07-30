#include "ShipStatus.h"

ShipStatus::ShipStatus(ID3D11Device* device):
	lives(3),
	score(0),
	gameover(false),
	leveledUp(false),
	show(false),
	maxHealth(100),
	health(100),
	armor(0),
	maxArmor(100),
	level(1),
	hitFrames(0),
	maxExperience(250),
	currentExperience(0)
{	
	m_fonts.push_back(new Font(L"Health", 10.0f, 10.0f));
	m_fonts.push_back(new Font(L"Lives", 130.0f, 10.0f));
	m_fonts.push_back(new Font(L"Score", 610.0f, 10.0f));
	m_fonts.push_back(new Font(L"Exp: ", 1100.0f, 10.0f));
	m_fonts.push_back(new Font(L"Lvl: ", 1100.0f, 40.0f));

	wstringstream wssHealth, wssLives, wssScore, wssExperience, wssLevel;
	wssHealth << health;
	wssLives << lives;
	wssScore << score;
	wssExperience << currentExperience;
	wssLevel << level;

	m_fonts.push_back(new Font(wssHealth.str(), 20.0f, 40.0f, XMFLOAT4(0.2f, 1.0f, 0.2f, 1.0f)));
	m_fonts.push_back(new Font(wssLives.str(), 150.0f, 40.0f, XMFLOAT4(0.4f, 0.4f, 1.0f, 1.0f)));
	m_fonts.push_back(new Font(wssScore.str(), 635.0f, 40.0f, XMFLOAT4(1.0f, 1.0f, 0.2f, 1.0f)));
	m_fonts.push_back(new Font(wssExperience.str(), 1170.0f, 10.0f, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)));
	m_fonts.push_back(new Font(wssLevel.str(), 1170.0f, 40.0f, XMFLOAT4(0.2f, 1.0f, 1.0f, 1.0f)));

	m_sprites.push_back(new Sprite(L"Textures/bombIcon.dds", 20, 640, 0.3f, device));
	m_sprites.push_back(new Sprite(L"Textures/life-3.dds", 1200, 640, 0.15f, device));
	m_sprites.push_back(new Sprite(L"Textures/zadekIcon.dds", 1120, 645, 0.3f, device));
}

ShipStatus::~ShipStatus(void)
{
	Shutdown();
}

void ShipStatus::Shutdown()
{

}

void ShipStatus::update()
{
    wstringstream wssHealth, wssLives, wssScore, wssExperience, wssLevel;
	wssHealth << health;
	wssLives << lives;
	wssScore << score;
	wssExperience << currentExperience;
	wssLevel << level;
    m_fonts.at(5)->setText(wssHealth.str());
    m_fonts.at(6)->setText(wssLives.str());
    m_fonts.at(7)->setText(wssScore.str());
    m_fonts.at(8)->setText(wssExperience.str());
    m_fonts.at(9)->setText(wssLevel.str());
	if (hitFrames > 0)
		hitFrames--;
}

void ShipStatus::levelUp()
{
	level += 1;
    currentExperience = currentExperience - maxExperience;
    maxExperience = maxExperience * level;
	maxHealth += 20;
	health = maxHealth;
	maxArmor += 50;
}
void ShipStatus::setGameOver(bool g) { gameover = g; }

bool ShipStatus::isGameOver() { return gameover; }

int ShipStatus::getLife() { return lives; }

vector<Font*>* ShipStatus::getFonts() { return &m_fonts; }

vector<Sprite*>* ShipStatus::getSprites() { return &m_sprites; }

bool ShipStatus::display() { return show; }

void ShipStatus::setShow(bool b) { show = b; }

void ShipStatus::die()
{
        if(lives -1 <= 0){
            setGameOver(true);
        }else{
            lives--;
            health = maxHealth;
            armor = maxArmor;
        }

}
bool ShipStatus::takeDamage(int damage) { 
	bool survive = true;
	if (hitFrames == 0)
	{
		if(health-damage <= 0){
			die();
			survive = false;   
		}else{
			health = health-damage;
			hitFrames = 30;
		}
	}
	update();
    return survive;
}
bool ShipStatus::gainExperience(int exp) { 
    bool level = false;
    currentExperience += exp;
    if(currentExperience >= maxExperience){
        levelUp();
        level = true;
    }

    score += exp*2;
    //m_sprites.at(0)->update(L"Textures/zadekIcon.dds");
    update();
    return level;
}

void ShipStatus::updateIcons(int num, WCHAR* filename){
    m_sprites.at(num)->update(filename);
}