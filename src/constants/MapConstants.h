#ifndef MAPCONSTANTS_H
#define MAPCONSTANTS_H

#include <string>
#include <unordered_map>

#include "../components/info/TileInfo.h"
#include "RenderConstants.h"

namespace ld
{

constexpr int NUM_CHUNKS_X(2);
constexpr int NUM_CHUNKS_Y(2);
constexpr int TILES_PER_CHUNK_X(12);
constexpr int TILES_PER_CHUNK_Y(12);
constexpr int MAP_SIZE_X(NUM_CHUNKS_X * TILES_PER_CHUNK_X);
constexpr int MAP_SIZE_Y(NUM_CHUNKS_Y * TILES_PER_CHUNK_Y);
constexpr int NUM_FLOORS(2);

static std::unordered_map<std::string, TileInfo> Tile_Data;

}

#endif /* MAPCONSTANTS_H */
