//
//  Effects.h
//  OpenMoM
//
//  Created by Jack on 7/21/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once

#include "common/Common.h"

#include <vector>
#include <stack>

enum class Property : u8;
class Unit;
class SkillEffect;

using SkillEffectGroupParam = value_t;
struct SkillEffectGroup
{
public:
  enum class Mode : u16 { KEEP_ALL, UNIQUE, KEEP_GREATER, KEEP_LESSER, PRIORITY };
 
private:
  Mode _mode;
  const SkillEffect* _specific;
  
public:
  SkillEffectGroup(Mode mode) : _mode(mode), _specific(nullptr) { }
  
  const SkillEffect* master() { return _specific; }
  Mode mode() const { return _mode; }
};

enum class Order { GREATER, LESSER, EQUAL, UNCOMPARABLE, DIFFERENT };

struct ModifierValue
{
  enum Priority { BASE, FIRST, ANY, LAST } priority;
  enum Type { ADDITIVE, ADDITIVE_LEVEL_BASED, MULTIPLICATIVE, FIXED  } type;
  union {
    float multiplier;
    value_t value;
  };

  ModifierValue(Type type, float value, Priority priority = Priority::ANY) : priority(priority), type(type), multiplier(value) 
  { 
    assert(isFloating());
  }

  ModifierValue(Type type, value_t value, Priority priority = Priority::ANY) : priority(priority), type(type), value(value) 
  { 
    assert(!isFloating());
  }

  ModifierValue(value_t value) : priority(Priority::ANY), type(Type::ADDITIVE), value(value) { }
  ModifierValue(float multiplier) : priority(Priority::ANY), type(Type::ADDITIVE_LEVEL_BASED), multiplier(multiplier) { }

  value_t transformValue(value_t value, const Unit* unit) const;
  
  bool isFloating() const { return type == Type::MULTIPLICATIVE || type == Type::ADDITIVE_LEVEL_BASED; }
  value_t truncatedValue() const { return isFloating() ? multiplier : value; }
 
  Order compareMagnitude(const Unit* unit, const ModifierValue& other) const
  {
    value_t v1 = transformValue(0, unit), v2 = transformValue(0, unit);
    
    assert(type == Type::ADDITIVE || type == Type::ADDITIVE_LEVEL_BASED);
    assert(other.type == Type::ADDITIVE || other.type == Type::ADDITIVE_LEVEL_BASED);

    if (v1 > v2) return Order::GREATER;
    else if (v1 < v2) return Order::LESSER;
    else return Order::EQUAL;
  }

  Order compareForSorting(const ModifierValue& other) const
  {
    if (priority < other.priority) return Order::LESSER;
    else if (priority > other.priority) return Order::GREATER;
    else
    {
      if (type < other.type) return Order::LESSER;
      else if (type > other.type) return Order::GREATER;
      else return Order::EQUAL;
    }

  }

  bool operator<(const ModifierValue& other) { return compareForSorting(other) == Order::LESSER; }
};

class SkillEffect
{
public:
  
protected:
  const SkillEffectGroup* _group;
  SkillEffectGroupParam _groupParam;
  
public:
  const enum class Type : u8
  {
    MOVEMENT,
    DISALLOW_MOVEMENT,
    
    ABILITY,
    PARAMETRIC_ABILITY,
    IMMUNITY,
    MAGIC_WEAPONS,
    
    COMBAT_BONUS,
    UNIT_BONUS,
    ARMY_BONUS,
    
    GRANT_SPELL,
    
    SPECIAL_ATTACK,
    
    WIZARD_BONUS,

    COMPOUND
  } type;
  
  
  SkillEffect(Type type, const SkillEffectGroup* group) : type(type), _group(group) { }
  SkillEffect(Type type) : SkillEffect(type, nullptr) { }
  
  virtual bool isModifier() const { return false; }
  virtual bool isCompound() const { return false; }
  virtual size_t size() const { return 1; }

  void setGroup(const SkillEffectGroup* group, SkillEffectGroupParam param = 0) { this->_group = group; this->_groupParam = param; }
  const SkillEffectGroup* group() const { return _group; }
  SkillEffectGroupParam groupParam() const { return _groupParam;  }

  virtual Order compare(const Unit* unit, const SkillEffect* other) const { return Order::UNCOMPARABLE; }
  
