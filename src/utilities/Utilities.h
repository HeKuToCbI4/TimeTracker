//
// Created by Nikita on 10/16/23.
//
#include <Windows.h>
#include <string>

#ifndef TIMETRACKER_UTILITIES_H
#define TIMETRACKER_UTILITIES_H

namespace utilities {

    class Utilities {
    public:
        // Convert a wide Unicode String to an UTF8 string
        static std::string utf8_encode(const std::wstring &wstr);

        // Convert an UTF8 string to a wide Unicode String
        static std::wstring utf8_decode(const std::string &str);
    };
}

#endif //TIMETRACKER_UTILITIES_H
