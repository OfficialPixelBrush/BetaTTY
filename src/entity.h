#pragma once
#include <string>
#include "datatypes.h"
#include "enum.h"
#include <cstdint>

class Entity {
    public:
        int32_t id;
        Double3 pos;
        Float3 rot;
        Entity() {}
        Entity(int32_t pId) : 
            id(pId) {}
};

class Player : public Entity {
    public:
        std::string username = "Zombie";
        Player() : username("Zombie") {}
        Player(int32_t pId, std::string pUsername) :
            Entity(pId), username(pUsername) {}
};

class Client {
    public:
        std::string username = "Steve";
        ConnectionState connectionState = ConnectionState::STATE_OFFLINE;
        Player player;

        Client() : username("Steve"), connectionState(ConnectionState::STATE_OFFLINE) {}
        Client(std::string pUsername) { username = pUsername; }
};
