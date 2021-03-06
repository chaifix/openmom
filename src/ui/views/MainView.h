//
//  MainView.h
//  OpenMoM
//
//  Created by Jack on 7/27/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _MAIN_VIEW_H_
#define _MAIN_VIEW_H_

#include "View.h"

class ViewManager;

class Army;
class Tile;
enum class MovementType;

class MainView : public View
{
private:
  enum : u8
  {
    GAME,
    SPELLS,
    ARMIES,
    CITIES,
    MAGIC,
    INFO,
    PLANE,
    NEXT,

    DONE,
    PATROL,
    WAIT,
    BUILD_ROAD,
    BUILD_OUTPOST,
    //CANCEL,
    PURIFY,
    OK,
    MELD,
    
    ROAD_OK,
    ROAD_CANCEL,
    
    SURVEYOR_CANCEL,
    
    SPELLCAST_CANCEL,
    
    BUTTON_COUNT
  };
  
  enum Substate : u8
  {
    MAIN,
    UNIT,
    SURVEYOR,
    ROAD_BUILDING,
    SPELL_CAST
  };
  
  Substate substate;
  
  Tile* hoveredTile;

  void draw() override;
  void drawPost() override;
  
  struct Surveyor
  {
    MainView& view;
    Tile* tile;
    bool cityCanBeBuilt;
    I18 cityForbidMsg;
    
    Surveyor(MainView& view) : view(view), tile(nullptr), cityCanBeBuilt(false) { }
    
    void updateInfo(Tile* tile);
    void draw();
  };
  
  Surveyor surveyor = Surveyor(*this);
  
  SpriteInfo movementIconForType(MovementType effect);
    
public:
  MainView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
  
  void switchToSpellCast();
  void switchToUnitSelection(Army* army);
  void switchToNormalState();
  void switchToSurveyor();
  void switchToRoadBuilding();
  void updateBuildButton();
  
  bool mouseReleased(u16 x, u16 y, MouseButton b) override;
  bool mouseMoved(u16 x, u16 y, MouseButton b) override;
  
  bool keyPressed(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod) override;
  bool keyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod) override;

};

#endif
