#pragma once
#ifndef _STATES_H_
#define _STATES_H_
#include <SDL_mixer.h>

#include "Enemy.h"
#include <vector>
#include <string>

#include "Bullet.h"
#include "Player.h"
#include "PlayerBullet.h"

using namespace std;


class State 
{
protected: // Private but inherited.
	State() = default;
public:
	virtual void Enter() = 0; // = 0 means pure virtual. Method MUST be defined in subclass.
	virtual void Update() = 0;
	virtual void Render();
	virtual void Exit() = 0;
	virtual void Resume() {};
};

class TitleState : public State
{
private: // Private properties.
public: // Public methods.
	TitleState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};

class PauseState : public State
{
public: // Public methods.
	PauseState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};

class GameState : public State
{
private:
	//static std::vector<Enemy*> s_enemies;
	//static std::vector<Bullet*> s_bullets;
	int m_spawnCtr;

public: // Public methods.
	GameState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	virtual void Resume();
	//static std::vector<Bullet*>& Bullets() { return s_bullets; }
	/*static std::vector<Enemy*>& Enemies() { return s_enemies; }*/

	//SDL_FRect collisionUp;
	//SDL_FRect collisionDown;
	//SDL_FRect collisionLeft;
	//SDL_FRect collisionRight;

	//SDL_Texture* p_cUp;
	//SDL_Texture* p_cDown;
	//SDL_Texture* p_cLeft;
	//SDL_Texture* p_cRight;

	SDL_Texture* m_pBGTexture;
	SDL_Texture* m_pBulletsTexture1;

	SDL_Texture* m_pDiedTexture; // for end state

	SDL_Rect m_dst;
	SDL_Rect m_bg1, m_bg2;
	SDL_Rect m_diedSrc;
	SDL_Rect m_diedDst;

	vector<Bullet*> m_Bullets;
	vector<Enemy*> m_Enemies;
	vector<PlayerBullet*> m_PlayerBullets;
	Player player;

	int m_enemyCtr = 0;
	int m_enemyMax = 300; // 180 frames = 3 seconds.

	int m_bulletCtr = 0;
	int m_bulletMax = 300;

	Mix_Chunk* m_pBoom;
	Mix_Chunk* m_pPlayerBullet;
	Mix_Chunk* m_pEnemyBullet;

	Mix_Music* m_pMusic;
	Mix_Music* m_pBGM;


};
#endif
