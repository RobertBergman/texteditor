// Simple Text Editor in C for Windows

#include <windows.h>
#include <commdlg.h> // Required for common dialog boxes (Open/Save)
#include <stdio.h>   // For file operations (fopen, fread, fclose, fseek, ftell)
#include <stdlib.h>  // For memory allocation (malloc, free)

// Global variables
HINSTANCE hInst;                                    // Handle to the current instance
LPCSTR szTitle = "Professional Text Editor";        // The title bar text
LPCSTR szWindowClass = "PROFESSIONAL_TEXTEDITOR";   // The main window class name
HWND hEdit = NULL;                                  // Handle to the edit control window (initialize to NULL)

// Function declarations (Forward declarations)
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // Processes messages for the main window
ATOM MyRegisterClass(HINSTANCE hInstance);             // Registers the window class
BOOL InitInstance(HINSTANCE, int);                  // Saves instance handle and creates main window
void CreateEditorWindow(HWND);                      // Creates the text editor control
void HandleSize(HWND hWnd, LPARAM lParam);          // Handles the WM_SIZE message
void DoFileOpen(HWND hWnd);                         // Handles the File->Open action
void DoFileSave(HWND hWnd);                         // Handles the File->Save action

/**
 * @brief The main entry point for the application.
 *
 * @param hInstance Handle to the current instance of the application.
 * @param hPrevInstance Handle to the previous instance of the application (always NULL for Win32).
 * @param lpCmdLine Command line for the application, excluding the program name.
 * @param nCmdShow Specifies how the window is to be shown.
 * @return If the function succeeds, terminating when it receives a WM_QUIT message, it should return the exit value contained in that message's wParam parameter. If the function terminates before entering the message loop, it should return zero.
 */
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register the window class. If registration fails, display an error message and exit.
    if (!MyRegisterClass(hInstance)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return FALSE; // Indicate failure
    }

    // Perform application initialization: create the main window.
    // If window creation fails, display an error message and exit.
    if (!InitInstance(hInstance, nCmdShow)) {
        MessageBox(NULL, "Window Initialization Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return FALSE; // Indicate failure
    }

    // Main message loop: Retrieve messages from the thread's message queue
    // and dispatch them to the appropriate window procedure.
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) { // GetMessage returns 0 when WM_QUIT is received
        TranslateMessage(&msg); // Translates virtual-key messages into character messages
        DispatchMessage(&msg);  // Dispatches a message to a window procedure
    }

    // Return the exit code specified in the WM_QUIT message.
    return (int) msg.wParam;
}

/**
 * @brief Registers the window class for the application's main window.
 *
 * This function fills a WNDCLASSEX structure with information about the window class
 * and then registers it with the system using RegisterClassEx.
 *
 * @param hInstance Handle to the instance that contains the window procedure for the class.
 * @return If the function succeeds, the return value is a class atom that uniquely identifies the class being registered.
 *         If the function fails, the return value is zero.
 */
ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEX wcex; // Structure containing window class information

    wcex.cbSize = sizeof(WNDCLASSEX); // Size of the structure
    wcex.style = CS_HREDRAW | CS_VREDRAW; // Redraw if size changes, and redraw if movement or size adjustment changes the client area
    wcex.lpfnWndProc = WndProc; // Pointer to the window procedure
    wcex.cbClsExtra = 0; // No extra class memory
    wcex.cbWndExtra = 0; // No extra window memory
    wcex.hInstance = hInstance; // Handle to the instance that contains the window procedure
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION); // Default application icon
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW); // Standard arrow cursor
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Default window background color
    wcex.lpszMenuName = NULL; // No menu for this window class
    wcex.lpszClassName = szWindowClass; // Name of the window class
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION); // Small icon for the class (default application icon)

    // Register the window class with the operating system.
    return RegisterClassEx(&wcex);
}

/**
 * @brief Saves instance handle and creates main window.
 *
 * In this function, we save the instance handle in a global variable and
 * create and display the main program window.
 *
 * @param hInstance Handle to the current instance of the application.
 * @param nCmdShow Specifies how the window is to be shown.
 * @return TRUE if the window was created successfully, FALSE otherwise.
 */
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance; // Store instance handle in our global variable

    // Create the main application window.
    HWND hWnd = CreateWindow(
        szWindowClass,                  // Window class name
        szTitle,                        // Window title
        WS_OVERLAPPEDWINDOW,            // Window style (standard overlapped window)
        CW_USEDEFAULT, 0,               // Initial position (x, y) - let system decide
        CW_USEDEFAULT, 0,               // Initial size (width, height) - let system decide
        NULL,                           // Parent window handle (no parent)
        NULL,                           // Menu handle (none for the main window initially)
        hInstance,                      // Application instance handle
        NULL                            // Additional application data (none)
    );

    // Check if window creation was successful.
    if (!hWnd) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return FALSE; // Indicate failure
    }

    // Make the window visible on the screen.
    ShowWindow(hWnd, nCmdShow);
    // Removed the success message box as it's disruptive during startup
    // MessageBox(hWnd, "Window Created Successfully!", "Info", MB_OK);

    // Send a WM_PAINT message to the window procedure to paint the client area.
    UpdateWindow(hWnd);

    return TRUE; // Indicate success
}

