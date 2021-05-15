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

    static void NewProject(void);
    static void OpenProject(void);
    static void SaveProject(void);
    static void SaveAsProject(void);
    static void Close(void);
    static void Export(void);
    static void Quit(void);

    static void OpenManual(void);
    static void Version(void);
};

