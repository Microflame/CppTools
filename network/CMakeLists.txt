add_library(Network
    network.cpp
)

add_executable(NetworkTest network_test.cpp)
target_link_libraries(NetworkTest PRIVATE Network)
