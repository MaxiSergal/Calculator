import QtQuick 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls 2.15

Rectangle
{
    id: display

    property real delaySeconds: 0.0

    Layout.fillWidth: true
    Layout.minimumHeight: 100
    color: "#ffffff"
    border.color: "#cccccc"

    function addText(str)
    {
       switch(str)
       {
           case "−":
           {
               displayText.text += " − "
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

    Rectangle
    {
        id: delayBox

        width: 60
        height: 26
        color: "#f0f0f0"
        border.color: "#aaaaaa"
        radius: 0

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 0

        RowLayout
        {
            anchors.fill: parent
            anchors.margins: 0
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
                     buttonHeight: 13
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
                     buttonHeight: 13
                     buttonText:   "▼"

                     onClicked:
                     {
                        display.delaySeconds = Math.max(display.delaySeconds - 0.5, 0.0);
                     }
                 }
            }
        }
    }
}
