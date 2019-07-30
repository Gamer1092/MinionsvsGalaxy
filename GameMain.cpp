//--------------------------------------------------------------------------------------
// Author: Sebastian Evoniak - 100726326
// File: Assignment5.cpp
//--------------------------------------------------------------------------------------

#ifndef UNICODE
#define UNICODE
#endif

//#define NUM_DEBRIS 500

#include <windows.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include "D3DClass.h"
#include "Camera.h"
#include "InputClass.h"
#include "Sound.h"
#include "Laser.h"
#include "Bomb.h"
#include "Asteroid.h"
#include "Skybox.h"
#include "Font.h"
#include "Sprite.h"
#include "ShipStatus.h"
#include "Explosion.h"
#include "NoviceEnemyShip.h"
#include "WarriorEnemyShip.h"
#include "EliteEnemyShip.h"
#include "Shapes/TexturedQuad.h"
using namespace std;


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = nullptr;
HWND                    g_hWnd = nullptr;
D3DClass*				g_pd3d;
ID3D11Device*           g_pd3dDevice;
ID3D11DeviceContext*    g_pImmediateContext;
InputClass*				g_pInput;
Camera*					g_pCamera;
Sound*					g_pSound;
int						screenWidth;
int						screenHeight;
bool					collided;
bool					startMenu;
bool					inCredit;
vector<GameObject*>		objs;
vector<Font*>			fonts;
vector<Sprite*>			sprites;
vector<ParticlesObject*> particles;
vector<Enemy*>			enemies;
vector<Bullet*>			bullets;
vector<Armor*>			armor;
vector<Health*>			health;
ShipStatus*				status;

//--------------------------------------------------------------------------------------
// Game Objects
//--------------------------------------------------------------------------------------
SphereObject*			sphere;
TexturedQuad*			menu;
Laser*					laser;
Bomb*					bomb;
Asteroid*				debris[NUM_DEBRIS];
Skybox*					skybox;
Explosion*				explosion;
NoviceEnemyShip*		noviceShip;
WarriorEnemyShip*		warriorShip;
EliteEnemyShip*			eliteShip;


