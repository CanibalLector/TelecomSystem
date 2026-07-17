
add_executable(TelecomClient
    src/main.cpp
)

target_include_directories(TelecomClient PRIVATE include)

target_link_libraries(TelecomClient PRIVATE
    Qt6::Core
    Qt6::Network
)
