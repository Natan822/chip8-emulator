#pragma once

bool inputProcessing(uint8_t* keypad);
void handleKeyDown(uint8_t* keypad, SDL_Event event, bool* quit);
void handleKeyUp(uint8_t* keypad, SDL_Event event);