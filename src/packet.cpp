#include "packet.h"
#include "datatypes.h"
#include "defines.h"
#include "entity.h"
#include "enum.h"
#include <cstdint>
#include <iostream>
#include <mutex>
#include <sys/socket.h>

std::deque<uint8_t> outgoingPacketBuffer;
std::deque<uint8_t> incomingPacketBuffer;
std::mutex bufferMutex;
std::condition_variable bufferCv;

void AsyncPacketReceive(int clientSocket) {
    uint8_t dataBuffer[PACKET_BUFFER_SIZE];

    while (true) {
        int n = recv(clientSocket, dataBuffer, sizeof(dataBuffer), 0);
        if (n <= 0) break;

        {
            std::lock_guard<std::mutex> lock(bufferMutex);
            for (int i = 0; i < n; ++i)
                incomingPacketBuffer.push_back(dataBuffer[i]);
        }

        bufferCv.notify_all();
    }
}

void ParsePacket(PacketType pt, int cs) {
    switch(PacketType(pt)) {
        case PacketType::KeepAlive:
        {
            KeepAlivePacket kaPacket;
            kaPacket.Receive();
            kaPacket.Send(cs);
            break;
        }
        case PacketType::Handshake:
        {
            // We're not handshaking, ignore
            if (client.connectionState != ConnectionState::STATE_HANDSHAKE) break;
            // Resume handshake
            HandshakePacket hsPacket;
            hsPacket.Receive();
            if (hsPacket.username == "-") {
                client.connectionState = ConnectionState::STATE_LOGIN;
                LoginPacket lPacket;
                lPacket.entityId = PROTOCOL_VERSION;
                lPacket.username = client.username;
                lPacket.Send(cs);
            }
            break;
        }
        case PacketType::LoginRequest:
        {
            // We're not logging in, ignore
            if (client.connectionState != ConnectionState::STATE_LOGIN) break;
            // Resume login
            LoginPacket lPacket;
            lPacket.Receive();
            client.connectionState = ConnectionState::STATE_ONLINE;
            break;
        }
        case PacketType::ChatMessage:
        {
            ChatMessagePacket cmPacket;
            cmPacket.Receive();
            break;
        }
        case PacketType::SpawnPosition:
        {
            SpawnPositionPacket spPacket;
            spPacket.Receive();
            break;
        }
        case PacketType::TimeUpdate:
        {
            TimeUpdatePacket tuPacket;
            tuPacket.Receive();
            break;
        }
        case PacketType::EntityEquipment:
        {
            EntityEquipmentPacket eePacket;
            eePacket.Receive();
            break;
        }
        case PacketType::NamedEntitySpawn:
        {
            SpawnPlayerPacket spPacket;
            spPacket.Receive();
            break;
        }
        case PacketType::PlayerPositionLook:
        {
            PlayerPositionLookPacket pplPacket;
            pplPacket.Receive();
            break;
        }
        case PacketType::EntityTeleport:
        {
            EntityPositionLookPacket eplPacket;
            eplPacket.Receive();
            break;
        }
        case PacketType::PreChunk:
        {
            PreChunkPacket pcPacket;
            pcPacket.Receive();
            break;
        }
        case PacketType::Chunk:
        {
            ChunkPacket cPacket;
            cPacket.Receive();
            break;
        }
        case PacketType::WindowItems:
        {
            WindowItemsPacket wiPacket;
            wiPacket.Receive();
            break;
        }
        default:
            std::cout << std::hex << "0x" << int(pt) << std::dec << "\n";
            incomingPacketBuffer.pop_front();
            break;
    }
}

void BasePacket::Send(int cs) {
    DataHandler::WriteByte(uint8_t(type));
    Serialize();
    uint8_t dataBuffer[PACKET_BUFFER_SIZE];
    size_t i = 0;
    while (!outgoingPacketBuffer.empty()) {
        dataBuffer[i++] = outgoingPacketBuffer.front();
        outgoingPacketBuffer.pop_front();
    }
    send(cs, dataBuffer, i, 0);
}

void BasePacket::Receive() {
    Deserialize();
}

void KeepAlivePacket::Serialize() {
    //std::cout << "> KeepAlive" << std::endl;
}

void KeepAlivePacket::Deserialize() {
    //std::cout << "< KeepAlive" << std::endl;
}

void LoginPacket::Serialize() {
    DataHandler::WriteInteger(PROTOCOL_VERSION);
    DataHandler::WriteString16(username);
    DataHandler::WriteLong(0);
    DataHandler::WriteByte(0);
    std::cout << "> Login: " << username << std::endl;
}

void LoginPacket::Deserialize() {
    entityId = DataHandler::ReadInteger();
    DataHandler::ReadString16();
    seed = DataHandler::ReadLong();
    dimension = DataHandler::ReadByte();
    std::cout << "< Login: " << entityId << " " << seed << std::endl;
}

void HandshakePacket::Serialize() {
    DataHandler::WriteString16(username);
    std::cout << "> Handshake: " << username << std::endl;
}

void HandshakePacket::Deserialize() {
    username = DataHandler::ReadString16();
    std::cout << "< Handshake: " << username << std::endl;
}

void ChatMessagePacket::Serialize() {
    DataHandler::WriteString16(message);
    std::cout << "> ChatMessage: " << message << std::endl;
}

void ChatMessagePacket::Deserialize() {
    message = DataHandler::ReadString16();
    std::cout << "< ChatMessage: " << message << std::endl;
}

void SpawnPositionPacket::Serialize() {
    DataHandler::WriteInteger(pos.x);
    DataHandler::WriteInteger(pos.y);
    DataHandler::WriteInteger(pos.z);
    std::cout << "> SpawnPosition: " << pos << std::endl;
}

