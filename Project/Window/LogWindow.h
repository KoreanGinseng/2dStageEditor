#pragma once

class LogWindow {
private:

    LogWindow(void) = delete;
    ~LogWindow(void) = delete;

    static int _save_log_count;

public:

    static void Show(void);

    static void Save(void);
};

