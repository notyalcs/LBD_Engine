/*

	Sample Game for testing the engine.

	Authors:
		Sean Clayton
		Mo Baydoun
		Joshua Krauchi

*/

namespace lbd {
	
	__declspec(dllimport) void Hello();

}

int main() {

	lbd::Hello();

	return 0;
}
