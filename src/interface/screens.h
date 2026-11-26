#pragma once
// interface.h and session.h both use this
// This is a separate header to avoid a circular dependency

enum class SCREENS {
    NONE, // A dummy value to ensure that no screen is displayed at the start. The interface detects that no screen is displayed and displays MAIN
    MAIN,
    REGISTRATION
};
