import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

Item
{
    id: root

    property int buttonWidth:  20
    property int buttonHeight: 20

    property string buttonText:  "▲"

    property var gradient: Gradient {GradientStop {position: 0; color: "lightgray"}}

    Layout.minimumWidth:  buttonWidth
    Layout.minimumHeight: buttonHeight

    Layout.preferredWidth:  Math.max(buttonWidth, width)
    Layout.preferredHeight: Math.max(buttonHeight, height)

    Layout.fillWidth:  false
    Layout.fillHeight: false

    signal clicked

    Button
    {
        id: button

        padding:        0
        leftPadding:    0
        rightPadding:   0
        topPadding:     0
        bottomPadding:  0
        font.pixelSize: 11

        anchors.fill: parent
        text:         buttonText

        scale: pressed ? 0.90 : 1.0
        Behavior on scale { NumberAnimation { duration: 100 } }

        onClicked: root.clicked()

        background: Rectangle
        {
            anchors.fill: parent

            color: "transparent"
            radius:   1

            Rectangle
            {
               id: overlay

               anchors.fill: parent

               width:  parent.width
               height: parent.height
               radius: parent.radius
               color:  "black"

               opacity: button.pressed ? 0.05 : 0.0

               Behavior on opacity
               {
                   NumberAnimation
                   {
                       duration: 150
                       easing.type: Easing.OutQuad
                   }
               }
           }
        }

        contentItem: Text
        {
            text: button.text
            font: button.font
            color: "black"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
            elide: Text.ElideRight
        }
    }
}
