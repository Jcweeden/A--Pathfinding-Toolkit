#include "Game.h"

#include "Environment.h"


Game* Game::s_pInstance = 0;

Game* Game::Instance()
  {
    if(s_pInstance == 0)
    {
      s_pInstance = new Game();
      return s_pInstance;
    }

    return s_pInstance;
  }


bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
  int flags = 0;

  if (fullscreen) {
    flags = SDL_WINDOW_SHOWN;
  }
  
  // initialise SDL
   if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
   {
     std::cout << "Game: SDL init success\n";
     // if succeeded create our window
     m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
 
 // if the window creation succeeded create our renderer
     if(m_pWindow != 0)
     {
       std::cout << "Game: Window creation success\n";
       m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

       if (m_pRenderer != 0) {
         std::cout << "Game: Renderer creation success\n";
         SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, 255);
       }
       else {
         std::cout << "Game: Renderer init failed\n";
         return false;
       }
     }
     else
     {
       std::cout << "Game: Window init failed\n";
       return false;
     }
   }
   else
   {
     std::cout << "Game: SDL init failed\n";
     return false; // sdl could not initialise
   }

   std::cout << "Game: Init success\n";
   m_bRunning = true;//start the main loop

   windowWidth = 600;
   windowHeight = 600;
   frameTime = 0;

   
   srand (time(NULL));

   //setup environment
   env = TheEnvironment::Instance();
   env->setup(1,1,10,7,5, getWindowHeight(), getWindowWidth(), 20, 99, true);
   std::cout << "Game.init() - env->setup() complete\n";
   std::cout << "\n\n9: random spawn (single agent)\n";
   return true;
}

void Game::render()
{
  SDL_SetRenderDrawColor(m_pRenderer, 0, 140, 0, 0xFF);

  SDL_RenderClear(m_pRenderer); // clear the renderer to the draw colour

  env->draw();

  SDL_RenderPresent(m_pRenderer);  //draw to the screen
}

void Game::update() {
  
  env->update();
}

void Game::clean() {
  std::cout << "Game: cleaning game\n";

  env->clean();
  delete TheEnvironment::Instance();
  
  SDL_DestroyWindow (m_pWindow);
  SDL_DestroyRenderer (m_pRenderer);
  SDL_Quit();
}

void Game::handleEvents()
{
  TheInputHandler::Instance()->update();

  if (TheInputHandler::Instance()->key1Pressed)
  {
    TheInputHandler::Instance()->key1Pressed = false;
    TheInputHandler::Instance()->reset();
    env->clean();
    //1: selection of nearest target to collect
    std::cout << "\n\n1: path to goal with obstacle avoidance\n";
    env->setup(1,10,0,0,0, getWindowHeight(), getWindowWidth(), 20, 1, false);
  }

  
  if (TheInputHandler::Instance()->key2Pressed)
  {
    TheInputHandler::Instance()->key2Pressed = false;
    TheInputHandler::Instance()->reset();
    env->clean();
    //2: path to goal with obstacle avoidance
    std::cout << "\n\n2: path to goal with obstacle avoidance\n";
    env->setup(1,1,20,0,0, getWindowHeight(), getWindowWidth(), 20, 2, false);
  }

  if (TheInputHandler::Instance()->key3Pressed)
  {
    TheInputHandler::Instance()->key3Pressed = false;
    TheInputHandler::Instance()->reset();
    env->clean();
    //3: dynamic obstacle avoidance
    std::cout << "\n\n3: dynamic obstacle avoidance\n";
    env->setup(1,1,6,0,0, getWindowHeight(), getWindowWidth(), 20, 3, false);
  }

  if (TheInputHandler::Instance()->key4Pressed)
  {
    TheInputHandler::Instance()->key4Pressed = false;
    TheInputHandler::Instance()->reset();
    env->clean();
    //4: ability to determine if a viable route exists
    std::cout << "\n\n4: ability to determine if a viable route exists\n";
    env->setup(1,1,4,0,0, getWindowHeight(), getWindowWidth(), 20, 4, false);
  }

  if (TheInputHandler::Instance()->key5Pressed)
  {
    TheInputHandler::Instance()->key5Pressed = false;
    TheInputHandler::Instance()->reset();
    env->clean();
    //5: resizable A* navigation grid (size 20)
    std::cout << "\n\n5: resizable A* navigation grid (size 20) \n";
    env->setup(1,1,11,0,0, getWindowHeight(), getWindowWidth(), 20, 5, false);
  }

  if (TheInputHandler::Instance()->key6Pressed)
  {
    TheInputHandler::Instance()->key6Pressed = false;
    TheInputHandler::Instance()->reset();
    env->clean();
    //6: resizable A* navigation grid (size 30)
    std::cout << "\n\n6: resizable A* navigation grid (size 30)\n";
    env->setup(1,1,11,0,0, getWindowHeight(), getWindowWidth(), 30, 6, false);
  }
  
  if (TheInputHandler::Instance()->key7Pressed)
  {
    TheInputHandler::Instance()->key7Pressed = false;
    TheInputHandler::Instance()->reset();
    env->clean();
    //7: weighted paths
    std::cout << "\n\n7: weighted paths\n";
    env->setup(1,1,0,1,2, getWindowHeight(), getWindowWidth(), 20, 7, false);
  }

  if (TheInputHandler::Instance()->key8Pressed)
  {
    TheInputHandler::Instance()->key8Pressed = false;
    TheInputHandler::Instance()->reset();
    env->clean();
    //8: blurred penalty costs
    std::cout << "\n\n8: weighted paths with blurred penalty costs\n";
    env->setup(1,1,0,1,2, getWindowHeight(), getWindowWidth(), 20, 8, true);
  }

  if (TheInputHandler::Instance()->key9Pressed)
  {
    TheInputHandler::Instance()->key9Pressed = false;
    TheInputHandler::Instance()->reset();
    env->clean();
    //9: random spawn (single agent)
    std::cout << "\n\n9: random spawn (single agent)\n";
    env->setup(1,1,10,7,5, getWindowHeight(), getWindowWidth(), 20, 99, true);
  }

  if (TheInputHandler::Instance()->key0Pressed)
  {
    TheInputHandler::Instance()->key0Pressed = false;
    TheInputHandler::Instance()->reset();
    env->clean();
    //0: random spawn (multiple agents)
    std::cout << "\n\n0: random spawn (multiple agents)\n";
    env->setup(10,40,10,7,5, getWindowHeight(), getWindowWidth(), 20, 99, true);
  }
}

void Game::quit() {
  m_bRunning = false;
}
