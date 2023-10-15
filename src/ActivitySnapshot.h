//
// Created by Nikita on 10/13/23.
//

#ifndef TIMETRACKER_ACTIVITYSNAPSHOT_H
#define TIMETRACKER_ACTIVITYSNAPSHOT_H

#include <cstdint>
#include <string>

namespace activity_monitor {

    class ActivitySnapshot {
    private:
        uint64_t last_frame = 0;
    public:
        uint64_t id;
        std::wstring *window_title;
        std::wstring *process_executable_path;
        uint64_t utc_timestamp;
        ActivitySnapshot();
        ActivitySnapshot(wchar_t* &window_title, wchar_t* &process_executable_path);
        ~ActivitySnapshot();
    };

} // activity_monitor

#endif //TIMETRACKER_ACTIVITYSNAPSHOT_H
