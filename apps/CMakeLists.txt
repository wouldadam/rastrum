# Example applications

# Use of the framebuffer
add_executable(framebuffer_example framebuffer_example.cpp)
target_compile_features(framebuffer_example PRIVATE cxx_std_20)
target_link_libraries(framebuffer_example PRIVATE rastrum)
target_clangformat_setup(framebuffer_example)

# Rendering a .obj model
add_executable(model_example model_example.cpp)
target_compile_features(model_example PRIVATE cxx_std_20)
target_link_libraries(model_example PRIVATE rastrum)
target_clangformat_setup(model_example)