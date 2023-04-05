#include <iostream>
#include <Windows.h>
#include <synchapi.h>
#include <Psapi.h>
#include <winver.h>

#define MAX_STR_LENGTH 256


int main() {
    // string stream to fetch data into
    auto *stringBuf = new wchar_t[MAX_STR_LENGTH];
    while (true) {
        auto foregroundWindowHandle = GetForegroundWindow();
        std::cout << foregroundWindowHandle << std::endl;
        auto textLen = GetWindowTextLength(foregroundWindowHandle);
        std::cout << textLen << std::endl;
        GetWindowTextW(foregroundWindowHandle, stringBuf, MAX_STR_LENGTH);
        std::cout << "Title: ";
        std::wcout << stringBuf << std::endl;
        DWORD pid;
        GetWindowThreadProcessId(foregroundWindowHandle, &pid);
        std::cout << "PID of owner: " << pid << std::endl;
        auto activeWindowProcessHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,
                                                     FALSE,
                                                     pid);
        std::cout << "parentProcessHanle: " << activeWindowProcessHandle << std::endl;
        if (activeWindowProcessHandle) {
            if (GetProcessImageFileNameW(activeWindowProcessHandle, stringBuf, MAX_STR_LENGTH)) {
                std::cout << "Process: ";
                std::wcout << stringBuf << std::endl;

            }
            CloseHandle(activeWindowProcessHandle);
        }
        Sleep(500);
    }
    free(stringBuf);
    return 0;
}
