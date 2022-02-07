#pragma once
#ifndef _STATES_H_
#define _STATES_H_
#include "Enemy.h"
#include <vector>
#include <string>

using namespace std;

class State 
{
protected: // Private but inherited.
	State() = default; // Or State() {};
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
	static vector<Enemy*> s_enemies;
	static vector<Bullet*> s_bullets;
	int m_spawnCtr;

public: // Public methods.
	GameState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	virtual void Resume();
	static vector<Bullet*>& Bullets() { return s_bullets; }
	static vector<Enemy*>& Enemies() { return s_enemies; }

	SDL_FRect collisionUp;
	SDL_FRect collisionDown;
	SDL_FRect collisionLeft;
	SDL_FRect collisionRight;

	SDL_Texture* p_cUp;
	SDL_Texture* p_cDown;
	SDL_Texture* p_cLeft;
	SDL_Texture* p_cRight;
};
#endif