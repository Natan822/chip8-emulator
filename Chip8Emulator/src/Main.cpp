#include "Chip8.h"
#include "Graphics.h"
#include "Input.h"
#include <iostream>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 320;

const float CYCLE_DELAY = 16.67F;

int main(int argc, char* argv[]) {

	if (argc != 2)
	{
		std::cerr << "Missing args" << std::endl;
		return 1;
	}

	const char* filename = argv[1];

	initialize(WINDOW_WIDTH, WINDOW_HEIGHT, VIDEO_WIDTH, VIDEO_HEIGHT);

	Chip8 chip8;
	chip8.LoadROM(filename);

	int videoPitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;

	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	bool quit = false;

	while (!quit)
	{
		quit = inputProcessing(chip8.keypad);

		auto currentCycleTime = std::chrono::high_resolution_clock::now();
		float delta = std::chrono::duration<float, std::chrono::milliseconds::period>(currentCycleTime - lastCycleTime).count();
		if (delta > CYCLE_DELAY)
		{
			lastCycleTime = currentCycleTime;
			chip8.Cycle();
			update(chip8.video, videoPitch);
		}
	}
	shutdown();
	return 0;
}