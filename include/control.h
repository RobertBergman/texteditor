/**
 * @file control.h
 * @brief Editor control functionality for the Professional Text Editor
 *
 * Contains functions for creating and managing the text editor control.
 */

#ifndef CONTROL_H
#define CONTROL_H

#include "editor.h"

/**
 * @brief Creates the text editor control within the parent window.
 *
 * @param hWnd Handle to the parent window.
 * @param hInstance Handle to the application instance.
 * @return Handle to the created edit control, or NULL if creation failed.
 */
HWND CreateEditorControl(HWND hWnd, HINSTANCE hInstance);

/**
 * @brief Gets the text from the editor control.
 *
 * @param hEdit Handle to the edit control.
 * @return A newly allocated string containing the text, or NULL on failure.
 *         The caller is responsible for freeing this memory.
 */
char* GetEditorText(HWND hEdit);

/**
 * @brief Sets the text in the editor control.
 *
 * @param hEdit Handle to the edit control.
 * @param text The text to set.
 * @return TRUE if successful, FALSE otherwise.
 */
BOOL SetEditorText(HWND hEdit, const char* text);

/**
 * @brief Clears all text from the editor control.
 *
 * @param hEdit Handle to the edit control.
 */
void ClearEditorText(HWND hEdit);

#endif /* CONTROL_H */
