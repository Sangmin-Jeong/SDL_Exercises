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
	player.SetTexture(IMG_LoadTexture(Engine::Instance().GetRenderer(), "Player.png"));
	m_pBGTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Background.png");
	m_pEnemyTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Enemy.png");
	m_pBulletsTexture1 = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Shoot2.png");
	m_pDiedTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Died.png");

	m_pBoom = Mix_LoadWAV("aud/boom.wav");
	m_pPlayerBullet = Mix_LoadWAV("aud/death1.wav");
	m_pEnemyBullet = Mix_LoadWAV("aud/laser.wav");

	m_pBGM = Mix_LoadMUS("aud/Turtles.mp3");
	m_pMusic = Mix_LoadMUS("aud/Thundercats.mp3");

	Mix_PlayMusic(m_pBGM, -1); // negative number means infinity loop
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
	

	randomX = rand() % 1024 + 1024; // 1024+1024  for off-screen
	randomY = rand() % 669; // 669 
	int positionX = randomX + 20;
	int positionY = randomY + 50;

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
		/*m_Enemies.push_back(new Enemy({ rand() % 1024 + 1024, rand() % 669 }));*/
		m_Enemies.push_back(new Enemy(randomX, randomY, 2));
		//cout << m_Enemies.size() << endl;
	}

	if (m_bulletCtr++ == m_bulletMax)
	{
		m_bulletCtr = 0;
		m_Bullets.push_back(new Bullet(positionX, positionY));
		Mix_PlayChannel(-1, m_pEnemyBullet, 1);
		cout << m_Bullets.size() << endl;
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

	for (unsigned i = 0; i < m_Enemies.size(); i++)
	{
		m_Enemies[i]->Update();
		if (m_Enemies[i]->m_enemyDst.x <= -m_Enemies[i]->m_enemyDst.w)
		{
			delete m_Enemies[i]; // Deallocates Missile through pointer.
			m_Enemies[i] = nullptr; // Ensures no dangling pointer.
			m_Enemies.erase(m_Enemies.begin() + i); // Erase element and resize array.
			m_Enemies.shrink_to_fit();
			cout << "Enemy object deleted\n";
			break;
		}
	}

	for (unsigned i = 0; i < m_Bullets.size(); i++)
	{
		m_Bullets[i]->Update();
		if (m_Bullets[i]->m_bulletDst.x <= -m_Bullets[i]->m_bulletSrc.w) // m_Bullets[i]->m_bulletSrc.w
		{
			delete m_Bullets[i]; // Deallocates Missile through pointer.
			m_Bullets[i] = nullptr; // Ensures no dangling pointer.
			m_Bullets.erase(m_Bullets.begin() + i); // Erase element and resize array.
			m_Bullets.shrink_to_fit();
			cout << "Bullet object deleted\n";
			break;
		}
	}

	for (unsigned i = 0; i < m_PlayerBullets.size(); i++)
	{
		m_PlayerBullets[i]->Update();
		if (m_PlayerBullets[i]->GetDst().x >= 1024) // m_Bullets[i]->m_bulletSrc.w
		{
			delete m_PlayerBullets[i]; // Deallocates Missile through pointer.
			m_PlayerBullets[i] = nullptr; // Ensures no dangling pointer.
			m_PlayerBullets.erase(m_PlayerBullets.begin() + i); // Erase element and resize array.
			m_PlayerBullets.shrink_to_fit();
			cout << "Player's Bullet object deleted\n";
			break;
		}
	}

	for (unsigned i = 0; i < m_Enemies.size(); i++) // For each missile.
	{
		// For each enemy. Well we only have one currently.
		// eg: for (unsigned j = 0; j < g_enemies.size(); j++) where g_enemies is your enemy vector
		if (SDL_HasIntersection(&m_Enemies[i]->m_enemyDst, &m_dst)) // Collision check. AABB.
		{
			cout << "Enemy Boom!" << endl;
			Mix_PlayChannel(-1, m_pBoom, 1);
			// Deallocate missile.
			delete m_Enemies[i]; // Deallocates Missile through pointer.
			m_Enemies[i] = nullptr; // Ensures no dangling pointer.
			m_Enemies.erase(m_Enemies.begin() + i); // Erase element and resize array.
			m_Enemies.shrink_to_fit();
			player.SetTint(player.GetTint() - 32);
			player.SetLife(player.GetLife() - 1);
			break;
		}

		for (unsigned i = 0; i < m_PlayerBullets.size(); i++) // For each missile.
		{
			if (SDL_HasIntersection(&m_PlayerBullets[i]->GetDst(), &m_Enemies[0]->m_enemyDst)) // Collision check. AABB.
			{
				cout << "Player Bullet Boom!" << endl;
				delete m_PlayerBullets[i]; // Deallocates Missile through pointer.
				m_PlayerBullets[i] = nullptr; // Ensures no dangling pointer.
				m_PlayerBullets.erase(m_PlayerBullets.begin() + i); // Erase element and resize array.
				m_PlayerBullets.shrink_to_fit();
				m_Enemies[i]->m_enemyLife--;
				break;
			}
		}
	}

	for (unsigned i = 0; i < m_Bullets.size(); i++) // For each missile.
	{
		// For each enemy. Well we only have one currently.
		// eg: for (unsigned j = 0; j < g_enemies.size(); j++) where g_enemies is your enemy vector
		if (SDL_HasIntersection(&m_Bullets[i]->m_bulletDst, &m_dst)) // Collision check. AABB.
		{
			cout << "Bullet Boom!" << endl;
			Mix_PlayChannel(-1, m_pBoom, 1);
			// Deallocate missile.
			delete m_Bullets[i]; // Deallocates Missile through pointer.
			m_Bullets[i] = nullptr; // Ensures no dangling pointer.
			m_Bullets.erase(m_Bullets.begin() + i); // Erase element and resize array.
			m_Bullets.shrink_to_fit();
			player.SetTint(player.GetTint() - 32);
			player.SetLife(player.GetLife() - 1);
			break;
		}
	}
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

	m_bg1 = { 0, 0, 1024, 768 };
	m_bg2 = { 1024, 0, 1024, 768 };
	m_diedSrc = { 0, 0, 1050, 1050 };
	m_diedDst = { 260, 100, 500, 500 };

	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	//SDL_RenderClear(Engine::Instance().GetRenderer());

	// Any drawing goes here...
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pBGTexture, NULL, &m_bg1);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pBGTexture, NULL, &m_bg2);

	player.Render();


	for (unsigned i = 0; i < m_Enemies.size(); i++)
	{
		if (m_Enemies[i]->m_enemyLife > 0)
		{
			SDL_RenderCopyEx(Engine::Instance().GetRenderer(), m_pEnemyTexture, &(m_Enemies[i]->m_enemySrc), &(m_Enemies[i]->m_enemyDst), NULL, NULL, SDL_FLIP_HORIZONTAL);
		}
	}


	for (unsigned i = 0; i < m_Bullets.size(); i++)
	{
		SDL_RenderCopyEx(Engine::Instance().GetRenderer(), m_pBulletsTexture1, &(m_Bullets[i]->m_bulletSrc), &(m_Bullets[i]->m_bulletDst), NULL, NULL, SDL_FLIP_HORIZONTAL);
	}

	if (player.GetLife() <= 0)
	{
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pDiedTexture, &m_diedSrc, &m_diedDst);
	}



	//for (unsigned i = 0; i < m_turrets.size(); i++)
	//	m_turrets[i]->Render();
	//for (unsigned i = 0; i < s_enemies.size(); i++)
	//	s_enemies[i]->Render();
	for (unsigned i = 0; i < m_PlayerBullets.size(); i++)
		m_PlayerBullets[i]->Render();

	//SDL_Rect spawnBox = { 50, 618, 100, 100 };
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 255, 255, 255);
	//SDL_RenderDrawRect(Engine::Instance().GetRenderer(), &spawnBox);

	// This code below prevents SDL_RenderPresent from running twice in one frame.
	if (dynamic_cast<GameState*>(STMA::GetStates().back())) // If current state is GameState.
		State::Render();

	SDL_RenderPresent(Engine::Instance().GetRenderer()); // Flip buffers - send data to window.
}