/**
 * @brief Processes messages sent to the main window.
 *
 * This function is the window procedure for the main application window. It handles
 * various messages, such as window creation, destruction, and command messages from menus.
 *
 * @param hWnd Handle to the window.
 * @param message The message identifier.
 * @param wParam Additional message information. The content depends on the message value.
 * @param lParam Additional message information. The content depends on the message value.
 * @return The result of the message processing, which depends on the message sent.
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    // Message sent when the window is first created.
    case WM_CREATE: {
        // Create the menu bar
        HMENU hMenubar = CreateMenu();
        HMENU hMenu = CreateMenu(); // Temporary handle for submenus

        // --- File Menu ---
        AppendMenu(hMenu, MF_STRING, 1, "&New");    // Menu item ID 1
        AppendMenu(hMenu, MF_STRING, 2, "&Open");   // Menu item ID 2
        AppendMenu(hMenu, MF_STRING, 3, "&Save");   // Menu item ID 3
        AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);   // Separator line
        AppendMenu(hMenu, MF_STRING, 4, "E&xit");   // Menu item ID 4
        AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, "&File"); // Add File menu to menubar

        // --- Edit Menu ---
        hMenu = CreateMenu(); // Reuse handle for the Edit menu
        AppendMenu(hMenu, MF_STRING, 5, "Cu&t");    // Menu item ID 5
        AppendMenu(hMenu, MF_STRING, 6, "&Copy");   // Menu item ID 6
        AppendMenu(hMenu, MF_STRING, 7, "&Paste");  // Menu item ID 7
        AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, "&Edit"); // Add Edit menu to menubar

        // --- Help Menu ---
        hMenu = CreateMenu(); // Reuse handle for the Help menu
        AppendMenu(hMenu, MF_STRING, 8, "&About");  // Menu item ID 8
        AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, "&Help"); // Add Help menu to menubar

        // Attach the menu bar to the window
        SetMenu(hWnd, hMenubar);

        // Create the main text editor control window
        CreateEditorWindow(hWnd);
    }
    break; // End of WM_CREATE handling

    // Message sent when a window is being destroyed.
    case WM_DESTROY:
        PostQuitMessage(0); // Posts a WM_QUIT message to the thread's message queue, indicating a request to terminate the application.
    break; // End of WM_DESTROY handling

    // Message sent when the window size changes.
    case WM_SIZE:
        HandleSize(hWnd, lParam); // Call our resize handler
        break; // End of WM_SIZE handling

    // Message sent when the user selects a command item from a menu,
    // when a control sends a notification message to its parent window,
    // or when an accelerator keystroke is translated.
    case WM_COMMAND: {
        int wmId = LOWORD(wParam); // Extract the menu item ID or control identifier

        // Parse the menu selections:
        switch (wmId) {
        case 1: // File -> New
            // Clear the edit control
            if (hEdit) {
                SetWindowText(hEdit, ""); // Set text to empty string
            }
            break;
        case 2: // File -> Open
            DoFileOpen(hWnd); // Call the open file function
            break;
        case 3: // File -> Save
            DoFileSave(hWnd); // Call the save file function
            break;
        case 4: // File -> Exit
            DestroyWindow(hWnd); // Close the window
            break;
        case 5: // Edit -> Cut
            // Send WM_CUT message to the edit control
            if (hEdit) {
                SendMessage(hEdit, WM_CUT, 0, 0);
            }
            break;
        case 6: // Edit -> Copy
            // Send WM_COPY message to the edit control
            if (hEdit) {
                SendMessage(hEdit, WM_COPY, 0, 0);
            }
            break;
        case 7: // Edit -> Paste
            // Send WM_PASTE message to the edit control
            if (hEdit) {
                SendMessage(hEdit, WM_PASTE, 0, 0);
            }
            break;
        case 8: // Help -> About
            MessageBox(hWnd, "Professional Text Editor\nVersion 1.0\n\nA simple text editor example.", "About", MB_OK | MB_ICONINFORMATION);
            break;
        default:
            // For messages that we don't handle, pass them to the default window procedure.
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break; // End of WM_COMMAND handling

    // Handle other messages by passing them to the default window procedure.
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    // Return 0 for messages that we processed.
    return 0;
}

/**
 * @brief Handles the WM_SIZE message to resize the edit control.
 *
 * @param hWnd Handle to the main window.
 * @param lParam Contains the new width and height of the client area.
 */
