#include "chunk.h"
#include "chat.h"
#include "datatypes.h"
#include <iostream>

int8_t Chunk::GetBlock(Int3 pos) {
    int32_t index = PosToIndex(pos);
    if (index < 0) return 0;
    return types[index];
}

void Chunk::SetBlock(int8_t type, Int3 pos) {
    types[PosToIndex(pos)] = type;
}