  template<typename T> const T* as() const { return static_cast<const T*>(this); }
};

template<typename T, SkillEffect::Type TYPE>
class SkillEnumEffect : public SkillEffect
{
private:
  const T _subType;
public:
  SkillEnumEffect(T type) : SkillEffect(TYPE), _subType(type) { }
  
  bool operator==(T type) const { return _subType == type; }
  T subType() const { return _subType; }
  
  Order compare(const Unit* unit, const SkillEffect* other) const override
  {
    if (other->type == type)
      return other->as<SkillEnumEffect<T, TYPE>>()->subType() == subType() ? Order::EQUAL : Order::DIFFERENT;
    else
      return Order::UNCOMPARABLE;
  }
};

class SimpleEffect : public SkillEffect
{
public:
  const enum class Type : u16
  {
    INVISIBILITY,
    
    
    IMMUNITY_MAGIC,
    IMMUNITY_ILLUSIONS,
    IMMUNITY_MISSILE,
    
    
    POISON,
    LIFE_STEALING,
    STONE_TOUCH,
    
    IMMOLATION,

    HEALER, 
    
    CREATE_OUTPOST,
    CREATE_ROAD,
    MELD_NODE,
    WALL_CRUSHING,
    PURIFY,
    
    FIRST_STRIKE,
    NEGATE_FIRST_STRIKE,
    ARMOR_PIERCING,
    LONG_RANGE,
    ILLUSIONARY_ATTACK,
    
    ALLOW_MELEE_ATTACKS_AGAINST_FLYING
  } effect;
  
  SimpleEffect(SkillEffect::Type type, Type effect) : SkillEffect(type), effect(effect) { }
};

class SimpleParametricEffect : public SimpleEffect
{
public:
  SimpleParametricEffect(SkillEffect::Type type, Type effect, value_t param) : SimpleEffect(type, effect), param(param) { }
  const value_t param;
};

class ModifierEffect : public SkillEffect
{
protected:
  ModifierValue _value;
  predicate<const Unit*> _predicate;
  value_t transformValue(value_t value, const Unit* unit) const { return _value.transformValue(value, unit); }

public:
  ModifierEffect(SkillEffect::Type type, ModifierValue value) : SkillEffect(type), _value(value), _predicate([](auto unit) { return true; }) { }
  ModifierEffect(SkillEffect::Type type, ModifierValue value, predicate<const Unit*> predicate) : SkillEffect(type), _value(value), _predicate(predicate) { }

  const ModifierValue& modifier() const { return _value; }
  bool isModifier() const override { return true; }

  Order compare(const Unit* unit, const SkillEffect* other) const override
  {
    //TODO: this doesn't check if kind of modifier is the same so it should be used only when this is sure (eg in an yaml defined SkillGroup) 
    if (other->isModifier())
    {
      return modifier().compareMagnitude(unit, other->as<ModifierEffect>()->modifier());
    }
    else
      return Order::UNCOMPARABLE;
  }
};

template<typename EnumType, SkillEffect::Type SkillType>
class PropertyModifierEffect : public ModifierEffect
{
private:
  EnumType _property;

public:
  PropertyModifierEffect(EnumType property, ModifierValue value) : ModifierEffect(SkillType, value), _property(property) { }
  PropertyModifierEffect(EnumType property, ModifierValue value, predicate<const Unit*> predicate) : ModifierEffect(SkillType, value, predicate), _property(property) { }

  PropertyModifierEffect(EnumType property, ModifierValue value, School school) : PropertyModifierEffect(property, value, [school](const Unit* unit) { return unit->school() == school; }) { }

  bool isAffecting(EnumType property) const { return _property == property; }

  value_t transformValue(EnumType property, value_t value, const Unit* unit) const
  {
    return property == _property ? _value.transformValue(value, unit) : value;
  }
};

using WizardAttributeModifier = PropertyModifierEffect<WizardAttribute, SkillEffect::Type::WIZARD_BONUS>;
using UnitPropertyBonus = PropertyModifierEffect<Property, SkillEffect::Type::UNIT_BONUS>;
using ArmyPropertyBonus = PropertyModifierEffect<Property, SkillEffect::Type::ARMY_BONUS>;

