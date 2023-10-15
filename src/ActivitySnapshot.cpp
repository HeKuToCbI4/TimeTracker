//
// Created by Nikita on 10/13/23.
//

#include "ActivitySnapshot.h"
#include <chrono>

namespace activity_monitor {
    ActivitySnapshot::ActivitySnapshot() {
        this->id = 0;
        this->process_executable_path = nullptr;
        this->window_title = nullptr;
        using namespace std::chrono;
        this->utc_timestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }

    ActivitySnapshot::ActivitySnapshot(wchar_t *&window_title, wchar_t *&process_executable_path) {
        this->id = this->last_frame++;
        this->process_executable_path = new std::wstring(process_executable_path);
        this->window_title = new std::wstring(window_title);
        using namespace std::chrono;
        this->utc_timestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }

    ActivitySnapshot::~ActivitySnapshot() {
        delete[] this->process_executable_path;
        delete[] this->window_title;
    }
} // activity_monitor