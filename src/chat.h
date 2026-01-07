#pragma once

#include <ncurses.h>
#include <string>
#include <vector>

#define CHAT_ALIVE_TICKS 200

extern std::vector<std::pair<int64_t, std::string>>chatHistory;

void AddMessage(std::string message);
int RenderChat();