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
	m_pTitleTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "title.png");
	m_pTitleTexture2 = IMG_LoadTexture(Engine::Instance().GetRenderer(), "title2.png");
	m_titleDst = {416,581,195,100};
	m_pTitleMusic = Mix_LoadMUS("aud/titleBGM.mp3");

	Mix_PlayMusic(m_pTitleMusic, -1);
}

void TitleState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_N))
	{

		STMA::ChangeState(new GameState());
	}

	if (CollisionManager::PointAABBCheck(EVMA::GetMousePos(), m_titleDst))
	{
		if(EVMA::MousePressed(1))
		{
			STMA::ChangeState(new GameState());
		}
	}
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	if (CollisionManager::PointAABBCheck(EVMA::GetMousePos(), m_titleDst))
	{
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pTitleTexture2, NULL, NULL);

	}
	else
	{
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pTitleTexture, NULL, NULL);
	}

	State::Render();
}

void TitleState::Exit()
{
	STMA::PopState();
	SDL_DestroyTexture(m_pTitleTexture);
	Mix_FreeMusic(m_pTitleMusic);
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
	player.SetTexture(IMG_LoadTexture(Engine::Instance().GetRenderer(), "Player.png"));
	m_pBGTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Background.png");
	m_pDiedTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Died.png");

	m_pBoom = Mix_LoadWAV("aud/boom.wav");
	m_pPlayerBullet = Mix_LoadWAV("aud/death1.wav");
	m_pEnemyBullet = Mix_LoadWAV("aud/laser.wav");

	m_pBGM = Mix_LoadMUS("aud/Turtles.mp3");
	m_pMusic = Mix_LoadMUS("aud/Thundercats.mp3");

	Mix_PlayMusic(m_pBGM, -1); // negative number means infinity loop

	m_bg1 = { 0, 0, 1024, 768 };
	m_bg2 = { 1024, 0, 1024, 768 };
	m_diedSrc = { 0, 0, 1050, 1050 };
	m_diedDst = { 260, 100, 500, 500 };
}

void GameState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_P))
	{
		STMA::PushState(new PauseState());
	}
	if (EVMA::KeyPressed(SDL_SCANCODE_SPACE))
	{
		m_PlayerBullets.push_back(new PlayerBullet(player.GetDst()));
		m_PlayerBullets.back()->SetTexture(IMG_LoadTexture(Engine::Instance().GetRenderer(), "Shoot1.png"));
	}

	m_bg1.x -= SPEED;
	m_bg2.x -= SPEED;

	if (m_bg1.x <= -m_bg1.w)
	{
		m_bg1.x = 0;
		m_bg2.x = 1024;
	}

	if (m_enemyCtr++ == m_enemyMax)
	{
		m_enemyCtr = 0;
		m_Enemies.push_back(new Enemy(player.GetDst()));
		m_Enemies.back()->SetTexture(IMG_LoadTexture(Engine::Instance().GetRenderer(), "Enemy.png"));
	}

	if (m_bulletCtr++ == m_bulletMax)
	{
		m_bulletCtr = 0;
		m_Bullets.push_back(new Bullet(m_Enemies.back()->GetDst()));
		m_Bullets.back()->SetTexture(IMG_LoadTexture(Engine::Instance().GetRenderer(), "Shoot2.png"));
		Mix_PlayChannel(-1, m_pEnemyBullet, 1);
	}

	if (player.GetDst().y < 688 && player.GetDst().y > 0 && player.GetDst().x < 930 && player.GetDst().x > 0)
	{
		if (EVMA::KeyHeld(SDL_SCANCODE_S))
			player.GetDst().y += SPEED;
		if (EVMA::KeyHeld(SDL_SCANCODE_W))
			player.GetDst().y -= SPEED;
		if (EVMA::KeyHeld(SDL_SCANCODE_A))
			player.GetDst().x -= SPEED;
		if (EVMA::KeyHeld(SDL_SCANCODE_D))
			player.GetDst().x += SPEED;
	}
	if (player.GetDst().x <= 0) { player.GetDst().x += 1; }
	else if (player.GetDst().x >= 900) { player.GetDst().x -= 1; }
	else if (player.GetDst().y <= 0) { player.GetDst().y += 1; }
	else if (player.GetDst().y >= 688) { player.GetDst().y -= 1; }

	// Collision for Enemy
	for (unsigned i = 0; i < m_Enemies.size(); i++)
	{
		m_Enemies[i]->Update();
		if (m_Enemies[i]->GetDst().x <= -m_Enemies[i]->GetDst().w)
		{
			delete m_Enemies[i]; 
			m_Enemies[i] = nullptr; 
			m_Enemies.erase(m_Enemies.begin() + i); 
			m_Enemies.shrink_to_fit();
			cout << "Enemy object deleted\n";
			break;
		}

		if(SDL_HasIntersection(&m_Enemies[i]->GetDst(), &player.GetDst())) 
		{
			cout << "Enemy Boom!" << endl;
			Mix_PlayChannel(-1, m_pBoom, 1);
			// Deallocate missile.
			delete m_Enemies[i]; 
			m_Enemies[i] = nullptr; 
			m_Enemies.erase(m_Enemies.begin() + i); 
			m_Enemies.shrink_to_fit();
			player.SetTint(player.GetTint() - 32);
			player.SetLife(player.GetLife() - 1);
			break;
		}
	}

	// Collision for enemies' bullets
	for (unsigned i = 0; i < m_Bullets.size(); i++)
	{
		m_Bullets[i]->Update();
		if (m_Bullets[i]->GetDst().x <= -m_Bullets[i]->GetSrc().w)
		{
			delete m_Bullets[i]; 
			m_Bullets[i] = nullptr; 
			m_Bullets.erase(m_Bullets.begin() + i); 
			m_Bullets.shrink_to_fit();
			cout << "Bullet object deleted\n";
			break;
		}

		if (SDL_HasIntersection(&m_Bullets[i]->GetDst(), &player.GetDst()))
		{
			cout << "Bullet Boom!" << endl;
			Mix_PlayChannel(-1, m_pBoom, 1);
			delete m_Bullets[i]; 
			m_Bullets[i] = nullptr; 
			m_Bullets.erase(m_Bullets.begin() + i); 
			m_Bullets.shrink_to_fit();
			player.SetTint(player.GetTint() - 32);
			player.SetLife(player.GetLife() - 1);
			break;
		}
	}

	// Collision for player's bullets
	for (unsigned i = 0; i < m_PlayerBullets.size(); i++)
	{
		m_PlayerBullets[i]->Update();
		if (m_PlayerBullets[i]->GetDst().x >= 1024)
		{
			delete m_PlayerBullets[i]; 
			m_PlayerBullets[i] = nullptr; 
			m_PlayerBullets.erase(m_PlayerBullets.begin() + i); 
			m_PlayerBullets.shrink_to_fit();
			cout << "Player's Bullet object deleted\n";
			break;
		}

		for (unsigned j = 0; j < m_Enemies.size(); j++)
		{
			if (SDL_HasIntersection(&m_PlayerBullets[i]->GetDst(), &m_Enemies[j]->GetDst())) 
			{
				cout << "Player Bullet Boom!" << endl;
				delete m_PlayerBullets[i]; 
				m_PlayerBullets[i] = nullptr; 
				m_PlayerBullets.erase(m_PlayerBullets.begin() + i); 
				m_PlayerBullets.shrink_to_fit();
				m_Enemies[j]->SetLife(m_Enemies[j]->GetLife() - 1);
				break;
			}
		}

	}

	/* Checking player's life*/
	if (player.GetLife() <= 0)
	{
		STMA::ChangeState(new LoseState());
	}
	
}

