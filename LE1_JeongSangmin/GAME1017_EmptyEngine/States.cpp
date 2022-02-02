#include "States.h"
#include "StateManager.h"
#include "Engine.h"
#include "SoundManager.h"
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
	//m_mus = Mix_LoadMUS("Assets/audio/boss.mp3");
	//m_mMus.emplace("boss", m_mus);
	//Mix_PlayMusic(m_mMus["boss"], -1);
	SoundManager::Load("Assets/audio/boss.mp3", "boss", SOUND_MUSIC);
	SoundManager::PlayMusic("boss", -1, 0);
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
	SoundManager::StopMusic(0);
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
	//m_sfx1 = Mix_LoadWAV("Assets/audio/buff.wav");
	//m_mSfx.emplace("buff", m_sfx1);
	SoundManager::Load("Assets/audio/coffee.mp3", "coffee", SOUND_MUSIC);
	SoundManager::PlayMusic("coffee", -1, 0);
}

void GameState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_P))
	{
		cout << "Changing to PauseState" << endl;
		// pause the music track.
		SoundManager::PauseMusic();
		STMA::PushState(new PauseState());
	}

	if (Engine::Instance().KeyDown(SDL_SCANCODE_X))
	{
		cout << "Changing to EndState" << endl;
		STMA::ChangeState(new EndState());
	}

	if (Engine::Instance().KeyDown(SDL_SCANCODE_1))
	{
		cout << "Buff Sound" << endl;
		SOMA::Load("Assets/audio/buff.wav", "buff", SOUND_SFX);
		SOMA::PlaySound("buff", 0, -1);
		/*Mix_PlayChannel(-1, m_mSfx["buff"], 0);*/
		SDL_Delay(500);
	}

	if (Engine::Instance().KeyDown(SDL_SCANCODE_2))
	{
		cout << "Reload Sound" << endl;
		SOMA::Load("Assets/audio/reload.wav", "reload", SOUND_SFX);
		SOMA::PlaySound("reload", 0, -1);
		SDL_Delay(500);
	}
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
	SoundManager::StopMusic(0);

}

void GameState::Resume()
{
	cout << "resuming GameState..." << endl;
	// Resume music track.
	SoundManager::ResumeMusic();
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
