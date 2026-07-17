# Добавляем статическую библиотеку SharedNetwork
add_library(SharedNetwork STATIC
    src/NetworkProtocol.cpp
)

# Указываем заголовочные файлы
target_include_directories(SharedNetwork PUBLIC include)


target_link_libraries(SharedNetwork PUBLIC
    Qt6::Core
    Qt6::Network
)
