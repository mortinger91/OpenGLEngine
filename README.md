# OpenGLEngine
![Compile using MSVC and g++](https://github.com/mortinger91/OpenGLEngine/actions/workflows/cmake.yml/badge.svg)<br/>
#### Work in progress 3D engine.

### Build instructions using CMAKE:
1 - git clone https://github.com/mortinger91/OpenGLEngine.git --recursive<br/>
2 - cd OpenGLEngine<br/>
#### Linux:<br/>
>3 - ./scripts/build_linux.sh<br/>
>4 - Open OpenGLEngine folder with VS Code.<br/>
#### Windows:<br/>
>3 - mkdir build<br/>
>4 - cd build<br/>
>5 - cmake ..<br/>
>6 - Open OpenGLEngine/build/OpenGLEngine.sln with Visual Studio, set "exec" as startup project.<br/>

### Update instructions:
1 - git pull origin master<br/>
2 - git submodule update --recursive<br/>