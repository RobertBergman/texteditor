/**
 * @file editor.h
 * @brief Main header file for the Professional Text Editor application
 *
 * This file contains common includes, constants, and declarations
 * used throughout the Professional Text Editor application.
 */

#ifndef EDITOR_H
#define EDITOR_H

#include <windows.h>
#include <commdlg.h> // Required for common dialog boxes (Open/Save)
#include <stdio.h>   // For file operations
#include <stdlib.h>  // For memory allocation
#include <stdbool.h> // For boolean values

// Global constants
#define EDITOR_CLASS_NAME "PROFESSIONAL_TEXTEDITOR"
#define EDITOR_TITLE "Professional Text Editor"
#define EDITOR_VERSION "1.0"

// Error handling macro
#define EDITOR_CHECK_ERROR(condition, message, title) \
    if (!(condition)) { \
        MessageBox(NULL, message, title, MB_ICONEXCLAMATION | MB_OK); \
        return FALSE; \
    }

// Exit codes
#define EDITOR_SUCCESS 0
#define EDITOR_ERROR 1

#endif /* EDITOR_H */
