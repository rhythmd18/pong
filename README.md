# Pong
Clone of the 1972 [Pong](https://en.wikipedia.org/wiki/Pong), a classic two-player game built using C and [SDL3](https://libsdl.org). 

## Demo


https://github.com/user-attachments/assets/b39c0f6b-8f8a-49d8-83db-c646202d09c4



## Prerequisites
- Install a C/C++ compiler of your choice (I used the preinstalled Apple Clang. Any compiler works).
- Install [CMake](https://github.com/Kitware/CMake).
- That's it!

## Usage
- Clone the repo and `cd` into it
```bash
git clone https://github.com/rhythmd18/pong.git
cd pong
```

- Get the SDL3 and SDL_ttf libraries
```bash
git clone https://github.com/libsdl-org/SDL.git vendored/SDL
git clone https://github.com/libsdl-org/SDL_ttf.git vendored/SDL_ttf
sh vendored/SDL_ttf/external/download.sh
```

- Build
```bash
cmake -S . -B build
cmake --build build
```

- Run
  - For Windows
  ```
  cd build/Debug
  ./pong
  ```
  - For Linux/MacOS
  ```bash
  cd build
  ./pong
  ```

- Enjoy with a friend!

## How to play
- W and S for the left paddle.
- UP and DOWN for the right paddle.

---
PS zero vibe coding. I swear!
