import QtQuick 2.12
import QtQuick.Controls 2.12
import filamentWindow 1.0
import "./"

FilamentWindow {
    id: mainWindow
    visible: true
    width: 600
    height: 600
    title: qsTr("Filament Window")
    Timer {
        interval: 500;
        running: true;
        repeat: true
        onTriggered: {
            mainWindow.updateParameter([Math.random(), Math.random(), Math.random(), 1]);
        }
    }
}
