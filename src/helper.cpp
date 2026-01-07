#include "helper.h"

std::size_t PosToIndex(const Int3& pos) {
    return pos.y + (pos.z * CHUNK_HEIGHT) + (pos.x * CHUNK_HEIGHT * CHUNK_WIDTH_Z);
}