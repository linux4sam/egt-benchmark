cmake_minimum_required(VERSION 3.23)

project(
    egt-benchmark
    VERSION 1.3
    DESCRIPTION "An application to benchmark the EGT library."
    LANGUAGES CXX
)
#set(PROJECT_VERSION_RC 1)
if (DEFINED PROJECT_VERSION_RC)
    string(APPEND PROJECT_VERSION "-rc${PROJECT_VERSION_RC}")
endif()

include(GNUInstallDirs)

set(CMAKE_VERBOSE_MAKEFILE True)

set(CMAKE_CXX_STANDARD 17)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBEGT REQUIRED libegt>=1.11)
pkg_check_modules(LIBINPUT REQUIRED libinput>=1.6.3)
pkg_check_modules(CAIRO REQUIRED cairo>=1.14.6)

add_executable(egt-benchmark src/main.cpp)

target_compile_definitions(egt-benchmark PRIVATE BENCHMARKDATA="${CMAKE_INSTALL_FULL_DATADIR}/egt/benchmark")

target_include_directories(egt-benchmark PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/external/hayai/src
)

target_link_libraries(egt-benchmark PRIVATE udev)

target_include_directories(egt-benchmark PRIVATE ${LIBEGT_INCLUDE_DIRS})
target_compile_options(egt-benchmark PRIVATE ${LIBEGT_CFLAGS_OTHER})
target_link_directories(egt-benchmark PRIVATE ${LIBEGT_LIBRARY_DIRS})
target_link_libraries(egt-benchmark PRIVATE ${LIBEGT_LIBRARIES})
target_link_options(egt-benchmark PRIVATE ${LIBEGT_LDFLAGS_OTHER})

target_include_directories(egt-benchmark PRIVATE ${LIBINPUT_INCLUDE_DIRS})
target_compile_options(egt-benchmark PRIVATE ${LIBINPUT_CFLAGS_OTHER})
target_link_directories(egt-benchmark PRIVATE ${LIBINPUT_LIBRARY_DIRS})
target_link_libraries(egt-benchmark PRIVATE ${LIBINPUT_LIBRARIES})
target_link_options(egt-benchmark PRIVATE ${LIBINPUT_LDFLAGS_OTHER})

target_include_directories(egt-benchmark PRIVATE ${CAIRO_INCLUDE_DIRS})
target_compile_options(egt-benchmark PRIVATE ${CAIRO_CFLAGS_OTHER})
target_link_directories(egt-benchmark PRIVATE ${CAIRO_LIBRARY_DIRS})
target_link_libraries(egt-benchmark PRIVATE ${CAIRO_LIBRARIES})
target_link_options(egt-benchmark PRIVATE ${CAIRO_LDFLAGS_OTHER})

install(TARGETS egt-benchmark RUNTIME)
install(DIRECTORY images/
        DESTINATION ${CMAKE_INSTALL_DATADIR}/egt/benchmark
        FILES_MATCHING
        PATTERN "*.png"
)
