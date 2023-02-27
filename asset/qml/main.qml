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
        anchors.centerIn: parent
        width: 500
        height: 500
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true;
        onPressed: {
            textureView.mouseEvent(mouse.x, mouse.y, 0)
        }
        onReleased: {
            textureView.mouseEvent(mouse.x, mouse.y, 2)
        }
        onPositionChanged: {
            textureView.mouseEvent(mouse.x, mouse.y, 1)
        }
    }
}
