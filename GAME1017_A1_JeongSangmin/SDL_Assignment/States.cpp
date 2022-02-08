#include <iostream>
#include "States.h"
#include "StateManager.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "Engine.h"
#include <string>

#include "CollisionManager.h"

using namespace std;

void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

TitleState::TitleState() {}

void TitleState::Enter()
{

}

void TitleState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_N))
	{

		STMA::ChangeState(new GameState());
	}
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	State::Render();
}

void TitleState::Exit()
{

}

PauseState::PauseState() {}

void PauseState::Enter()
{

}

void PauseState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_R))
		STMA::PopState();
}

void PauseState::Render()
{
	// First render the GameState.
	STMA::GetStates().front()->Render();
	// Now render the rest of PauseState.
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 255, 128);
	SDL_Rect rect = { 255, 128, 512, 512 };
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &rect);
	State::Render();
}

void PauseState::Exit()
{

}

GameState::GameState() :m_spawnCtr(0) {}

void GameState::Enter()
{

}

void GameState::Update()
{

}

void GameState::Render()
{
	collisionUp = { -10,-20,WIDTH + 20,20 };
	collisionDown = { -10,HEIGHT + 50,WIDTH + 20,20 };
	collisionLeft = { -10,-20,20,HEIGHT + 20 };
	collisionRight = { WIDTH,-20,20,HEIGHT + 20 };
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderDrawRectF(Engine::Instance().GetRenderer(), &collisionUp);
	SDL_RenderDrawRectF(Engine::Instance().GetRenderer(), &collisionDown);
	SDL_RenderDrawRectF(Engine::Instance().GetRenderer(), &collisionLeft);
	SDL_RenderDrawRectF(Engine::Instance().GetRenderer(), &collisionRight);
	SDL_RenderClear(Engine::Instance().GetRenderer());

	//for (unsigned i = 0; i < m_turrets.size(); i++)
	//	m_turrets[i]->Render();
	//for (unsigned i = 0; i < s_enemies.size(); i++)
	//	s_enemies[i]->Render();
	//for (unsigned i = 0; i < s_bullets.size(); i++)
	//	s_bullets[i]->Render();

	//SDL_Rect spawnBox = { 50, 618, 100, 100 };
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 255, 255, 255);
	//SDL_RenderDrawRect(Engine::Instance().GetRenderer(), &spawnBox);

	// This code below prevents SDL_RenderPresent from running twice in one frame.
	if (dynamic_cast<GameState*>(STMA::GetStates().back())) // If current state is GameState.
		State::Render();
}

void GameState::Exit()
{


}

void GameState::Resume()
{

}

//std::vector<Bullet*> GameState::s_bullets;
//std::vector<Enemy*> GameState::s_enemies;