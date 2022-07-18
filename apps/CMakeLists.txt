# Example applications

# Print hello
add_executable(hello hello.cpp)
target_compile_features(hello PRIVATE cxx_std_20)
target_link_libraries(hello PRIVATE rastrum)
target_clangformat_setup(hello)