#include "ViewManager.h"

#include "LocalPlayer.h"
#include "Game.h"
#include "Animations.h"
#include "View.h"
#include "SDLHelper.h"
#include "Texture.h"
#include "Gfx.h"

#include "ui/views/AlchemyView.h"
#include "ui/views/ArmiesItemsView.h"
#include "ui/views/ArmiesView.h"
#include "ui/views/ArmyView.h"
#include "ui/views/CitiesView.h"
#include "ui/views/CityView.h"
#include "ui/views/CombatView.h"
#include "ui/views/ItemCraftView.h"
#include "ui/views/ItemCraftChargesView.h"
#include "ui/views/MagicView.h"
#include "ui/views/MainView.h"
#include "ui/views/MessageView.h"
#include "ui/views/MirrorView.h"
#include "ui/views/NewGameView.h"
#include "ui/views/LoadView.h"
#include "ui/views/OptionsView.h"
#include "ui/views/OutpostView.h"
#include "ui/views/ProductionView.h"
#include "ui/views/ResearchView.h"
#include "ui/views/SpellBookView.h"
#include "ui/views/UnitDetailView.h"
#include "ui/views/StartView.h"
#include "ui/views/IntroView.h"
#include "ui/views/InfoMenuView.h"
#include "ui/views/CartographerView.h"
#include "ui/views/DiplomacyView.h"
#include "ui/views/AstrologerView.h"
#include "ui/views/HistorianView.h"
#include "ui/views/MerchantView.h"

#include "ui/views/extra/MapEditorView.h"

#include "ui/views/ConsoleView.h"
#include "ui/views/debug/DataView.h"

using namespace std;
using namespace anims;

ViewManager::ViewManager() : views{nullptr}, animating(false), current(nullptr)
{
  views[VIEW_ALCHEMY] = new AlchemyView(this);
  views[VIEW_ARMIES_ITEMS] = new ArmiesItemsView(this);
  views[VIEW_ARMIES] = new ArmiesView(this);
  views[VIEW_ARMY] = new ArmyView(this);
  views[VIEW_CITY] = new CityView(this);
  views[VIEW_CITIES] = new CitiesView(this);
  views[VIEW_COMBAT] = new CombatView(this);
  views[VIEW_ITEM_CRAFT] = new ItemCraftView(this);
  views[VIEW_ITEM_CRAFT_CHARGES] = new ItemCraftChargesView(this);
  views[VIEW_MAGIC] = new MagicView(this);
  views[VIEW_MAIN] = new MainView(this);
  views[VIEW_MESSAGE] = new MessageView(this);
  views[VIEW_MIRROR] = new MirrorView(this);
  views[VIEW_NEW_GAME] = new NewGameView(this);
  views[VIEW_OUTPOST] = new OutpostView(this);
  views[VIEW_PRODUCTION] = new ProductionView(this);
  views[VIEW_RESEARCH] = new ResearchView(this);
  views[VIEW_SPELL_BOOK] = new SpellBookView(this);
  views[VIEW_UNIT] = new UnitDetailView(this);
  views[VIEW_LOAD] = new LoadView(this);
  views[VIEW_START] = new StartView(this);
  views[VIEW_INTRO] = new IntroView(this);
  views[VIEW_OPTIONS] = new OptionsView(this);
  views[VIEW_INFO_MENU] = new InfoMenuView(this);
  views[VIEW_CARTOGRAPHER] = new CartographerView(this);
  views[VIEW_DIPLOMACY] = new DiplomacyView(this);
  views[VIEW_ASTROLOGER] = new AstrologerView(this);
  views[VIEW_HISTORIAN] = new HistorianView(this);
  views[VIEW_MERCHANT] = new MerchantView(this);
  
  views[VIEW_MAP_EDITOR] = new MapEditorView(this);
  
  views[VIEW_CONSOLE] = new ConsoleView(this);
  views[VIEW_DATA] = new DataView(this);
}

AlchemyView* ViewManager::alchemyView() { return static_cast<AlchemyView*>(views[VIEW_ALCHEMY]); }
ArmiesItemsView* ViewManager::armiesItemsView() { return static_cast<ArmiesItemsView*>(views[VIEW_ARMIES_ITEMS]); }
ArmiesView* ViewManager::armiesView() { return static_cast<ArmiesView*>(views[VIEW_ARMIES]); }
ArmyView* ViewManager::armyView() { return static_cast<ArmyView*>(views[VIEW_ARMY]); }
CitiesView* ViewManager::citiesView() { return static_cast<CitiesView*>(views[VIEW_CITIES]); }
CityView* ViewManager::cityView() { return static_cast<CityView*>(views[VIEW_CITY]); }
CombatView* ViewManager::combatView() { return static_cast<CombatView*>(views[VIEW_COMBAT]); }
ItemCraftView* ViewManager::itemCraftView() { return static_cast<ItemCraftView*>(views[VIEW_ITEM_CRAFT]); }
ItemCraftChargesView* ViewManager::itemCraftChargesView() { return static_cast<ItemCraftChargesView*>(views[VIEW_ITEM_CRAFT_CHARGES]); }
MagicView* ViewManager::magicView() { return static_cast<MagicView*>(views[VIEW_MAGIC]); }
MainView* ViewManager::mainView() { return static_cast<MainView*>(views[VIEW_MAIN]); }
MessageView* ViewManager::messageView() { return static_cast<MessageView*>(views[VIEW_MESSAGE]); }
MirrorView* ViewManager::mirrorView() { return static_cast<MirrorView*>(views[VIEW_MIRROR]); }
NewGameView* ViewManager::newGameView() { return static_cast<NewGameView*>(views[VIEW_NEW_GAME]); }
OutpostView* ViewManager::outpostView() { return static_cast<OutpostView*>(views[VIEW_OUTPOST]); }
ProductionView* ViewManager::productionView() { return static_cast<ProductionView*>(views[VIEW_PRODUCTION]); }
ResearchView* ViewManager::researchView() { return static_cast<ResearchView*>(views[VIEW_RESEARCH]); }
SpellBookView* ViewManager::spellBookView() { return static_cast<SpellBookView*>(views[VIEW_SPELL_BOOK]); }
UnitDetailView* ViewManager::unitDetailView() { return static_cast<UnitDetailView*>(views[VIEW_UNIT]); }
StartView* ViewManager::startView() { return static_cast<StartView*>(views[VIEW_START]); }
IntroView* ViewManager::introView() { return static_cast<IntroView*>(views[VIEW_INTRO]); }
InfoMenuView* ViewManager::infoMenuView() { return static_cast<InfoMenuView*>(views[VIEW_INFO_MENU]); }
CartographerView* ViewManager::cartographerView() { return static_cast<CartographerView*>(views[VIEW_CARTOGRAPHER]); }
DiplomacyView* ViewManager::diplomacyView() { return static_cast<DiplomacyView*>(views[VIEW_DIPLOMACY]); }
AstrologerView* ViewManager::astrologerView() { return static_cast<AstrologerView*>(views[VIEW_ASTROLOGER]); }
HistorianView* ViewManager::historianView() { return static_cast<HistorianView*>(views[VIEW_HISTORIAN]); }

