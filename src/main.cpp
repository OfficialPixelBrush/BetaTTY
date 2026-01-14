#include "datatypes.h"
#include "defines.h"
#include "helper.h"
#include "packet.h"
#include <chrono>
#include <cmath>
#include <cstring>
#include <iostream>
#include <ncurses.h>
#include <cstdint>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ratio>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include "enum.h"
#include <utility>
#include <vector>
#include "entity.h"
#include "global.h"
#include "chat.h"
#include "world.h"

struct BlockTex {
    // Block Icon
    char icon = ' ';
    // Foreground rgb
    short fg;
    // Background rgb
    short bg;
};

struct nCol {
    short r,g,b;
};

enum Col : short {
    COLOR_GREY   = 8,
    COLOR_BROWN  = 9,
    COLOR_BEIGE  = 10,
    COLOR_ORANGE = 11,
};

std::vector<BlockTex> blockTextures = {
    BlockTex{ ' ', COLOR_GREY, COLOR_GREY },
    BlockTex{ ' ', COLOR_GREY, COLOR_GREY },
    BlockTex{ '.', COLOR_GREEN, COLOR_GREEN },
    BlockTex{ '.', COLOR_GREY, COLOR_BROWN },
    BlockTex{ '#', COLOR_BLACK, COLOR_GREY },
    BlockTex{ '=', COLOR_BLACK, COLOR_BEIGE },
    BlockTex{ 't', COLOR_BROWN, COLOR_GREEN },
    BlockTex{ ' ', COLOR_BLACK, COLOR_BLACK },
    BlockTex{ '~', COLOR_CYAN, COLOR_BLUE },
    BlockTex{ '~', COLOR_CYAN, COLOR_BLUE },
    BlockTex{ '~', COLOR_RED, COLOR_ORANGE },
    BlockTex{ '~', COLOR_RED, COLOR_ORANGE },
    BlockTex{ '.', COLOR_YELLOW, COLOR_BEIGE },
    BlockTex{ '.', COLOR_WHITE, COLOR_GREY },
    BlockTex{ '*', COLOR_YELLOW, COLOR_GREY },
    BlockTex{ '*', COLOR_BEIGE, COLOR_GREY },
    BlockTex{ '*', COLOR_BLACK, COLOR_GREY },
    BlockTex{ 'O', COLOR_BEIGE, COLOR_BROWN },
    BlockTex{ '#', COLOR_GREEN, COLOR_BLACK },
    BlockTex{ ':', COLOR_BLACK, COLOR_YELLOW },
    BlockTex{ '/', COLOR_CYAN, COLOR_WHITE },
    BlockTex{ '*', COLOR_BLUE, COLOR_GREY },
    BlockTex{ ' ', COLOR_BLUE, COLOR_BLUE },
    BlockTex{ 'o', COLOR_BLACK, COLOR_GREY },
    BlockTex{ '=', COLOR_ORANGE, COLOR_YELLOW },
    BlockTex{ '#', COLOR_BLACK, COLOR_BROWN },
    BlockTex{ '[', COLOR_GREY, COLOR_WHITE },
    BlockTex{ '=', COLOR_RED, COLOR_BROWN },
    BlockTex{ '=', COLOR_ORANGE, COLOR_BROWN },
    BlockTex{ 'T', COLOR_GREEN, COLOR_GREY },
    BlockTex{ '#', COLOR_WHITE, COLOR_BLACK },
    BlockTex{ '^', COLOR_BLACK, COLOR_GREEN },
    BlockTex{ 't', COLOR_BROWN, COLOR_BEIGE },
    BlockTex{ 'T', COLOR_BROWN, COLOR_GREY },
    BlockTex{ 'T', COLOR_BROWN, COLOR_GREY },
    BlockTex{ ' ', COLOR_WHITE, COLOR_WHITE },
    BlockTex{ 'X', COLOR_RED, COLOR_BLACK },
    BlockTex{ 't', COLOR_YELLOW, COLOR_GREEN },
    BlockTex{ 't', COLOR_RED, COLOR_GREEN },
    BlockTex{ 'o', COLOR_BROWN, COLOR_GREY },
    BlockTex{ 'o', COLOR_RED, COLOR_GREY },
    BlockTex{ '/', COLOR_WHITE, COLOR_YELLOW },
    BlockTex{ '/', COLOR_WHITE, COLOR_GREY },
    BlockTex{ '-', COLOR_WHITE, COLOR_GREY },
    BlockTex{ '_', COLOR_WHITE, COLOR_GREY },
    BlockTex{ '#', COLOR_WHITE, COLOR_RED },
    BlockTex{ '=', COLOR_WHITE, COLOR_RED },
    BlockTex{ '=', COLOR_RED, COLOR_BROWN },
    BlockTex{ '#', COLOR_GREEN, COLOR_GREY },
    BlockTex{ '#', COLOR_MAGENTA, COLOR_BLACK },
    BlockTex{ 'i', COLOR_ORANGE, COLOR_YELLOW },
    BlockTex{ 'M', COLOR_RED, COLOR_ORANGE },
    BlockTex{ '#', COLOR_BLUE, COLOR_BLACK },
    BlockTex{ '/', COLOR_BLACK, COLOR_BEIGE },
    BlockTex{ '=', COLOR_BLACK, COLOR_ORANGE },
    BlockTex{ '~', COLOR_RED, COLOR_BLACK },
    BlockTex{ '*', COLOR_CYAN, COLOR_GREY },
    BlockTex{ '/', COLOR_WHITE, COLOR_CYAN },
    BlockTex{ 'W', COLOR_BEIGE, COLOR_BROWN },
    BlockTex{ 'm', COLOR_YELLOW, COLOR_BROWN },
    BlockTex{ '~', COLOR_BLACK, COLOR_BROWN },
    BlockTex{ 'n', COLOR_GREY, COLOR_BLACK },
    BlockTex{ 'n', COLOR_ORANGE, COLOR_BLACK },
    BlockTex{ 'T', COLOR_BEIGE, COLOR_BLACK },
    BlockTex{ 'D', COLOR_BEIGE, COLOR_BLACK },
    BlockTex{ 'H', COLOR_BEIGE, COLOR_BLACK },
    BlockTex{ '=', COLOR_GREY, COLOR_BROWN },
    BlockTex{ '/', COLOR_BLACK, COLOR_GREY },
    BlockTex{ 'T', COLOR_BEIGE, COLOR_BLACK },
    BlockTex{ '\\', COLOR_BROWN, COLOR_GREY },
    BlockTex{ 'X', COLOR_BLACK, COLOR_GREY },
    BlockTex{ 'D', COLOR_WHITE, COLOR_GREY },
    BlockTex{ 'X', COLOR_BLACK, COLOR_BEIGE },
    BlockTex{ '*', COLOR_RED, COLOR_GREY },
    BlockTex{ '*', COLOR_ORANGE, COLOR_GREY },
    BlockTex{ 'i', COLOR_RED, COLOR_BLACK },
    BlockTex{ 'i', COLOR_ORANGE, COLOR_RED },
    BlockTex{ '.', COLOR_GREY, COLOR_BLACK },
    BlockTex{ '.', COLOR_CYAN, COLOR_WHITE },
    BlockTex{ '/', COLOR_BLUE, COLOR_CYAN },
    BlockTex{ ' ', COLOR_WHITE, COLOR_WHITE },
    BlockTex{ 'O', COLOR_YELLOW, COLOR_GREEN },
    BlockTex{ '~', COLOR_BROWN, COLOR_GREY },
    BlockTex{ ':', COLOR_GREEN, COLOR_BEIGE },
    BlockTex{ 'o', COLOR_BLACK, COLOR_BROWN },
    BlockTex{ 'T', COLOR_BROWN, COLOR_GREEN },
    BlockTex{ '.', COLOR_GREEN, COLOR_ORANGE },
    BlockTex{ '#', COLOR_BLACK, COLOR_RED },
    BlockTex{ '~', COLOR_BLACK, COLOR_BROWN },
    BlockTex{ '#', COLOR_ORANGE, COLOR_YELLOW },
    BlockTex{ '~', COLOR_BLACK, COLOR_MAGENTA },
    BlockTex{ '.', COLOR_YELLOW, COLOR_ORANGE },
    BlockTex{ ':', COLOR_RED, COLOR_WHITE },
    BlockTex{ '>', COLOR_BLACK, COLOR_GREY },
    BlockTex{ '>', COLOR_RED, COLOR_GREY },
    BlockTex{ 'X', COLOR_RED, COLOR_BLACK },
    BlockTex{ '#', COLOR_BEIGE, COLOR_ORANGE },
};

