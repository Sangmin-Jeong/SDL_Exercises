#include "Engine.h"

int Engine::Init(const char* title, int xPos, int yPos, int width, int height, int flags)
{
	cout << "Initializing game..." << endl;
	srand((unsigned)time(NULL));
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) // If initialization is okay.
	{
		// Try to create the SDL_Window.
		m_pWindow = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (m_pWindow != nullptr) // Window creation passes.
		{
			// Try to create the SDL_Renderer. (Back buffer)
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (m_pRenderer != nullptr)
			{
				if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
				{
					m_pBGTexture = IMG_LoadTexture(m_pRenderer, "Background.png");
					m_pPlayerTexture = IMG_LoadTexture(m_pRenderer, "Player.png");
					m_pEnemyTexture = IMG_LoadTexture(m_pRenderer, "Enemy.png");
					m_pBulletsTexture1 = IMG_LoadTexture(m_pRenderer, "Shoot2.png");
					m_pPlayerBulletTexture = IMG_LoadTexture(m_pRenderer, "Shoot1.png");
					m_pDiedTexture = IMG_LoadTexture(m_pRenderer, "Died.png");
				}
				else return false;

				if (Mix_Init(MIX_INIT_MP3) != 0)
				{
					// Configure mixer
					Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096); //If any lag in playing sounds, lower to 2048.
					Mix_AllocateChannels(16);
					// Load sounds
					m_pBoom = Mix_LoadWAV("aud/boom.wav");
					m_pPlayerBullet = Mix_LoadWAV("aud/death1.wav");
					m_pEnemyBullet = Mix_LoadWAV("aud/laser.wav");

					m_pBGM = Mix_LoadMUS("aud/Turtles.mp3");
					m_pMusic = Mix_LoadMUS("aud/Thundercats.mp3");
				}
				else return false; // Mixer init failed.
			}
			else return false; // Renderer creation failed.
		}
		else return false; // Window creation failed.
	}
	else return false; // Initialization has failed.
	// If everything is okay, we are here...
	/*m_fps = (Uint32)round(1 / (double)FPS) * 1000;*/
	m_fps = 200 / FPS;
	m_keystates = SDL_GetKeyboardState(nullptr);
	//m_player = { WIDTH / 2, HEIGHT / 2, 35, 55 };
	m_dst = { WIDTH / 8, HEIGHT / 2, 94, 80 };

	m_Enemies.reserve(7); //capacity = 7 , size = 0
	m_Bullets.reserve(7); //capacity = 7 , size = 0
	m_PlayerBullets.reserve(7);


	cout << m_Enemies.size() << endl;
	cout << m_Bullets.size() << endl;
	cout << m_PlayerBullets.size() << endl;

	m_life = 4; // Player Life, 0 = game over

	m_bg1 = { 0, 0, 1024, 768 };
	m_bg2 = { 1024, 0, 1024, 768 };
	m_diedSrc = { 0, 0, 1050, 1050 };
	m_diedDst = { 260, 100, 500, 500 };

	Mix_VolumeMusic(16); // 0~128  , single channel
	Mix_Volume(-1, 64); // All sfx
	Mix_PlayMusic(m_pBGM, -1); // negative number means infinity loop

	cout << "Initialization successful!" << endl;
	m_running = true;
	return true;
}

void Engine::HandleEvents()
{
	//cout << "Getting input..." << endl;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: // Pressing 'X' button of window.
			m_running = false;
			break;
		case SDL_KEYDOWN: // Essential parses every frame. Just like keystates.
			if (event.key.keysym.sym == SDLK_SPACE) // Pressing spacebar.
			{
				m_PlayerBullets.push_back(new PlayerBullet((m_dst.x + 30), (m_dst.y + 30)));
				m_PlayerBullets.shrink_to_fit();
				Mix_PlayChannel(-1, m_pPlayerBullet, 0);
				cout << m_PlayerBullets.size() << endl;
				/*m_fire = true;*/
			}
			break;
		case SDL_KEYUP: // One-shot
			if (event.key.keysym.sym == ' ') // Releasing spacebar.
			{
				/*m_fire = false;*/
				/*cout << "Done pews..." << endl;*/
			}
			//else if (event.key.keysym.sym == 13) // Enter.
			//{
			//	// Fire dynamic Missile.
			//	
			//
			//}
			break;
		}
	}
}

bool Engine::KeyDown(SDL_Scancode c)
{
	if (m_keystates != nullptr)
	{
		if (m_keystates[c] == 1) // Key we're testing for is down.
			return true;
	}
	return false;
} 

