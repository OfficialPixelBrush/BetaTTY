#pragma once
#include "datatypes.h"
#include "defines.h"
#include <array>
#include <cstdint>
#include "helper.h"

class Chunk {
    std::array<int8_t, CHUNK_TOTAL> types;
    public:
        int8_t GetBlock(Int3 pos);
        void SetBlock(int8_t type, Int3 pos);
};