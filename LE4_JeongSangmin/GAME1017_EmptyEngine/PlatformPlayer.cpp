#include "PlatformPlayer.h"
#include "EventManager.h"
#include "TextureManager.h"
#include <cmath>

PlatformPlayer::PlatformPlayer(SDL_Rect s, SDL_FRect d): AnimatedSpriteObject(s,d),
m_state(STATE_IDLING), m_isGrounded(false), m_isFacingLeft(false),
m_maxVelX(9.0), m_maxVelY(JUMPFORCE), m_grav(GRAVITY), m_drag(0.85)
{
	m_src = { 0,0,128,128 };
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	TextureManager::Load("Img/Player.png", "player");
	// HINT! SetAnimation(?, ?, ?, ?(0 or 256 or 512, using which sprites )); Initialize jump animation
	SetAnimation(12, 0, 0, 512);
}

void PlatformPlayer::Update()
{
	switch (m_state) // Inside each case is the behaviour in and transitions from that state
	{
	case STATE_IDLING:
		// Transition to run
		if (EVMA::KeyPressed(SDL_SCANCODE_A))
		{
			m_state = STATE_RUNNING;
			SetAnimation(12, 1, 8, 512);
		}
		else if (EVMA::KeyPressed(SDL_SCANCODE_D))
		{
			m_state = STATE_RUNNING;
			SetAnimation(12, 1, 8, 512);
		}
		// Transition to jump
		else if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_isGrounded)
		{
			m_accelY = -JUMPFORCE;
			m_isGrounded = false;
			m_state = STATE_JUMPING;
			SetAnimation(12, 8, 9, 512);
		}
		break;
	case STATE_RUNNING:
		// Move on ground
		if (EVMA::KeyHeld(SDL_SCANCODE_A) && m_dst.x > 40)
		{
			m_accelX = -1.5;
			if (!m_isFacingLeft)
			{
				m_isFacingLeft = true;
				SetAnimation(12, 1, 9, 512);
				
			}
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D) && m_dst.x < WIDTH - (m_dst.w + 40))
		{
			m_accelX = 1.5;
			if (m_isFacingLeft)
			{
				m_isFacingLeft = false;
				SetAnimation(12, 1, 9, 512);
			}
		}
		// Transition to idle
		if (!EVMA::KeyHeld(SDL_SCANCODE_A) && !EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			m_state = STATE_IDLING;
			SetAnimation(12, 0, 0, 512);
		}
		// Transition to jump
		else if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_isGrounded)
		{
			m_accelY = -JUMPFORCE;
			m_isGrounded = false;
			m_state = STATE_JUMPING;
			SetAnimation(12, 8, 9, 512);
		}
		break;
	case STATE_JUMPING:
		// Move in mid-air
		if (EVMA::KeyHeld(SDL_SCANCODE_A) && m_dst.x > 0)
		{
			m_accelX = -1.5;
			if (!m_isFacingLeft)
			{
				m_isFacingLeft = true;
			}
			
		}
		else if(EVMA::KeyHeld(SDL_SCANCODE_D) && m_dst.x < WIDTH - (m_dst.w + 40))
		{
			m_accelX = 1.5;
			if (m_isFacingLeft)
			{
				m_isFacingLeft = false;
			}
		}
		// Transition to run
		if (m_isGrounded)
		{
			m_state = STATE_RUNNING; // Doesn't matter if this is Running or Idle
			SetAnimation(12, 0, 8, 512);
			
		}
		break;
	}
	// Player movement. Universal for all states. X-axis first.
	m_velX += m_accelX; // Add acceleration to velocity
	m_velX *= (m_isGrounded ? m_drag : 1.0);
	m_velX = std::min(std::max(m_velX, -m_maxVelX), m_maxVelX);
	m_dst.x += (float)m_velX;
	// Wrap the player on-screen.

	// Y-axis now
	m_velY += m_accelY + m_grav;
	m_velY = std::min(std::max(m_velY, -m_maxVelY), m_maxVelY);
	m_dst.y += (float)m_velY;

	m_accelX = m_accelY = 0.0; // We want to move once when we press the key
	 Animate();
}

void PlatformPlayer::Render()
{
	// HINT! to animate the sprite, use SDL_RenderCopyExF() and I will have to access the TextureManager
	// and pass in a key such as "player"
	// But for this part, we are just going to use a colored square
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 0, 0, 255);
	//SDL_RenderFillRectF(Engine::Instance().GetRenderer(), &m_dst);
	SDL_RendererFlip temp;
	if (m_isFacingLeft == 0)
	{
		temp = SDL_FLIP_NONE;
	}
	else
	{
		temp = SDL_FLIP_HORIZONTAL;
	}
	SDL_RenderCopyExF(Engine::Instance().GetRenderer(), TEMA::GetTexture("player"), &m_src, &m_dst, 0, 0, temp);
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

void PlatformPlayer::Stop()
{
	StopX();
	StopY();
}

void PlatformPlayer::StopX()
{
	m_velX = 0.0;
}

void PlatformPlayer::StopY()
{
	m_velY = 0.0;
}

void PlatformPlayer::SetAccelX(double a)
{
	m_accelX = a;
}

void PlatformPlayer::SetAccelY(double a)
{
	m_accelY = a;
}

void PlatformPlayer::SetX(float x)
{
	m_dst.x = x;
}

void PlatformPlayer::SetY(float y)
{
	m_dst.y = y;
}

void PlatformPlayer::SetGrounded(bool g)
{
	m_isGrounded = g;
}

bool PlatformPlayer::IsGrounded()
{
	return m_isGrounded;
}

double PlatformPlayer::GetVelX()
{
	return m_velX;
}

double PlatformPlayer::GetVelY()
{
	return m_velY;
}
