/**
 * @file main.c
 * @brief Main entry point for the Professional Text Editor
 *
 * Initializes the application, registers the window class,
 * creates the main window, and runs the message loop.
 */

#include "../include/editor.h"
#include "../include/window.h"
#include <commctrl.h> // Required for InitCommonControlsEx

// Global instance handle
static HINSTANCE g_hInstance = NULL;

/**
 * @brief The main entry point for the application.
 *
 * @param hInstance Handle to the current instance of the application.
 * @param hPrevInstance Handle to the previous instance (always NULL for Win32).
 * @param lpCmdLine Command line for the application.
 * @param nCmdShow Specifies how the window is to be shown.
 * @return Exit code: 0 for success, non-zero for failure.
 */
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Silence unused parameter warnings
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    // Store the instance handle
    g_hInstance = hInstance;

    // Initialize common controls (required for status bar)
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_BAR_CLASSES; // Load status bar control class
    if (!InitCommonControlsEx(&icex)) {
        MessageBox(NULL, "Failed to initialize common controls!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Register the window class
    ATOM classAtom = RegisterMainWindow(hInstance);
    EDITOR_CHECK_ERROR(classAtom, "Window Registration Failed!", "Error");

    // Create the main window
    BOOL windowCreated = CreateMainWindow(hInstance, nCmdShow);
    EDITOR_CHECK_ERROR(windowCreated, "Window Initialization Failed!", "Error");
    
    // Main message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return (int)msg.wParam;
}
