#ifndef COLORS_H
#define COLORS_H

#include <iostream>
#include <string>

/**
 * @file COLORS.h
 * @brief ANSI escape code color formatting utilities for colored terminal output
 * @author Bandile Mnyandu
 */

/// @brief Wraps a string in black ANSI color codes
inline std::string to_black(std::string x){
    return("\033[30m"+x+"\033[0m");
}

/// @brief Wraps a string in red ANSI color codes
inline std::string to_red(std::string x){
    return("\033[31m"+x+"\033[0m");
}

/// @brief Wraps a string in green ANSI color codes
inline std::string to_green(std::string x){
    return("\033[32m"+x+"\033[0m");
}

/// @brief Wraps a string in yellow ANSI color codes
inline std::string to_yellow(std::string x){
    return("\033[33m"+x+"\033[0m");
}

/// @brief Wraps a string in blue ANSI color codes
inline std::string to_blue(std::string x){
    return("\033[34m"+x+"\033[0m");
}

/// @brief Wraps a string in magenta ANSI color codes
inline std::string to_magenta(std::string x){
    return("\033[35m"+x+"\033[0m");
}

/// @brief Wraps a string in cyan ANSI color codes
inline std::string to_cyan(std::string x){
    return("\033[36m"+x+"\033[0m");
}

/// @brief Wraps a string in whire ANSI color codes
inline std::string to_white(std::string x){
    return("\033[37m"+x+"\033[0m");
}

/// @brief Wraps a string in grey ANSI color codes
inline std::string to_grey(std::string x){
    return("\033[90m"+x+"\033[0m");
}

/// @brief Wraps a string in bright red ANSI color codes
inline std::string to_brightRed(std::string x){
    return("\033[91m"+x+"\033[0m");
}

/// @brief Wraps a string in bright green ANSI color codes
inline std::string to_brightGreen(std::string x){
    return("\033[92m"+x+"\033[0m");
}

/// @brief Wraps a string in bright blue ANSI color codes
inline std::string to_brightBlue(std::string x){
    return("\033[94m"+x+"\033[0m");
}

/// @brief Wraps a string in bright magenta ANSI color codes
inline std::string to_brightMagenta(std::string x){
    return("\033[95m"+x+"\033[0m");
}

/// @brief Wraps a string in red ANSI color codes
inline std::string to_brightCyan(std::string x){
    return("\033[96m"+x+"\033[0m");
}

/// @brief Wraps a string in bright white ANSI color codes
inline std::string to_brightWhite(std::string x){
    return("\033[97m"+x+"\033[0m");
}

#endif