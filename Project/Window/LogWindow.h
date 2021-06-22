#pragma once

class LogWindow {
private:

    LogWindow(void) = delete;
    ~LogWindow(void) = delete;

public:

    static void Show(void);

    static void Save(void);
};

