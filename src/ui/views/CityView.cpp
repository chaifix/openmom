//
//  CityView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "CityView.h"

#include "common/mystrings.h"

#include "City.h"
#include "CityScape.h"
#include "LocalPlayer.h"
#include "Game.h"
#include "Army.h"
#include "World.h"
#include "Tile.h"
#include "CityMechanics.h"
#include "Viewport.h"

#include "Localization.h"

#include "ViewManager.h"

#include "Gfx.h"
#include "GfxData.h"
#include "Texture.h"
#include "Font.h"
#include "Buttons.h"
#include "Messages.h"
#include "UnitDraw.h"

#include "ProductionView.h"

#include "LBX.h"

constexpr size_t CITY_ENCHANT_PER_PAGE = 6;

CityView::CityView(ViewManager* gvm) : View(gvm)
{
  cityEnchantPage = 0;
  
  buttons.resize(BUTTON_COUNT);
  
  buttons[BUY] = Button::buildTristate("Buy", 214, 188, LSI(BACKGRND, 7), LSI(BACKGRND, 14));
  buttons[CHANGE] = Button::buildBistate("Change", 247, 188, LSI(BACKGRND, 8));
  buttons[OK] = Button::buildBistate("Ok", 286, 188, LSI(BACKGRND, 9));
  
  /* TODO: add behavior */
  buttons[PREV_CITY_ENCHANT] = Button::buildBistate("Prev City Enchant", 201, 50, LSI(BACKGRND,15))->setAction([this](){
    const size_t pageCount = Math::roundWithMod(city->getSpells().size(), CITY_ENCHANT_PER_PAGE);
    cityEnchantPage = Math::negativeWrap(cityEnchantPage, pageCount);
  });
  
  buttons[NEXT_CITY_ENCHANT] = Button::buildBistate("Prev City Enchant", 201, 85, LSI(BACKGRND,16))->setAction([this](){
    const size_t pageCount = Math::roundWithMod(city->getSpells().size(), CITY_ENCHANT_PER_PAGE);
    cityEnchantPage = (cityEnchantPage + 1) % pageCount;
  });

  
  buttons[BUY]->deactivate();
  
  buttons[BUY]->setAction([this](){
    player->send(new msgs::Confirm(fmt::sprintf("Do you wish to spend %d Gold by purchasing a %s?", city->getProductable()->productionCost(), city->getProductable()->productionName()), [&]() {
      g->cityMechanics.buyProduction(city);
    }));
  });
  
  buttons[OK]->setAction([gvm](){ gvm->switchView(VIEW_MAIN); });
  buttons[CHANGE]->setAction([gvm,this]{ gvm->productionView()->setCity(city); gvm->switchOverview(VIEW_PRODUCTION); });
  
  addArea(new ClickableGrid(139, 50, 60, 7, 6, 1))->setCellAction([this](coord_t x, coord_t y) { 
    clickOnCitySpell(y);
  });
}

void CityView::clickOnCitySpell(size_t index)
{
  const size_t realIndex = CITY_ENCHANT_PER_PAGE*cityEnchantPage + index;
  const auto& spells = city->getSpells();
  
  if (realIndex < spells.size())
  {
    const auto& cast = spells[realIndex];
    assert(cast.caster().isWizard());
    if (cast.caster().player() == player)
    {
      player->send(new msgs::Confirm(fmt::format("Do you wish to turn off ^s{}^^?", cast.spell()->name), [&]() {
        city->removeSpell(cast);
      }));
    }
  }
}

void CityView::setCity(City *city)
{
  this->city = city;
  cityEnchantPage = 0;
  const size_t spellsCount = city->getSpells().size();

  
  buttons[BUY]->activateIf(g->cityMechanics.isProductionBuyable(city));
  
  buttons[PREV_CITY_ENCHANT]->showIf(spellsCount > CITY_ENCHANT_PER_PAGE);
  buttons[NEXT_CITY_ENCHANT]->showIf(spellsCount > CITY_ENCHANT_PER_PAGE);

  if (!CityLayout::contains(city))
    CityLayout::createLayout(city);
  else /*if (city->revalidateLayout)*/
    CityLayout::updateLayout(city);
}

s16 CityView::drawCityProp(s16 value, UpkeepSymbol type, s16 sx, s16 sy)
{
  for (s16 i = value; i > 0; --i)
    if (i >= 10)
    {
      i -= 9;
      Gfx::draw(GfxData::upkeepGfxSpec(type).tenTimes, sx, sy);
      sx += 15;
    }
    else
    {
      Gfx::draw(GfxData::upkeepGfxSpec(type).single, sx, sy);
      sx += 6;
    }
  
  return sx;
}