MapEditorView* ViewManager::mapEditorView() { return static_cast<MapEditorView*>(views[VIEW_MAP_EDITOR]); }


ConsoleView* ViewManager::consoleView() { return static_cast<ConsoleView*>(views[VIEW_CONSOLE]); }


void ViewManager::switchView(ViewID type)
{
  if (current)
    current->doDeactivate();
  current = views[type];
  current->doActivate(LocalGame::i->currentPlayer());
}

void ViewManager::switchOverview(ViewID type)
{
  View* overview = views[type];
  overview->doActivate(LocalGame::i->currentPlayer());
  overviews.push_back(overview);
}

void ViewManager::closeOverview()
{
  if (!overviews.empty())
  {
    overviews.back()->doDeactivate();
    overviews.pop_back();
  }
}

void ViewManager::draw()
{
  auto it = animations.begin();
  auto cend = animations.end();
  
  while (it != cend)
  {
    Animation* a = it->get();
    if (a->hasFinished())
    {
      
      if (a->next())
      {
        a->reset();
        animations.push_back(std::move(a->next()));
      }
      
      it = animations.erase(it);
    }
    else
      ++it;
  }
  
  if (animations.empty())
    animating = false;
  
  current->drawPre();
  
  current->doDraw();
  
  if (LocalGame::i->currentPlayer()->hasMessage() && !isThereOverview())
    switchOverview(VIEW_MESSAGE);
  
  if (isThereOverview())
  {
    Gfx::fillRect(0, 0, WIDTH, HEIGHT, Color(0,0,0,180));

    for (View *v : overviews)
    {
      v->doDraw();
      v->drawPost();
    }
  }
  
  for (const auto& a : animations)
    a->doStep();
  
  current->drawPost();
}

bool ViewManager::mouseClicked(u16 x, u16 y, MouseButton b)
{
  if (animating)
    return (*animations.begin())->mouseClicked(x, y, b);
  else
    return respondingView()->doMouseClicked(x, y, b);
}

bool ViewManager::mouseDragged(u16 x, u16 y, MouseButton b)
{
  if (animating)
    return (*animations.begin())->mouseDragged(x, y, b);
  else
    return respondingView()->doMouseDragged(x, y, b);
}

bool ViewManager::mouseMoved(u16 x, u16 y, MouseButton b)
{
  if (animating)
    return (*animations.begin())->mouseMoved(x, y, b);
  else
    return respondingView()->doMouseMoved(x, y, b);
}

bool ViewManager::mouseReleased(u16 x, u16 y, MouseButton b)
{
  if (animating)
    return (*animations.begin())->mouseReleased(x, y, b);
  else
    return respondingView()->doMouseReleased(x, y, b);
}

bool ViewManager::mousePressed(u16 x, u16 y, MouseButton b)
{
  if (animating)
    return (*animations.begin())->mousePressed(x, y, b);
  else
    return respondingView()->doMousePressed(x, y, b);
}

bool ViewManager::mouseWheel(s16 dx, s16 dy, u16 d)
{
  if (animating)
    return (*animations.begin())->mouseWheel(dx, dy, d);
  else
    return respondingView()->doMouseWheel(dx, dy, d);
}

bool ViewManager::keyPressed(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod)
{
  if (animating)
    return (*animations.begin())->keyPressed(key, kkey, mod);
  else
    return respondingView()->doKeyPressed(key, kkey, mod);
}

bool ViewManager::keyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod)
{
  if (animating)
    return (*animations.begin())->keyReleased(key, kkey, mod);
  else
    return respondingView()->doKeyReleased(key, kkey, mod);
}

bool ViewManager::textInput(sdl_text_input data)
{
  if (animating)
    return (*animations.begin())->textInput(data);
  else
    return respondingView()->textInput(data);
}

void ViewManager::setPlayer(LocalPlayer* player)
{
  std::for_each(views, views+VIEW_COUNT, [player] (View* view) { view->setPlayer(player); });
}


//CityView* ViewManager::cityView() { return static_cast<CityView*>(views[VIEW_CITY]); }



void ViewManager::showMessage(const msgs::Message* message) { messageView()->showMessage(message); }
void ViewManager::showUnitDetail(const Unit* unit, bool withButtons)
{
  unitDetailView()->setUnit(unit, withButtons);
  switchOverview(VIEW_UNIT);
}
