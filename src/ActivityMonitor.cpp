//
// Created by Nikita on 04/04/23.
//

#include "ActivityMonitor.h"

namespace activity_monitor {
    ActivityMonitor::ActivityMonitor() = default;

    ActivitySnapshot *ActivityMonitor::GetSnapshot() {
        // pointers to returned values
        wchar_t *processExecutablePath = nullptr;
        wchar_t *titleBuf = nullptr;
        auto foregroundWindowHandle = GetForegroundWindow();
        if (foregroundWindowHandle == INVALID_HANDLE_VALUE) {
            return nullptr;
        }
        auto textLen = GetWindowTextLength(foregroundWindowHandle);
        if (textLen) {
            titleBuf = new wchar_t[textLen];
            if (GetWindowTextW(foregroundWindowHandle, titleBuf, MAX_STR_LENGTH)) {
                // std::wcout << titleBuf << std::endl;
            } else {
                std::cout << "Problem getting title;" << std::endl;
                delete[] titleBuf;
                titleBuf = nullptr;
            }
        }
        DWORD pid;
        if (!GetWindowThreadProcessId(foregroundWindowHandle, &pid)) {
            std::cout << "Problem getting pid;" << std::endl;
        } else {
            auto activeWindowProcessHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,
                                                         FALSE,
                                                         pid);

            if (activeWindowProcessHandle == INVALID_HANDLE_VALUE) {
                processExecutablePath = nullptr;
            } else {
                processExecutablePath = new wchar_t[MAX_STR_LENGTH];
                DWORD size = MAX_STR_LENGTH;
                if (QueryFullProcessImageNameW(activeWindowProcessHandle, 0, processExecutablePath, &size)) {
//                    std::wcout << processExecutablePath << " " << size <<  std::endl;
                } else {
                    auto error = GetLastError();
                    std::cout << error << std::endl;
                    delete[] processExecutablePath;
                    processExecutablePath = nullptr;
                }
                CloseHandle(activeWindowProcessHandle);
            }
        }
        return new ActivitySnapshot(titleBuf, processExecutablePath);
    }

    ActivitySnapshot *ActivityMonitor::GetAfkSnapshot() {
        auto AFK = L"AFK";
        return new ActivitySnapshot(AFK, AFK);
    }
}// namespace activity_monitor