# List all headers and source files for the lib here
set(HEADERS ${PROJECT_SOURCE_DIR}/include/rastrum/rastrum.h)
set(SOURCES rastrum.cpp)

# The main library
add_library(rastrum ${SOURCES} ${HEADERS})
target_include_directories(rastrum PUBLIC ../include)
target_compile_features(rastrum PUBLIC cxx_std_20)
target_clangformat_setup(rastrum)