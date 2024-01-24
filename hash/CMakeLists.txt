add_library(Hash
    md5.cpp
)

add_executable(Md5Test md5_test.cpp)
target_link_libraries(Md5Test PRIVATE Hash)