void InitColors() {
    nCol cols[] = {
        nCol { 500, 500, 500 },
        nCol { 500,400, 200 },
        nCol { 900, 900, 800 },
        nCol { 900, 700, 0 },
    };
    short ci = COLOR_GREY;
    for (const auto &c : cols) {
        init_color(ci++, c.r, c.g, c.b);
    }
    short bi = 1;
    for (const auto &b : blockTextures) {
        init_pair(bi++, b.fg, b.bg);
    }
}

Int2 WorldToScreen(Int2 worldPos) {
    Int2 screenPos;
    screenPos.x = worldPos.x - int32_t(client.player.pos.x) + max_width / 2;
    screenPos.y = worldPos.y - int32_t(client.player.pos.z) + max_height / 2;
    return screenPos;
}

Int2 ScreenToWorld(Int2 screenPos) {
    Int2 worldPos;
    worldPos.x = int32_t(client.player.pos.x) + max_width / 2 - screenPos.x;
    worldPos.y = int32_t(client.player.pos.z) + max_height / 2 - screenPos.y;
    return worldPos;
}

// Render Block at presented screen coordinate
int RenderBlock(Int2 screenPos) {
    Int2 worldPos = ScreenToWorld(screenPos);
    int8_t type;
    std::pair<int8_t, int8_t> typeTop = GetTopBlock(Int2{worldPos.x, worldPos.y});
    type = typeTop.first;
    /*
    if (typeTop.second >= client.player.pos.y) {
        type = GetBlock(Int3{worldPos.x, int32_t(client.player.pos.y), worldPos.y});
        //color = 0;
    } else {
        type = typeTop.first;
    }
    */
    int32_t color = COLOR_BLACK;
    char icon = '/';
    if (type >= 0 && type < blockTextures.size()) {
        color = COLOR_PAIR(type+1);
        icon = blockTextures[type].icon;
    }
    attron(color);
    addch(icon);
    return 0;
}

