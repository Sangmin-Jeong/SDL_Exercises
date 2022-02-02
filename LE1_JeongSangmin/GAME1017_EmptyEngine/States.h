#pragma once
#ifndef _STATES_H_
#define _STATES_H_
#include "SoundManager.h"


class State
{
protected: // Private but inherited.
	State() = default;

public: // Public Methods.
	virtual void Enter() = 0; // = 0 means pure virtual - must be define in subclass.
	virtual void Update() = 0;
	virtual void Render();
	virtual void Exit() = 0;
	virtual void Resume() {};

private:
	
};

class TitleState : public State
{
private:
	// map for music track goes here.
	//Mix_Music* m_mus;
	//std::map<std::string, Mix_Music*> m_mMus;

public:
	TitleState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};

class PauseState : public State
{
private:


public:
	PauseState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};

class GameState : public State
{
private:
	// map for music track goes here.
	// map for sfx goes here.
	//Mix_Chunk* m_sfx1;
	//Mix_Chunk* m_sfx2;
	//std::map<std::string, Mix_Chunk*> m_mSfx;

public:
	GameState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	virtual void Resume();
};

// Add EndState here just like the others.
class EndState : public State
{
private:
	// map for music track goes here.
	// map for sfx goes here.


public:
	EndState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};



#endif