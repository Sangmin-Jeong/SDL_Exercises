#include <iostream>
#include "States.h"
#include "StateManager.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "Engine.h"
#include "tinyxml2.h"
#include <string>

#include "CollisionManager.h"

using namespace std;
using namespace tinyxml2;

void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

TitleState::TitleState(){}

void TitleState::Enter()
{
	
}

void TitleState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_N))
	{
		
		STMA::ChangeState( new GameState() );
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

PauseState::PauseState(){}

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

void GameState::ClearTurrets()
{
	for (unsigned i = 0; i < m_turrets.size(); i++)
	{
		delete m_turrets[i];
		m_turrets[i] = nullptr;
	}
	m_turrets.clear();
	m_turrets.shrink_to_fit();
}

GameState::GameState():m_spawnCtr(0) {}

void GameState::Enter()
{
	TEMA::Load("Img/Turret.png", "turret");
	TEMA::Load("Img/Enemies.png", "enemy");
	s_enemies.push_back(new Enemy({512, -200, 40, 57}));
	// //Create the DOM and load the XML file.
	xmlDoc.LoadFile("TurretPosition.xml");
	 //Iterate through the Turret elements in the file and push_back new Turrets into the m_turrets vector.
		// Look at the last two examples from Week 3
	XMLNode* p_root = xmlDoc.FirstChildElement("Root");
	XMLElement* p_element = p_root->FirstChildElement("Turret");
	while (p_element != nullptr)
	{
		int x, y;
		p_element->QueryIntAttribute("position.x", &x);
		p_element->QueryIntAttribute("position.y", &y);
		const SDL_Rect tempRect = { x, y, 100, 100 };
		Turret* temp = new Turret(tempRect);
		m_turrets.push_back(temp);
		p_element = p_element->NextSiblingElement("Turret");
	}
	
}

void GameState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_P))
	{
		STMA::PushState(new PauseState());
	}
	if (EVMA::KeyPressed(SDL_SCANCODE_T))
	{
		m_turrets.push_back(new Turret({ 50,618,100,100 }));
	}
	if (EVMA::KeyPressed(SDL_SCANCODE_C))
	{
		ClearTurrets();

	}
	if (m_spawnCtr++ % 180 == 0)
		s_enemies.push_back(new Enemy({ rand() % (1024 - 40), -57, 40, 57 }));
	for (unsigned i = 0; i < m_turrets.size(); i++)
		m_turrets[i]->Update();
	for (unsigned i = 0; i < s_enemies.size(); i++)
	{
		s_enemies[i]->Update();
		// Collision Check when enemies go outside if the screen
		if (CollisionManager::AABBCheck(s_enemies[i]->GetDst(), collisionDown))
		{
			delete s_enemies[i]; // Deallocates Missile through pointer.
			s_enemies[i] = nullptr; // Ensures no dangling pointer.
			s_enemies.erase(s_enemies.begin() + i); // Erase element and resize array.
			s_enemies.shrink_to_fit();
			cout << "Enemy object deleted (DOWN)\n";
			break;
		}
	}
		

	for (unsigned i = 0; i < s_bullets.size(); i++)
	{
		s_bullets[i]->Update();
		// Collision Check when bullets go outside
		if (CollisionManager::CircleAABBCheck(s_bullets[i]->GetPos(), 1, collisionUp))
		{
			delete s_bullets[i]; // Deallocates Missile through pointer.
			s_bullets[i] = nullptr; // Ensures no dangling pointer.
			s_bullets.erase(s_bullets.begin() + i); // Erase element and resize array.
			s_bullets.shrink_to_fit();
			cout << "Bullet object deleted (UP)\n";
			break;
		}
		if (CollisionManager::CircleAABBCheck(s_bullets[i]->GetPos(), 1, collisionDown))
		{
			delete s_bullets[i]; // Deallocates Missile through pointer.
			s_bullets[i] = nullptr; // Ensures no dangling pointer.
			s_bullets.erase(s_bullets.begin() + i); // Erase element and resize array.
			s_bullets.shrink_to_fit();
			cout << "Bullet object deleted (DOWN)\n";
			break;
		}
		if (CollisionManager::CircleAABBCheck(s_bullets[i]->GetPos(), 1, collisionLeft))
		{
			delete s_bullets[i]; // Deallocates Missile through pointer.
			s_bullets[i] = nullptr; // Ensures no dangling pointer.
			s_bullets.erase(s_bullets.begin() + i); // Erase element and resize array.
			s_bullets.shrink_to_fit();
			cout << "Bullet object deleted (LEFT)\n";
			break;
		}
		if (CollisionManager::CircleAABBCheck(s_bullets[i]->GetPos(), 1, collisionRight))
		{
			delete s_bullets[i]; // Deallocates Missile through pointer.
			s_bullets[i] = nullptr; // Ensures no dangling pointer.
			s_bullets.erase(s_bullets.begin() + i); // Erase element and resize array.
			s_bullets.shrink_to_fit();
			cout << "Bullet object deleted (RIGHT)\n";
			break;
		}
	}
		

	// Cleanup bullets and enemies that go off screen.

		// for all bullets
			// if bullet goes off screen (four bounds checks)
				// delete s_bullets[i]
				// set s_bullets[i] to nullptr
	/*m_turrets[i] = nullptr;*/
	
		// for all enemies, similar to above

	// Check for collisions with bullets and enemies.
	
		// for all bullets
			// for all enemies
				// check collision
}

