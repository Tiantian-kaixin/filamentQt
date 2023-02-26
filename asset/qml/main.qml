import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import filament 1.0
import "./"

Window {
    id: mainWindow
    visible: true
    width: 600
    height: 600
    title: qsTr("Filament Window")
    TextureView {
        id: textureView
        anchors.left: parent.left
        anchors.top: parent.top
        width: 300
        height: 300
    }
}
