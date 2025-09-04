cmake_minimum_required(VERSION 3.15)
project(windows-shutdown LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRCDIR "${CMAKE_SOURCE_DIR}/windows-shutdown")

set(SRCS
    ${SRCDIR}/main.cpp
    ${SRCDIR}/config.cpp
    ${SRCDIR}/controller.cpp
    ${SRCDIR}/i18n.cpp
    ${SRCDIR}/image-button.cpp
    ${SRCDIR}/render.cpp
    ${SRCDIR}/window.cpp
)

# Resource handling: MSVC supports .rc directly; MinGW needs windres to produce an object
# The goal is to make the resource file part of the dependency graph so changes to the
# .rc trigger an incremental rebuild. For MinGW we create a generated object from the .rc
# and add that object into the sources list. For MSVC we add the .rc file directly which
# the MSVC compiler will process.
set(RC_FILE ${SRCDIR}/windows-shutdown.rc)
if(MINGW)
    find_program(WINDRES_EXE windres)
    if(WINDRES_EXE)
        set(RC_OBJ ${CMAKE_BINARY_DIR}/windows-shutdown_rc.o)
        add_custom_command(
            OUTPUT ${RC_OBJ}
            COMMAND ${WINDRES_EXE} --target=pe-x86-64 -i ${RC_FILE} -o ${RC_OBJ}
            DEPENDS ${RC_FILE}
            COMMENT "Compiling resource with windres"
        )
        list(APPEND SRCS ${RC_OBJ})
    else()
        message(WARNING "windres not found; resource file will be skipped for MinGW builds")
    endif()
else()
    # For MSVC/Visual Studio generators, include the .rc so the IDE and build system
    # will treat it as a resource compilation unit and rebuild when it changes.
    list(APPEND SRCS ${RC_FILE})
endif()

add_executable(windows-shutdown WIN32 ${SRCS})

target_include_directories(windows-shutdown PRIVATE ${SRCDIR})
target_compile_definitions(windows-shutdown PRIVATE _WINDOWS UNICODE _UNICODE)

target_link_libraries(windows-shutdown PRIVATE
    user32
    gdi32
    gdiplus
    kernel32
    shell32
    comctl32
    advapi32
    ole32
    powrprof
)

# Output directories
set_target_properties(windows-shutdown PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/x64/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/x64/Release
)
