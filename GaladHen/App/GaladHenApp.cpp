
#include <GaladHen/Editor/Editor.h>

using namespace GaladHen;

int main()
{
    // FIRST THING FIRST, INIT SYSTEMS
    SystemsCoordinator::GetInstance()->InitSystems();

    Editor::Init();

    Editor::Run();

    // LASTLY, QUIT SYSTEMS
    SystemsCoordinator::GetInstance()->QuitSystems();

    return 0;
}
