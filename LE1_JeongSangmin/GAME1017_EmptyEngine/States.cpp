#include "States.h"
#include "StateManager.h"
#include "Engine.h"
#include <iostream>
using namespace std;

void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

TitleState::TitleState() {}


void TitleState::Enter()
{
	cout << "Entering TitleState..." << endl;
	// Load music track, add it to map, and play it.
}

void TitleState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_N))
	{
		cout << "Changing to GameState" << endl;
		STMA::ChangeState(new GameState());
	}
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0,0,255,255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	State::Render();
}

void TitleState::Exit()
{
	cout << "Exiting TitleState..." << endl;
	// call Mix_FreeMusic on your music track

}

PauseState::PauseState() {}


void PauseState::Enter()
{
	cout << "Entering PauseState..." << endl;
}

void PauseState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_R))
	{
		STMA::PopState();
	}
}

void PauseState::Render()
{
	// First render the GameState.
	STMA::GetStates().front()->Render();

	// Now render the rest of PauseState.
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 0, 0, 128);
	SDL_Rect rect = { 255, 128, 512, 512 };
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &rect);
	State::Render();
}

void PauseState::Exit()
{
	cout << "Exiting GameState..." << endl;
}

GameState::GameState() {}


void GameState::Enter()
{
	cout << "Entering GameState..." << endl;
	// Load music sfx, add them to map.
	// Load music track, add it to map, and play it.
}

void GameState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_P))
	{
		cout << "Changing to PauseState" << endl;
		// pause the music track.
		STMA::PushState(new PauseState());
	}

	if (Engine::Instance().KeyDown(SDL_SCANCODE_X))
	{
		cout << "Changing to EndState" << endl;
		STMA::ChangeState(new EndState());
	}
	// Pause 'X' key and changeState to new End State.
	// Parse 1 key and play first sfx.
	// parse 2 key and play second sfx.

	// look at the Audio Manager in previous Engine

}

void GameState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	// This code below prevents SDL_RenderPresent from running twice in one frame.
	if (dynamic_cast<GameState*>(STMA::GetStates().back())) // = If current state is GameState.
	{
		State::Render();
	}
}

void GameState::Exit()
{
	cout << "Exiting GameState..." << endl;
}

void GameState::Resume()
{
	cout << "resuming GameState..." << endl;
	// Resume music track.
}

EndState::EndState() {}


void EndState::Enter()
{
}

void EndState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_R))
	{
		cout << "Changing to TitleState" << endl;
		STMA::ChangeState(new TitleState());
	}
}

void EndState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 120, 50, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	// This code below prevents SDL_RenderPresent from running twice in one frame.
	State::Render();
}

void EndState::Exit()
{
}
