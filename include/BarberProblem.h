#pragma once
#include <condition_variable>
#include <thread>
#include <queue>
#include <mutex>
#include <random>
#include <chrono>
#include <iostream>

struct Client {
    int time{0};

    Client(const int time) : time(time) {}
};



class BarberProblem {
private:
    std::condition_variable cv_barber{};

    std::queue<Client> clientsQueue{};
    int queueSize{0};

    std::mutex mtx{};

    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<int> clientsTime{3, 6};
    std::uniform_int_distribution<int> clientsGeneratingTime{1, 2};

    Client generateClient();
    void proceedClient();
    void proceedBarber();

public:
    BarberProblem(const int queueSize) : queueSize(queueSize) {}

    void run();
};
