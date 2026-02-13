#include "BarberProblem.h"


Client BarberProblem::generateClient() {
    return (clientsTime(gen));
}

void BarberProblem::proceedClient() {
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

void BarberProblem::proceedBarber() {
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

void BarberProblem::run() {
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
