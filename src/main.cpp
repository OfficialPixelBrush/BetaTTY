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
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include "enum.h"
#include <vector>
#include "entity.h"
#include "global.h"

// Max Terminal size
int max_width;
int max_height;

// Chat
std::string chatHistory[CHAT_HISTORY_SIZE];
int8_t chatHistoryIndex = 0;

int RenderChat() {
    for (int i = chatHistoryIndex; i < chatHistoryIndex+CHAT_HISTORY_SIZE; i++) {
        printw("%s", chatHistory[i%CHAT_HISTORY_SIZE].c_str());
    }
    return 0;
}

int RenderEntities() {
    return 0;    
}

int RenderWorld() {

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

    std::cout << "Connecting to " << server_ip_string  << ":" << htons(server_port)
              << " as " << client.username << "\n";

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
    std::cout << "Connected to server!\n";
    
    std::thread networkThread(AsyncPacketReceive, clientSocket);
    networkThread.detach();

    // Send out handshake packet
    HandshakePacket hsPacket;
    hsPacket.username = client.username;
    client.connectionState = ConnectionState::STATE_HANDSHAKE;
    hsPacket.Send(clientSocket);

    uint8_t packetIndex = 0;

    while(true) {
        std::unique_lock<std::mutex> lock(bufferMutex);

        bufferCv.wait(lock, [&] {
            return !incomingPacketBuffer.empty();
        });

        PacketType pt = static_cast<PacketType>(incomingPacketBuffer.front());
        incomingPacketBuffer.pop_front();

        lock.unlock(); // do NOT hold the mutex while parsing
        ParsePacket(pt, clientSocket);
        packetIndex++;
        if (packetIndex % 10 == 0) {
            KeepAlivePacket kaPacket;
            kaPacket.Send(clientSocket);
            packetIndex = 0;
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }


    /*

    // Curses Init
    initscr();
    noecho();
    raw();
    cbreak();
    keypad(stdscr, true);
    if (has_colors() == false) {
        endwin();
        std::cerr << "Your terminal does not support colors!" << std::endl;
        return 1;
    }

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);

    attron(COLOR_PAIR(1));

    getmaxyx(stdscr, max_height, max_width);
    if (max_height < 0 || max_width < 0 || max_height < MINIMUM_SIZE_HEIGHT || max_width < MINIMUM_SIZE_WIDTH) {
        endwin();
        std::cerr << "Your terminal is too small! (" << MINIMUM_SIZE_WIDTH << ", " << MINIMUM_SIZE_HEIGHT << ")" << std::endl;
        return 1;
    }

    mvwprintw(stdscr, 2, 2, "Hello!");
    refresh();
    attroff(COLOR_PAIR(1));

    getch();

    endwin();
    */
    close(clientSocket);
    networkThread.join();

    return 0;
}
