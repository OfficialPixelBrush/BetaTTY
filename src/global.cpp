#include "global.h"

bool running = true;

Client client;
std::unordered_map<std::int32_t, Player> players;
int64_t worldTime = 0;

// Max Terminal size
int max_width;
int max_height;