//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
HRESULT InitDevice();
void InitGameObjects();
void CleanupDevice();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void UpdateObjects(int);
void Render(int);
bool handleShipCollisions(int);
void handleLaserCollisions(int);
void handleBombCollisions(int);
void handleInput();
float getAngle();
void calcFPS();


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if(FAILED(InitWindow(hInstance, nCmdShow)))
        return 0;

    if(FAILED(InitDevice()))
    {
        CleanupDevice();
        return 0;
    }

	InitGameObjects();

    // Main message loop
    MSG msg = {0};
    while(WM_QUIT != msg.message)
    {
        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
			static int time = 0;
			time++;
			if (time == 30000)
				time = 0;

			UpdateObjects(time);
            Render(time);
        }
    }

    CleanupDevice();

    return (int)msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(0, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"GameDevProjectClass";
    wcex.hIconSm = 0;
	startMenu = true;
	inCredit = false;
    if(!RegisterClassEx(&wcex))
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = {0, 0, 1280, 720};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    g_hWnd = CreateWindow(L"GameDevProjectClass", L"COMP3501 Project", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
						  rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
    if(!g_hWnd)
        return E_FAIL;

    ShowWindow(g_hWnd, nCmdShow);

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
	HRESULT hr = S_OK;

	collided = false;

    RECT rc;
    GetClientRect(g_hWnd, &rc);
    screenWidth = rc.right - rc.left;
    screenHeight = rc.bottom - rc.top;

	g_pInput = new InputClass();
	g_pCamera = new Camera(screenWidth, screenHeight);
	g_pd3d = new D3DClass(g_pCamera);
	g_pd3d->initialize(screenWidth, screenHeight, g_hWnd);

	g_pd3dDevice = g_pd3d->getDevice();
	g_pImmediateContext = g_pd3d->getDeviceContext();

	g_pSound = new Sound();
	g_pSound->Initialize(g_hWnd);

	g_pSound->PlayBGM(0);

	status = new ShipStatus(g_pd3dDevice);
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create and initialize the Model for each GameObject
//--------------------------------------------------------------------------------------
void InitGameObjects()
{
	menu = new TexturedQuad(10.5, 6, L"Textures/home-page.dds", false);
	objs.push_back(menu);

	skybox = new Skybox(g_pCamera);
	vector<GameObject*> temp = skybox->getModels();
	objs.insert(objs.end(), temp.begin(), temp.end());

	vector<Sprite*>* spriteTemp = status->getSprites();
	sprites.insert(sprites.end(), spriteTemp->begin(), spriteTemp->end());

	vector<Font*>* fontTemp = status->getFonts();
	fonts.insert(fonts.end(), fontTemp->begin(), fontTemp->end());

	explosion = new Explosion(L"Textures/explosion1.dds");
	particles.push_back(explosion);
	for (unsigned int i = 0; i < particles.size(); i++)
		particles.at(i)->getModel()->initialize(g_pd3dDevice);

	
	//Init game objects

	sphere = new SphereObject(L"Textures/sunTexture.dds");
	objs.push_back(sphere);
	sphere->translate(70.0f, 70.0f, 70.0f);
	sphere->scale(50.0f, 50.0f, 50.0f);

	bomb = new Bomb();
	objs.push_back(bomb->getModel());

	laser = new Laser();
	temp = laser->getModels();
	objs.insert(objs.end(), temp.begin(), temp.end());

	Armor* a = new Armor();
	armor.push_back(a);
	objs.push_back(a->getModel());
	a->translate(-8.0f, 2.0f, 2.0f);

	Health* h = new Health();
	health.push_back(h);
	objs.push_back(h->getModel());
	h->translate(-8.0f, -2.0f, 2.0f);

	for (int i = 0; i < NUM_DEBRIS; i++)
	{
		debris[i] = new Asteroid();
		XMVECTOR angles = XMVectorSet(3.14f*(rand() % 1000) / 1000.0f, 3.14f*(rand() % 1000) / 1000.0f, 3.14f*(rand() % 1000) / 1000.0f, 1.0f);
		debris[i]->setPos(XMFLOAT4(200 * (rand() % 1000) / 1000.0f, 200 * (rand() % 1000) / 1000.0f, 200 * (rand() % 1000) / 1000.0f, 1.0f));
		XMFLOAT4 quat;
		XMStoreFloat4(&quat, XMQuaternionRotationRollPitchYawFromVector(angles));
		debris[i]->setOri(quat);
		angles = XMVectorSet(0.01f*3.14f*(rand() % 2000 - 1000) / 1000.0f, 0.01f*3.14f*(rand() % 2000 - 1000) / 1000.0f, 0.01f*3.14f*(rand() % 2000 - 1000) / 1000.0f, 1.0f);
		XMStoreFloat4(&quat, XMQuaternionRotationRollPitchYawFromVector(angles));
		debris[i]->setL(quat);

		objs.push_back(debris[i]->getModel());
	}

	for (int i = 0; i < 10; i++)
	{
		noviceShip = new NoviceEnemyShip();
		temp = noviceShip->getModels();
		objs.insert(objs.end(), temp.begin(), temp.end());
		noviceShip->translate(70.0f, 70.0f, 70.0f);
		noviceShip->translate(70.0f * cos(i*XM_2PI/10.0f), 0.0f, 70.0f * sin(i*XM_2PI/10.0f));
		enemies.push_back(noviceShip);

		warriorShip = new WarriorEnemyShip();
		temp = warriorShip->getModels();
		objs.insert(objs.end(), temp.begin(), temp.end());
		warriorShip->translate(70.0f, 70.0f, 70.0f);
		warriorShip->translate(0.0f, 50.0f * cos(i*XM_2PI/10.0f), 50.0f * sin(i*XM_2PI/10.0f));
		enemies.push_back(warriorShip);

		if (i%2 ==0)
		{
			eliteShip = new EliteEnemyShip();
			temp = eliteShip->getModels();
			objs.insert(objs.end(), temp.begin(), temp.end());
			eliteShip->translate(70.0f, 70.0f, 70.0f);
			eliteShip->translate(50.0f * sin(i*XM_2PI/10.0f), 50.0f * cos(i*XM_2PI/10.0f), 0.0f);
			enemies.push_back(eliteShip);
		}
	}

	//init all game object models
	for (unsigned int i = 0; i < objs.size(); i++)
		objs.at(i)->getModel()->initialize(g_pd3dDevice);

	objs.erase(remove(objs.begin(), objs.end(), laser->getModel()), objs.end());
	objs.erase(remove(objs.begin(), objs.end(), laser->getModel2()), objs.end());
	objs.erase(remove(objs.begin(), objs.end(), laser->getTargetModel()), objs.end());
	objs.erase(remove(objs.begin(), objs.end(), bomb->getModel()), objs.end());
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
	if (g_pd3d) g_pd3d->shutdown();

	if (g_pInput) delete g_pInput;

	if (g_pCamera) delete g_pCamera;

	if (g_pSound) g_pSound->Shutdown();

	for (UINT i = 0; i < objs.size(); i++)
		if (objs.at(i)) objs.at(i)->shutdown();
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

		case WM_KEYDOWN:
			g_pInput->keyDown((unsigned int)wParam);
			break;

		case WM_KEYUP:
			g_pInput->keyUp((unsigned int)wParam);
			break;

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }
    return 0;
}


//--------------------------------------------------------------------------------------
// Update all objects this frame
//--------------------------------------------------------------------------------------
void UpdateObjects(int pk)
{
	float t = pk / 60.0f;

	static int frames = 0;
	if (frames == 30000)
		frames = 0;
	handleInput();

	g_pCamera->updateMatrices();

	laser->setParentMatrix(g_pCamera->getOri(), g_pCamera->getPos());

	for (int i = 0; i < NUM_DEBRIS; i++)
		debris[i]->advance();
	XMFLOAT3 pos = g_pCamera->getPos();
	XMFLOAT3 dir = g_pCamera->getFwd();
	if (handleShipCollisions(pk))
		frames = 29;
	if (frames % 8 == 5)
		skybox->changeTexture(g_pd3dDevice, L"Textures/spaceTextureRed.dds");
	if (frames % 8 == 1)
		skybox->changeTexture(g_pd3dDevice, L"Textures/spaceTexture.dds");
	if (frames > 0)
		frames--;

	if (laser->isFiring())
		handleLaserCollisions(pk);

	if (bomb->isFiring())
	{
		if (!bomb->hasExploded())
		{
			bomb->advance();
			handleBombCollisions(pk);
		}
		else
		{
			g_pSound->PlaySE(9);
			explosion->addInstance(bomb->getPos(), -t+0.02f, 1.0f);
			bomb->resetBomb();
			objs.erase(remove(objs.begin(), objs.end(), bomb->getModel()), objs.end());
			status->updateIcons(0, L"Textures/bombIcon.dds");
		}
	}

	explosion->update(t);

	bullets.clear();

	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		if (!enemies.at(i)->isDead())
		{
			bool shot = enemies.at(i)->update(g_pCamera->getPos(), g_pd3dDevice);
			vector<Bullet*> bulletTemp = enemies.at(i)->getBullets();
			bullets.insert(bullets.end(), bulletTemp.begin(), bulletTemp.end());
			if (shot)
				g_pSound->PlaySE(18);
		}
	}

	status->update();
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render(int pk)
{
	float t = pk / 60.0f;
	calcFPS();

	g_pd3d->beginScene();
	for (unsigned int i = 0; i < objs.size(); i++)
	{
		// Put model on pipeline
		objs.at(i)->getModel()->render(g_pImmediateContext);

		//render the model
		g_pd3d->renderVertices(objs.at(i));
	}

	//render sprites and fonts
	if (status->display())
		g_pd3d->renderSpritesAndFonts(&sprites, &fonts);

	//render particles
	for (unsigned int i = 0; i < particles.size(); i++)
	{
		ParticlesObject* gameobj = particles.at(i);

		// Put model on pipeline
		gameobj->getModel()->render(g_pImmediateContext);

		//render the model
		for (int j = 0; j < gameobj->getNumInst(); j++)
		{
			float phase = gameobj->getPhase(j);
		//	float time = t % 300;
			XMFLOAT2 timer = XMFLOAT2(t+phase, gameobj->getColor(j));
			g_pd3d->renderParticles(gameobj, timer, gameobj->getOrigin(j));
		}
	}

	for (unsigned int i = 0; i < bullets.size(); i++)
	{
		// Put model on pipeline
		bullets.at(i)->getModel()->getModel()->render(g_pImmediateContext);

		//render the model
		g_pd3d->renderVertices(bullets.at(i)->getModel());
	}

	g_pd3d->endScene();
}

bool handleShipCollisions(int pk)
{
	float t = pk/60.0f;
	// Sphere collision between camera (ship) and asteroid debris objects
	XMFLOAT3 camPos = g_pCamera->getPos();
	for (int i = 0; i < NUM_DEBRIS; i++)
	{
		if (!debris[i]->isDestroyed())
		{
			XMFLOAT3 debrisPos = debris[i]->getPos();
			float distSquared = (camPos.x-debrisPos.x)*(camPos.x-debrisPos.x)+(camPos.y-debrisPos.y)*(camPos.y-debrisPos.y)+(camPos.z-debrisPos.z)*(camPos.z-debrisPos.z);
			if (distSquared < 1.0f)
			{
				objs.erase(remove(objs.begin(), objs.end(), debris[i]->getModel()), objs.end());
				debris[i]->destroy();
				g_pSound->PlaySE(8);
				if(!status->takeDamage(10)){
                    g_pSound->PlaySE(17);
                   if(status->getLife() == 2)
                    status->updateIcons(1, L"Textures/life-2.dds");
                   else if(status->getLife() == 1)
                       status->updateIcons(1, L"Textures/life-1.dds");
                }
				return true;
			}
		}
	}

	// Sphere collision between camera (ship) and enemy ship objects
	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		Enemy* e = enemies.at(i);

		if (!e->isDead())
		{
			XMFLOAT3 debrisPos = e->getPosition();
			float distSquared = (camPos.x-debrisPos.x)*(camPos.x-debrisPos.x)+(camPos.y-debrisPos.y)*(camPos.y-debrisPos.y)+(camPos.z-debrisPos.z)*(camPos.z-debrisPos.z);
			if (distSquared < 1.0f)
			{
				g_pSound->PlaySE(8);
				if (e->damage(1000))
				{
					vector<GameObject*> temp = e->getModels();
					for (unsigned int i = 0; i < temp.size(); i++)
						objs.erase(remove(objs.begin(), objs.end(), temp.at(i)), objs.end());
					explosion->addInstance(e->getPosition(), -t+0.02f, 0.0f);
					g_pSound->PlaySE(9);

					if(!status->takeDamage(20))
					{
						g_pSound->PlaySE(17);
						if(status->getLife() == 2)
							status->updateIcons(1, L"Textures/life-2.dds");
						 else if(status->getLife() == 1)
							status->updateIcons(1, L"Textures/life-1.dds");
					}	

					if(status->gainExperience(e->getExp()))
					   g_pSound->PlaySE(16);
				}

				return true;
			}	
		}
	}

	// Sphere collision between camera (ship) and enemy laser shots
	for (int i = 0; i < bullets.size(); i++)
	{
		Bullet* b = bullets.at(i);
		XMFLOAT3 bPos = b->getPos();
		float distSquared = (camPos.x-bPos.x)*(camPos.x-bPos.x)+(camPos.y-bPos.y)*(camPos.y-bPos.y)+(camPos.z-bPos.z)*(camPos.z-bPos.z);
		if (distSquared < 2.0f)
		{
			g_pSound->PlaySE(8);
			if(!status->takeDamage(15)){
                g_pSound->PlaySE(17);
                if(status->getLife() == 2)
                status->updateIcons(1, L"Textures/life-2.dds");
                else if(status->getLife() == 1)
                    status->updateIcons(1, L"Textures/life-1.dds");
            }
			return true;
		}
	}
	return false;
}

