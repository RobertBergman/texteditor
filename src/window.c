/**
 * @file window.c
 * @brief Window management implementation for the Professional Text Editor
 *
 * Contains the functions for window registration, creation, and message handling.
 */

#include "../include/window.h"
#include "../include/control.h"
#include "../include/fileops.h"

// Global variables
static HINSTANCE g_hInstance = NULL;  // Application instance handle
static HWND g_hEdit = NULL;          // Global handle to the edit control

/**
 * @brief Registers the main window class for the application.
 *
 * @param hInstance Handle to the instance that contains the window procedure.
 * @return If the function succeeds, the return value is a class atom.
 *         If the function fails, the return value is zero.
 */
ATOM RegisterMainWindow(HINSTANCE hInstance) {
    g_hInstance = hInstance;
    
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(wcex));
    
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL; // Menu will be created in WindowProc
    wcex.lpszClassName = EDITOR_CLASS_NAME;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    
    return RegisterClassEx(&wcex);
}

/**
 * @brief Creates and initializes the main application window.
 *
 * @param hInstance Handle to the current instance of the application.
 * @param nCmdShow Specifies how the window is to be shown.
 * @return TRUE if the window was created successfully, FALSE otherwise.
 */
BOOL CreateMainWindow(HINSTANCE hInstance, int nCmdShow) {
    HWND hWnd = CreateWindow(
        EDITOR_CLASS_NAME,
        EDITOR_TITLE,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0,
        CW_USEDEFAULT, 0,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    
    if (!hWnd) {
        return FALSE;
    }
    
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    
    return TRUE;
}

/**
 * @brief Creates the menu bar for the main window.
 *
 * @param hWnd Handle to the window.
 * @return Handle to the created menu.
 */
static HMENU CreateMenuBar(HWND hWnd) {
    HMENU hMenubar = CreateMenu();
    HMENU hMenu;
    
    // File menu
    hMenu = CreateMenu();
    AppendMenu(hMenu, MF_STRING, 1, "&New");
    AppendMenu(hMenu, MF_STRING, 2, "&Open");
    AppendMenu(hMenu, MF_STRING, 3, "&Save");
    AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hMenu, MF_STRING, 4, "E&xit");
    AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, "&File");
    
    // Edit menu
    hMenu = CreateMenu();
    AppendMenu(hMenu, MF_STRING, 5, "Cu&t");
    AppendMenu(hMenu, MF_STRING, 6, "&Copy");
    AppendMenu(hMenu, MF_STRING, 7, "&Paste");
    AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, "&Edit");
    
    // Help menu
    hMenu = CreateMenu();
    AppendMenu(hMenu, MF_STRING, 8, "&About");
    AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, "&Help");
    
    return hMenubar;
}

/**
 * @brief Window procedure for the main application window.
 *
 * @param hWnd Handle to the window.
 * @param message The message.
 * @param wParam Additional message information.
 * @param lParam Additional message information.
 * @return The result of the message processing.
 */
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE: {
            // Create and set the menu bar
            HMENU hMenubar = CreateMenuBar(hWnd);
            SetMenu(hWnd, hMenubar);
            
            // Create the editor control
            g_hEdit = CreateEditorControl(hWnd, g_hInstance);
            if (!g_hEdit) {
                MessageBox(hWnd, "Failed to create editor control!", "Error", MB_ICONERROR | MB_OK);
                return -1;
            }
            break;
        }
        
        case WM_SIZE:
            HandleWindowResize(hWnd, lParam);
            break;
            
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            
            // Handle menu commands
            switch (wmId) {
                case 1: // File -> New
                    EditorNewFile(g_hEdit);
                    break;
                    
                case 2: // File -> Open
                    EditorOpenFile(hWnd, g_hEdit);
                    break;
                    
                case 3: // File -> Save
                    EditorSaveFile(hWnd, g_hEdit);
                    break;
                    
                case 4: // File -> Exit
                    DestroyWindow(hWnd);
                    break;
                    
                case 5: // Edit -> Cut
                    if (g_hEdit) {
                        SendMessage(g_hEdit, WM_CUT, 0, 0);
                    }
                    break;
                    
                case 6: // Edit -> Copy
                    if (g_hEdit) {
                        SendMessage(g_hEdit, WM_COPY, 0, 0);
                    }
                    break;
                    
                case 7: // Edit -> Paste
                    if (g_hEdit) {
                        SendMessage(g_hEdit, WM_PASTE, 0, 0);
                    }
                    break;
                    
                case 8: // Help -> About
                    {
                        char aboutMsg[256];
                        sprintf_s(aboutMsg, sizeof(aboutMsg), "%s\nVersion %s\n\nA professional text editor example.",
                                EDITOR_TITLE, EDITOR_VERSION);
                        MessageBox(hWnd, aboutMsg, "About", MB_OK | MB_ICONINFORMATION);
                    }
                    break;
                    
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        }
        
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
            
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    
    return 0;
}

/**
 * @brief Handles window resize events.
 *
 * @param hWnd Handle to the main window.
 * @param lParam Contains the new width and height of the client area.
 */
void HandleWindowResize(HWND hWnd, LPARAM lParam) {
    if (g_hEdit) {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        SetWindowPos(g_hEdit, NULL, 0, 0, width, height, SWP_NOZORDER);
    }
}
