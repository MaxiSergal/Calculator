import QtQuick 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls 2.15

Rectangle
{
    id: display

    property real delaySeconds:   0.0
    property int  requestsCount:  0
    property int  responsesCount: 0
    property int  currentMode:    1

    Layout.fillWidth: true
    Layout.minimumHeight: 100
    color: "#ffffff"
    border.color: "#cccccc"

    signal clicked

    function addText(str)
    {
       switch(str)
       {
           case "−":
           {
               displayText.text += " − "
               break;
           }
           case "-":
           {
               displayText.text += "-"
               break;
           }
           case "+":
           {
               displayText.text += " + "
               break;
           }
           case "÷":
           {
               displayText.text += " ÷ "
               break;
           }
           case "×":
           {
               displayText.text += " × "
               break;
           }
           case "=":
           {
               displayText.text += " = "
               break;
           }
           default:
           {
               displayText.text += str
               break;
           }
       }
    }

    function removeLast()
    {
       if(displayText.text.slice(-1) === " ")
           displayText.text = displayText.text.slice(0, -3);
       else
        displayText.text = displayText.text.slice(0, -1);
    }

    function getText()
    {
        return displayText.text
    }

    function clearText()
    {
        return displayText.text = ""
    }

    Text
    {
        id: displayText
        anchors.fill: parent
        anchors.margins: 10
        text: ""
        font.pixelSize: 32
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    RowLayout
    {
        spacing: 1

        Rectangle
        {
            id: delayBox

            width: 60
            height: 26
            color: "#f0f0f0"
            border.color: "#aaaaaa"
            radius: 3

            RowLayout
            {
                anchors.fill: parent
                spacing: 0

                Text
                {
                    text: display.delaySeconds.toFixed(1) + " s"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 14
                    leftPadding: 5

                    height: parent.height
                    width:  45

                    Layout.fillWidth: true
                }

                ColumnLayout
                {
                     spacing: 0
                     width:  15
                     height: parent.height

                     UpDownButton
                     {
                         id: upButton
                         buttonWidth:  15
                         buttonHeight: 10
                         buttonText:   "▲"

                         onClicked:
                         {
                             display.delaySeconds = Math.min(display.delaySeconds + 0.5, 10.0);
                         }
                     }
                     UpDownButton
                     {
                         id: downButton
                         buttonWidth:  15
                         buttonHeight: 10
                         buttonText:   "▼"

                         onClicked:
                         {
                            display.delaySeconds = Math.max(display.delaySeconds - 0.5, 0.0);
                         }
                     }
                }
            }

            WheelHandler
            {
                target: delayBox
                onWheel: function(event)
                {
                    if (event.angleDelta.y > 0)
                    {
                        upButton.clicked();
                    }
                    else if (event.angleDelta.y < 0)
                    {
                        downButton.clicked();
                    }
                    event.accepted = true;
                }
            }

        }

        Rectangle
        {
            id: requestsCount

            width: 70
            height: 26
            color: "#f0f0f0"
            border.color: "#aaaaaa"
            radius: 3

            Text
            {
                text: "Rqsts: " + display.requestsCount
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 14
                leftPadding: 5

                height: parent.height
                width:  45

                Layout.fillWidth: true
            }

        }

        Rectangle
        {
            id: responsesCount

            width: 70
            height: 26
            color: "#f0f0f0"
            border.color: "#aaaaaa"
            radius: 3

            Text
            {
                text: "Rspns: " + display.responsesCount
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 14
                leftPadding: 5

                height: parent.height
                width:  45

                Layout.fillWidth: true
            }

        }

        Item
        {
            id: root

            property int minButtonWidth:  70
            property int minButtonHeight: 26


            QtObject
            {
                id: enums
                readonly property int dll:  0
                readonly property int func: 1
            }

            property var colorArray: ["lightgrey", "white"]
            property var buttonMode: ["DLL",       "FUNC"]
            property string currentMode:  buttonMode[enums.func]
            property string currentColor: colorArray[enums.func]

            Layout.minimumWidth:  minButtonWidth
            Layout.minimumHeight: minButtonHeight

            Layout.preferredWidth:  Math.max(minButtonWidth, width)
            Layout.preferredHeight: Math.max(minButtonHeight, height)

            Layout.fillWidth:  true
            Layout.fillHeight: true

            signal clicked

            Button
            {
                id: button

                anchors.fill: parent
                text:         root.currentMode

                scale: pressed ? 0.90 : 1.0
                Behavior on scale { NumberAnimation { duration: 100 } }

                onClicked:
                {
                    if(root.currentMode === "FUNC")
                    {
                        root.currentMode    = root.buttonMode[enums.dll]
                        root.currentColor   = root.colorArray[enums.dll]
                        display.currentMode = 1
                    }
                    else
                    {
                        root.currentMode    = root.buttonMode[enums.func]
                        root.currentColor   = root.colorArray[enums.func]
                        display.currentMode = 0
                    }

                    display.clicked()
                }

                background: Rectangle
                {
                    anchors.fill: parent

                    border.color: "#aaaaaa"
                    color: root.currentColor
                    radius:   3

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
                }
            }
        }

        Item
        {
            id: help

            property int minButtonWidth:  26
            property int minButtonHeight: 26

            Layout.minimumWidth:  minButtonWidth
            Layout.minimumHeight: minButtonHeight

            Layout.preferredWidth:  Math.max(minButtonWidth, width)
            Layout.preferredHeight: Math.max(minButtonHeight, height)

            Layout.fillWidth:  true
            Layout.fillHeight: true

            HelpWindow
            {
                id: helpWindow

                posX: display.x
                posY: display.y
            }

            Button
            {
                id: helpButton

                anchors.fill: parent
                text:         "?"

                scale: pressed ? 0.90 : 1.0
                Behavior on scale { NumberAnimation { duration: 100 } }

                onClicked: helpWindow.show()

                background: Rectangle
                {
                    anchors.fill: parent

                    border.color: "#aaaaaa"
                    color:        "#f0f0f0"
                    radius:   3

                    Rectangle
                    {
                       id: helpButtonOverlay

                       anchors.fill: parent

                       width:  parent.width
                       height: parent.height
                       radius: parent.radius
                       color:  "black"

                       opacity: helpButton.pressed ? 0.05 : 0.0

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
                    text: helpButton.text
                    font: helpButton.font
                    color: "black"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.fill: parent
                }
            }
        }

    }
}
