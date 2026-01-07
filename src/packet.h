#pragma once
#include "datatypes.h"
#include "defines.h"
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <sys/socket.h>
#include "enum.h"
#include "global.h"

extern std::deque<uint8_t> outgoingPacketBuffer;
extern std::deque<uint8_t> incomingPacketBuffer;
extern std::mutex bufferMutex;
extern std::condition_variable bufferCv;

#include "dataHandler.h"

// network thread
void AsyncPacketReceive(int clientSocket);
void AsyncPacketSend(int clientSocket);

void ParsePacket(PacketType pt, int cs);
bool EnoughBytesReceived(PacketType pt);

class BasePacket {
    public:
        PacketType type = PacketType::KeepAlive;
        virtual ~BasePacket() = default;
        virtual void Serialize() = 0;
        virtual void Deserialize() = 0;
		void Send(int cs);
		void Receive();
};

class KeepAlivePacket : public BasePacket {
    public:
        KeepAlivePacket() { type = PacketType::KeepAlive; }
        void Serialize() override;
        void Deserialize() override;
};

class LoginPacket : public BasePacket {
    public:
        int32_t entityId;
		std::string username;
		int64_t seed;
		int8_t dimension;
        LoginPacket() { type = PacketType::LoginRequest; }
        LoginPacket(std::string pUsername) {
            type = PacketType::LoginRequest;
            username = pUsername;
        }
        void Serialize() override;
        void Deserialize() override;
};

class HandshakePacket : public BasePacket {
    public:
        std::string username;
        HandshakePacket() { type = PacketType::Handshake; }
        HandshakePacket(std::string pUsername) {
            type = PacketType::Handshake;
            username = pUsername;
        }
        void Serialize() override;
        void Deserialize() override;
};

class ChatMessagePacket : public BasePacket {
    public:
        std::string message;
        ChatMessagePacket() { type = PacketType::ChatMessage; }
        ChatMessagePacket(std::string pMessage) {
            type = PacketType::ChatMessage;
            message = pMessage;
        }
        void Serialize() override;
        void Deserialize() override;
};

class SpawnPositionPacket : public BasePacket {
    public:
        Int3 pos;
        SpawnPositionPacket() { type = PacketType::SpawnPosition; }
        SpawnPositionPacket(Int3 pPos) {
            type = PacketType::ChatMessage;
            pos = pPos;
        }
        void Serialize() override;
        void Deserialize() override;
};

class TimeUpdatePacket : public BasePacket {
    public:
        int64_t time;
        TimeUpdatePacket() { type = PacketType::TimeUpdate; }
        TimeUpdatePacket(int64_t pTime) {
            type = PacketType::TimeUpdate;
            time = pTime;
        }
        void Serialize() override;
        void Deserialize() override;
};

class EntityEquipmentPacket : public BasePacket {
    public:
        int32_t entityId;
        int16_t inventorySlot;
        int16_t itemId;
        int16_t itemMeta;
        EntityEquipmentPacket() { type = PacketType::EntityEquipment; }
        void Serialize() override;
        void Deserialize() override;
};

class SpawnPlayerPacket : public BasePacket {
    public:
        Player player;
        SpawnPlayerPacket() { type = PacketType::NamedEntitySpawn; }
        SpawnPlayerPacket(Player pPlayer) {
            type = PacketType::NamedEntitySpawn;
            player = pPlayer;
        }
        void Serialize() override;
        void Deserialize() override;
};

class DestroyEntityPacket : public BasePacket {
    public:
        int32_t entityId;
        DestroyEntityPacket() { type = PacketType::DestroyEntity; }
        void Serialize() override;
        void Deserialize() override;
};

class PlayerPositionLookPacket : public BasePacket {
    public:
        Double3 pos;
		double cameraY;
		Float2 rot;
		bool onGround;
        PlayerPositionLookPacket() { type = PacketType::PlayerPositionLook; }
        PlayerPositionLookPacket(
			Double3 pPos,
			double pCameraY,
			Float2 pRot,
			bool pOnGround
		) {
            type = PacketType::PlayerPositionLook;
            pos = pPos;
			cameraY = pCameraY;
			rot = pRot;
			onGround = pOnGround;
        }
        void Serialize() override;
        void Deserialize() override;
};

class EntityPositionLookPacket : public BasePacket {
    public:
        int32_t entityId;
        Int3 pos;
		Byte2 rot;
        EntityPositionLookPacket() { type = PacketType::EntityTeleport; }
        void Serialize() override;
        void Deserialize() override;
};

class ChunkPacket : public BasePacket {
    public:
        Int3 pos;
        UByte3 area;
        int32_t compressedSize;
        std::vector<uint8_t> compressedData;
        ChunkPacket() { type = PacketType::Chunk; }
        void Serialize() override;
        void Deserialize() override;
};

class PreChunkPacket : public BasePacket {
    public:
        Int2 pos;
        bool load;
        PreChunkPacket() { type = PacketType::PreChunk; }
        void Serialize() override;
        void Deserialize() override;
};

class WindowItemsPacket : public BasePacket {
    public:
        WindowItemsPacket() { type = PacketType::WindowItems; }
        void Serialize() override;
        void Deserialize() override;
};

class DisconnectPacket : public BasePacket {
    public:
        std::string message;
        DisconnectPacket() { type = PacketType::Disconnect; }
        void Serialize() override;
        void Deserialize() override;
};