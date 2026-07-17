# Исходные файлы
add_executable(TelecomClient
    src/main.cpp
)
# Заголовочные файлы
target_include_directories(TelecomClient PRIVATE include)

target_link_libraries(TelecomClient PRIVATE
    Qt6::Core
    Qt6::Network
    SharedNetwork # Добавляем общую статическую библиотеку для записи/чтения JSON в сокет
)
