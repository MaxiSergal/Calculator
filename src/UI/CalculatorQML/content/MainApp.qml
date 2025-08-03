    import QtQuick 6.2
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

Window
{
    id: mainWindowQml

    readonly property int marginH:            5
    readonly property int marginV:            5
    readonly property int buttonGridSpacingH: 2
    readonly property int buttonGridSpacingV: 2
    readonly property int gridColumns:        4
    readonly property int minButtonWidth:     90
    readonly property int minButtonHeight:    65

    property int lastX: x
    property int lastY: y
    property int lastW: width
    property int lastH: height

    minimumWidth: minButtonWidth * gridColumns + marginH * 2 + (gridColumns - 1) * buttonGridSpacingH
    minimumHeight: 620
    width: minimumWidth
    height: minimumHeight

    visible: true
    title: "Калькулятор"

    property var buttonData:
    [
        {text: "CE", collor1: "#c4bcb8", collor2: "#b2aca4", action: function() { clearAll()     }}, {text: "C", collor1: "#c4bcb8", collor2: "#b2aca4", action: function() { clearExpression()    }},
        {text: "⌫",  collor1: "#c4bcb8", collor2: "#b2aca4", action: function() { removeLast()   }}, {text: "÷", collor1: "#c4bcb8", collor2: "#b2aca4", action: function() { addSymbol("÷")       }},
        {text: "7",  collor1: "#e4e2e0", collor2: "#e4e2e0", action: function() { addSymbol("7") }}, {text: "8", collor1: "#e4e2e0", collor2: "#e4e2e0", action: function() { addSymbol("8")       }},
        {text: "9",  collor1: "#e4e2e0", collor2: "#e4e2e0", action: function() { addSymbol("9") }}, {text: "×", collor1: "#c4bcb8", collor2: "#b2aca4", action: function() { addSymbol("×")       }},
        {text: "4",  collor1: "#e4e2e0", collor2: "#e4e2e0", action: function() { addSymbol("4") }}, {text: "5", collor1: "#e4e2e0", collor2: "#e4e2e0", action: function() { addSymbol("5")       }},
        {text: "6",  collor1: "#e4e2e0", collor2: "#e4e2e0", action: function() { addSymbol("6") }}, {text: "−", collor1: "#b4b4b4", collor2: "#b4b4b4", action: function() { addSymbol("−")       }},
        {text: "1",  collor1: "#e4e2e0", collor2: "#e4e2e0", action: function() { addSymbol("1") }}, {text: "2", collor1: "#e4e2e0", collor2: "#e4e2e0", action: function() { addSymbol("2")       }},
        {text: "3",  collor1: "#e4e2e0", collor2: "#e4e2e0", action: function() { addSymbol("3") }}, {text: "+", collor1: "#b4b4b4", collor2: "#b4b4b4", action: function() { addSymbol("+")       }},
        {text: "±",  collor1: "#b4b4b4", collor2: "#b4b4b4", action: function() { reverseSign()  }}, {text: "0", collor1: "#e4e2e0", collor2: "#e4e2e0", action: function() { addSymbol("0")       }},
        {text: ".",  collor1: "#b4b4b4", collor2: "#b4b4b4", action: function() { addSymbol(".") }}, {text: "=", collor1: "#b4b4b4", collor2: "#b4b4b4", action: function() { evaluateExpression() }}
    ]

    Rectangle
    {
        id: screen

        anchors.margins: marginH
        anchors.fill: parent

        ColumnLayout
        {
            spacing: 1

            anchors.fill: parent

            Layout.fillWidth:  true
            Layout.fillHeight: true

            Console
            {
                id: mainConsole
            }

            Display
            {
                id: display

                onClicked: mainWindow.reveiveProcessMode(
                               {
                                   mode: display.currentMode
                               })
            }

            GridLayout
            {
                id: buttonGrid

                Layout.fillWidth:  true
                Layout.fillHeight: true

                columns:       4
                columnSpacing: mainWindowQml.buttonGridSpacingH
                rowSpacing:    mainWindowQml.buttonGridSpacingV

                Repeater
                {
                    model: 20

                    SquareButton
                    {
                        minButtonWidth:  mainWindowQml.minButtonWidth
                        minButtonHeight: mainWindowQml.minButtonHeight

                        Layout.fillWidth:  true
                        Layout.fillHeight: true

                        buttonText:  buttonData[index].text
                        gradient:    Gradient {GradientStop {position: 0; color: buttonData[index].collor1} GradientStop{position: 1; color: buttonData[index].collor2}}

                        onClicked:
                        {
                            buttonData[index].action()
                        }
                    }
                }
            }
        }
    }

    FocusScope
    {
        id: root
        focus: true
        Keys.onPressed: (event) =>
            {
                switch (event.key)
                {
                    case Qt.Key_0:
                        addSymbol(event.text)
                        break
                    case Qt.Key_1:
                        addSymbol(event.text)
                        break
                    case Qt.Key_2:
                        addSymbol(event.text)
                        break
                    case Qt.Key_3:
                        addSymbol(event.text)
                        break
                    case Qt.Key_4:
                        addSymbol(event.text)
                        break
                    case Qt.Key_5:
                        addSymbol(event.text)
                        break
                    case Qt.Key_6:
                        addSymbol(event.text)
                        break
                    case Qt.Key_7:
                        addSymbol(event.text)
                        break
                    case Qt.Key_8:
                        addSymbol(event.text)
                        break
                    case Qt.Key_9:
                        addSymbol(event.text)
                        break
                    case Qt.Key_Numpad0:
                        addSymbol(event.text)
                        break
                    case Qt.Key_Numpad1:
                        addSymbol(event.text)
                        break
                    case Qt.Key_Numpad2:
                        addSymbol(event.text)
                        break
                    case Qt.Key_Numpad3:
                        addSymbol(event.text)
                        break
                    case Qt.Key_Numpad4:
                        addSymbol(event.text)
                        break
                    case Qt.Key_Numpad5:
                        addSymbol(event.text)
                        break
                    case Qt.Key_Numpad6:
                        addSymbol(event.text)
                        break
                    case Qt.Key_Numpad7:
                        addSymbol(event.text)
                        break
                    case Qt.Key_Numpad8:
                        addSymbol(event.text)
                        break
                    case Qt.Key_Numpad9:
                        addSymbol(event.text)
                        break
                    case Qt.Key_Plus:
                        addSymbol("+")
                        break
                    case Qt.Key_Minus:
                        addSymbol("−")
                         break
                    case Qt.Key_Asterisk:
                        addSymbol("×")
                        break
                    case Qt.Key_Slash:
                        addSymbol("÷")
                        break
                    case Qt.Key_Period:
                        addSymbol(".")
                        break
                    case Qt.Key_Enter:
                        evaluateExpression()
                        break
                    case Qt.Key_Return:
                        evaluateExpression()
                        break
                    case Qt.Key_Backspace:
                        removeLast()
                        break
                }
                event.accepted = true
        }

        onActiveFocusChanged:
        {
            if(!activeFocus)
            {
                root.forceActiveFocus()
            }
        }
    }

    onXChanged:      handleGeometryChange()
    onYChanged:      handleGeometryChange()
    onWidthChanged:  handleGeometryChange()
    onHeightChanged: handleGeometryChange()

    Connections
    {
        target: mainWindow
        function onResponseChanged(response)
        {
            mainConsole.appendToEntry(response.id, response.result, response.error_code === 1 ? Console.Colors.Green : Console.Colors.Red)
        }
        function onRequestQueueSizeChanged(size)
        {
            display.requestsCount = size;
        }
        function onResponseQueueSizeChanged(size)
        {
            display.responsesCount = size;
        }
        function onGeometryChanged(geometry)
        {
            mainWindowQml.setGeometry(geometry.x, geometry.y, geometry.width, geometry.height)
        }
        function onInfoMessageChanged(message)
        {
            mainConsole.addEntry(message.message, message.error_code === 1 ? Console.Colors.Green : Console.Colors.Red)
        }
    }

    function handleGeometryChange()
    {
        lastX = x
        lastY = y
        lastW = width
        lastH = height

        mainWindow.receiveGeometry(
        {
            x: lastX,
            y: lastY,
            width: lastW,
            height: lastH,
        })
    }

    function addSymbol(sym)
    {
        const ops = ["+", "−", "×", "÷", "="]
        let text = display.getText()

        const tokens = text.trim().split(/\s+/)
        const lastToken = tokens.length > 0 ? tokens[tokens.length - 1] : ""

        if(sym === "−")
        {
            if(text === "" || ops.includes(lastToken))
            {
                display.addText("-")
                return true
            }

            if(lastToken === "-")
                return false

            display.addText("−")
            return true
        }

        if(ops.includes(sym))
        {
            if(text === "" || ops.includes(lastToken))
            {
                return false
            }
            display.addText(" " + sym + " ")
            return true
        }

        if(sym === ".")
        {
            const currentNumber = ops.includes(lastToken) ? "" : lastToken
            if(currentNumber.includes("."))
            {
                return false
            }
            if(currentNumber === "")
            {
                display.addText("0.")
            }
            else
            {
                display.addText(".")
            }
            return true
        }

        display.addText(sym)

        return true
    }

    function removeLast()
    {
        display.removeLast()
    }

    function evaluateExpression()
    {
        if(display.getText() === "")
            return
        if(!addSymbol("="))
            return

        let evalExpr = display.getText()
            .replace(/×/g, "*")
            .replace(/÷/g, "/")
            .replace(/−/g, "-")


        let id = mainConsole.addEntry(display.getText(), Console.Colors.Blue)
        mainWindow.receiveRequest(
        {
            expression: evalExpr,
            delay: display.delaySeconds,
            error_code: 0,
            id: id
        })
        clearExpression()
    }

    function reverseSign()
    {
        let text  = display.getText()
        const ops = ["+", "−", "×", "÷", "="]

        if(text === "")
        {
            addSymbol("-")
            return
        }

        let tokens    = text.split(/\s+/)
        let lastToken = tokens[tokens.length - 1]

        if(lastToken === "-")
        {
            display.removeLast()
            return
        }

        if(lastToken === "")
        {
            addSymbol("-")
            return
        }

        function isNumber(str)
        {
            return /^-?\d*\.?\d+$/.test(str)
        }

        if(isNumber(lastToken))
        {
            let newNumber = null
            if(lastToken.startsWith("-"))
                newNumber = lastToken.substring(1)
            else
                newNumber = "-" + lastToken

            for(let i = 0; i < lastToken.length; i++)
                display.removeLast()

            for(let ch of newNumber)
                display.addText(ch)

            return
        }

        if(ops.includes(lastToken))
        {
            let prevToken = tokens.length >= 2 ? tokens[tokens.length - 2] : ""

            if(prevToken === "-")
            {
                display.removeLast()
                display.removeLast()
                return
            }
            else
            {
                addSymbol("-")
                return
            }
        }
    }

    function clearExpression()
    {
        display.clearText()
    }

    function clearAll()
    {
        display.clearText()
        mainConsole.clearConsole()
    }
}