class CombatBonus : public SkillEffect
{
public:
  enum class Phase : u8
  {
    BOTH,
    ATTACKING,
    DEFENDING
  };
  
  enum class Target : u8
  {
    ATTACKER,
    DEFENDER,
  };
  
  CombatBonus(Property property, value_t value, Phase trigger, Target target, bool boundToSkill) : SkillEffect(SkillEffect::Type::COMBAT_BONUS), property(property), value(value), trigger(trigger), target(target), boundToSkill(boundToSkill) { }
  
  const Phase trigger;
  const Target target;
  const Property property;
  const value_t value;
  const bool boundToSkill; // TODO wtf?
};


enum class MovementType
{
  NORMAL = 0,
  FORESTWALK,
  FLYING,
  UNDERGROUND,
  MOUNTAINWALK,
  NON_CORPOREAL,
  PATH_FINDER,
  PLANAR_TRAVEL,
  TELEPORT,
  SWIMMING,
  WINDWALK,
  SAILING,
};

using MovementEffect = SkillEnumEffect<MovementType, SkillEffect::Type::MOVEMENT>;
using MovementDisallowEffect = SkillEnumEffect<MovementType, SkillEffect::Type::DISALLOW_MOVEMENT>;

enum SpecialAttackType
{
  THROWN_ATTACK,
  
  FIRE_BREATH,
  
  POISON_TOUCH
};

class SpecialAttackEffect : public SkillEnumEffect<SpecialAttackType, SkillEffect::Type::SPECIAL_ATTACK>
{
private:
  value_t _strength;
  
public:
  SpecialAttackEffect(SpecialAttackType type, value_t strength) : SkillEnumEffect(type), _strength(strength) { }
  
  value_t strength() const { return _strength; }
};

//TODO: technically is a combat instant spell
//TODO: implement mechanics
class Spell;
class SpellGrantEffect : public SkillEffect
{
private:
  const Spell* _spell;
  const value_t _times;
  const value_t _strength;
  
public:
  SpellGrantEffect(const Spell* spell, const value_t times, const value_t strength = 0) : SkillEffect(SkillEffect::Type::GRANT_SPELL), _spell(spell), _times(times), _strength(strength) { }
  
  const Spell* spell() const { return _spell; }
  value_t times() const { return _times; }
  value_t strength() const { return _strength; }
};

using effect_init_list = const std::initializer_list<const SkillEffect*>;
struct effect_list;
struct effect_list_deep_iterator;

struct effect_list
{
private:
  using inner_type_t = std::vector<const SkillEffect*>;
  inner_type_t data;
  
public:
  using iterator = inner_type_t::const_iterator;
  using value_type = inner_type_t::value_type;
  
  effect_list() { }
  effect_list(const inner_type_t& effects) : data(effects) { }
  effect_list(const effect_init_list& list) : data(list) { }
  
  void push_back(const SkillEffect* effect) { data.push_back(effect); }
  void resize(size_t size) { data.resize(size); }
  
  iterator begin() const { return data.begin(); }
  iterator end() const { return data.end(); }
  
  effect_list_deep_iterator dbegin() const;
  effect_list_deep_iterator dend() const;
  
  size_t size() const { return data.size(); }
  size_t flatSize() const { return std::accumulate(data.begin(), data.end(), 0UL, [](size_t v, const SkillEffect* effect) { return v + effect->size(); }); }

  void filter(std::function<bool(const SkillEffect*)> predicate)
  {
    auto nend = std::remove_if(data.begin(), data.end(), [&predicate](const SkillEffect* effect) { return !predicate(effect); });
    data.erase(nend, data.end());
  }
  
  const SkillEffect* operator[](size_t index) const { return data[index]; }
  
  effect_list operator+(const effect_list& other) const {
    effect_list result;
    auto& rdata = result.data;
    rdata.reserve(data.size() + other.data.size());
    rdata.insert(rdata.end(), data.begin(), data.end());
    rdata.insert(rdata.end(), other.data.begin(), other.data.end());
    return result;
  }
  
  effect_list& operator+=(const effect_list& other) {
    data.reserve(data.size()+other.data.size());
    data.insert(data.end(), other.data.begin(), other.data.end());
    return *this;
  }