void CityView::draw()
{
  CityLayout::draw(city, player);
  
  /* draw minimap */
  Viewport::drawCityViewport(player, g->world, city->getPosition());
  
  Gfx::draw(LSI(BACKGRND, 6), 0, 0); // bg
  
  /* city name*/
  std::string cityName = fmt::sprintf("%s of %s", i18n::c(i18n::CITY_SIZE_NAMES[city->tileSize()]), city->getName());
  
  Fonts::drawString(cityName, FontFaces::Huge::GOLD, 104, 2, ALIGN_CENTER);
  
  const auto& labourGfx = GfxData::raceGfxSpec(city->race);
  
  int popX = 4, reqFarmers = city->reservedPopulation;
  for (int i = 0; i < (city->population / 1000); ++i)
  {
    if (i < reqFarmers)
      Gfx::draw(labourGfx.cityFarmer, popX, 27);
    else if (i < reqFarmers + city->farmers)
      Gfx::draw(labourGfx.cityFarmer, popX, 27);
    else
      Gfx::draw(labourGfx.cityWorker, popX, 27);
    
    if (i == reqFarmers - 1)
      popX += 4;
    
    popX += 9;
  }
  
  int sx = 6;
  int sy = 52;
  
  //TODO: spacing between icons still incorrect
  
  sx += drawCityProp(city->necessaryFood, UpkeepSymbol::FOOD, sx, sy) + 5;
  drawCityProp(city->food, UpkeepSymbol::FOOD, sx, sy);
  sx = 6;
  sy += 8;
  drawCityProp(city->work, UpkeepSymbol::WORK, sx, sy);
  sx = 6;
  sy += 8;
  sx += drawCityProp(city->upkeep.gold, UpkeepSymbol::GOLD, sx, sy) + 5;
  drawCityProp(city->gold, UpkeepSymbol::GOLD, sx, sy);
  sx = 6;
  sy += 8;
  drawCityProp(city->magicPower, UpkeepSymbol::MANA, sx, sy);
  sx = 6;
  sy += 8;
  drawCityProp(city->knowledge, UpkeepSymbol::RESEARCH, sx, sy);
  
  Army* army = g->world->get(city->getPosition())->army;
  
  if (army)
  {
    for (int i = 0; i < army->size(); ++i)
    {
      const Unit* u = army->get(i);
      //if (!army.isPatrol())
      UnitDraw::drawStatic(u, 215+20*(i%5), 102+17*(i/5), true, false);
    }
  }
  
  /* draw city info: race and population */
  Fonts::drawString(i18n::s(GfxData::raceGfxSpec(city->race).name), FontFaces::Small::YELLOW, 5, 18, ALIGN_LEFT);
  //TODO: localize digits
  Fonts::drawString(fmt::sprintf("Population: %s (+%d)", strings::groupDigits(city->population), city->growthRate), FontFaces::Small::YELLOW, 209, 18, ALIGN_RIGHT);
  
  const Productable* production = city->getProductable();
  
  /* draw current production */
  if (production == Building::HOUSING || production == Building::TRADE_GOODS)
  {
    Gfx::draw(LSI(BACKGRND, 13), 260, 149);

    CityLayout::drawBuildingCentered(city, static_cast<const Building*>(production), 217, 179);
    
    /* TODO: localize and center align */
    if (production == Building::HOUSING)
      Fonts::drawStringBounded("Increases population growth rate.", FontFaces::Tiny::WHITE, 286, 158, 50, ALIGN_CENTER);
    else if (production == Building::TRADE_GOODS)
      Fonts::drawStringBounded("Converts production to gold.", FontFaces::Tiny::WHITE, 286, 158, 50, ALIGN_CENTER);
    
  }
  else
  {
    if (production->productionType() == Productable::Type::BUILDING)
    {
      CityLayout::drawBuildingCentered(city, static_cast<const Building*>(city->getProductable()), 217, 177);
    }
    else if (production->productionType() == Productable::Type::UNIT)
    {
      UnitDraw::drawUnitIso(static_cast<const UnitSpec*>(production), 218, 144, nullptr, player);
    }
    
    /* compute and draw missing turns to complete production */
    /* TODO: localize */
    int turns = g->cityMechanics.turnsRequiredForProduction(city);
    Fonts::drawString(fmt::format("{} Turn{}", turns, (turns == 1 ? "" : "s")), FontFaces::Small::YELLOW, 316, 139, ALIGN_RIGHT);
    
    /* draw production coins */
    int max = 10;
    for (int i = 0; i < std::min(production->productionCost()/10, 40); ++i)
    {
      /* TODO: fixare il draw parziale delle monete */
      if (i*10 < city->productionPool)
        Gfx::draw(LSI(BACKGRND, 12), 262+i%max*5, 151+i/max*7);
      else
        Gfx::draw(LSI(BACKGRND, 11), 262+i%max*5, 151+i/max*7);
    }
  }
  
  /* TODO: in realtà il font sarebbe obreggiato dall'alto / gestire wrapping nomi lunghi (ship wrights guild etc) */
  if (Fonts::stringWidth(FontFaces::Small::WHITE, production->productionName()) < 40)
    Fonts::drawString(production->productionName(), FontFaces::Small::WHITE, 237, 178, ALIGN_CENTER);
  else
    Fonts::drawString(production->productionName(), FontFaces::Tiny::WHITE, 235, 177, ALIGN_CENTER);
  
  
  /* draw city enchantments */
  const auto spells = city->getSpells();
  auto it = spells.begin();
  std::advance(it, cityEnchantPage*CITY_ENCHANT_PER_PAGE);
  for (int i = 0; i < CITY_ENCHANT_PER_PAGE && it != spells.end(); ++i, ++it)
  {
    const auto& cast = *it;
    assert(cast.caster().isWizard());
    const FontSpriteSheet* face = Fonts::fontForColor(cast.caster().player()->color);
    //Fonts::drawString(i18n::s(cast.spell->name), face, 138, 50+7*i, ALIGN_LEFT);
    Fonts::drawString(fmt::sprintf("%s%d", i18n::s(cast.spell()->name), cityEnchantPage*CITY_ENCHANT_PER_PAGE + i), face, 138, 50+7*i, ALIGN_LEFT);
  }
}
