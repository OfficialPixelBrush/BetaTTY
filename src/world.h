#include <cstdint>
#include <unordered_map>
#include "chunk.h"
extern std::unordered_map<int64_t, Chunk> chunks;

int64_t GetChunkId(Int2 pos);
Int2 GetChunkPos(int64_t index);
int8_t GetBlock(Int3 pos);
void SetBlock(int8_t type, Int3 pos);
int8_t GetSolidHeight(Int2 pos);
int8_t GetTopBlock(Int2 pos);