project(raptor-cli C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build)

FILE(GLOB SOURCES_CLI *.h *.c behaviour/*.c image/*.c map/*.c weapon/*.c ship/*.c item/*.c)

find_package(LibXml2 REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${LIBXML2_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES_CLI})
target_link_libraries(${PROJECT_NAME} dependencies::res-packager dependencies::caengine -lxml2)

