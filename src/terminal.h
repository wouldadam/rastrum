#ifndef RASTRUM_TERMINAL_H
#define RASTRUM_TERMINAL_H

#include "rastrum/FrameBuffer.h"

namespace rastrum::terminal {

/** Sets the RGB color of the terminal. The terminal needs to support ANSI truecolor. */
void setColor(RGBA col);

/** Clears the terminal. */
void clear();

/** Resets the terminal color. */
void reset();
}  // namespace rastrum::terminal

#endif