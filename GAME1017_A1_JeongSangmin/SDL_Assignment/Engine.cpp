#include "Engine.h"

#include "EventManager.h"
#include "StateManager.h"
#include "TextureManager.h"

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
				EVMA::Init();
				/*TEMA::Init();*/
				if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
				{

				}
				else return false;

				if (Mix_Init(MIX_INIT_MP3) != 0)
				{
					// Configure mixer
					Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096); //If any lag in playing sounds, lower to 2048.
					Mix_AllocateChannels(16);
					// Load sounds

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

	//m_Enemies.reserve(7); //capacity = 7 , size = 0
	//m_Bullets.reserve(7); //capacity = 7 , size = 0
	//m_PlayerBullets.reserve(7);


	//cout << m_Enemies.size() << endl;
	//cout << m_Bullets.size() << endl;
	//cout << m_PlayerBullets.size() << endl;

	



	Mix_VolumeMusic(16); // 0~128  , single channel
	Mix_Volume(-1, 64); // All sfx


	STMA::ChangeState(new TitleState());
	cout << "Initialization successful!" << endl;
	m_running = true;
	return true;
}

void Engine::HandleEvents()
{
	EVMA::HandleEvents();
}

// Update function. Moves objects, performs physics, e.g. projectiles, gravity, collisions.
void Engine::Update()
{
	cout << "Updating game..." << endl;
	STMA::Update();
}

Engine& Engine::Instance()
{
	static Engine instance;
	return instance;
}

// Render function. Renders changes in game objects to window.
void Engine::Render()
{
	cout << "Rendering changes to window..." << endl;
	STMA::Render();
}

// Clean function. De-initialize SDL.
void Engine::Clean()
{
	cout << "Cleaning up..." << endl;
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	EVMA::Quit();
	STMA::Quit();
	TEMA::Quit();
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

// Run function. Contains the game loop.
int Engine::Run()
{
	if (m_running == true) // If engine is already running.
	{
		return 1;
	}
	if (Init("GAME1017_A1_JeongSangmin", SDL_WINDOWPOS_CENTERED,
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