  template<typename EnumType, SkillEffect::Type Type> 
  value_t reduceAsModifier(EnumType property, const Unit* unit, value_t base = 0) const
  {
    //TODO: begin or deep begin?
    return std::accumulate(begin(), end(), base, [property, unit](value_t v, const SkillEffect* effect) {
      return effect->as<PropertyModifierEffect<EnumType, Type>>()->transformValue(property, v, unit);
    });
  }

  void sort() { std::sort(data.begin(), data.end()); }

  /* flatten nested effects */
  effect_list flatten();
  
  /* this method builds the actual effects taking into consideration
   that some effects override or replace others */
  effect_list actuals(const Unit* unit) const;
};

//TODO: to remove after hardcoded effects has been removed
static const effect_list unit_bonus_build(std::initializer_list<Property> properties, value_t value)
{
  effect_list effects;
  effects.resize(properties.size());
  std::transform(properties.begin(), properties.end(), std::back_inserter(effects), [&] (const Property& property) { return new UnitPropertyBonus(property, value); });
  return effects;
}

class CompoundEffect : public SkillEffect
{
private:
  effect_list effects;

public:
  CompoundEffect() : SkillEffect(SkillEffect::Type::COMPOUND) { }
  CompoundEffect(SkillEffectGroup* group) : SkillEffect(SkillEffect::Type::COMPOUND, group) { }
  CompoundEffect(const effect_list& effects) : SkillEffect(SkillEffect::Type::COMPOUND), effects(effects) { }

  bool isCompound() const override { return true; }
  size_t size() const override { return effects.size(); }

  effect_list::iterator begin() const { return effects.begin(); }
  effect_list::iterator end() const { return effects.end(); }

  friend class effect_list_deep_iterator;
};

struct effect_list_deep_iterator
{
private:
  struct stack_v {
    const effect_list* list;
    effect_list::iterator it;
    bool end() const { return it == list->end(); }
    const SkillEffect* operator*() const { return *it; }
    stack_v(const effect_list* list, effect_list::iterator it) : list(list), it(it) { }
    bool operator==(const stack_v& other) const { return list == other.list && it == other.it; }
  };
  
  std::stack<stack_v> stack;
  
  void adjust()
  {
    if (stack.top().end())
      return;
    
    const SkillEffect* c = operator*();
    
    while (c && c->isCompound())
    {
      const CompoundEffect* ce = c->as<CompoundEffect>();
      stack.emplace(&ce->effects, ce->effects.begin());
      adjust();
      
      if (!stack.top().end())
        c = operator*();
      else
      {
        stack.pop();
        break;
      }
    }
  }
  
public:
  using difference_type = ptrdiff_t;
  using value_type = SkillEffect * ;
  using reference = const SkillEffect&;
  using pointer = const SkillEffect*;
  using iterator_category = std::forward_iterator_tag;
  
public:
  effect_list_deep_iterator(const effect_list* list, effect_list::iterator it) { stack.emplace(list, it); adjust(); }
  
  effect_list_deep_iterator& operator++()
  {
    auto& it = stack.top().it;
    
    /* iterator is not at end */
    if (!stack.top().end())
    {
      ++it;
      adjust();
    }
    
    {
      /* iterator is end of current child: pop and advance parent */
      while (stack.top().end() && stack.size() > 1)
      {
        /* pop scope: we're at end */
        stack.pop();
        
        /* now we're on parent, we should advance it */
        ++stack.top().it;
      }
    }
    
    return *this;
  }
  
  bool operator==(const effect_list_deep_iterator& other) const { return stack == other.stack; }
  bool operator!=(const effect_list_deep_iterator& other) const { return stack != other.stack; }
  
  const SkillEffect* operator*() const { return *stack.top(); }
};

inline effect_list_deep_iterator effect_list::dbegin() const { return effect_list_deep_iterator(this, begin()); }
inline effect_list_deep_iterator effect_list::dend() const { return effect_list_deep_iterator(this, end()); }


struct modifier_list : public std::vector<const ModifierValue*>
{
public:
  modifier_list(const effect_list& effects)
  {

  }

  value_t get(const Unit* unit, value_t base = 0)
  {
    return std::accumulate(begin(), end(), base, [unit](value_t a, const ModifierValue* modifier) {
      return modifier->transformValue(a, unit);
    });
  }
};