/**
 * @file window.c
 * @brief Window management implementation for the Professional Text Editor
 *
 * Contains the functions for window registration, creation, and message handling.
 */

#include "../include/window.h"
#include "../include/control.h"
#include "../include/fileops.h"
#include <commctrl.h> // Required for status bar
#include <Shlwapi.h> // Required for PathFindFileName

// Global variables
HINSTANCE g_hInstance = NULL;  // Application instance handle (made non-static)
HWND g_hEdit = NULL;          // Global handle to the edit control (made non-static)
HWND g_hStatusBar = NULL;     // Global handle to the status bar control
EditorState g_editorState;    // Global editor state (file path, size, etc.)

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

            // Initialize editor state
            ZeroMemory(&g_editorState, sizeof(EditorState));
            strcpy_s(g_editorState.currentFilePath, MAX_PATH, "Untitled");
            g_editorState.currentFileSize = 0;

            // Create the status bar
            g_hStatusBar = CreateWindowEx(
                0,                          // no extended styles
                STATUSCLASSNAME,            // status bar class name
                NULL,                       // no text initially
                WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, // styles
                0, 0, 0, 0,                 // x, y, width, height (handled by WM_SIZE)
                hWnd,                       // parent window
                (HMENU)ID_STATUSBAR,        // control ID
                g_hInstance,                // instance
                NULL);                      // no creation data

            if (!g_hStatusBar) {
                MessageBox(hWnd, "Failed to create status bar!", "Error", MB_ICONERROR | MB_OK);
                return -1; // Fail window creation
            }

            // Initial status bar update
            UpdateStatusBar(g_hStatusBar, &g_editorState);

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
    // Send WM_SIZE to status bar first so it can resize itself
    if (g_hStatusBar) {
        SendMessage(g_hStatusBar, WM_SIZE, 0, 0);
    }

    // Calculate the height of the status bar
    RECT statusBarRect;
    int statusBarHeight = 0;
    if (g_hStatusBar && GetWindowRect(g_hStatusBar, &statusBarRect)) {
        statusBarHeight = statusBarRect.bottom - statusBarRect.top;
    }

    // Resize the edit control to fill the remaining client area
    if (g_hEdit) {
        int clientWidth = LOWORD(lParam);
        int clientHeight = HIWORD(lParam);
        int editHeight = clientHeight - statusBarHeight;
        if (editHeight < 0) editHeight = 0; // Prevent negative height

        SetWindowPos(g_hEdit, NULL, 0, 0, clientWidth, editHeight, SWP_NOZORDER);
    }
}

/**
 * @brief Updates the status bar text with the current editor state.
 *
 * @param hStatusBar Handle to the status bar control.
 * @param state Pointer to the EditorState structure containing file info.
 */
void UpdateStatusBar(HWND hStatusBar, const EditorState* state) {
    if (!hStatusBar || !state) {
        return;
    }

    char statusText[MAX_PATH + 50]; // Buffer for formatted text
    char* fileName = PathFindFileName(state->currentFilePath); // Extract just the filename

    // Format the status text
    sprintf_s(statusText, sizeof(statusText), "File: %s | Size: %ld bytes",
              fileName ? fileName : "Untitled", // Show "Untitled" if path is empty or invalid
              state->currentFileSize);

    // Set the text in the first part of the status bar
    SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM)statusText);
}
