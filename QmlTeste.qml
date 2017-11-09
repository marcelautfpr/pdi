import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Templates 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Scene3D 2.0
import QtQuick.Dialogs 1.2

Item {
    id: item1
    Button {
        id: button1
        x: 84
        y: 52
        text: "Button"
        checkable: false
        highlighted: true
        spacing: 2
    }

    Rectangle {
        id: rectangle1
        x: 153
        y: 40
        width: 219
        height: 86
        color: "#ffffff"
    }

    Slider {
        id: slider1
        x: 104
        y: 220
        anchors.verticalCenterOffset: 0
        orientation: Qt.Horizontal
        value: 0.5
        anchors.verticalCenter: parent.verticalCenter
    }

    SpinBox {
        id: spinBox1
        x: 358
        y: 220
        anchors.verticalCenter: parent.verticalCenter
        anchors.top: slider1.top
        anchors.bottom: slider1.bottom
    }

    Connections {
        target: slider1
        onStateChanged: print("clicked")
    }

    Slider {
        id: slider2
        x: 214
        y: 225
        anchors.horizontalCenterOffset: -121
        value: 0.5
        anchors.horizontalCenter: parent.horizontalCenter
    }

    SpinBox {
        id: spinBox2
        x: 193
        y: 225
        anchors.verticalCenter: slider2.verticalCenter
        anchors.bottom: slider2.bottom
        anchors.top: slider2.top
        anchors.horizontalCenterOffset: 69
        anchors.horizontalCenter: rectangle1.horizontalCenter
    }

    Slider {
        id: slider3
        x: 138
        y: 191
        value: 0.5
    }

    SpinBox {
        id: spinBox3
        x: 363
        y: 191
        anchors.verticalCenter: slider3.verticalCenter
        anchors.top: slider3.top
        anchors.bottom: slider3.bottom
    }


}
