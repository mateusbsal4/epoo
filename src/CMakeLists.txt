add_executable("${PROJECT_NAME}" main.cpp)

add_subdirectory(drone)
target_link_libraries("${PROJECT_NAME}"
    Bateria
    Drone
    S1000
)
