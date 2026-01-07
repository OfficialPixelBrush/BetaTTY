#include "chunk.h"

int8_t Chunk::GetBlock(Int3 pos) {
    return types[PosToIndex(pos)];
}

void Chunk::SetBlock(int8_t type, Int3 pos) {
    types[PosToIndex(pos)] = type;
}