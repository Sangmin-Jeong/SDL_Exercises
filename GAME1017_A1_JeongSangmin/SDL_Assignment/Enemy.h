#pragma once
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <ctime>
#include <vector>

#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
#define SPEED 2

using namespace std;

class Enemy
{
private:
	

public:
	SDL_Rect m_enemySrc;
	SDL_Rect m_enemyDst;
	int m_enemyLife;
	Enemy(int = 0, int = 0, int = 0);
	void Update();
};
#endif

