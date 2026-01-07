#include "chat.h"
#include "global.h"
#include <ncurses.h>

std::vector<std::pair<int64_t, std::string>>chatHistory;

void AddMessage(std::string message) {
    chatHistory.push_back(std::make_pair(worldTime,message));
}

int RenderChat() {
    int i = 0;

    for (auto it = chatHistory.rbegin(); it != chatHistory.rend(); ) {
        if (it->first <= worldTime - CHAT_ALIVE_TICKS) {
            // erase returns normal iterator, need base() for reverse_iterator
            it = decltype(it)(chatHistory.erase(std::next(it).base()));
        } else {
            mvwprintw(stdscr, max_height - 1 - i, 0, "%s", it->second.c_str());
            ++i;
            ++it;
        }
    }

    return 0;
}