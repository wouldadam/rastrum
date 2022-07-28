#ifndef RASTRUM_OBJLOADER_H
#define RASTRUM_OBJLOADER_H

#include <string>

#include "rastrum/Model.h"

namespace rastrum::obj {

/**
 * Loads a .obj file and returns the loaded model.
 * Only supports a small subset of the .obj format:
 *  - XYZ vertices (other values are ignored, terminates if XYZ not provided)
 *  - 3 sided faces (other values cause termination)
 * All other parts of the file are ignored.
 */
auto load(const std::string& filename) -> Model;

}  // namespace rastrum::obj

#endif