#pragma once
#ifndef _BOX_
#define _BOX_
#include <SDL.h>
#include <string>

using namespace std;

class Sprite  // could be renamed Obstacle for A2
{
private:
	// SDL_Rect m_src; // HINT! for A2 or use GameObject.h
	SDL_Rect m_dst;
	SDL_Rect m_src;
	SDL_Color m_color; // HINT! Don't need color for A2

	int m_pSelection;
	SDL_Texture* m_pSaw;
	SDL_Texture* m_pSpike_wall;
	SDL_Texture* m_pSpike_lg;
	SDL_Texture* m_pSpike_sm;

public:
	Sprite(); // If you have a dynamic array of Sprite*
	Sprite(int select, SDL_Rect d, SDL_Rect s);
	//void Update(); // HINT! may need for A2, e.g. spinning blade
	void Render();
	friend class Box; // Alternately I can make a setter for m_dst

};

class Box // Proxy for the Obstacle/sprite
{
private:
	SDL_Point m_pos; // Position on screen. Y is optional
	Sprite* m_pSprite; // Pointer to a dynamic Sprite object(S) // HINT! Dynamic allocate?
										// m_pSprite = new Sprite[m_numSprites];
	int m_numSprites; // HINT! For bonus of A2

public:
	Box(const SDL_Point p, bool makeSprite = false, int select = 0,
		const SDL_Rect d = {0,0,0,0}, const SDL_Rect s = { 0,0,0,0 });
	~Box();
	Box* Clone(); // We are making Box also a prototype
	void Update();
	void Render();
	const SDL_Point GetPos() { return m_pos; }
	// Sprite access
	//Sprite* GetSprite() const { return m_pSprite; }
	//void SetSprite(Sprite* s) { m_pSprite = s; }

	// Map stuff later


};

#endif

