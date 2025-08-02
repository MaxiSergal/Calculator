import QtQuick 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls 2.15

Rectangle
{
    id: mainConsole

    Layout.fillWidth:     true
    Layout.minimumHeight: 100

    border.color: "#cccccc"

    ScrollView
    {
        anchors.fill:     parent
        Layout.fillWidth: true

        clip: true
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff

        TextArea
        {
            id: consoleTextArea

            readOnly:       true
            wrapMode:       Text.Wrap
            font.pixelSize: 16
            color:          "black"
            background:     Rectangle { color: "transparent" }

            text: ">> Добро пожаловать!\n"
        }
    }

    // Тестовая кнопка
    MouseArea
    {
        anchors.fill: parent
        onClicked:
        {
            consoleTextArea.text += ">> Новое сообщение " + Math.random().toFixed(3) + "\n"
        }
    }
}