void handleLaserCollisions(int pk)
{
	float t = pk/60.0f;
	XMFLOAT4X4 orientation = *(laser->getModel()->getWorldMatrix());
	XMVECTOR rayDir = XMVector3Normalize(XMVector4Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMLoadFloat4x4(&orientation)));
	XMVECTOR camPos = XMLoadFloat3(&(g_pCamera->getPos()));

	// Ray-Sphere collision detection between the laser and asteroid debris objects
	for (int i = 0; i < NUM_DEBRIS; i++)
	{
		if (!debris[i]->isDestroyed())
		{
			XMVECTOR center = XMLoadFloat3(&(debris[i]->getPos()));
			XMVECTOR diff   = XMVectorSubtract(camPos, center);

			float rayDirDotDiff = (XMVector3Dot(rayDir, diff)).m128_f32[0];
			float magDiff = (XMVector3Length(diff)).m128_f32[0];

			float radius = debris[i]->getRadius();

			float discriminant = rayDirDotDiff * rayDirDotDiff - magDiff * magDiff + radius * radius; 
			if (discriminant >= 0)
			{
				debris[i]->destroy();
				objs.erase(remove(objs.begin(), objs.end(), debris[i]->getModel()), objs.end());
				g_pSound->PlaySE(4);
				if(status->gainExperience(10))
                    g_pSound->PlaySE(16);
			}
		}
	}

	// Ray-Sphere collision detection between the laser and asteroid enemy objects
	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		Enemy* e = enemies.at(i);

		if (!e->isDead())
		{
			XMVECTOR center = XMLoadFloat3(&(e->getPosition()));
			XMVECTOR diff   = XMVectorSubtract(camPos, center);

			float rayDirDotDiff = (XMVector3Dot(rayDir, diff)).m128_f32[0];
			float magDiff = (XMVector3Length(diff)).m128_f32[0];

			float radius = e->getRadius();

			float discriminant = rayDirDotDiff * rayDirDotDiff - magDiff * magDiff + radius * radius; 
			if (discriminant >= 0)
			{
				g_pSound->PlaySE(8);
				if (e->damage(20))
				{
					vector<GameObject*> temp = e->getModels();
					for (unsigned int i = 0; i < temp.size(); i++)
						objs.erase(remove(objs.begin(), objs.end(), temp.at(i)), objs.end());
					explosion->addInstance(e->getPosition(), -t+0.02f, 0.0f);
					g_pSound->PlaySE(9);
					if(status->gainExperience(e->getExp()))
					   g_pSound->PlaySE(16);
				}	
			}
		}
	}
}

