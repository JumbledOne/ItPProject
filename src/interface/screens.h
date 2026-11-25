#pragma once
// interface.h and session.h both use this
// This is a separate header to avoid a circular dependency

#include <unordered_map>
#include <functional>
#include <optional>

#include <tan.h>

enum class SCREENS {
    MAIN
};

const std::unordered_map<SCREENS, std::function<std::optional<int>()>> SCREEN_FUNCTION_TABLE = {
    {SCREENS::MAIN, DisplayMainScreen}
};