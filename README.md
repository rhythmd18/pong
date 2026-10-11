# pong
Clone of the 1972 Pong Game built using C and [SDL3](https://libsdl.org).

## Demo


https://github.com/user-attachments/assets/b39c0f6b-8f8a-49d8-83db-c646202d09c4



## Prerequisites
- Install a C/C++ compiler of your choice (I used the preinstalled Apple Clang. Any compiler works).
- Install [CMake](https://github.com/Kitware/CMake).

## Usage
- Clone the repo
```
https://github.com/rhythmd18/pong.git
```

- Get the SDL3 and SDL_ttf libraries
```
git clone https://github.com/libsdl-org/SDL.git vendored/SDL
git clone https://github.com/libsdl-org/SDL_ttf.git vendored/SDL_ttf
sh vendored/SDL_ttf/external/download.sh
```

- Build
```
cmake -S . -B build
cmake --build build
```

- Run
```
cd build
./pong
```

- Enjoy!
