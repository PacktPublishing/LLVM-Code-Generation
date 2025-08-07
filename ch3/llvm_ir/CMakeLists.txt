set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")
set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")

cmake_minimum_required(VERSION 3.22)

project(CH3
        LANGUAGES CXX C)

include(../../cmake/utils/set-llvm-install-prefix.cmake)

set(CURR_TARGET
  build_ir
)

set(SRC
  "main.cpp"
  "solution/populate_function.cpp"
  "your_turn/populate_function.cpp"
)

set(INC
)

set(CMAKE_CXX_FLAGS -fno-rtti)

# Now build our tools
add_executable(${CURR_TARGET} ${SRC} ${INC})

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core)

# Link against LLVM libraries
target_link_libraries(${CURR_TARGET} ${llvm_libs})
