#include "world.h"
#include "chat.h"
#include "datatypes.h"
#include "defines.h"
#include "global.h"
#include "lut.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
std::unordered_map<int64_t, Chunk> chunks;

int64_t GetChunkId(Int2 pos) {
    return (int64_t(pos.x) << 32) | pos.y;
}

Int2 GetChunkPos(int64_t index) {
    return Int2{
        int32_t(index >> 32),
        int32_t(index & ((1ULL << 32) - 1))
    };
}

int8_t GetSolidHeight(Int2 pos) {
    Int2 cPos = Int2{pos.x / CHUNK_WIDTH_X, pos.y / CHUNK_WIDTH_Z};
    for(int8_t y = CHUNK_HEIGHT-1; y >= 0; y--) {
        int8_t type = GetBlock(Int3{pos.x,y,pos.y});
        if (IsSolid(type)) {
            return y;
        }
    }
    return 0;
}

int8_t GetTopBlock(Int2 pos) {
    Int2 cPos = Int2{pos.x / CHUNK_WIDTH_X, pos.y / CHUNK_WIDTH_Z};
    for(int8_t y = CHUNK_HEIGHT-1; y >= 0; y--) {
        int8_t type = GetBlock(Int3{pos.x,y,pos.y});
        if (type != 0) {
            return type;
        }
    }
    return 0;
}

int8_t GetBlock(Int3 pos) {
    Int2 cPos = Int2{
    pos.x >= 0 ? pos.x / CHUNK_WIDTH_X : (pos.x - CHUNK_WIDTH_X + 1) / CHUNK_WIDTH_X,
    pos.z >= 0 ? pos.z / CHUNK_WIDTH_Z : (pos.z - CHUNK_WIDTH_Z + 1) / CHUNK_WIDTH_Z
    };
    int64_t id = GetChunkId(cPos);
    auto it = chunks.find(id);
    if (it == chunks.end()) return 0;
    Chunk& c = it->second;
    return c.GetBlock(
        Int3{
            pos.x & 15,
            pos.y,
            pos.z & 15,
        }
    );
    return 0;
}

void SetBlock(int8_t type, Int3 pos) {
    auto c = &chunks[GetChunkId(Int2{pos.x / CHUNK_WIDTH_X, pos.z / CHUNK_WIDTH_Z})];
    if (!c) return;
    c->SetBlock(
        type,
        Int3{
            pos.x & 15,
            pos.y,
            pos.z & 15,
        }
    );
}