/**
 * @file fileops.c
 * @brief File operations implementation for the Professional Text Editor
 *
 * Contains functions for opening, saving, and managing files.
 */

#include "../include/fileops.h"
#include "../include/control.h"
#include "../include/window.h" // Needed for UpdateStatusBar and EditorState

// External global variables defined in window.c
extern HWND g_hStatusBar;
extern EditorState g_editorState;

/**
 * @brief Displays an Open file dialog and loads the selected file into the editor.
 *
 * @param hWnd Handle to the parent window for the dialog.
 * @param hEdit Handle to the edit control where the file will be loaded.
 * @return TRUE if a file was successfully opened, FALSE otherwise.
 */
BOOL EditorOpenFile(HWND hWnd, HWND hEdit) {
    if (!hWnd || !hEdit) {
        return FALSE;
    }
    
    OPENFILENAME ofn;
    char szFile[MAX_PATH] = {0};
    
    // Initialize OPENFILENAME structure
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
    
    // Display the Open dialog box
    if (GetOpenFileName(&ofn) != TRUE) {
        // User cancelled or an error occurred
        DWORD dwError = CommDlgExtendedError();
        if (dwError != 0) {
            char errorMsg[100];
            sprintf_s(errorMsg, sizeof(errorMsg), "GetOpenFileName failed with error code: %lu", dwError);
            MessageBox(hWnd, errorMsg, "Dialog Error", MB_OK | MB_ICONWARNING);
        }
        return FALSE;
    }
    
    // Read the file
    long fileSize = 0;
    char* fileText = ReadFileToBuffer(ofn.lpstrFile, &fileSize);
    if (!fileText) {
        MessageBox(hWnd, "Failed to read file.", "Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }
    
    // Set the text in the edit control
    BOOL result = SetEditorText(hEdit, fileText);
    
    // Clean up
    free(fileText);

    // Update editor state and status bar if successful
    if (result) {
        strcpy_s(g_editorState.currentFilePath, MAX_PATH, ofn.lpstrFile);
        g_editorState.currentFileSize = fileSize;
        UpdateStatusBar(g_hStatusBar, &g_editorState);
    }

    return result;
}

/**
 * @brief Displays a Save As dialog and saves the editor content to the selected file.
 *
 * @param hWnd Handle to the parent window for the dialog.
 * @param hEdit Handle to the edit control containing the text to save.
 * @return TRUE if a file was successfully saved, FALSE otherwise.
 */
BOOL EditorSaveFile(HWND hWnd, HWND hEdit) {
    if (!hWnd || !hEdit) {
        return FALSE;
    }
    
    OPENFILENAME ofn;
    char szFile[MAX_PATH] = {0};
    
    // Initialize OPENFILENAME structure
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrDefExt = "txt";  // Default extension
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_EXPLORER;
    
    // Display the Save As dialog box
    if (GetSaveFileName(&ofn) != TRUE) {
        // User cancelled or an error occurred
        DWORD dwError = CommDlgExtendedError();
        if (dwError != 0) {
            char errorMsg[100];
            sprintf_s(errorMsg, sizeof(errorMsg), "GetSaveFileName failed with error code: %lu", dwError);
            MessageBox(hWnd, errorMsg, "Dialog Error", MB_OK | MB_ICONWARNING);
        }
        return FALSE;
    }
    
    // Get the text from the edit control
    char* editorText = GetEditorText(hEdit);
    if (!editorText) {
        MessageBox(hWnd, "Failed to get text from editor.", "Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }
    
    // Write the text to the file
    BOOL result = WriteBufferToFile(ofn.lpstrFile, editorText, strlen(editorText));
    
    long savedSize = strlen(editorText); // Get the size before freeing
    free(editorText);

    if (!result) {
        MessageBox(hWnd, "Failed to write file.", "Error", MB_OK | MB_ICONERROR);
    } else {
        // Update editor state and status bar on successful save
        strcpy_s(g_editorState.currentFilePath, MAX_PATH, ofn.lpstrFile);
        g_editorState.currentFileSize = savedSize;
        UpdateStatusBar(g_hStatusBar, &g_editorState);
    }

    return result;
}

/**
 * @brief Creates a new empty document in the editor.
 *
 * @param hEdit Handle to the edit control.
 * @return TRUE if successful, FALSE otherwise.
 */
BOOL EditorNewFile(HWND hEdit) {
    if (!hEdit) {
        return FALSE;
    }

    BOOL result = SetEditorText(hEdit, "");
    if (result) {
        // Update editor state and status bar for new file
        strcpy_s(g_editorState.currentFilePath, MAX_PATH, "Untitled");
        g_editorState.currentFileSize = 0;
        UpdateStatusBar(g_hStatusBar, &g_editorState);
    }
    return result;
}

/**
 * @brief Reads a file into a buffer.
 *
 * @param filePath Path to the file to read.
 * @param[out] fileSize Pointer to a variable that will receive the file size.
 * @return A newly allocated buffer containing the file contents, or NULL on failure.
 *         The caller is responsible for freeing this memory.
 */
char* ReadFileToBuffer(const char* filePath, long* fileSize) {
    if (!filePath || !fileSize) {
        return NULL;
    }
    
    FILE* file = fopen(filePath, "rb");
    if (!file) {
        return NULL;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    *fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (*fileSize < 0) {
        fclose(file);
        return NULL;
    }
    
    // Allocate memory for file content (+1 for null terminator)
    char* buffer = (char*)malloc(*fileSize + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }
    
    // Read file content
    size_t bytesRead = fread(buffer, 1, *fileSize, file);
    fclose(file);
    
    if ((long)bytesRead != *fileSize) {
        free(buffer);
        return NULL;
    }
    
    // Null-terminate the buffer
    buffer[*fileSize] = '\0';
    
    return buffer;
}

/**
 * @brief Writes a buffer to a file.
 *
 * @param filePath Path to the file to write.
 * @param buffer The data to write.
 * @param bufferSize Size of the data in bytes.
 * @return TRUE if successful, FALSE otherwise.
 */
BOOL WriteBufferToFile(const char* filePath, const char* buffer, long bufferSize) {
    if (!filePath || !buffer) {
        return FALSE;
    }
    
    FILE* file = fopen(filePath, "wb");
    if (!file) {
        return FALSE;
    }
    
    // Write the buffer to the file
    size_t bytesWritten = fwrite(buffer, 1, bufferSize, file);
    fclose(file);
    
    return ((long)bytesWritten == bufferSize);
}
