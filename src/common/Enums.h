#pragma once

#include "common/Types.h"

#pragma mark Game Related

enum School : s8
{
  ARCANE = 0,
  CHAOS,
  DEATH,
  LIFE,
  NATURE,
  SORCERY,

  SCHOOL_COUNT,
  SCHOOL_NO_ARCANE_COUNT = 5,

  SCHOOL_FIRST = 1,
  SCHOOL_LAST = 5,

  NO_SCHOOL = -1
};

enum PlayerColor : u8
{
  GREEN = 0,
  BLUE,
  RED,
  PURPLE,
  YELLOW,
  NEUTRAL,
};


#pragma mark Map Related

enum Plane : u8
{
  ARCANUS = 0,
  MYRRAN,

  PLANE_COUNT
};

enum class TileType : u8
{
  NONE = 0,
  GRASS = 1,
  OCEAN,
  SHORE,
  MOUNTAIN,
  VOLCANO,
  HILL,
  FOREST,
  SWAMP,
  DESERT,
  TUNDRA,
  RIVER,
  RIVER_MOUTH,

};

static constexpr size_t TILE_TYPES = 12;

enum class Resource : u8
{
  ADAMANTIUM = 0,
  COAL,
  CRYSX_CRYSTAL,
  GEMS,
  GOLD,
  IRON_ORE,
  MITHRIL,
  NIGHT_SHADE,
  QOURK_CRYSTAL,
  SILVER,
  WILD_GAME,
  NONE
};

enum class PlaceType : u8
{
  TOWER_OF_WIZARDRY = 1,

  MYSTERIOUS_CAVE,
  DUNGEON,
  ANCIENT_TEMPLE,
  ABANDONED_KEEP,
  MONSTER_LAIR,
  RUINS,
  FALLEN_TEMPLE,

  MANA_NODE
};

#pragma mark Engine Related

enum ViewID
{
  VIEW_MAIN,
  VIEW_SPELL_BOOK,
  VIEW_MAGIC,
  VIEW_RESEARCH,
  VIEW_ALCHEMY,
  VIEW_ARMIES,
  VIEW_ARMIES_ITEMS,
  VIEW_ITEM_CRAFT,
  VIEW_ITEM_CRAFT_CHARGES,
  VIEW_MERCHANT,
  VIEW_CITY,
  VIEW_UNIT,
  VIEW_ARMY,
  VIEW_MESSAGE,
  VIEW_CITIES,
  VIEW_MIRROR,
  VIEW_INFO_MENU,
  VIEW_CARTOGRAPHER,
  VIEW_DIPLOMACY,
  VIEW_ASTROLOGER,
  VIEW_HISTORIAN,
  VIEW_COMBAT,
  VIEW_PRODUCTION,
  VIEW_OUTPOST,
  VIEW_NEW_GAME,
  VIEW_LOAD,
  VIEW_OPTIONS,
  VIEW_START,
  VIEW_INTRO,

  VIEW_MAP_EDITOR,

  VIEW_CONSOLE,
  VIEW_DATA,

  VIEW_COUNT
};

enum class LBXID : u8
{
  ARMYLIST = 0, BACKGRND, BOOK, BUILDDAT, BUILDESC,
  CHRIVER, CITYNAME, CITYSCAP, CITYWALL, CMBDESRC,
  CMBDESRT, CMBGRASC, CMBGRASS, CMBMAGIC, CMBMOUNC,
  CMBMOUNT, CMBTCITY, CMBTFX, CMBTSND, CMBTUNDC,
  CMBTUNDR, CMBTWALL, COMBAT, COMPIX, CONQUEST, DESC,
  DESERT, DIPLOMAC, DIPLOMSG, EVENTMSG, EVENTS, FIGURES1,
  FIGURES2, FIGURES3, FIGURES4, FIGURES5, FIGURES6,
  FIGURES7, FIGURES8, FIGURES9, FIGURE10, FIGURE11,
  FIGURE12, FIGURE13, FIGURE14, FIGURE15, FIGURE16,
  FONTS, HALOFAM, HELP, HERODATA, HIRE, HLPENTRY,
  INTRO, INTROSFX, INTROSND, ITEMDATA, ITEMISC,
  ITEMPOW, ITEMS, LILWIZ, LISTDAT, LOAD, LOSE, MAGIC,
  MAIN, MAINSCRN, MAPBACK, MESSAGE, MONSTER, MOODWIZ,
  MUSIC, NAMES, NEWGAME, NEWSOUND, PORTRAIT, RELOAD,
  RESOURCE, SCROLL, SNDDRV, SOUNDFX, SPECFX, SPECIAL,
  SPECIAL2, SPELLDAT, SPELLOSE, SPELLS, SPELLSCR,
  SPLMASTR, TERRAIN, TERRSTAT, TERRTYPE, TUNDRA,
  UNITS1, UNITS2, UNITVIEW, VORTEX, WALLRISE, WIN,
  WIZARDS, WIZLAB,

  COUNT
};