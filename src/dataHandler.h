#pragma once
#include <condition_variable>
#include <cstdint>
#include <string>
#include "packet.h"

extern std::condition_variable cv;

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
        static void PushIncoming(uint8_t value);
};