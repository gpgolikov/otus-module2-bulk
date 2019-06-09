list(APPEND ${PROJECT_NAME}_SOURCES
    command.cpp
    command_factory.cpp
    executer.cpp
    execution_logger.cpp
    reader.cpp
    interpreter.cpp
    main.cpp)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)