# OpenGLEngine
![Compile using MSVC and g++](https://github.com/mortinger91/OpenGLEngine/actions/workflows/cmake.yml/badge.svg)<br/>
An OpenGL 3D Engine, made with C++, that includes external libraries such as:

- imgui: UI
- assimp: 3D model loading

## Build instructions using CMAKE:
1 - clone

2 - cd OpenGLEngine

3- `git submodule update --init --recursive`

### Linux:

4 - Install these packages (on Debian): ```sudo apt install libxi-dev libxcursor-dev libxinerama-dev libxrandr-dev libxss-dev libgl1-mesa-dev```

5 - ```./scripts/build_linux.sh```

6 - Open OpenGLEngine folder with VS Code.

7 - Build using DEBUG or RELEASE presets.

### Windows:<br/>
4 - ```mkdir build```

5 - ```cd build```

6 - ```cmake ..```

7 - copy "imgui.ini" from root to build folder

8 - Open OpenGLEngine/build/OpenGLEngine.sln with Visual Studio, set "exec" as startup project.

## Update instructions:
1 - ```git pull origin master```

2 - ```git submodule update --recursive```
