#pragma once
#ifndef _STATES_H_
#define _STATES_H_

class State
{
protected: // Private but inherited.
	State() {};

public: // Public Methods.
	virtual ~State() = default;
	virtual void Enter() = 0; // = 0 means pure virtual - must be define in subclass.
	virtual void Update() = 0;
	virtual void Render();
	virtual void Exit() = 0;
	virtual void Resume() {};

};


#endif