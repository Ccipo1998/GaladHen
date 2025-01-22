# GaladHen
![immagine](https://github.com/user-attachments/assets/8b4ceb36-41fe-4ca3-bd76-98a721569af7)

From Sindarin _**Galad-Hen**_ (Glittering Eye)

# Build
## Preliminary steps
Before building, a change to the include directive inside used *imgui*'s backend files is needed:
  1. Libs/imgui/imgui/backends/imgui_impl_glfw.cpp
  2. Libs/imgui/imgui/backends/imgui_impl_opengl3.cpp
  3. Libs/imgui/imgui/backends/imgui_impl_win32.cpp
  
Change:
```#include "imgui.h"```
to:
```#include <imgui/imgui.h>```

>[!NOTE]
>These steps are needed because the original *imgui* repository uses local path to include *imgui.h* inside backend files, and i'm lazy so i don't want to include the entire *imgui* repo to **GaladHen** for just a change nor to create an imgui lib folder with only needed files (i like submodules)

>[!NOTE]
>All file paths in GaladHen code are relative to repo directory, so be sure that your working directory is set to repo folder (cmake does not provide a method to set the working directory on all platforms and with any IDE)

## Build commands
GaladHen uses CMake as building toolchain.
Via command line:
  1. Navigate to project folder
  2. ```cmake -S . -B build```
  3. ```cmake --build build```
