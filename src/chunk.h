#pragma once
#include "datatypes.h"
#include "defines.h"
#include <array>
#include <cstdint>
#include "helper.h"

class Chunk {
    std::array<int8_t, CHUNK_TOTAL> types;
    std::array<int8_t, CHUNK_AREA_XY> heightmap;
    void GenerateHeightmap();
    int8_t CalculateHeight(Int2 pos);
    bool dirty = false;
    public:
        Chunk() { GenerateHeightmap(); }
        int8_t GetBlock(Int3 pos);
        int8_t GetHeight(Int2 pos);
        void SetBlock(int8_t type, Int3 pos);
        std::pair<int8_t, int8_t> GetTopBlock(Int2 pos);
};