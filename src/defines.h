#pragma once

// Gameplay
#define TICK_SPEED 20
#define MAX_REACH 4

// Terminal related
#define MINIMUM_SIZE_WIDTH  40
#define MINIMUM_SIZE_HEIGHT 15

// Packet related
#define PACKET_BUFFER_SIZE 4096
#define PACKET_INTERVAL 1000/20
#define PROTOCOL_VERSION 14

// Chat-related
#define CHAT_HISTORY_SIZE 5

// Chunk related
#define CHUNK_WIDTH_X 16
#define CHUNK_WIDTH_Z 16
#define CHUNK_HEIGHT 128
#define CHUNK_TOTAL CHUNK_WIDTH_X * CHUNK_WIDTH_Z * CHUNK_HEIGHT