void GameState::Render()
{
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pBGTexture, NULL, &m_bg1);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pBGTexture, NULL, &m_bg2);

	player.Render();


	if (player.GetLife() <= 0)
	{
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pDiedTexture, &m_diedSrc, &m_diedDst);
	}



	for (unsigned i = 0; i < m_Bullets.size(); i++)
		m_Bullets[i]->Render();
	for (unsigned i = 0; i < m_Enemies.size(); i++)
		m_Enemies[i]->Render();
	for (unsigned i = 0; i < m_PlayerBullets.size(); i++)
		m_PlayerBullets[i]->Render();

	if (dynamic_cast<GameState*>(STMA::GetStates().back())) // If current state is GameState.
		State::Render();

	SDL_RenderPresent(Engine::Instance().GetRenderer()); // Flip buffers - send data to window.
}

void GameState::Exit()
{
	for (unsigned i = 0; i < m_Enemies.size(); i++)
	{
		delete m_Enemies[i]; 
		m_Enemies[i] = nullptr; 
	}
	m_Enemies.clear();
	m_Enemies.shrink_to_fit(); 

	for (unsigned i = 0; i < m_Bullets.size(); i++)
	{
		delete m_Bullets[i]; 
		m_Bullets[i] = nullptr; 
	}
	m_Bullets.clear(); 
	m_Bullets.shrink_to_fit(); 

	for (unsigned i = 0; i < m_PlayerBullets.size(); i++)
	{
		delete m_PlayerBullets[i]; 
		m_PlayerBullets[i] = nullptr; 
	}
	m_PlayerBullets.clear(); 
	m_PlayerBullets.shrink_to_fit();

	SDL_DestroyTexture(m_pBGTexture);
	SDL_DestroyTexture(m_pBulletsTexture1);
	SDL_DestroyTexture(m_pDiedTexture);

	Mix_FreeChunk(m_pBoom);
	Mix_FreeChunk(m_pPlayerBullet);
	Mix_FreeChunk(m_pEnemyBullet);
	Mix_FreeMusic(m_pBGM);
	Mix_FreeMusic(m_pMusic);
	
}

void GameState::Resume()
{

}

LoseState::LoseState() { }

void LoseState::Enter()
{
	m_pLoseTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "lose.png");
	m_pLoseTexture2 = IMG_LoadTexture(Engine::Instance().GetRenderer(), "lose2.png");
	m_pLoseMusic = Mix_LoadMUS("aud/loseBGM.mp3");
	m_loseDst = { 416,581,195,100 };
	Mix_PlayMusic(m_pLoseMusic, -1);
}

void LoseState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_R))
	{
		STMA::ChangeState(new TitleState());
	}

	if (CollisionManager::PointAABBCheck(EVMA::GetMousePos(), m_loseDst))
	{
		if (EVMA::MousePressed(1))
		{
			STMA::ChangeState(new TitleState());
		}
	}
}

void LoseState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 150, 150, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	if (CollisionManager::PointAABBCheck(EVMA::GetMousePos(), m_loseDst))
	{
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pLoseTexture2, NULL, NULL);

	}
	else
	{
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pLoseTexture, NULL, NULL);
	}
	State::Render();
}

void LoseState::Exit()
{
	SDL_DestroyTexture(m_pLoseTexture);
	Mix_FreeMusic(m_pLoseMusic);
}
