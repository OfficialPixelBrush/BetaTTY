#include "datatypes.h"
#include "defines.h"
#include "helper.h"
#include "packet.h"
#include <chrono>
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

// Render Block at presented screen coordinate
int RenderBlock(Int2 screenPos) {
    Int2 worldPos;
    worldPos.x = int32_t(client.player.pos.x) + max_width / 2 - screenPos.x;
    worldPos.y = int32_t(client.player.pos.z) + max_height / 2 - screenPos.y;
    int8_t type = GetBlock(Int3{worldPos.x, int32_t(client.player.pos.y), worldPos.y});
    switch(type) {
        default:
            attron(COLOR_PAIR(3));
            addch('.');
            break;
        case 8: // Water
        case 9: // Water
            attron(COLOR_PAIR(4));
            addch('~');
            break;
        case 2: // Grass
            attron(COLOR_PAIR(5));
            addch(' ');
            break;
        case 12: // Sand
            attron(COLOR_PAIR(6));
            addch('#');
            break;
    }
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
    mvwprintw(stdscr, 1, 0, "%ld", worldTime);
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

int ProcessInputs(int cs) {
    switch (getch()) {
        case 'q':
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
    }
    if (client.hasMoved) {
        client.player.pos.y = double(1+GetSolidHeight(Int2{int32_t(client.player.pos.x-0.5), int32_t(client.player.pos.z)}));
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
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // UI
    init_pair(2, COLOR_WHITE, COLOR_RED);   // Player
    init_pair(3, COLOR_WHITE, COLOR_CYAN);  // Sky
    init_pair(4, COLOR_CYAN, COLOR_BLUE);  // Water
    init_pair(5, COLOR_GREEN, COLOR_GREEN);  // Grass
    init_pair(6, COLOR_YELLOW, COLOR_YELLOW);  // Sand

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
        }
        refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ProcessInputs(clientSocket);
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