void HandleSize(HWND hWnd, LPARAM lParam) {
    if (hEdit) { // Check if the edit control handle is valid
        // Extract the new width and height from lParam
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        // Resize the edit control to fill the client area
        SetWindowPos(hEdit, NULL, 0, 0, width, height, SWP_NOZORDER);
    }
}


/**
 * @brief Handles the File->Save menu command.
 *
 * Displays the standard Windows 'Save As' dialog box and saves the content
 * of the edit control to the selected file.
 *
 * @param hWnd Handle to the main window.
 */
void DoFileSave(HWND hWnd) {
    OPENFILENAME ofn;       // Common dialog box structure
    char szFile[MAX_PATH];  // Buffer for file name
    HANDLE hFile;           // File handle (using Windows API for potential future enhancements)
    DWORD dwBytesWritten;   // Number of bytes written
    char* pszEditText;      // Pointer to buffer for edit control text
    int nLength;            // Length of the text in the edit control

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    // Set lpstrFile[0] to '\0' so that GetSaveFileName does not
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0"; // Filter
    ofn.nFilterIndex = 1; // Default filter
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrDefExt = "txt"; // Default extension
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_EXPLORER; // Flags

    // Display the Save As dialog box.
    if (GetSaveFileName(&ofn) == TRUE) {
        // Get the length of the text in the edit control
        nLength = GetWindowTextLength(hEdit);
        if (nLength == 0 && GetLastError() != 0) {
             MessageBox(hWnd, "Could not get text length.", "Error", MB_OK | MB_ICONERROR);
             return;
        }

        // Allocate memory for the text (+1 for null terminator)
        pszEditText = (char*)malloc(nLength + 1);
        if (pszEditText == NULL) {
            MessageBox(hWnd, "Memory allocation failed.", "Error", MB_OK | MB_ICONERROR);
            return;
        }

        // Get the text from the edit control
        if (GetWindowText(hEdit, pszEditText, nLength + 1) == 0 && nLength > 0) {
             // GetWindowText returns 0 if it fails (and text length > 0) or if the text is empty.
             // We already checked nLength, so this indicates failure.
             MessageBox(hWnd, "Could not get text from editor.", "Error", MB_OK | MB_ICONERROR);
             free(pszEditText);
             return;
        }
        // Ensure null termination just in case (though GetWindowText should do it)
        pszEditText[nLength] = '\0';


        // Use standard C file I/O for simplicity
        FILE *pFile = fopen(ofn.lpstrFile, "wb"); // Open for binary write
        if (pFile == NULL) {
            MessageBox(hWnd, "Could not open file for saving.", "Error", MB_OK | MB_ICONERROR);
            free(pszEditText);
            return;
        }

        // Write the text to the file
        size_t bytesWritten = fwrite(pszEditText, 1, nLength, pFile); // Write nLength bytes

        // Clean up
        free(pszEditText);
        fclose(pFile);

        if (bytesWritten != nLength) {
            MessageBox(hWnd, "Error writing file.", "Error", MB_OK | MB_ICONERROR);
            // Optionally, delete the partially written file?
            // remove(ofn.lpstrFile);
        } else {
            // Optional: Display success message or update window title
            // MessageBox(hWnd, "File saved successfully.", "Success", MB_OK | MB_ICONINFORMATION);
        }

    } else {
        // User cancelled or an error occurred. Check CommDlgExtendedError() for details if needed.
        DWORD dwError = CommDlgExtendedError();
        if (dwError != 0) {
             char errorMsg[100];
             sprintf(errorMsg, "GetSaveFileName failed with error code: %lu", dwError);
             MessageBox(hWnd, errorMsg, "Dialog Error", MB_OK | MB_ICONWARNING);
        }
    }
}


/**
 * @brief Handles the File->Open menu command.
 *
 * Displays the standard Windows 'Open' dialog box and loads the selected file
 * into the edit control.
 *
 * @param hWnd Handle to the main window.
 */