void handleBombCollisions(int pk)
{
	float t = pk/60.0f;
	// Sphere collision between bomb and asteroid debris objects
	XMFLOAT3 bombPos = bomb->getPos();

	for (int i = 0; i < NUM_DEBRIS; i++)
	{
		if (!debris[i]->isDestroyed())
		{
			XMFLOAT3 debrisPos = debris[i]->getPos();
			float distSquared = (bombPos.x - debrisPos.x)*(bombPos.x - debrisPos.x) + (bombPos.y - debrisPos.y)*(bombPos.y - debrisPos.y) + (bombPos.z - debrisPos.z)*(bombPos.z - debrisPos.z);
			if (distSquared < 5.0f)
			{
				objs.erase(remove(objs.begin(), objs.end(), debris[i]->getModel()), objs.end());
				debris[i]->destroy();
				g_pSound->PlaySE(4);
				if(status->gainExperience(15))
                    g_pSound->PlaySE(16);
			}
		}
	}

	// Sphere collision between bomb and enemy ship objects
	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		Enemy* e = enemies.at(i);

		if (!e->isDead())
		{
			XMFLOAT3 debrisPos = e->getPosition();
			float distSquared = (bombPos.x - debrisPos.x)*(bombPos.x - debrisPos.x) + (bombPos.y - debrisPos.y)*(bombPos.y - debrisPos.y) + (bombPos.z - debrisPos.z)*(bombPos.z - debrisPos.z);
			if (distSquared < 5.0f)
			{

				g_pSound->PlaySE(8);
				if (e->damage(100))
				{
					vector<GameObject*> temp = e->getModels();
					for (unsigned int i = 0; i < temp.size(); i++)
						objs.erase(remove(objs.begin(), objs.end(), temp.at(i)), objs.end());
					explosion->addInstance(e->getPosition(), -t+0.02f, 0.0f);
					g_pSound->PlaySE(9);
					if(status->gainExperience(e->getExp()+25))
					   g_pSound->PlaySE(16);
				}
			}
		}
	}
}

