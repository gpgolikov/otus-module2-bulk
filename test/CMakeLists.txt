project(${PROJECT_NAME}_tests)

list(APPEND ${PROJECT_NAME}_SOURCES
    ../src/command.cpp
    ../src/command_factory.cpp
    ../src/executer.cpp
    ../src/reader.cpp
    ../src/interpreter.cpp
    test_executers.cpp
    test_command_factory.cpp
    test_reader.cpp
    test_interpreter.cpp
    main.cpp)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME} CONAN_PKG::Catch2)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
    INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src
)

enable_testing()

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})