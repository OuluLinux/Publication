cmake_minimum_required(VERSION 3.16)
project(PublicationApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PUBLICATION_SOURCES
    src/PublicationApp/main.cpp
)

find_package(Wt REQUIRED COMPONENTS Wt HTTP)
find_package(SQLite3 REQUIRED)

add_executable(${PROJECT_NAME} ${PUBLICATION_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE Wt::Wt Wt::HTTP SQLite::SQLite3)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -pedantic>
)

# Convenience target mirroring run_app.sh usage.
add_custom_target(run
    COMMAND ${CMAKE_SOURCE_DIR}/run_app.sh $<TARGET_FILE:${PROJECT_NAME}>
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Starting PublicationApp via run_app.sh"
)
