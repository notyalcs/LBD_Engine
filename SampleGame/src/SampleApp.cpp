/*

	Sample Game for testing the engine.

	Authors:
		Sean Clayton
		Mo Baydoun
		Joshua Krauchi

*/

#include <LBD_Engine.h>

class SampleGame : public lbd::Application {
public:
	SampleGame() {

	}

	~SampleGame() {

	}
};

lbd::Application* lbd::CreateGame() {
	return new SampleGame();
}
