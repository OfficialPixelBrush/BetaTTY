#include "dataHandler.h"
#include "packet.h"
#include <condition_variable>
#include <cstdint>

void DataHandler::WaitForBytes(size_t bytes) {
    std::unique_lock lock(bufferMutex);
    bufferCv.wait(lock, [&] {
        return incomingPacketBuffer.size() >= bytes;
    });
}

void DataHandler::PushIncoming(uint8_t value) {
    {
        std::lock_guard lock(bufferMutex);
        incomingPacketBuffer.push_back(value);
    }
    bufferCv.notify_one();
}

uint8_t DataHandler::ReadByte() {
    std::unique_lock<std::mutex> lock(bufferMutex);
    bufferCv.wait(lock, [&] {
        return !incomingPacketBuffer.empty();
    });

    uint8_t v = incomingPacketBuffer.front();
    incomingPacketBuffer.pop_front();
    return v;
}

void DataHandler::WriteByte(uint8_t value) {
    outgoingPacketBuffer.push_back(value);
}

void DataHandler::WriteShort(int16_t value) {
    WriteByte(static_cast<uint8_t>(value >> 8));
    WriteByte(static_cast<uint8_t>(value));
}

int16_t DataHandler::ReadShort() {
    WaitForBytes(sizeof(int16_t));
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
    WaitForBytes(sizeof(int32_t));
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
    WaitForBytes(sizeof(int64_t));
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
    WaitForBytes(sizeof(float));
    return std::bit_cast<float>(ReadInteger());
}

void DataHandler::WriteDouble(double value) {
    return WriteLong(std::bit_cast<int64_t>(value));
}

double DataHandler::ReadDouble() {
    WaitForBytes(sizeof(double));
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
    WaitForBytes(size * 2);
    std::string value;
    for (int16_t i = 0; i < size; i++) {
        ReadByte();
        value += ReadByte();
    }
    return value;
}