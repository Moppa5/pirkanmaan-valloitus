#ifndef RESOURCEMAPS2_H
#define RESOURCEMAPS2_H

#include "core/basicresources.h"
#include "core/resourcemaps.h"

using namespace Course;

namespace Game {

const Course::ResourceMap STARTING_RESOURCEMAP = {
    {MONEY, 200},
    {FOOD, 200},
    {WOOD, 200},
    {STONE, 200},
    {ORE, 0}
};

const Course::ResourceMapDouble RESOURCEMAP_NEGATIVE = {
    {MONEY, -1},
    {FOOD, -1},
    {WOOD, -1},
    {STONE, -1},
    {ORE, -1}
};

const Course::ResourceMap RESOURCEMAP_ZERO = {
    {MONEY, 0},
    {FOOD, 0},
    {WOOD, 0},
    {STONE, 0},
    {ORE, 0}
};

const Course::ResourceMapDouble RESOURCEMAP_ZERO_DOUBLE = {
    {MONEY, (double)0},
    {FOOD, (double)0},
    {WOOD, (double)0},
    {STONE, (double)0},
    {ORE, (double)0}
};

const ResourceMap CLAIM_COST = {
    {MONEY, -25}
};

const ResourceMap FOREST_BP = {
    {MONEY, 2},
    {FOOD, 0},
    {WOOD, 5},
    {STONE, 0},
    {ORE, 0}
};

const ResourceMap GRASSLAND_BP = {
    {MONEY, 2},
    {FOOD, 5},
    {WOOD, 0},
    {STONE, 0},
    {ORE, 0}
};

const Course::ResourceMap MOUNTAIN_BP = {
    {MONEY, 5},
    {FOOD, 0},
    {WOOD, 0},
    {STONE, 5},
    {ORE, 3}
};

const Course::ResourceMap LAKE_BP = {
    {MONEY, 2},
    {FOOD, 8},
    {WOOD, 0},
    {STONE, 0},
    {ORE, 0}
};

const Course::ResourceMap OCEAN_BP = {
    {MONEY, 2},
    {FOOD, 8},
    {WOOD, 0},
    {STONE, 0},
    {ORE, 0}
};

// Building - Mine
const ResourceMap MINE_BUILD_COST = {
    {MONEY, 100},
    {FOOD, 150},
    {WOOD, 200},
    {STONE, 100}
};
const ResourceMap MINE_PRODUCTION = {
    {MONEY, 5},
    {STONE, 8},
    {ORE, 8}
};
// Building - Lake Cottage
const ResourceMap LAKE_COTTAGE_BUILD_COST = {
    {MONEY, 50},
    {FOOD, 50},
    {WOOD, 100},
    {STONE, 50}
};
const ResourceMap LAKE_COTTAGE_PRODUCTION = {
    {MONEY, 4},
    {FOOD, 8}
};

// Building - Fishing Boat
const ResourceMap FISHING_BOAT_BUILD_COST = {
    {MONEY, 50},
    {FOOD, 100},
    {WOOD, 25}
};
const ResourceMap FISHING_BOAT_PRODUCTION = {
    {MONEY, 2},
    {FOOD, 10}
};

const ResourceMap SHOP_VALUES = {
    {FOOD, 0},
    {WOOD, 0},
    {STONE, 0},
    {ORE, 0}
};

// BasicWorker
const ResourceMapDouble BW_WORKER_EFFICIENCY = {
    {MONEY, 1.00},
    {FOOD, 1.00},
    {WOOD, 0.75},
    {STONE, 0.50},
    {ORE, 0.50}
};

// Farmer - Cost and efficiency
const ResourceMap FARMER_RECRUITMENT_COST = {
    {MONEY, 25},
    {FOOD, 25}
};

const ResourceMapDouble FARMER_EFFICIENCY = {
    {MONEY, 1.00},
    {FOOD, 2.00},
    {WOOD, 0.50},
    {STONE, 0.5},
    {ORE, 0.5}
};

// Miner - Cost and efficiency
const ResourceMap MINER_RECRUITMENT_COST = {
    {MONEY, 10},
    {FOOD, 30},
    {STONE, 25},
    {ORE, 25}
};

const ResourceMapDouble MINER_EFFICIENCY = {
    {MONEY, 2.0},
    {FOOD, 0.5},
    {WOOD, 0.5},
    {STONE, 2},
    {ORE, 2}
};

}

#endif // RESOURCEMAPS2_H
