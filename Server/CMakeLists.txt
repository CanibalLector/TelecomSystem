
find_package(Qt6 6.5.2 REQUIRED COMPONENTS Gui Qml Quick)

#
qt_add_executable(TelecomServer
    include/NetworkWorker.hpp
    include/ClientData.hpp
    src/main.cpp
    src/NetworkWorker.cpp
    src/ClientData.cpp
)

# Set QML Module
qt_add_qml_module(TelecomServer
    URI Telecom.Server.Components
    VERSION 1.0
    QML_FILES
        qml/Main.qml
)

target_include_directories(TelecomServer PRIVATE include)

target_link_libraries(TelecomServer PRIVATE
    Qt6::Core
    Qt6::Network
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    SharedNetwork # Добавляем общую статическую библиотеку для записи/чтения JSON в сокет
)
