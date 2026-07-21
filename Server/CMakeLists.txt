
find_package(Qt6 6.5.2 REQUIRED COMPONENTS Gui Qml Quick QuickControls2)

#
qt_add_executable(TelecomServer
    include/NetworkWorker.hpp
    include/ClientData.hpp
    include/ServerController.hpp
    src/main.cpp
    src/NetworkWorker.cpp
    src/ClientData.cpp
    src/ServerController.cpp
)

# Set QML Module
qt_add_qml_module(TelecomServer
    URI Telecom.Server.Components
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/ControlButton.qml
        qml/ClientsView.qml
        qml/EventsView.qml
        qml/DataView.qml
        qml/ControlView.qml
        qml/SettingsView.qml
    RESOURCES
        images/bg.png
        images/area-chart-96.png
        images/console-96.png
        images/settings-96.png
        images/structure-96.png
        images/list-96.png
)


target_include_directories(TelecomServer PRIVATE include)

target_link_libraries(TelecomServer PRIVATE
    Qt6::Core
    Qt6::Network
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    SharedNetwork # Добавляем общую статическую библиотеку для записи/чтения JSON в сокет
)
