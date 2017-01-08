#include "YAML.h"

#include <iostream>

#include "yaml-cpp/yaml.h"
#include "Platform.h"

#include "Data.h"
#include "GfxData.h"
#include "Localization.h"

#include "UnitSpec.h"
#include "Skill.h"
#include "Effects.h"

#define FETCH_OR_FAIL(map, n) do { \
  auto it = map.find(n.asString()); \
  if (it != map.end()) return it->second; \
  assert(false); \
  } while(false)

using namespace YAML;
using namespace yaml;

using NNN = YAML::Node;

bool operator==(const N& n, const std::string& string) { return n.Scalar() == string; }

N yaml::parse(const std::string& fileName)
{
  return N(YAML::LoadFile(yamlPath(fileName)));
}

std::string yaml::yamlPath(const std::string& fileName)
{
  return Platform::instance()->getResourcePath() + "/data/yaml/" + fileName;
}

template<typename T, typename std::enable_if<!std::is_pointer<T>::value, int>::type> T yaml::parse(const N& node)
{
  return node.as<T>();
}

template<> LBXID yaml::parse(const N& node)
{
  static const std::unordered_map<std::string, LBXID> mapping = {
    { "special", LBXID::SPECIAL },
    { "special2", LBXID::SPECIAL2 }
  };
  
  FETCH_OR_FAIL(mapping, node);
}

template<> Property yaml::parse(const N& node)
{
  static const std::unordered_map<std::string, Property> mapping = {
    { "to_hit", Property::TO_HIT },
    { "melee", Property::MELEE },
    { "defense", Property::SHIELDS },
    { "defense_ranged", Property::SHIELDS_RANGED },
    { "defense_chaos", Property::SHIELDS_CHAOS },
    { "defense_sorcery", Property::SHIELDS_SORCERY },
    { "defense_nature", Property::SHIELDS_NATURE },
    { "defense_life", Property::SHIELDS_LIFE },
    { "defense_death", Property::SHIELDS_DEATH },
    { "resistance", Property::RESIST },
    { "hits", Property::HIT_POINTS },
    { "figures", Property::FIGURES }
  };
  
  FETCH_OR_FAIL(mapping, node);
}

template<> skills::Type yaml::parse(const N& node)
{
  if (node == "native")
    return skills::Type::NATIVE;
  else
    assert(false);
}

template<typename T> void yaml::parse(const N& node, std::vector<T>& dest)
{
  assert(node.IsSequence());
  //dest.reserve(node.size());
  for (const auto& entry : node)
    dest.push_back(parse<T>(entry));
}

template<typename T> T yaml::optionalParse(const N& node, T def)
{
  return node.IsDefined() ? parse<T>(node) : def;
}

template<> void yaml::parse(const N& node, SpriteInfo& v)
{
  assert(node.IsSequence());
  if (node[0] == "lbx")
    v = SpriteInfo(parse<LBXID>(node[1]), node[2]);
  else
    assert(false);
}

template<> const SkillEffect* yaml::parse(const N& node)
{
  const std::string& type = node["type"];
  
  if (type == "unit_bonus")
  {
    Property property = parse<Property>(node["property"]);
    s16 value = parse<s16>(node["value"]);
    return new UnitBonus(property, value);
  }
  else
  {
    assert(false);
    return nullptr;
  }
}

template<> const Skill* yaml::parse(const N& node)
{
  skills::Type type = optionalParse(node["type"], skills::Type::NATIVE);
  
  effect_list effects;
  parse(node["effects"], effects);
  
  auto visuals = node["visuals"];
  
  skills::VisualInfo visualInfo;
  visualInfo.name = i18n::keyForString(visuals["i18n"]);
  visualInfo.hideValue =  optionalParse(visuals["hideValue"], false);
  parse(visuals["icon"], visualInfo.icon);
  
  return new skills::ConcreteSkill(type, effects, visualInfo);
}


void yaml::parseSkills()
{
  N file = parse("skills.yaml");
  auto skills = file["skills"];
  
  for (const auto& yskill : skills)
  {
    const std::string& identifier = yskill["identifier"];
    const Skill* skill = parse<const Skill*>(yskill);
    Data::registerData(identifier, skill);
  }
}

void yaml::parseLocalization()
{
  N file = parse("english.yaml");
  auto entries = file["entries"];
  
  for (const auto entry : entries)
  {
    for (const auto& map : entry)
    {
      const std::string& key = map.first;
      const std::string& value = map.second;
      i18n::mapCustomEntry(key, value);
    }
  }

}

void yaml::parse()
{
  parseLocalization();
  parseSkills();
  
  Data::getInfo<const Skill*>();
}