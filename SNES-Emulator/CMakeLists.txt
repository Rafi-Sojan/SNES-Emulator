# CMakeList.txt : CMake project for SNES-Emulator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SNES-Emulator")

# Add source to this project's executable.
add_executable (SNES-Emulator "src/emulator-main.cpp" "include/emulator.h" "src/cpu.cpp" "src/ppu.cpp" "src/apu.cpp" "src/memory.cpp" "src/bus.cpp" "include/cpu.h" "include/ppu.h" "include/apu.h" "include/cartridge.h" "include/bus.h" "include/memory.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SNES-Emulator PROPERTY CXX_STANDARD 20)
endif()

find_package(SFML CONFIG REQUIRED COMPONENTS system window graphics audio)

message(STATUS "CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")
message(STATUS "VCPKG_TOOLCHAIN = ${CMAKE_TOOLCHAIN_FILE}")

find_package(SFML CONFIG REQUIRED COMPONENTS system window graphics audio)

target_include_directories(SNES-Emulator PRIVATE include)

target_link_libraries(SNES-Emulator PRIVATE
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
)
# TODO: Add tests and install targets if needed.
