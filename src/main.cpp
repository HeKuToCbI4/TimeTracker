#include <csignal>
#include <thread>

#include "FrameInfoService.h"

volatile sig_atomic_t stopRequested;

int main() {
    auto server = FrameInfoServiceImpl();
    signal(SIGTERM, [](int signum) { std::cout << "SIGTERM RECEIVED!" << std::endl; stopRequested = 1; });
    signal(SIGINT, [](int signum) { std::cout << "SIGINT RECEIVED!" << std::endl; stopRequested = 1; });
    std::thread serverThread([&] { server.RunServer(); });
    while (!stopRequested) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    server.StopServer();
    serverThread.join();
    return 0;
}
