//
//  Player.h
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "common/Common.h"
#include "Diplomacy.h"
#include "Spells.h"
#include "SpellBook.h"
#include "Army.h"

#include <list>
#include <string>
#include <unordered_set>

class FogMap
{
private:
  bool*** map;
  const u16 w, h;
  Player *player;
  
public:
  FogMap(Player* player, u16 w, u16 h);
  
  bool get(const Position& position) const;
  void set(const Position& position);
  
  void setRect(s16 x, s16 y, s16 w, s16 h, Plane plane);
  void setRange(const Position& position, s16 range);
  
  ~FogMap();
};

struct TaxRate
{
  float goldPerCitizen;
  float unrestPercent;
};

class PlayerInterface
{
public:
  virtual void selectAll() = 0;
  virtual Army* getSelectedArmy() const = 0;
  virtual const unit_list& getSelectedUnits() const = 0;
  
  virtual void push(anims::Animation* animation) = 0;
  virtual void send(msgs::Message* message) = 0;
  virtual s16 selectedCount() const = 0;
  virtual void discoverTile(const Position& position) = 0;
  virtual void setSpellTarget(Target target) = 0;
  virtual void moveCombatUnit(combat::CombatUnit* unit) = 0;
};

class CombatPlayerInterface
{
public:
  virtual void combatTurnBegun() = 0;
  virtual void combatTurnEnded() = 0;
};

class Player : public PlayerInterface, public CombatPlayerInterface
{
protected:
  std::list<City*> cities;
  std::list<Army*> armies;
  std::list<Hero*> heroes;
  std::list<ManaNode*> nodes;
  std::list<const SpellCast> spells;

  std::unordered_set<const Trait*> traits;

  SpellBook spellBook;
  Relations relations;
  const combat::Combat* combat;
  mutable FogMap *fogMap;

  u8 taxRate; // TODO: maybe enum

  s32 manaRatios[3];
  s32 fame;

  s32 goldGain, manaGain, foodGain;
  s32 goldUpkeep, manaUpkeep, foodUpkeep;
  s32 goldPool, manaPool;

  s32 researchGain;
  
  s32 castingSkillCounter;
  s32 castingSkillGained_;
  s32 availableMana;

  bool alive;

  Game *g;
  
public:
  Player(Game *game, std::string name, const Wizard& wizard, PlayerColor color, const Race* race, u16 mapWidth, u16 mapHeight);

  bool isAlive() const { return alive; }
  
  void spendGold(s32 amount) { goldPool -= amount; }
  
  s32 totalGoldPool() const { return goldPool; }
  s32 totalManaPool() const { return manaPool; }
  
  s32 goldDelta() const { return goldGain - goldUpkeep; }
  s32 manaDelta() const { return manaRatios[0] - manaUpkeep; }
  s32 foodDelta() const { return foodGain - foodUpkeep; }
  
  s32 castingSkillBase() const;
  s32 castingSkill() const;
  s32 castingSkillGained() const { return castingSkillGained_; }
  s32 manaRatio(size_t index) const { return manaRatios[index]; }
  s32 getAvailableMana() const { return availableMana; }
  s32 getManaGain() const { return manaGain; }
  
  const TaxRate& getTaxRate();
  s32 getFame() const { return fame; }
  
  void alchemy(s32 gold, s32 mana) { goldPool += gold; manaPool -= mana; }
  
  const combat::Combat* getCombat() const { return combat; }
  void setCombat(combat::Combat* combat) { this->combat = combat; }
  
  s32 baseResearchPoints() const { return researchGain + manaRatios[1]; }
  s32 researchPoints() const;
  
  void setManaRatios(s32 m, s32 r, s32 s) { manaRatios[0] = m; manaRatios[1] = r; manaRatios[2] = s; }
  
  void combatCast(const Spell* spell) { spellBook.combatCast(spell); }

  void add(City* city) { cities.push_back(city); }
  void add(Army* army) { armies.push_back(army); }
  void add(Hero* hero) { heroes.push_back(hero); }
  void add(ManaNode* node) { nodes.push_back(node); } // TODO: here we store a raw pointer of a unique_ptr allocated object
  void add(const SpellCast& spell) { spells.push_back(spell); }
  const cast_list& getSpells() const { return spells; }
  
  void updateCities();
  void growCities();
  
  void fameGain(s32 fame) { this->fame += fame; }
  void fameLoss(s32 fame) { this->fame -= fame; }

  void turnBeginArmies();
  void refreshArmies();
  void remove(Army* army) { armies.remove(army); }
  
  SpellBook* book() { return &spellBook; }
  const SpellBook* book() const { return &spellBook; }
  
  Relations* getRelations() { return &relations; }
  
  const std::list<Hero*>& getHeroes() const { return heroes; }
  const std::list<Army*>& getArmies() const { return armies; }
  const std::list<City*>& getCities() const { return cities; }
  
  s16 globalSkillSpellsCount(const Unit* u) const;
  const SkillGlobalSpell* nthGlobalSkillSpell(u16 i, const Unit* u) const;
  bool hasSpell(const GlobalSpell* spell) const;

  
  FogMap* fog() const { return fogMap; }
  
  Game *game() const { return g; }
  
  
  City* cityWithFortress() const;
  City* cityWithSummoningCircle() const;
  
  bool hasTrait(const std::string& ident) const {
    //TODO: Data::isKeyValid<Trait>(ident)
    return std::find_if(traits.begin(), traits.end(), [&ident](const Trait* trait) { return trait->identifier == ident; }) != traits.end();
  }
  bool hasMastery(School school) const
  {
    return (school == School::NATURE && hasTrait("nature_mastery")) ||
      (school == School::CHAOS && hasTrait("chaos_mastery")) ||
      (school == School::SORCERY && hasTrait("sorcery_mastery"));
  }
  
  const std::string name;
  const Wizard& wizard;
  const PlayerColor color;
  const Race* const race;
    
  friend class PlayerMechanics;
};

#endif