void DoFileOpen(HWND hWnd) {
    OPENFILENAME ofn;       // Common dialog box structure
    char szFile[MAX_PATH];  // Buffer for file name
    HANDLE hFile;           // File handle (using Windows API for potential future enhancements)
    DWORD dwFileSize;       // Size of the file
    DWORD dwBytesRead;      // Number of bytes read
    char* pszFileText;      // Pointer to buffer for file text

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0"; // Filter for text files and all files
    ofn.nFilterIndex = 1; // Default filter index (Text Files)
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER; // Flags for the dialog

    // Display the Open dialog box.
    if (GetOpenFileName(&ofn) == TRUE) {
        // Use standard C file I/O for simplicity here
        FILE *pFile = fopen(ofn.lpstrFile, "rb"); // Open for binary read to handle different line endings correctly
        if (pFile == NULL) {
            MessageBox(hWnd, "Could not open file.", "Error", MB_OK | MB_ICONERROR);
            return;
        }

        // Get file size
        fseek(pFile, 0, SEEK_END);
        long fileSize = ftell(pFile);
        fseek(pFile, 0, SEEK_SET); // Go back to start

        // Allocate memory for the file content (+1 for null terminator)
        pszFileText = (char*)malloc(fileSize + 1);
        if (pszFileText == NULL) {
            MessageBox(hWnd, "Memory allocation failed.", "Error", MB_OK | MB_ICONERROR);
            fclose(pFile);
            return;
        }

        // Read file content
        size_t bytesRead = fread(pszFileText, 1, fileSize, pFile);
        if (bytesRead != fileSize) {
            // Handle potential read error (though fread might return less than fileSize at EOF without error)
            if (ferror(pFile)) {
                 MessageBox(hWnd, "Error reading file.", "Error", MB_OK | MB_ICONERROR);
                 free(pszFileText);
                 fclose(pFile);
                 return;
            }
            // If bytesRead < fileSize but no error, it might be okay, but we'll null-terminate based on bytesRead
            fileSize = bytesRead; // Adjust size to what was actually read
        }

        // Null-terminate the buffer
        pszFileText[fileSize] = '\0';

        // Set the text in the edit control
        if (hEdit) {
            // Consider potential issues with very large files and SetWindowText limit
            // For extremely large files, alternative approaches might be needed.
            SetWindowText(hEdit, pszFileText);
        }

        // Clean up
        free(pszFileText);
        fclose(pFile);

    } else {
        // User cancelled or an error occurred. Check CommDlgExtendedError() for details if needed.
        DWORD dwError = CommDlgExtendedError();
        if (dwError != 0) {
             char errorMsg[100];
             sprintf(errorMsg, "GetOpenFileName failed with error code: %lu", dwError);
             MessageBox(hWnd, errorMsg, "Dialog Error", MB_OK | MB_ICONWARNING);
        }
    }
}


/**
 * @brief Creates the main text editing control within the parent window.
 *
 * This function creates a standard Windows EDIT control, which serves as the
 * text area for the editor. It's configured as a multiline, scrollable text box.
 *
 * @param hWnd Handle to the parent window where the edit control will be placed.
 */
void CreateEditorWindow(HWND hWnd) {
    // Create an EDIT control (a multiline text box)
    // WS_CHILD: The window is a child window.
    // WS_VISIBLE: The window is initially visible.
    // WS_VSCROLL: The window has a vertical scroll bar.
    // ES_LEFT: Text is left-aligned.
    // ES_MULTILINE: The edit control allows multiple lines of text.
    // ES_AUTOVSCROLL: Automatically scrolls text vertically when the user types near the bottom.
    // The position (0,0) and size (500,500) are hardcoded for simplicity.
    // A more robust implementation would resize the edit control when the parent window resizes.
    CreateWindowEx(
        0,                              // Optional window styles (extended styles)
        "EDIT",                         // Predefined class name for an edit control
        NULL,                           // Window text (initial content, none here)
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, // Window style
        0, 0,                           // Position (x, y) relative to parent
        500, 500,                       // Size (width, height) - Hardcoded for now
        hWnd,                           // Parent window handle
        NULL,                           // Control ID (or menu handle for top-level windows) - None needed here
        hInst,                          // Instance handle
        NULL                            // Additional application data
    );

    // Store the handle to the edit control in the global variable
    // Use GetDlgItem if we assign an ID, or find it. For simplicity, find the first child.
    hEdit = FindWindowEx(hWnd, NULL, "EDIT", NULL);

    if (!hEdit) {
        MessageBox(hWnd, "Edit Control Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
    }
}