void GameState::Exit()
{
	for (unsigned i = 0; i < m_Enemies.size(); i++)
	{
		delete m_Enemies[i]; // Deallocates Missile through pointer.
		m_Enemies[i] = nullptr; // Ensures no dangling pointer.
	}
	m_Enemies.clear(); // Removes all elements. Size = 0.
	m_Enemies.shrink_to_fit(); // Sets capacity to size.

	for (unsigned i = 0; i < m_Bullets.size(); i++)
	{
		delete m_Bullets[i]; // Deallocates Missile through pointer.
		m_Bullets[i] = nullptr; // Ensures no dangling pointer.
	}
	m_Bullets.clear(); // Removes all elements. Size = 0.
	m_Bullets.shrink_to_fit(); // Sets capacity to size.

	for (unsigned i = 0; i < m_PlayerBullets.size(); i++)
	{
		delete m_PlayerBullets[i]; // Deallocates Missile through pointer.
		m_PlayerBullets[i] = nullptr; // Ensures no dangling pointer.
	}
	m_PlayerBullets.clear(); // Removes all elements. Size = 0.
	m_PlayerBullets.shrink_to_fit();

	//SDL_DestroyTexture(player.GetTexture());
	SDL_DestroyTexture(m_pBGTexture);
	SDL_DestroyTexture(m_pEnemyTexture);
	SDL_DestroyTexture(m_pBulletsTexture1);
	//SDL_DestroyTexture(m_PlayerBullets.);
	SDL_DestroyTexture(m_pDiedTexture);

	Mix_FreeChunk(m_pBoom);
	Mix_FreeChunk(m_pPlayerBullet);
	Mix_FreeChunk(m_pEnemyBullet);
	Mix_FreeMusic(m_pBGM);
	Mix_FreeMusic(m_pMusic);

	Mix_CloseAudio();
	Mix_Quit();

	IMG_Quit();
	SDL_Quit();

}

void GameState::Resume()
{

}

//std::vector<Bullet*> GameState::s_bullets;
//std::vector<Enemy*> GameState::s_enemies;