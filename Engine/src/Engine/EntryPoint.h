#pragma once

#include <iostream>

extern lbd::Application* lbd::CreateGame();

int main(int argc, char** argv) {
	std::cout << "Start your engines!" << std::endl;

	auto game = lbd::CreateGame();
	game->run();
	delete(game);

	return 0;
}
