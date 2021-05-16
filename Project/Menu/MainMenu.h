#pragma once

class MainMenu {
private:
    
    MainMenu(void) = delete;
    ~MainMenu(void) = delete;

    static void File(void);
    static void Edit(void);
    static void View(void);
    static void Help(void);

public:

    static void Show(void);

    static bool NewProject(void);
    static bool OpenProject(void);
    static bool SaveProject(void);
    static bool SaveAsProject(void);
    static bool Close(void);
    static bool Export(void);
    static void Quit(void);

    static void OpenManual(void);
    static void Version(void);
};

