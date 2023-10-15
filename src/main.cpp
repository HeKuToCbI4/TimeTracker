#include <Windows.h>
#include "ActivityMonitor.h"
#include "proto/FrameInfo.pb.h"

std::string utf8_encode(const std::wstring &wstr) {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int) wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int) wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string &str) {
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

int main() {
    // string stream to fetch data into
    while (true) {
        auto snapshot = activity_monitor::ActivityMonitor::getSnapshot();
        auto frameInfo = telemetry::TimeFrameInfo();
        if (snapshot->window_title != nullptr) {
            frameInfo.set_window_title(utf8_encode(*(snapshot->window_title)));
        } else {
            frameInfo.set_window_title("");
        }
        if (snapshot->process_executable_path != nullptr) {
            frameInfo.set_process_executable_path(utf8_encode(*(snapshot->process_executable_path)));
        } else {
            frameInfo.set_process_executable_path("");
        }
        frameInfo.set_id(snapshot->id);
        frameInfo.set_utc_timestamp(snapshot->utc_timestamp);
        std::cout << frameInfo.SerializeAsString() << std::endl;
//        std::cout << (snapshot->window_title)->size() << std::endl;
//        std::cout << utf8_encode(*(snapshot->window_title)).size() << std::endl;
//        std::cout << utf8_encode(*(snapshot->process_executable_path)) << std::endl;
        Sleep(500);
    }
    return 0;
}