int RenderWorld() {
    move(0,0);
    for (int y = 0; y < max_height; y++) {
        for (int x = 0; x < max_width; x++) {
            RenderBlock(
                Int2{
                    x,
                    y,
                }
            );
        }
    }
    return 0;
}

int RenderUI() {
    mvwprintw(stdscr, 0, 0, "%s", to_string(client.player.pos).c_str());
    mvwprintw(stdscr, 1, 0, "Time: %ld", worldTime);
    mvwprintw(stdscr, 2, 0, "Chunks: %zu", chunks.size());
    return 0;
}

int RenderEntities() {
    return 0;    
}

bool UpdateSize() {
    getmaxyx(stdscr, max_height, max_width);
    if (max_height < 0 || max_width < 0 || max_height < MINIMUM_SIZE_HEIGHT || max_width < MINIMUM_SIZE_WIDTH) {
        mvwprintw(stdscr,max_height/2,max_width/2,"Too small!");
        return false;
    }
    return true;
}

float GetDistance(Int3 posA, Int3 posB) {
    float dx = float(posB.x - posA.x);
    float dy = float(posB.y - posA.y);
    float dz = float(posB.z - posA.z);
    return std::sqrt((dx*dx) + (dy*dy) + (dz*dz));
}

int ProcessInputs(int cs) {
    int ch = getch();  // non-blocking input
    static std::string typingBuffer;
    
    if (client.isTyping) {
        mvaddch(max_height - 1, 0, '>');
        printw("%s", typingBuffer.c_str());
        if (ch != ERR) {
            if (ch == '\n') {
                // finished typing
                ChatMessagePacket cmPacket;
                cmPacket.message = typingBuffer;
                cmPacket.Send(cs);
                typingBuffer.clear();
                client.isTyping = false;
            } else if (ch == KEY_BACKSPACE || ch == 127) {
                if (!typingBuffer.empty()) {
                    typingBuffer.pop_back();
                }
            } else if (ch >= 32 && ch <= 126) {
                typingBuffer.push_back(char(ch));
            }
        }
        return 0;  // skip movement while typing
    }
    
    Int3 oldCursor = client.cursor;
    bool cursorMoved = false;
    
    switch (ch) {
        case 27:
            running = false;
            break;
        case 'w':
            client.player.pos.z += 1.0;
            client.hasMoved = true;
            break;
        case 's':
            client.player.pos.z -= 1.0;
            client.hasMoved = true;
            break;
        case 'a':
            client.player.pos.x += 1.0;
            client.hasMoved = true;
            break;
        case 'd':
            client.player.pos.x -= 1.0;
            client.hasMoved = true;
            break;
        case 't':
            client.isTyping = true;
            typingBuffer.clear();
            move(max_height - 1, 0);
            addch('>');
            move(max_height - 1, 1);
            break;
        case KEY_UP:
            client.cursor.z -= 1;
            cursorMoved = true;
            break;
        case KEY_DOWN:
            client.cursor.z += 1;
            cursorMoved = true;
            break;
        case KEY_LEFT:
            client.cursor.x -= 1;
            cursorMoved = true;
            break;
        case KEY_RIGHT:
            client.cursor.x += 1;
            cursorMoved = true;
            break;
    }
    
    // Clamp cursor to max reach
    float cursorDist = sqrtf(float(client.cursor.x * client.cursor.x + 
                                   client.cursor.y * client.cursor.y + 
                                   client.cursor.z * client.cursor.z));
    if (cursorDist > MAX_REACH) {
        float scale = MAX_REACH / cursorDist;
        client.cursor.x = int(float(client.cursor.x) * scale);
        client.cursor.y = int(float(client.cursor.y) * scale);
        client.cursor.z = int(float(client.cursor.z) * scale);
    }
    
    // Update player look direction whenever cursor moves
    if (cursorMoved || client.hasMoved) {
        // Compute world position of cursor relative to player
        Float3 target{
            float(client.player.pos.x) + float(client.cursor.x),
            float(client.player.pos.y) + float(client.cursor.y),
            float(client.player.pos.z) + float(client.cursor.z)
        };
        
        // Compute look direction toward cursor
        Float3 dir{
            target.x - float(client.player.pos.x),
            target.y - float(client.player.pos.y),
            target.z - float(client.player.pos.z)
        };
        
        // Calculate yaw and pitch
        client.player.rot.y = atan2(dir.z, dir.x);  // yaw
        float horizontalDist = sqrt(dir.x * dir.x + dir.z * dir.z);
        client.player.rot.x = -atan2(dir.y, horizontalDist);  // pitch
    }
    
    if (cursorMoved || client.hasMoved) {
        client.player.pos.y = double(1 + GetSolidHeight(Int2{
            int32_t(client.player.pos.x - 0.5),
            int32_t(client.player.pos.z)
        }));
        
        PlayerPositionLookPacket pplPacket;
        pplPacket.pos = client.player.pos;
        pplPacket.cameraY = client.player.cameraY;
        pplPacket.rot = Float2{client.player.rot.x, client.player.rot.y};
        pplPacket.onGround = true;
        pplPacket.Send(cs);
    }
    
    return 0;
}

