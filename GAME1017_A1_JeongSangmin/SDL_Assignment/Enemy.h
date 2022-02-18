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
	SDL_Rect m_dst;
	SDL_Rect m_src;

	SDL_Texture* m_pEnemyTexture;

	int m_life;
	int m_tint;

public:
	Enemy(SDL_Rect d);
	void Update();
	void Render();
	SDL_Rect& GetDst() { return m_dst; }
	SDL_Rect& GetSrc() { return m_src; }
	SDL_Texture* GetTexture() const { return m_pEnemyTexture; }
	int GetLife() { return m_life; }
	int GetTint() { return m_tint; }

	void SetLife(int l) { m_life = l; }
	void SetTexture(SDL_Texture* t) { m_pEnemyTexture = t; }
	void SetDst(SDL_Rect d) { m_dst = d; }
	void SetTint(int t) { m_tint = t; }
};
#endif

