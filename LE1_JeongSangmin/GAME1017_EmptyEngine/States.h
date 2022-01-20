#pragma once
#ifndef _STATES_H_
#define _STATES_H_

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

};

class TitleState : public State
{
private:
	// map for music track goes here.


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