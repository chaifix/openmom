#include <iostream>

#include <SDL2/SDL.h>

#include "SDL.h"
#include "Gfx.h"
#include "Texture.h"

#include "Race.h"

#include <unordered_map>

#include "EventListener.h"

#include "Settings.h"

#include "Game.h"
#include "LocalPlayer.h"
#include "ViewManager.h"

#include "Localization.h"

#include "Skill.h"

#include "WorldGenerator.h"

void init()
{
  Game *game = new Game();
  LocalPlayer *player = new LocalPlayer(game, "Kali", Data::wizard(KALI), GREEN, Race::race(RaceID::BARBARIANS), 60, 40);
  
  LocalGame *localGame = new LocalGame();
  localGame->currentPlayer = player;
  LocalGame::i = localGame;
}



int main(int argc, char * arg[])
{
  //Util::seed(time(nullptr));
  AtlasMap* map = new AtlasMap(60,40,'w');
  map->initialize(60, 40, 12, 1, 28, IslandType::MIXED, CoastWaters::CALM, true, false, CoastMode::BLOBBY);
  
  for (int y = 0; y < 40; ++y)
  {
    for (int x = 0; x < 60; ++x)
    {
      printf("%c", map->get(x, y));
    }
    printf("\n");
  }
  
  //const Race &race = Race::race(RACE_BARBARIANS);
  
  //std::unordered_map<const void*, SpriteInfo> map;
  //map[reinterpret_cast<const Race *>(&race)] = {MAIN_BACKDROP, 15,15};
  
  //const Race* ptRace = reinterpret_cast<const Race *>(&race);
  
  // init SDL
      
  init();
  
  SDL::init();
  Gfx::init();
  Texture::load();

  SDL::gvm->switchView(VIEW_NEW_GAME);
  
  SDL::loop();
  
  
  // wait 5 secs
  
  // clean up
  Texture::unload();
  Gfx::deinit();
  SDL::deinit();
  
  return 0;
}