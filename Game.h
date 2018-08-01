#ifndef _Game_
#define _Game_
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "SDL2/SDL2_gfxPrimitives.h" 

#include "InputHandler.h"

#include <iostream>
#include <vector>

class Environment;

class Game
{
public:

  static Game* Instance();
        
  bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
  void render();
  void update();
  void handleEvents();
  void clean();
  void quit();

  
  SDL_Renderer* getRenderer() const { return m_pRenderer; }

  //returns true if a call to Quit has not yet been received
  bool running() {return m_bRunning;}

  int getUIHeight(){return UIHeight;}
  int getWindowHeight(){return windowHeight;}
  int getWindowWidth(){return windowWidth;}

  //get the time the last frame took to render
  Uint32 getFrameTime() { return frameTime; }
  void setFrameTime(Uint32 val) { frameTime = val; }
  
private:

  Game(){}

  //static instance variable
  static Game* s_pInstance;

  //instance of the environment containing all agents, targets, obstacles and the grid for pathfinding
  Environment* env;
  
  //the time it took to render the last frame
  Uint32 frameTime;

  SDL_Window* m_pWindow;

  //renderer of which the whole program uses
  SDL_Renderer* m_pRenderer;

  //width and height of the window in pixels
  int windowWidth;
  int windowHeight;

  //the height of the UI drawn at the bottom of the screen (width is same as that of the window)
  int UIHeight;

  //start the game loop
  bool m_bRunning;

};

typedef Game TheGame;

#endif
