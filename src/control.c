/**
 * @file control.c
 * @brief Editor control implementation for the Professional Text Editor
 *
 * Contains functions for creating and managing the text editor control.
 */

#include "../include/control.h"

/**
 * @brief Creates the text editor control within the parent window.
 *
 * @param hWnd Handle to the parent window.
 * @param hInstance Handle to the application instance.
 * @return Handle to the created edit control, or NULL if creation failed.
 */
HWND CreateEditorControl(HWND hWnd, HINSTANCE hInstance) {
    // Create a multiline, scrollable edit control
    HWND hEdit = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_LEFT | 
        ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
        0, 0, 0, 0,  // Position and size will be set by WM_SIZE handler
        hWnd,
        NULL,
        hInstance,
        NULL
    );
    
    if (!hEdit) {
        return NULL;
    }
    
    // Set a reasonable default font (system font)
    HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    if (hFont) {
        SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
    }
    
    return hEdit;
}

/**
 * @brief Gets the text from the editor control.
 *
 * @param hEdit Handle to the edit control.
 * @return A newly allocated string containing the text, or NULL on failure.
 *         The caller is responsible for freeing this memory.
 */
char* GetEditorText(HWND hEdit) {
    if (!hEdit) {
        return NULL;
    }
    
    // Get the length of the text
    int length = GetWindowTextLength(hEdit);
    if (length <= 0) {
        // No text or error
        if (GetLastError() != 0) {
            // An error occurred
            return NULL;
        }
        
        // Empty text, return an empty string
        char* emptyText = (char*)malloc(1);
        if (emptyText) {
            emptyText[0] = '\0';
        }
        return emptyText;
    }
    
    // Allocate memory for the text (+1 for null terminator)
    char* buffer = (char*)malloc(length + 1);
    if (!buffer) {
        return NULL;
    }
    
    // Get the text
    if (GetWindowText(hEdit, buffer, length + 1) == 0) {
        free(buffer);
        return NULL;
    }
    
    return buffer;
}

/**
 * @brief Sets the text in the editor control.
 *
 * @param hEdit Handle to the edit control.
 * @param text The text to set.
 * @return TRUE if successful, FALSE otherwise.
 */
BOOL SetEditorText(HWND hEdit, const char* text) {
    if (!hEdit) {
        return FALSE;
    }
    
    // NULL text is treated as empty string
    if (text == NULL) {
        text = "";
    }
    
    return SetWindowText(hEdit, text);
}

/**
 * @brief Clears all text from the editor control.
 *
 * @param hEdit Handle to the edit control.
 */
void ClearEditorText(HWND hEdit) {
    SetEditorText(hEdit, "");
}
