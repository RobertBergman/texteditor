/**
 * @file fileops.h
 * @brief File operations for the Professional Text Editor
 *
 * Contains functions for opening, saving, and managing files.
 */

#ifndef FILEOPS_H
#define FILEOPS_H

#include "editor.h"

/**
 * @brief Displays an Open file dialog and loads the selected file into the editor.
 *
 * @param hWnd Handle to the parent window for the dialog.
 * @param hEdit Handle to the edit control where the file will be loaded.
 * @return TRUE if a file was successfully opened, FALSE otherwise.
 */
BOOL EditorOpenFile(HWND hWnd, HWND hEdit);

/**
 * @brief Displays a Save As dialog and saves the editor content to the selected file.
 *
 * @param hWnd Handle to the parent window for the dialog.
 * @param hEdit Handle to the edit control containing the text to save.
 * @return TRUE if a file was successfully saved, FALSE otherwise.
 */
BOOL EditorSaveFile(HWND hWnd, HWND hEdit);

/**
 * @brief Creates a new empty document in the editor.
 *
 * @param hEdit Handle to the edit control.
 * @return TRUE if successful, FALSE otherwise.
 */
BOOL EditorNewFile(HWND hEdit);

/**
 * @brief Reads a file into a buffer.
 *
 * @param filePath Path to the file to read.
 * @param[out] fileSize Pointer to a variable that will receive the file size.
 * @return A newly allocated buffer containing the file contents, or NULL on failure.
 *         The caller is responsible for freeing this memory.
 */
char* ReadFileToBuffer(const char* filePath, long* fileSize);

/**
 * @brief Writes a buffer to a file.
 *
 * @param filePath Path to the file to write.
 * @param buffer The data to write.
 * @param bufferSize Size of the data in bytes.
 * @return TRUE if successful, FALSE otherwise.
 */
BOOL WriteBufferToFile(const char* filePath, const char* buffer, long bufferSize);

#endif /* FILEOPS_H */
