import QtQuick 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls 2.15

Rectangle
{
    id: mainConsole

    property var entries: []

    property var colorArray: ["blue", "green", "red"]
    property int maxLines:   10
    property int nextId: 1

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
            const id = mainConsole.addEntry("Добро пожаловать", 0);
            Qt.callLater(() =>
            {
                mainConsole.appendToEntry(id, "!", 1);
            })
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
        for (let i = 0; i < entries.length; ++i)
        {
            if (entries[i].id === id)
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
        // Отбираем только последние maxLines HTML-блоков
        const limited = entries.slice(Math.max(0, entries.length - maxLines));
        let body = limited.map(entry => entry.html).join("");
        consoleText.text = "<body>" + body + "</body>";
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