void handleInput()
{
	if(startMenu){
		if(g_pInput->isKeyDown(VK_RETURN)){
			startMenu = false;
			status->setShow(true);
			menu->scale(0.0f, 0.0f, 0.0f);
			objs.push_back(laser->getTargetModel());
			g_pSound->StopBGM();
			g_pSound->PlayBGM(1);
			g_pSound->PlaySE(13);
		}
		if(g_pInput->isKeyDown(ascii_C)){
			inCredit = true;
			menu->getModel()->initTexture(g_pd3dDevice, L"Textures/credit.dds");
			g_pSound->StopBGM();
			g_pSound->PlayBGM(2);
		}
		if(inCredit && g_pInput->isKeyDown(VK_SPACE)){
			inCredit = false;
			g_pSound->StopBGM();
			g_pSound->PlayBGM(0);
			menu->getModel()->initTexture(g_pd3dDevice, L"Textures/home-page.dds");
		}		
		if(!inCredit && g_pInput->isKeyDown(VK_ESCAPE)){
			exit(0);
		}
	}
	else
	{
		if (g_pInput->isKeyDown(ascii_I))		//laser movement
			laser->orbitX(-1);
		if (g_pInput->isKeyDown(ascii_J))
			laser->orbitY(-1);
		if (g_pInput->isKeyDown(ascii_K))
			laser->orbitX(1);
		if (g_pInput->isKeyDown(ascii_L))
			laser->orbitY(1);
		if (g_pInput->isKeyDown(VK_SPACE) && !laser->isFiring())
		{
			//This is used to test the change icon of life and font 
            /*if(!status->takeDamage(25)){
                    g_pSound->PlaySE(18);
                   if(status->getLife() == 2)
                    status->updateIcons(1, L"Textures/life-2.dds");
                   else if(status->getLife() == 1)
                       status->updateIcons(1, L"Textures/life-1.dds");
                }*/
				//ship
            status->updateIcons(2, L"Textures/zadek-2.dds");
			g_pSound->PlaySE(0);
            g_pSound->PlaySE(10);
			laser->toggle();
			objs.push_back(laser->getModel());
			objs.push_back(laser->getModel2());
		}
		if (!g_pInput->isKeyDown(VK_SPACE) && laser->isFiring())
		{
			status->updateIcons(2, L"Textures/zadekIcon.dds");
			laser->toggle();
			objs.erase(remove(objs.begin(), objs.end(), laser->getModel()), objs.end());
			objs.erase(remove(objs.begin(), objs.end(), laser->getModel2()), objs.end());
		}


		//Bomb
		if (g_pInput->isKeyDown(ascii_B) && !bomb->isFiring())
		{
			g_pSound->PlaySE(15);
			status->updateIcons(0, L"Textures/bomb-2.dds");
			bomb->setFiring(true);
			bomb->spawnBomb(g_pCamera->getPos(), g_pCamera->getFwd());
			objs.push_back(bomb->getModel());
		}

		if (g_pInput->isKeyDown(ascii_C))		//C key for camera cranes
		{
			if (g_pInput->isKeyDown(ascii_W))
				g_pCamera->craneUp();
			if (g_pInput->isKeyDown(ascii_S))
				g_pCamera->craneDown();
		}
		else if (g_pInput->isKeyDown(ascii_Z))		//Z key for camera zooms
		{
			if (g_pInput->isKeyDown(VK_UP))
				g_pCamera->zoomIn();
			if (g_pInput->isKeyDown(VK_DOWN))
				g_pCamera->zoomOut();
			if (g_pInput->isKeyDown(VK_RIGHT))
				g_pCamera->resetZoom();
		}
		else
		{
			if (g_pInput->isKeyDown(ascii_A))		// camera movement
			{
				g_pCamera->strafeLeft();
				skybox->strafeLeft();
			}
			if (g_pInput->isKeyDown(ascii_D))
			{
				g_pCamera->strafeRight();
				skybox->strafeRight();
			}
			if (g_pInput->isKeyDown(ascii_W))
			{
				g_pCamera->moveForward();
				skybox->moveForward();
			}
			if (g_pInput->isKeyDown(ascii_S))
			{
				g_pCamera->moveBackward();
				skybox->moveBackward();
			}
			if (g_pInput->isKeyDown(VK_LEFT))		//camera pans and tilts
				g_pCamera->panLeft();
			if (g_pInput->isKeyDown(VK_RIGHT))
				g_pCamera->panRight();
			if (g_pInput->isKeyDown(VK_UP))
				g_pCamera->tiltUp();
			if (g_pInput->isKeyDown(VK_DOWN))
				g_pCamera->tiltDown();
			if (g_pInput->isKeyDown(ascii_E)) 
				g_pCamera->rollLeft();
			if (g_pInput->isKeyDown(ascii_Q)) 
				g_pCamera->rollRight();	
		}
	}
}

