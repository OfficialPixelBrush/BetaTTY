#pragma once
#include "entity.h"
#include <unordered_map>

extern bool running;

extern Client client;
extern std::unordered_map<std::int32_t, Player> players;
extern int64_t worldTime;

// Max Terminal size
extern int max_width;
extern int max_height;