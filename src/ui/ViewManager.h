#ifndef _VIEW_MANAGER_H_
#define _VIEW_MANAGER_H_

#include "common/Common.h"
#include "EventListener.h"
#include "Animations.h"

#include <list>

namespace anims { class Animation; }

class View;
class AlchemyView;
class ArmiesItemsView;
class ArmiesView;
class ArmyView;
class CityView;
class CitiesView;
class CombatView;
class ItemCraftView;
class ItemCraftChargesView;
class MagicView;
class MainView;
class MessageView;
class MirrorView;
class NewGameView;
class OutpostView;
class ProductionView;
class ResearchView;
class SpellBookView;
class UnitDetailView;
class LoadView;
class OptionsView;
class StartView;
class IntroView;
class InfoMenuView;
class CartographerView;
class DiplomacyView;
class AstrologerView;
class HistorianView;
class MerchantView;

class MapEditorView;

class ConsoleView;
class DataView;


class LocalPlayer;

class ViewManagerInterface
{
public:
  virtual void showMessage(const msgs::Message* message) = 0;
  virtual void showUnitDetail(const Unit* unit, bool withButtons) = 0;
};

class ViewManager : public EventListener, public ViewManagerInterface
{
private:
  View* views[VIEW_COUNT];
  View* current;
  std::list<View*> overviews;
  
  std::list<std::unique_ptr<anims::Animation>> animations;
  bool animating;
  
public:
  ViewManager();
  
  void push(anims::Animation* animation)
  {
    animating = true;
    animations.push_back(std::unique_ptr<anims::Animation>(animation));
  }
  
  void setPlayer(LocalPlayer* player);
  
  void switchView(ViewID type);
  void switchOverview(ViewID type);
  void closeOverview();
  
  void draw();
  
  bool mousePressed(u16 x, u16 y, MouseButton b) override;
  bool mouseClicked(u16 x, u16 y, MouseButton b) override;
  bool mouseReleased(u16 x, u16 y, MouseButton b) override;
  bool mouseMoved(u16 x, u16 y, MouseButton b) override;
  bool mouseDragged(u16 x, u16 y, MouseButton b) override;
  bool mouseWheel(s16 dx, s16 dy, u16 d) override;
  
  bool keyPressed(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod) override;
  bool keyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod) override;
  bool textInput(sdl_text_input data) override;
  
  inline bool isThereOverview() { return !overviews.empty(); }
  inline View* firstOverview() { return isThereOverview() ? overviews.back() : nullptr; }
  inline View* respondingView() { return isThereOverview() ? overviews.back() : current; }
  
  AlchemyView* alchemyView();
  ArmiesItemsView* armiesItemsView();
  ArmiesView* armiesView();
  ArmyView* armyView();
  CitiesView* citiesView();
  CityView* cityView();
  CombatView* combatView();
  ItemCraftView* itemCraftView();
  ItemCraftChargesView* itemCraftChargesView();
  MagicView* magicView();
  MainView* mainView();
  MessageView* messageView();
  MirrorView* mirrorView();
  NewGameView* newGameView();
  OutpostView* outpostView();
  ProductionView* productionView();
  ResearchView* researchView();
  SpellBookView* spellBookView();
  UnitDetailView* unitDetailView();
  StartView* startView();
  IntroView* introView();
  InfoMenuView* infoMenuView();
  CartographerView* cartographerView();
  DiplomacyView* diplomacyView();
  AstrologerView* astrologerView();
  HistorianView* historianView();
  MerchantView* merchantView();
  
  MapEditorView* mapEditorView();
  
  ConsoleView* consoleView();
  
  
  
  void showMessage(const msgs::Message* message) override;
  void showUnitDetail(const Unit* unit, bool withButtons = true) override;
};

#endif
