#include "ActivityMonitor.h"
#include "proto/FrameInfo.pb.h"
#include "utilities/Utilities.h"

int main() {
    // string stream to fetch data into
    while (true) {
        auto snapshot = activity_monitor::ActivityMonitor::getSnapshot();
        auto frameInfo = telemetry::TimeFrameInfo();
        if (snapshot->window_title != nullptr) {
            frameInfo.set_window_title(utilities::Utilities::utf8_encode(*(snapshot->window_title)));
        } else {
            frameInfo.set_window_title("");
        }
        if (snapshot->process_executable_path != nullptr) {
            frameInfo.set_process_executable_path(utilities::Utilities::utf8_encode(*(snapshot->process_executable_path)));
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
