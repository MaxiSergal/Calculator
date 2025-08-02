import QtQuick 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls 2.15

Rectangle
{
    id: mainConsole

    property var colorArray: ["blue", "green", "red"]
    property int maxLines:   10

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

        clip: true
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn

        TextArea
        {
            id: consoleText

            wrapMode:       Text.Wrap
            font.pixelSize: 16
            color:          "black"
            readOnly: true
            width: parent.width

            text: ">> Добро пожаловать!"
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

    // Тестовая кнопка
    MouseArea
    {
        anchors.fill: parent
        onClicked:
        {
            let updatedText = consoleText.text.replace(/<\/body>/i, "<div style=\"color:" + "red" + ";\">" + ">> Добро пожаловать!" + "</div>");
            consoleText.text = trimParagraphs(updatedText, maxLines);
        }
    }

    function trimParagraphs(htmlText, maxLines)
    {
        let paragraphs = htmlText.match(/<p\b[^>]*>[\s\S]*?<\/p>/gi) || [];

        if (paragraphs.length <= maxLines)
            return htmlText;

        let paragraphsToKeep = paragraphs.slice(paragraphs.length - maxLines);

        let headerMatch = htmlText.match(/^[\s\S]*?<body[^>]*>/i);
        let footerMatch = htmlText.match(/<\/body>[\s\S]*$/i);

        let header = headerMatch ? headerMatch[0] : "";
        let footer = footerMatch ? footerMatch[0] : "";

        let newBodyContent = paragraphsToKeep.join("");

        return header + newBodyContent + footer;
    }

    function addText(text, color)
    {
        switch(color)
        {
            case Color.Blue:
            {
                let updatedText = consoleText.text.replace(/<\/body>/i, "<div style=\"color:" + colorArray[0] + ";\">>> " + text + "</div>");
                consoleText.text = trimParagraphs(updatedText, maxLines);
                break
            }
            case Color.Green:
            {
                let updatedText = consoleText.text.replace(/<\/body>/i, "<div style=\"color:" + colorArray[1] + ";\">>> " + text + "</div>");
                consoleText.text = trimParagraphs(updatedText, maxLines);
                break
            }
            case Color.Red:
            {
                let updatedText = consoleText.text.replace(/<\/body>/i, "<div style=\"color:" + colorArray[2] + ";\">>> " + text + "</div>");
                consoleText.text = trimParagraphs(updatedText, maxLines);
                break
            }
        }
    }

    function clearConsole()
    {
        consoleText.text = ">> Добро пожаловать!"
    }
}

