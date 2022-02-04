#pragma once
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <SDL.h>

class Enemy
{
private:
	SDL_Rect m_src, m_dst;
	int m_speed;
	int m_hits; // Maybe after 4 hits, the enemy is destroyed.
	SDL_FRect m_Fdst;

public:
	Enemy(SDL_Rect dst);
	void Update();
	void Render();
	SDL_Point GetPos() const { return {m_dst.x + m_dst.w/2, m_dst.y + m_dst.h/2}; }
	void setHits(int);
	int getHits();
	SDL_Rect GetDst() const { return { m_dst.x, m_dst.y, m_dst.w, m_dst.h }; }
	SDL_FRect GetFDst() const;
	void setCurrentDst();
	SDL_Rect getCurrentDst();
	int& Hits() { return m_hits; } // Getter and setter.
	int m_tint;
	
};

#endif