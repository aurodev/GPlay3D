set(GAME_NAME sample-spaceship)

set(GAME_SRC
    src/SpaceshipGame.cpp
    src/SpaceshipGame.h
)

add_executable(${GAME_NAME} ${GAME_SRC})

target_link_libraries(${GAME_NAME} ${GPLAY3D_LIBRARIES})

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/game.config ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${GAME_NAME}.config COPYONLY)

