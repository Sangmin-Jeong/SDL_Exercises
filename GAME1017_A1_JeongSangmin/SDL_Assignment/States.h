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
	Mix_Music* m_pTitleMusic;

	SDL_Texture* m_pTitleTexture;
	SDL_Texture* m_pTitleTexture2;

	SDL_Rect m_titleDst;

public: // Public methods.
	TitleState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};

class PauseState : public State
{
private:

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
	int m_spawnCtr;

public: // Public methods.
	GameState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	virtual void Resume();

	SDL_Texture* m_pBGTexture;
	SDL_Texture* m_pBulletsTexture1;
	SDL_Texture* m_pDiedTexture;

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

class LoseState : public State
{
private: // Private properties.
	Mix_Music* m_pLoseMusic;

	SDL_Texture* m_pLoseTexture;
	SDL_Texture* m_pLoseTexture2;

	SDL_Rect m_loseDst;
public: // Public methods.
	LoseState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};
#endif
