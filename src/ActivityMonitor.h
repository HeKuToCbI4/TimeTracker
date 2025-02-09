//
// Created by Nikita on 04/04/23.
//
#include "ActivitySnapshot.h"
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include "winver.h"

#ifndef TIMETRACKER_ACTIVITYMONITOR_H
#define TIMETRACKER_ACTIVITYMONITOR_H
#define MAX_STR_LENGTH 512

namespace activity_monitor {

    class ActivityMonitor {
    private:
    public:
        ActivityMonitor();

        static ActivitySnapshot *GetSnapshot();

        static ActivitySnapshot *GetAfkSnapshot();
    };

} // activity_monitor

#endif //TIMETRACKER_ACTIVITYMONITOR_H
