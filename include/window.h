/**
 * @file window.h
 * @brief Window management functionality for the Professional Text Editor
 *
 * Contains functions for window registration, creation, and message handling.
 */

#ifndef WINDOW_H
#define WINDOW_H

#include "editor.h"

/**
 * @brief Registers the main window class for the application.
 *
 * @param hInstance Handle to the instance that contains the window procedure.
 * @return If the function succeeds, the return value is a class atom that uniquely identifies the class.
 *         If the function fails, the return value is zero.
 */
ATOM RegisterMainWindow(HINSTANCE hInstance);

/**
 * @brief Creates and initializes the main application window.
 *
 * @param hInstance Handle to the current instance of the application.
 * @param nCmdShow Specifies how the window is to be shown.
 * @return TRUE if the window was created successfully, FALSE otherwise.
 */
BOOL CreateMainWindow(HINSTANCE hInstance, int nCmdShow);

/**
 * @brief Window procedure for the main application window.
 *
 * @param hWnd Handle to the window.
 * @param message The message.
 * @param wParam Additional message information.
 * @param lParam Additional message information.
 * @return The result of the message processing.
 */
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/**
 * @brief Handles window resize events.
 *
 * @param hWnd Handle to the main window.
 * @param lParam Contains the new width and height of the client area.
 */
void HandleWindowResize(HWND hWnd, LPARAM lParam);

/**
 * @brief Updates the status bar text with the current editor state.
 *
 * @param hStatusBar Handle to the status bar control.
 * @param state Pointer to the EditorState structure containing file info.
 */
void UpdateStatusBar(HWND hStatusBar, const EditorState* state);

#endif /* WINDOW_H */
