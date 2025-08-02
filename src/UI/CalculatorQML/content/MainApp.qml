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
        {text: "±",  collor1: "#b4b4b4", collor2: "#b4b4b4", action: function() { addSymbol("±") }}, {text: "0", collor1: "#e4e2e0", collor2: "#e4e2e0", action: function() { addSymbol("0")       }},
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

    function addSymbol(sym)
    {
        const ops = ["+", "−", "×", "÷"]
        let text = display.getText()

        if(text.endsWith("="))
        {
            if(ops.includes(sym))
            {
                text = text.slice(0, -1).trim()
            }
            else
            {
                display.clearText()
                text = ""
            }
        }

        const tokens = text.trim().split(/\s+/)
        const lastToken = tokens.length > 0 ? tokens[tokens.length - 1] : ""

        if(ops.includes(sym))
        {
            if(text === "" || ops.includes(lastToken))
            {
                return
            }
            display.addText(" " + sym + " ")
            return
        }

        if(sym === ".")
        {
            const currentNumber = ops.includes(lastToken) ? "" : lastToken
            if(currentNumber.includes("."))
            {
                return
            }
            if(currentNumber === "")
            {
                display.addText("0.")
            }
            else
            {
                display.addText(".")
            }
            return
        }

        display.addText(sym)
    }

    function removeLast()
    {
        display.removeLast()
    }

    function evaluateExpression()
    {
        try
        {
            addSymbol("=")

            let evalExpr = display.getText()
                .replace(/×/g, "*")
                .replace(/÷/g, "/")
                .replace(/−/g, "-")


            mainWindow.getExpression(evalExpr, display.delaySeconds)
            clearExpression()
        }
        catch(e)
        {
            console.log(e)
            // displayText.text = "Ошибка"
            // consoleTextArea.text += ">> Ошибка в выражении\n"
            // expression = ""
        }
    }

    function clearExpression()
    {
        display.clearText()
    }

    function clearAll()
    {
        display.clearText()
//        mainConsole.clearText()
    }
}

