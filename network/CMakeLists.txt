add_library(Network
    network.cpp
    http.cpp
)
target_link_libraries(Network PRIVATE Strings)

add_executable(NetworkTest network_test.cpp)
target_link_libraries(NetworkTest PRIVATE Network)
