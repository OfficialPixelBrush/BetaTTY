#include "chunk.h"
#include "chat.h"
#include "datatypes.h"
#include "defines.h"
#include <cstdint>
#include <iostream>
#include <utility>

void Chunk::GenerateHeightmap() {
    for (int8_t x = 0; x < CHUNK_WIDTH_X; x++) {
        for (int8_t z = 0; z < CHUNK_WIDTH_Z; z++) {
            heightmap[z + x * CHUNK_WIDTH_Z] = CalculateHeight(Int2{x,z});
        }
    }
    dirty = false;
}

int8_t Chunk::CalculateHeight(Int2 pos) {
    for (int8_t y = int8_t(CHUNK_HEIGHT-1); y >= 0; y--) {
        if (GetBlock(Int3{pos.x, y, pos.y}) != 0) {
            heightmap[pos.y + pos.x * CHUNK_WIDTH_Z] = y;
            return y;
        }
    }
    return 0;
}

int8_t Chunk::GetBlock(Int3 pos) {
    int32_t index = PosToIndex(pos);
    if (index < 0) return 0;
    return types[index];
}

int8_t Chunk::GetHeight(Int2 pos) {
    if (dirty) GenerateHeightmap();
    return heightmap[pos.y + pos.x * CHUNK_WIDTH_Z];
}

std::pair<int8_t, int8_t> Chunk::GetTopBlock(Int2 pos) {
    int8_t h = GetHeight(pos);
    return std::make_pair(
        GetBlock(Int3{pos.x, h, pos.y}),
        h
    );
}

void Chunk::SetBlock(int8_t type, Int3 pos) {
    types[PosToIndex(pos)] = type;
    dirty = true;
}