// Update function. Moves objects, performs physics, e.g. projectiles, gravity, collisions.
void Engine::Update()
{
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

	if (m_dst.y < 688 && m_dst.y > 0 && m_dst.x < 930 && m_dst.x > 0)
	{
		if (KeyDown(SDL_SCANCODE_S))
			m_dst.y += SPEED;
		if (KeyDown(SDL_SCANCODE_W))
			m_dst.y -= SPEED;
		if (KeyDown(SDL_SCANCODE_A))
			m_dst.x -= SPEED;
		if (KeyDown(SDL_SCANCODE_D))
			m_dst.x += SPEED;
	}
	if (m_dst.x <= 0) { m_dst.x += 1; }
	else if (m_dst.x >= 900) { m_dst.x -= 1; }
	else if (m_dst.y <= 0) { m_dst.y += 1; }
	else if (m_dst.y >= 688) { m_dst.y -= 1; }

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
		if (m_PlayerBullets[i]->m_playerBulletDst.x >= 1024) // m_Bullets[i]->m_bulletSrc.w
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
			m_tint -= 32;
			m_life--;
			break;
		}

		for (unsigned i = 0; i < m_PlayerBullets.size(); i++) // For each missile.
		{
			if (SDL_HasIntersection(&m_PlayerBullets[i]->m_playerBulletDst, &m_Enemies[0]->m_enemyDst)) // Collision check. AABB.
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
			m_tint -= 32;
			m_life--;
			break;
		}
	}
	//cout << "Updating game..." << endl;

	//if (m_life >= 0)
	//{

	//}
}

// Render function. Renders changes in game objects to window.
void Engine::Render()
{
	//cout << "Rendering changes to window..." << endl;
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer);
	// Any drawing goes here...
	SDL_RenderCopy(m_pRenderer, m_pBGTexture, NULL, &m_bg1);
	SDL_RenderCopy(m_pRenderer, m_pBGTexture, NULL, &m_bg2);
	SDL_SetTextureColorMod(m_pPlayerTexture, 255, m_tint, m_tint);

	if (m_life > 0)
	{
		SDL_RenderCopy(m_pRenderer, m_pPlayerTexture, NULL, &m_dst);
	}

	for (unsigned i = 0; i < m_Enemies.size(); i++)
	{
		if (m_Enemies[i]->m_enemyLife > 0)
		{
			SDL_RenderCopyEx(m_pRenderer, m_pEnemyTexture, &(m_Enemies[i]->m_enemySrc), &(m_Enemies[i]->m_enemyDst), NULL, NULL, SDL_FLIP_HORIZONTAL);
		}
	}


	for (unsigned i = 0; i < m_Bullets.size(); i++)
	{
		SDL_RenderCopyEx(m_pRenderer, m_pBulletsTexture1, &(m_Bullets[i]->m_bulletSrc), &(m_Bullets[i]->m_bulletDst), NULL, NULL, SDL_FLIP_HORIZONTAL);
	}

	for (unsigned i = 0; i < m_PlayerBullets.size(); i++)
	{
		SDL_RenderCopy(m_pRenderer, m_pPlayerBulletTexture, &(m_PlayerBullets[i]->m_playerBulletSrc), &(m_PlayerBullets[i]->m_playerBulletDst));
	}

	if (m_life <= 0)
	{
		SDL_RenderCopy(m_pRenderer, m_pDiedTexture, &m_diedSrc, &m_diedDst);
	}

	SDL_RenderPresent(m_pRenderer); // Flip buffers - send data to window.
}

// Clean function. De-initialize SDL.
void Engine::Clean()
{
	cout << "Cleaning up..." << endl;
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

	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyTexture(m_pPlayerTexture);
	SDL_DestroyTexture(m_pBGTexture);
	SDL_DestroyTexture(m_pEnemyTexture);
	SDL_DestroyTexture(m_pBulletsTexture1);
	SDL_DestroyTexture(m_pPlayerBulletTexture);
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

void Engine::Wake()
{
	m_start = SDL_GetTicks(); // Gets milliseconds since SDL initialization.
}

void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start; // 1055 -1050 = 5ms
	if (m_delta < m_fps) // if (5ms < 17ms)
	{
		SDL_Delay(m_fps - m_delta); // Engine sleeps for 12ms.
		/*m_fps*/
	}
}

Engine& Engine::Instance()
{
	static Engine instance;
	return instance;
}

// Run function. Contains the game loop.
int Engine::Run()
{
	if (m_running == true) // If engine is already running.
	{
		return 1;
	}
	if (Init("Sangmin Jeong", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0) == false) // If initialization failed.
	{
		return 2;
	}
	// If initialization passes, we are here.
	while (m_running) // Primary game loop. "frame"
	{
		Wake();
		HandleEvents(); // Input.
		Update();		// Processing.
		Render();		// Output.
		if (m_running == true)
			Sleep();
	}
	Clean();
	return 0;
}
