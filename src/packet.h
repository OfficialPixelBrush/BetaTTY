#pragma once
#include "datatypes.h"
#include "defines.h"
#include <cstdint>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <sys/socket.h>

extern std::deque<uint8_t> outgoingPacketBuffer;
extern std::mutex bufferMutex;
extern std::deque<uint8_t> incomingPacketBuffer;

// network thread
void AsyncPacketReceive(int clientSocket);

enum class PacketType : uint8_t {
	KeepAlive = 0x00,
	LoginRequest = 0x01,
	Handshake = 0x02,
	ChatMessage = 0x03,
	TimeUpdate = 0x04,
	EntityEquipment = 0x05,
	SpawnPosition = 0x06,
	UseEntity = 0x07,
	UpdateHealth = 0x08,
	Respawn = 0x09,
	Player = 0x0A,
	PlayerPosition = 0x0B,
	PlayerLook = 0x0C,
	PlayerPositionLook = 0x0D,
	PlayerDigging = 0x0E,
	PlayerBlockPlacement = 0x0F,
	HoldingChange = 0x10,
	UseBed = 0x11,
	Animation = 0x12,
	EntityAction = 0x13,
	NamedEntitySpawn = 0x14,
	PickupSpawn = 0x15,
	CollectItem = 0x16,
	AddObjectVehicle = 0x17,
	MobSpawn = 0x18,
	EntityPainting = 0x19,
	StanceUpdate = 0x1B,
	EntityVelocity = 0x1C,
	DestroyEntity = 0x1D,
	Entity = 0x1E,
	EntityRelativeMove = 0x1F,
	EntityLook = 0x20,
	EntityLookRelativeMove = 0x21,
	EntityTeleport = 0x22,
	EntityStatus = 0x26,
	AttachEntity = 0x27,
	EntityMetadata = 0x28,
	PreChunk = 0x32,
	Chunk = 0x33,
	MultiBlockChange = 0x34,
	BlockChange = 0x35,
	BlockAction = 0x36,
	Explosion = 0x3C,
	Soundeffect = 0x3D,
	NewInvalidState = 0x46,
	Thunderbolt = 0x47,
	OpenWindow = 0x64,
	CloseWindow = 0x65,
	WindowClick = 0x66,
	SetSlot = 0x67,
	WindowItems = 0x68,
	UpdateProgressBar = 0x69,
	Transaction = 0x6A,
	UpdateSign = 0x82,
	MapData = 0x83,
	IncrementStatistic = 0xC8,
	Disconnect = 0xFF
};

class DataHandler {
    public:
        // Reading
        static uint8_t ReadByte();
        static int16_t ReadShort();
        static int32_t ReadInteger();
        static int64_t ReadLong();
        static float ReadFloat();
        static double ReadDouble();
        static std::string ReadString8();
        static std::string ReadString16();
        // Writing
        static void WriteByte(uint8_t value);
        static void WriteShort(int16_t value);
        static void WriteInteger(int32_t value);
        static void WriteLong(int64_t value);
        static void WriteFloat(float value);
        static void WriteDouble(double value);
        static void WriteString8(std::string value);
        static void WriteString16(std::string value);
		// Wait for bytes
		static void WaitForBytes(size_t bytes);
};

void ParsePacket(PacketType pt);
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

class ChunkPacket : public BasePacket {
    public:
        ChunkPacket() { type = PacketType::Chunk; }
        void Serialize() override;
        void Deserialize() override;
};

class WindowItemsPacket : public BasePacket {
    public:
        WindowItemsPacket() { type = PacketType::WindowItems; }
        void Serialize() override;
        void Deserialize() override;
};