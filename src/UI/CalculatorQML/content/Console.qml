import QtQuick 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls 2.15

Rectangle
{
    id: mainConsole

    property var entries:    []
    property var colorArray: ["blue", "green", "red"]
    property int maxLines:   100
    property int nextId:     1

    enum Colors
    {
        Blue,
        Green,
        Red
    }

    Layout.fillWidth:     true
    Layout.minimumHeight: 100

    border.color: "#cccccc"

    ScrollView
    {
        id: scrollView

        anchors.fill:     parent
        Layout.fillWidth: true
        clip:             true

        ScrollBar.vertical.policy: ScrollBar.AlwaysOn

        TextArea
        {
            id: consoleText

            wrapMode:       Text.Wrap
            font.pixelSize: 16
            color:          "black"
            readOnly:       true
            width:          parent.width

            text:       ">>"
            textFormat: Text.RichText

            onTextChanged:
            {
                Qt.callLater(() =>
                {
                    scrollView.contentItem.contentY = consoleText.height - scrollView.height

                })
            }

            Component.onCompleted:
            {
                Qt.callLater(() =>
                {
                    scrollView.contentItem.contentY = consoleText.height - scrollView.height
                })
            }
        }
    }

    function addEntry(text, color)
    {
        const id = nextId++;
        const html = "<p><font color=\"" + colorArray[color] + "\">>> " + text + "</font></p>";
        entries.push({ id: id, html: html });
        rebuildConsoleText();
        return id;
    }

    function appendToEntry(id, appendText, color)
    {
        for(let i = 0; i < entries.length; ++i)
        {
            if(entries[i].id === id)
            {
                const appendHtml = `<font color="${colorArray[color]}">${appendText}</font>`;
                entries[i].html = entries[i].html.replace(/<\/p>$/, appendHtml + "</p>");
                break;
            }
        }
        rebuildConsoleText();
    }

    function rebuildConsoleText()
    {
        const limited = entries.slice(Math.max(0, entries.length - maxLines));
        let body = limited.map(entry => entry.html).join("");
        consoleText.text = "<body>" + body + "</body>";
    }

    function clearConsole()
    {
        consoleText.text = ">>"
    }
}

