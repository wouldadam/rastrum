# List all headers and source files for the lib here
set(HEADERS ${PROJECT_SOURCE_DIR}/include/rastrum/FrameBuffer.h
            ${PROJECT_SOURCE_DIR}/include/rastrum/Model.h
            ${PROJECT_SOURCE_DIR}/include/rastrum/Obj.h
            ${PROJECT_SOURCE_DIR}/include/rastrum/Vector.h)
set(SOURCES FrameBuffer.cpp
            Model.cpp
            Obj.cpp
            stb.cpp)

# The main library
add_library(rastrum ${SOURCES} ${HEADERS})
target_include_directories(rastrum PUBLIC ../include)
target_include_directories(rastrum PRIVATE ../extern)
target_compile_features(rastrum PUBLIC cxx_std_20)
target_clangformat_setup(rastrum)