void SpawnPositionPacket::Deserialize() {
    pos = Int3{
        DataHandler::ReadInteger(),
        DataHandler::ReadInteger(),
        DataHandler::ReadInteger(),
    };
    std::cout << "< SpawnPosition: " << pos << std::endl;
}

void TimeUpdatePacket::Serialize() {
    DataHandler::WriteLong(time);
    std::cout << "> TimeUpdate: " << time << std::endl;
}

void TimeUpdatePacket::Deserialize() {
    time = DataHandler::ReadLong();
    std::cout << "< TimeUpdate: " << time << std::endl;
}

void EntityEquipmentPacket::Serialize() {
    DataHandler::WriteInteger(entityId);
    DataHandler::WriteShort(inventorySlot);
    DataHandler::WriteShort(itemId);
    DataHandler::WriteShort(itemMeta);
    std::cout << "> EntityEquipment: " << itemId << std::endl;
}

void EntityEquipmentPacket::Deserialize() {
    entityId = DataHandler::ReadInteger();
    inventorySlot = DataHandler::ReadShort();
    itemId = DataHandler::ReadShort();
    itemMeta = DataHandler::ReadShort();
    std::cout << "< EntityEquipment: " << itemId << std::endl;
}

void SpawnPlayerPacket::Serialize() {
}

void SpawnPlayerPacket::Deserialize() {
    player.id = DataHandler::ReadInteger();
    player.username = DataHandler::ReadString16();
    player.pos = Double3 {
        double(DataHandler::ReadInteger()),
        double(DataHandler::ReadInteger()),
        double(DataHandler::ReadInteger()),
    };
    player.rot = Float3 {
        (float(DataHandler::ReadByte()) / 255.0f) * 360.0f,
        (float(DataHandler::ReadByte()) / 255.0f) * 360.0f,
        0.0f
    };
    DataHandler::ReadShort();
    std::cout << "< SpawnPlayer: " << player.username << " " << player.pos << std::endl;
}

void PlayerPositionLookPacket::Serialize() {
    DataHandler::WriteDouble(pos.x);
    DataHandler::WriteDouble(pos.y);
    DataHandler::WriteDouble(cameraY);
    DataHandler::WriteDouble(pos.z);
    DataHandler::WriteFloat(rot.x);
    DataHandler::WriteDouble(rot.y);
    DataHandler::WriteByte(uint8_t(onGround));
    std::cout << "> PlayerPositionAndLook: " << pos << " " << rot << std::endl;
}

void PlayerPositionLookPacket::Deserialize() {
    pos.x = DataHandler::ReadDouble();
    pos.y = DataHandler::ReadDouble();
    cameraY = DataHandler::ReadDouble();
    pos.z = DataHandler::ReadDouble();
    rot = Float2{
        DataHandler::ReadFloat(),
        DataHandler::ReadFloat()
    };
    onGround = bool(DataHandler::ReadByte());
    std::cout << "< PlayerPositionAndLook: " << pos << " " << rot << std::endl;
}

void EntityPositionLookPacket::Serialize() {
    DataHandler::WriteInteger(entityId);
    DataHandler::WriteInteger(pos.x);
    DataHandler::WriteInteger(pos.y);
    DataHandler::WriteInteger(pos.z);
    DataHandler::WriteByte(rot.x);
    DataHandler::WriteByte(rot.y);
    std::cout << "> EntityPositionLook: " << pos << std::endl;
}

void EntityPositionLookPacket::Deserialize() {
    entityId = DataHandler::ReadInteger();
    pos.x = DataHandler::ReadInteger();
    pos.y = DataHandler::ReadInteger();
    pos.z = DataHandler::ReadInteger();
    rot.x = DataHandler::ReadByte();
    rot.y = DataHandler::ReadByte();
    std::cout << "< EntityPositionLook: " << pos << std::endl;
}

void PreChunkPacket::Serialize() {
    DataHandler::WriteInteger(pos.x);
    DataHandler::WriteInteger(pos.y);
    DataHandler::WriteByte(uint8_t(load));
    //std::cout << "> PreChunk: " << pos << std::endl;
}

void PreChunkPacket::Deserialize() {
    pos = Int2 {
        DataHandler::ReadInteger(),
        DataHandler::ReadInteger(),
    };
    load = bool(DataHandler::ReadByte());
    //std::cout << "< PreChunk: " << pos << std::endl;
}


void ChunkPacket::Serialize() {
    //DataHandler::WriteLong(time);
    std::cout << "> Chunk: " << time << std::endl;
}

void ChunkPacket::Deserialize() {
    DataHandler::ReadInteger();
    DataHandler::ReadShort();
    DataHandler::ReadInteger();
    DataHandler::ReadByte();
    DataHandler::ReadByte();
    DataHandler::ReadByte();
    int32_t size = DataHandler::ReadInteger();
    for (int32_t i = 0; i < size; i++) {
        DataHandler::ReadByte();
    }
    //std::cout << "< Chunk: " << size << std::endl;
}

void WindowItemsPacket::Serialize() {
    //DataHandler::WriteLong(time);
    std::cout << "> WindowItems: " << time << std::endl;
}

void WindowItemsPacket::Deserialize() {
    DataHandler::ReadByte();
    int16_t size = DataHandler::ReadShort();
    for (int16_t i = 0; i < size; i++) {
        int16_t id = DataHandler::ReadShort();
        if (id > -1) {
            DataHandler::ReadByte();
            DataHandler::ReadShort();
        }
    }
    std::cout << "< WindowItems: " << size << std::endl;
}

