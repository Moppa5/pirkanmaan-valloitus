#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include <QString>
#include <QColor>
#include <vector>

// Window Titles
const QString MAIN_TITLE = "Pirkanmaan Valloitus";
const QString DIALOG_TITLE = "Configure the Game";
const QString SCOREDIALOG_TITLE = "ScoreBoard";

// Image paths
const QString TEST_IMAGE = ":/images/images/test.png";
const QString FOREST_IMAGE = ":/images/images/Tiles/forest.png";
const QString GRASSLAND_IMAGE = ":/images/images/Tiles/grassland.png";
const QString MOUNTAIN_IMAGE = ":/images/images/Tiles/mountain.png";
const QString LAKE_IMAGE = ":/images/images/Tiles/lake.png";
const QString OCEAN_IMAGE = ":/images/images/Tiles/ocean2.png";
const QString FARMLAND_IMAGE = ":/images/images/Tiles/farmland.png";
const QString SHIP_IMAGE = ":/images/images/Buildings/ship.png";
const QString BUILDING_IMAGE = ":/images/images/Buildings/building.png";
const QString FARM_IMAGE = ":/images/images/Buildings/building.png";
const QString OUTPOST_IMAGE = ":/images/images/Buildings/outpost.png";
const QString HQ_IMAGE = ":/images/images/Buildings/headquarters.png";
const QString MINE_IMAGE = ":/images/images/Buildings/mine.png";
const QString LAKE_COTTAGE_IMAGE = ":/images/images/Buildings/cottage.png";
const QString WORKER_SINGLE_IMAGE = ":/images/images/Workers/worker.png";
const QString WORKER_DOUBLE_IMAGE = ":/images/images/Workers/worker_double.png";
const QString WORKER_TRIPLE_IMAGE = ":/images/images/Workers/worker_triple.png";

// Icon images
const QString WOOD_ICON = ":/images/images/Icons/logs.png";
const QString FOOD_ICON = ":/images/images/Icons/food.png";
const QString STONE_ICON = ":/images/images/Icons/stone.png";
const QString ORE_ICON = ":/images/images/Icons/iron.png";
const QString GOLD_ICON = ":/images/images/Icons/coins.png";

// UI values
const int MAX_SHOP_VALUE = 500;
const QColor HIGHLIGHT_COLOR = Qt::red;

// Buttons and elements changing
const QString GAME_BUTTON_START = "Start The Game";
const QString GAME_BUTTON_WHILE_GAME = "End Turn";
const QString PLAYERS_ADDED = "Players added";
const QString BUILDING_ADDED = "Building added";
const QString WORKER_ADDED = "Worker(s) added";
const QString NO_BUILDING_SELECTED = "Select building first";
const QString GAME_END = "Game Over";

// Error and other related messages
const QString EMPTY = "";
const QString TOO_LONG_NAME = "Name too long";
const QString PLAYERNAME_TAKEN = "Name already taken";
const QString PLAYERNAME_EMPTY = "Name can't be empty";
const QString COLOR_TAKEN = "Color already taken";
const QString COLOR = "Pick a color";

const QString NOT_ENOUGH_MONEY = "Not enough money";
const QString NOT_ENOUGH_RESOURCES = "Not enough resources";
const QString CLAIM_REQUIRED = "Tile not claimed";
const QString TOO_MANY_BUILDINGS = "Too many buildings";
const QString TOO_MANY_BUILDINGS_THIS_TURN = "Can't build more this turn";
const QString CANT_BE_BUILT = "Can't be built to this tile";
const QString NOT_OWNED_TILE = "You do not own this tile";
const QString ALREADY_OWNED_TILE = "Tile is already owned!";

// WorldGenerator
const int FOREST_RARITY = 10;
const int GRASSLAND_RARITY = 10;
const int MOUNTAIN_RARITY = 2;

// Game related consts
const int MAX_PLAYERNAME_LENGTH = 12;
const int DEFAULT_ROUND_COUNT = 30;
const int MIN_ROUND_COUNT = 10;
const int MAX_ROUND_COUNT = 1000;
const int MIN_MAP_WIDTH = 3;
const int MIN_MAP_HEIGHT = 2;

// Building and claim limits
const int MAX_CLAIMS_PER_TURN = 2;
const int MAX_BUILDINGS_PER_TURN = 2;

// Tiles
const QString GRASSLAND = "Grassland";
const QString FOREST = "Forest";
const QString LAKE = "Lake";
const QString MOUNTAIN = "Mountain";
const QString OCEAN = "Ocean";

// Buildings
const QString FARM = "Farm";
const QString HQ = "HeadQuarters";
const QString OUTPOST = "Outpost";
const QString MINE = "Mine";
const QString FISHING_BOAT = "Fishing Boat";
const QString LAKE_COTTAGE = "Lake Cottage";

// Workers
const QString WORKER_BASIC = "Basic Worker";
const QString WORKER_FARMER = "Farmer";
const QString WORKER_MINER = "Miner";
const QString WORKER_SABOTEUR = "Saboteur";

const std::pair<int, int> SCENE_SCALE_LIMITS = {50, 100};

#endif // CONSTANTS_HH