int main(int argc, char *argv[]) {
    std::string server_ip_string = "127.0.0.1"; // Defaults to localhost
    uint32_t server_ip = 0;
    uint16_t server_port = 25565; // Defaults to default MC port
    // Only IP
    if (argc > 1) {
        server_ip_string = std::string(argv[1]);
    }
    if (inet_pton(AF_INET, server_ip_string.c_str(), &server_ip) != 1) {
        std::cerr << "Invalid IP address\n";
        return 1;
    }
    // IP + Port
    if (argc > 2) {
        int port = std::stoi(argv[2]);
        if (port < 0 || port > 65535) {
            std::cerr << "Invalid port\n";
            return 1;
        }
    }
    server_port = htons(uint16_t(server_port));
    // IP + Port + Username
    if (argc > 3) {
        client.username = std::string(argv[3]);
    }

    // std::cout << "Connecting to " << server_ip_string  << ":" << htons(server_port)
    //          << " as " << client.username << "\n";

    // Init client socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = server_port;
    serverAddress.sin_addr.s_addr = server_ip;
    int status = connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (status == -1) {
        std::cerr << "Connection failed: " << strerror(errno) << " (" << errno << ")" << std::endl;
        return 1;
    }
    // std::cout << "Connected to server!\n";
    
    std::thread networkThread(AsyncPacketReceive, clientSocket);
    networkThread.detach();
    std::thread packetThread(AsyncPacketSend, clientSocket);
    packetThread.detach();

    // Send out handshake packet
    HandshakePacket hsPacket;
    hsPacket.username = client.username;
    client.connectionState = ConnectionState::STATE_HANDSHAKE;
    hsPacket.Send(clientSocket);

    // Curses Init
    initscr();
    noecho();
    raw();
    cbreak();
    nodelay(stdscr, TRUE);
    keypad(stdscr, true);
    if (has_colors() == false) {
        endwin();
        std::cerr << "Your terminal does not support colors!" << std::endl;
        return 1;
    }

    start_color();
    InitColors();

    while(running) {
        attron(COLOR_PAIR(3));
        clear();
        if (UpdateSize()) {
            RenderWorld();
            attron(COLOR_PAIR(1));
            RenderChat();
            RenderUI();
            attroff(COLOR_PAIR(1));
            // Player
            attron(COLOR_PAIR(2));
            for (auto it = players.begin(); it != players.end(); ) {
                Double3 relPos = it->second.pos - client.player.pos;
                mvwaddch(stdscr, max_height/2 - int(relPos.z - 0.5), max_width/2 - int(relPos.x - 0.5), 'O');
                ++it;
            }
            mvwaddch(stdscr, (max_height+0.5)/2, (max_width+0.5)/2, '+');
            attroff(COLOR_PAIR(2));
            mvwaddch(stdscr, max_height/2 + client.cursor.z, max_width/2 + client.cursor.x, 'X');
            move(0,0);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ProcessInputs(clientSocket);
        refresh();
    }
    endwin();

    DisconnectPacket dcPacket;
    dcPacket.message = "Disconnected";
    dcPacket.Send(clientSocket);
    
    close(clientSocket);
    packetThread.join();
    networkThread.join();

    return 0;
}