void GameState::Render()
{
	collisionUp = { -10,-20,WIDTH + 20,20 };
	collisionDown = { -10,HEIGHT+50,WIDTH + 20,20 };
	collisionLeft = { -10,-20,20,HEIGHT + 20 };
	collisionRight = { WIDTH,-20,20,HEIGHT + 20 };
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderDrawRectF(Engine::Instance().GetRenderer(), &collisionUp);
	SDL_RenderDrawRectF(Engine::Instance().GetRenderer(), &collisionDown);
	SDL_RenderDrawRectF(Engine::Instance().GetRenderer(), &collisionLeft);
	SDL_RenderDrawRectF(Engine::Instance().GetRenderer(), &collisionRight);
	SDL_RenderClear(Engine::Instance().GetRenderer());

	for (unsigned i = 0; i < m_turrets.size(); i++)
		m_turrets[i]->Render();
	for (unsigned i = 0; i < s_enemies.size(); i++)
		s_enemies[i]->Render();
	for (unsigned i = 0; i < s_bullets.size(); i++)
		s_bullets[i]->Render();

	SDL_Rect spawnBox = { 50, 618, 100, 100 };
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 255, 255, 255);
	SDL_RenderDrawRect(Engine::Instance().GetRenderer(), &spawnBox);

	// This code below prevents SDL_RenderPresent from running twice in one frame.
	if ( dynamic_cast<GameState*>( STMA::GetStates().back() ) ) // If current state is GameState.
		State::Render();
}

void GameState::Exit()
{
	// You can clear all children of the root node by calling .DeleteChildren(); and this will essentially clear the DOM.
	// Iterate through all the turrets and save their positions as child elements of the root node in the DOM.
	// Make sure to save to the XML file.
	xmlDoc.DeleteChildren();
	XMLNode* p_root = xmlDoc.NewElement("Root");
	xmlDoc.InsertEndChild(p_root);
	for (unsigned i = 0; i < m_turrets.size(); i++)
	{
		XMLElement* p_element = xmlDoc.NewElement("Turret");
		p_element->SetAttribute("TurretCount", i);
		p_element->SetAttribute("position.x", m_turrets[i]->GetPos().x);
		p_element->SetAttribute("position.y", m_turrets[i]->GetPos().y);
		p_root->InsertEndChild(p_element);

		delete m_turrets[i];
		m_turrets[i] = nullptr;
	}
	xmlDoc.SaveFile("TurretPosition.xml");
	m_turrets.clear();
	m_turrets.shrink_to_fit();
	ClearTurrets();
	for (unsigned i = 0; i < s_enemies.size(); i++)
	{
		delete s_enemies[i];
		s_enemies[i] = nullptr;
	}
	s_enemies.clear();
	s_enemies.shrink_to_fit();
	for (unsigned i = 0; i < s_bullets.size(); i++)
	{
		delete s_bullets[i];
		s_bullets[i] = nullptr;
	}
	s_bullets.clear();
	s_bullets.shrink_to_fit();

}

void GameState::Resume()
{
	
}

// This is how static properties are allocated.
std::vector<Bullet*> GameState::s_bullets;
std::vector<Enemy*> GameState::s_enemies;