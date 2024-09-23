#include "SDL.h"
#include "Input.h"

bool inputProcessing(uint8_t* keypad) {
	bool quit = false;

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;

		case SDL_KEYDOWN:
			handleKeyDown(keypad, event, &quit);
			break;
		case SDL_KEYUP:
			handleKeyUp(keypad, event);
			break;
		}
	}
	return quit;
}

void handleKeyDown(uint8_t* keypad, SDL_Event event, bool* quit) {
	switch (event.key.keysym.sym)
	{
	case SDLK_ESCAPE:
		*quit = true;
		break;

	case SDLK_x:
		keypad[0] = 1;
		break;

	case SDLK_1:
		keypad[1] = 1;
		break;

	case SDLK_2:
		keypad[2] = 1;
		break;

	case SDLK_3:
		keypad[3] = 1;
		break;

	case SDLK_q:
		keypad[4] = 1;
		break;

	case SDLK_w:
		keypad[5] = 1;
		break;

	case SDLK_e:
		keypad[6] = 1;
		break;

	case SDLK_a:
		keypad[7] = 1;
		break;

	case SDLK_s:
		keypad[8] = 1;
		break;

	case SDLK_d:
		keypad[9] = 1;
		break;

	case SDLK_z:
		keypad[0xA] = 1;
		break;

	case SDLK_c:
		keypad[0xB] = 1;
		break;

	case SDLK_4:
		keypad[0xC] = 1;
		break;

	case SDLK_r:
		keypad[0xD] = 1;
		break;

	case SDLK_f:
		keypad[0xE] = 1;
		break;

	case SDLK_v:
		keypad[0xF] = 1;
		break;
	}
}
void handleKeyUp(uint8_t* keypad, SDL_Event event) {
	switch (event.key.keysym.sym)
	{
	case SDLK_x:
		keypad[0] = 0;
		break;

	case SDLK_1:
		keypad[1] = 0;
		break;

	case SDLK_2:
		keypad[2] = 0;
		break;

	case SDLK_3:
		keypad[3] = 0;
		break;

	case SDLK_q:
		keypad[4] = 0;
		break;

	case SDLK_w:
		keypad[5] = 0;
		break;

	case SDLK_e:
		keypad[6] = 0;
		break;

	case SDLK_a:
		keypad[7] = 0;
		break;

	case SDLK_s:
		keypad[8] = 0;
		break;

	case SDLK_d:
		keypad[9] = 0;
		break;

	case SDLK_z:
		keypad[0xA] = 0;
		break;

	case SDLK_c:
		keypad[0xB] = 0;
		break;

	case SDLK_4:
		keypad[0xC] = 0;
		break;

	case SDLK_r:
		keypad[0xD] = 0;
		break;

	case SDLK_f:
		keypad[0xE] = 0;
		break;

	case SDLK_v:
		keypad[0xF] = 0;
		break;
	}
}