#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

struct ViewPort
{
    const unsigned int LEFT;
    const unsigned int RIGHT;
    const unsigned int TOP;
    const unsigned int BOTTOM;
    const unsigned int WIDTH;
    const unsigned int HEIGHT;
};

namespace Globals
{
    constexpr unsigned int SCR_WIDTH = 800;
    constexpr unsigned int SCR_HEIGHT = 600;
    constexpr unsigned int PADDING = 15;
    constexpr ViewPort VIEWPORT = {.LEFT = PADDING, .RIGHT = SCR_WIDTH - PADDING, .TOP = PADDING, .BOTTOM = SCR_HEIGHT - PADDING, .WIDTH = SCR_WIDTH - 2 * PADDING, .HEIGHT = SCR_HEIGHT - 2 * PADDING};
    constexpr unsigned int LINESPACING = 6;
    constexpr unsigned int FONTSIZE = 14;
    constexpr const char *FONTSRC = "./assets/fonts/Futura.ttf";
}

#endif