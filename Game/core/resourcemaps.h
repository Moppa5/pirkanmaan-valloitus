#ifndef RESOURCEMAPS_H
#define RESOURCEMAPS_H

#include "basicresources.h"

namespace Course {

namespace ConstResourceMaps {

const ResourceMap EMPTY = {};

// Building - Farm
const ResourceMap FARM_BUILD_COST = {
    {BasicResource::MONEY, 50},
    {BasicResource::FOOD, 100},
    {BasicResource::WOOD, 25}
};
const ResourceMap FARM_PRODUCTION = {
    {BasicResource::MONEY, 1},
    {BasicResource::FOOD, 5}
};


// Building - HeadQuarters
const ResourceMap HQ_BUILD_COST = {
    {BasicResource::MONEY, 750},
    {BasicResource::FOOD, 1000},
    {BasicResource::WOOD, 500},
    {BasicResource::STONE, 250}
};
const ResourceMap HQ_PRODUCTION = {
    {BasicResource::MONEY, 10},
    {BasicResource::FOOD, 2},
};


// Building - Outpost
const ResourceMap OUTPOST_BUILD_COST = {
    {BasicResource::MONEY, 150},
    {BasicResource::FOOD, 200},
    {BasicResource::WOOD, 200},
    {BasicResource::STONE, 25}
};

const ResourceMap OUTPOST_PRODUCTION = {
    {BasicResource::MONEY, -5},
    {BasicResource::FOOD, -2}
};


// Worker - BasicWorker
const ResourceMapDouble BW_WORKER_EFFICIENCY = {
    {MONEY, 0.25},
    {FOOD, 1.00},
    {WOOD, 0.75},
    {STONE, 0.50},
    {ORE, 0.50}
};

const ResourceMap BW_RECRUITMENT_COST = {
    {MONEY, 10},
    {FOOD, 25}
};


const ResourceMap FOREST_BP = {
    {MONEY, 1},
    {FOOD, 3},
    {WOOD, 5},
    {STONE, 1},
    {ORE, 0},
};

const ResourceMap GRASSLAND_BP = {
    {MONEY, 2},
    {FOOD, 5},
    {WOOD, 1},
    {STONE, 1},
    {ORE, 0},
};


}
}
#endif // RESOURCEMAPS_H
