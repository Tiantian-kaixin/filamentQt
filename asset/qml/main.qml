import QtQuick 2.12
import filamentWindow 1.0

FilamentWindow {
    id: mainWindow
    visible: true
    width: 600
    height: 600
    title: qsTr("Filament Window")
    property var tt: Timer {
        interval: 500;
        running: true;
        repeat: true
        onTriggered: {
            mainWindow.updateFrame();
        }
    }
}
