#include "Chip8.h"
#include "Graphics.h"
#include "Input.h"

#include <boost/program_options.hpp>
#include <iostream>

int windowScale = 10;
float cycleDelay = 8.0f;
std::string filePath;

namespace po = boost::program_options;

int handleArguments(int argc, char* argv[]) {
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "Print this help message")
		("file-path,f", po::value<std::string>(), "Path to input file")
		("cycle-delay,c", po::value<int>(), "Set the cycle delay, default = 8(ms)")
		("window-scale,w", po::value<int>(), "Set the window scale, default = 10");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help") || argc == 0)
	{
		std::cout << std::endl;
		std::cout << "Usage: .\\Chip8Emulator.exe [-h | -f <file-path> | -c <cycle-delay> | -w <window-scale>]" << std::endl;
		std::cout << std::endl;
		std::cout << desc << std::endl;
		return 0;
	}

	if (vm.count("file-path"))
	{
		filePath = vm["file-path"].as<std::string>();
	}
	else
	{
		std::cerr << std::endl << "File path was not specified. Try \".\\Chip8Emulator.exe -h\" for more info." << std::endl;
		std::cerr << std::endl;
		return 0;
	}

	if (vm.count("cycle-delay"))
	{
		cycleDelay = vm["cycle-delay"].as<int>();
	}
	
	if (vm.count("window-scale"))
	{
		windowScale = vm["window-scale"].as<int>();
	}
	return 1;
}

int main(int argc, char* argv[]) {

	if (handleArguments(argc, argv) == 0) {
		return 1;
	}

	Chip8 chip8;
	try {
		chip8.LoadROM(filePath);
	}
	catch (std::runtime_error& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		std::cerr << "Please verify the file path specified." << std::endl;
		return 1;
	}

	initialize((VIDEO_WIDTH * windowScale), (VIDEO_HEIGHT * windowScale), VIDEO_WIDTH, VIDEO_HEIGHT);

	int videoPitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;

	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	bool quit = false;

	while (!quit)
	{
		quit = inputProcessing(chip8.keypad);

		auto currentCycleTime = std::chrono::high_resolution_clock::now();
		float delta = std::chrono::duration<float, std::chrono::milliseconds::period>(currentCycleTime - lastCycleTime).count();
		if (delta > cycleDelay)
		{
			lastCycleTime = currentCycleTime;
			chip8.Cycle();
			if (screenUpdate)
			{
				update(chip8.video, videoPitch);
				screenUpdate = false;
			}
		}
	}
	shutdown();
	return 0;
}