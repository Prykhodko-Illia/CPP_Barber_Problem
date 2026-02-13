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

    Client generateClient() {
        return (clientsTime(gen));
    }

    void proceedClient() {
        auto c = generateClient();

        std::unique_lock<std::mutex> lock(mtx);
        if (clientsQueue.size() < queueSize) {
            clientsQueue.push(c);
            cv_barber.notify_one();
            std::cout << "[PROCEED_CLIENT] Client sat on the chair." << std::endl;
        } else {
            std::cout << "[PROCEED_CLIENT] Client goes without haircut. (no chairs)" << std::endl;
        }

        lock.unlock();
    }

    void proceedBarber() {
        std::unique_lock<std::mutex> lock(mtx);

        cv_barber.wait(lock, [this] {
            return !clientsQueue.empty();
        });

        auto client = clientsQueue.front();
        clientsQueue.pop();
        lock.unlock();

        std::cout << "[PROCEED_BARBER] START OF CUTTING" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(client.time));
        std::cout << "[PROCEED_BARBER] END OF CUTTING" << std::endl;
    }

public:
    BarberProblem(const int queueSize) : queueSize(queueSize) {}

    void run() {
        std::thread barberThread([this]() {
            while (true) {
                proceedBarber();
            }
        });

        std::thread clientGeneratorThread([this]() {
            while (true) {
                proceedClient();
                std::this_thread::sleep_for(
                    std::chrono::seconds(clientsGeneratingTime(gen))
                );
            }
        });

        barberThread.join();
        clientGeneratorThread.join();
    }
};
