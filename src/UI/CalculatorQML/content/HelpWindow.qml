import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window
{
    id: helpWindow

    property int posX: 0
    property int posY: 0

    x: posX
    y: posY
    width:  400
    height: 300
    minimumWidth:  400
    minimumHeight: 300
    visible: false
    title: "Справка"
    color: "#ffffff"

    Text
    {
        anchors.centerIn: parent
        text: "Здесь находится информация о программе."
        wrapMode: Text.WordWrap
        font.pixelSize: 16
        padding: 20
    }
}
