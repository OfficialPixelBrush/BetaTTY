#include "packet.h"
#include <iostream>
#include <sys/socket.h>

std::deque<uint8_t> outgoingPacketBuffer;
std::deque<uint8_t> incomingPacketBuffer;
std::mutex bufferMutex;

void AsyncPacketReceive(int clientSocket) {
    uint8_t dataBuffer[PACKET_BUFFER_SIZE];
    while (true) {
        int n = recv(clientSocket, dataBuffer, sizeof(dataBuffer), 0);
        if (n <= 0) break;
        std::lock_guard<std::mutex> lock(bufferMutex);
        for (int i = 0; i < n; i++) incomingPacketBuffer.push_back(dataBuffer[i]);
    }
}

void DataHandler::WaitForBytes(size_t bytes) {
    while(incomingPacketBuffer.size() < bytes) {}
}

void DataHandler::WriteByte(uint8_t value) {
    outgoingPacketBuffer.push_back(value);
}

uint8_t DataHandler::ReadByte() {
    uint8_t value = incomingPacketBuffer.front();
    incomingPacketBuffer.pop_front();
    return value;
}

void DataHandler::WriteShort(int16_t value) {
    WriteByte(value >> 8);
    WriteByte(value & 0xFF);
}

int16_t DataHandler::ReadShort() {
    return 
        int16_t(ReadByte()) << 8 |
        int16_t(ReadByte())
    ;
}

void DataHandler::WriteInteger(int32_t value) {
    WriteByte(value >> 24 & 0xFF);
    WriteByte(value >> 16 & 0xFF);
    WriteByte(value >> 8 & 0xFF);
    WriteByte(value & 0xFF);
}

int32_t DataHandler::ReadInteger() {
    return 
        int32_t(ReadByte()) << 24 |
        int32_t(ReadByte()) << 16 |
        int32_t(ReadByte()) << 8 |
        int32_t(ReadByte())
    ;
}

void DataHandler::WriteLong(int64_t value) {
    WriteByte(value >> 56 & 0xFF);
    WriteByte(value >> 48 & 0xFF);
    WriteByte(value >> 40 & 0xFF);
    WriteByte(value >> 32 & 0xFF);
    WriteByte(value >> 24 & 0xFF);
    WriteByte(value >> 16 & 0xFF);
    WriteByte(value >> 8 & 0xFF);
    WriteByte(value & 0xFF);
}

int64_t DataHandler::ReadLong() {
    return 
        int64_t(ReadByte()) << 56 |
        int64_t(ReadByte()) << 48 |
        int64_t(ReadByte()) << 40 |
        int64_t(ReadByte()) << 32 |
        int64_t(ReadByte()) << 24 |
        int64_t(ReadByte()) << 16 |
        int64_t(ReadByte()) << 8 |
        int64_t(ReadByte())
    ;
}

void DataHandler::WriteFloat(float value) {
    return WriteInteger(std::bit_cast<int32_t>(value));
}

float DataHandler::ReadFloat() {
    return std::bit_cast<float>(ReadInteger());
}

void DataHandler::WriteDouble(double value) {
    return WriteLong(std::bit_cast<int64_t>(value));
}

double DataHandler::ReadDouble() {
    return std::bit_cast<double>(ReadLong());
}

void DataHandler::WriteString16(std::string value) {
    WriteShort(value.size());
    for (auto c : value) {
        WriteByte(0x00);
        WriteByte(c);
    }
}

std::string DataHandler::ReadString16() {
    int16_t size = ReadShort();
    std::string value;
    for (int16_t i = 0; i < size; i++) {
        ReadByte();
        value += ReadByte();
    }
    return value;
}

void ParsePacket(PacketType pt) {
    switch(PacketType(pt)) {
        case PacketType::LoginRequest:
        {
            LoginPacket lPacket;
            lPacket.Receive();
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
        case PacketType::PlayerPositionLook:
        {
            PlayerPositionLookPacket pplPacket;
            pplPacket.Receive();
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
    type = PacketType(DataHandler::ReadByte());
    Deserialize();
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

void PlayerPositionLookPacket::Serialize() {
    DataHandler::WriteDouble(pos.x);
    DataHandler::WriteDouble(pos.y);
    DataHandler::WriteDouble(cameraY);
    DataHandler::WriteDouble(pos.z);
    DataHandler::WriteFloat(rot.x);
    DataHandler::WriteDouble(rot.y);
    DataHandler::WriteByte(uint8_t(onGround));
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
    std::cout << "< Chunk: " << size << std::endl;
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