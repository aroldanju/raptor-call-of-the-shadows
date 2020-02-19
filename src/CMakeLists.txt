project(raptor C)

add_definitions(-D__DEVELOPER__)
add_definitions(-D__SHAREWARE__)
add_definitions(-D__DEBUG__)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdiscarded-qualifiers")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build)

FILE(GLOB SOURCES_GAME *.h *.c )

add_executable(${PROJECT_NAME} ${SOURCES_GAME})

target_link_libraries(${PROJECT_NAME} dependencies::ini-reader dependencies::res-packager dependencies::caengine)

#file(COPY ../resources/dist/DATA.RES DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/DATA)
#file(COPY ../resources/dist/ICON.BMP DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/DATA)
#file(COPY ../resources/dist/RAPTOR.INI DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/DATA)
