#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <ctime>
#include <vector>
#include "Enemy.h"
#include "Bullet.h"
#include "PlayerBullet.h"

#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
#define SPEED 2

using namespace std;

class Engine
{
private:

	bool m_running = false;
	bool m_fire = false;
	Uint32 m_start, m_end, m_delta, m_fps;
	const Uint8* m_keystates;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	SDL_Texture* m_pPlayerTexture;  // Every source image needs a SDL_Texture*
	SDL_Texture* m_pBGTexture;
	SDL_Texture* m_pEnemyTexture;
	SDL_Texture* m_pBulletsTexture1;
	SDL_Texture* m_pPlayerBulletTexture;
	SDL_Texture* m_pDiedTexture;

private:
	int Init(const char* title, int xPos, int yPos, int width, int height, int flags);
	void Clean();
	void Wake();
	void HandleEvents();
	void Render();
	void Sleep();
	bool KeyDown(SDL_Scancode c);
	void Update();
	Engine()
	{
		cout << "Creating instance of Engine..." << endl;
	}

public:
	static Engine& Instance();
	int Run();

	SDL_Rect m_player;
	SDL_Rect m_dst;
	SDL_Rect m_bg1, m_bg2;	
	SDL_Rect m_diedSrc;
	SDL_Rect m_diedDst;

	vector<Bullet*> m_Bullets;
	vector<Enemy*> m_Enemies;
	vector<PlayerBullet*> m_PlayerBullets;
	//vector<SDL_Rect*> m_spawn;

	Enemy en;
	Bullet bul;

	int randomX, randomY;
	int m_enemyCtr = 0;
	int m_enemyMax = 300; // 180 frames = 3 seconds.

	int m_bulletCtr = 0;
	int m_bulletMax = 300;

	//int m_playerBulletCtr = 0;
	//int m_playerBulletMax = 300;

	int m_tint = 255;

	Mix_Chunk* m_pBoom;
	Mix_Chunk* m_pPlayerBullet;
	Mix_Chunk* m_pEnemyBullet;

	Mix_Music* m_pMusic;
	Mix_Music* m_pBGM;

	int m_life;
};
#endif