float getAngle()
{
	static float earAngle = XM_PIDIV4;
	static bool closing = true;
	float angleRate = XM_PIDIV4 / 20000;

	if (earAngle - angleRate < 0.0f)
		closing = false;
	if (earAngle + angleRate > XM_PIDIV4)
		closing = true;

	if (closing)
		earAngle -= angleRate;
	else
		earAngle += angleRate;
	return earAngle;
}

void calcFPS()
{
	// Update our time	
	static float t = 0.0f;
	static float previous_t = 0.0f;
	static int sampleCount = 0;
	static float sampleStartTime = 0.0f;

    static DWORD dwTimeStart = 0;

    DWORD dwTimeCur = GetTickCount();
    if( dwTimeStart == 0 ) dwTimeStart = dwTimeCur;
	previous_t = t;
	t = ( dwTimeCur - dwTimeStart ) / 1000.0f; //elapsed time in seconds
	if(t -sampleStartTime > 1.0){
		//produce FPS stats every second
		sampleCount++;
		float averageFramePeriod = (t-sampleStartTime)/sampleCount;
		sampleCount = 0;
		sampleStartTime = t;


		//Write Framestats on Window title caption
		std::wostringstream captionStrm;
		int ms = (int)(1000*averageFramePeriod);
		int mss = (int)(10000*averageFramePeriod) - 10*ms;
		captionStrm << L"COMP3501 Project. FPS: " << (int)(1.0f/averageFramePeriod) << L", frame time: " << ms << L"." << mss << L"ms";
        SetWindowText( g_hWnd, captionStrm.str().c_str() ); //use FPS stats as title caption to windows window.

	}
	else{
		sampleCount++;
	}
}