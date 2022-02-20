#include "../../Common/Window.h"
#include "PushdownMachine.h"
#include "MainMenu.h"

using namespace NCL;
using namespace CSC8503;


int main() {

	Window*w = Window::CreateGameWindow("Screwball!", 1920, 1080, true);
	
	if (!w->HasInitialised()) {
		return -1;
	}
	
	srand(time(0));
	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);

	PushdownMachine *machine = new PushdownMachine(new MainMenu());
	
	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!

	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {
		float dt = w->GetTimer()->GetTimeDeltaSeconds();
		if (dt > 0.1f) {
			std::cout << "Skipping large time delta" << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::PRIOR)) {
			w->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NEXT)) {
			w->ShowConsole(false);
		}

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::T)) {
			w->SetWindowPosition(0, 0);
		}

		w->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));

		machine->Update(dt);
	}
	Window::DestroyGameWindow();
}