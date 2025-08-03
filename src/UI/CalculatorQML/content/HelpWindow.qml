import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window
{
    id: helpWindow

    minimumWidth:  950
    minimumHeight: 300
    maximumWidth:  950
    maximumHeight: 300

    visible: false
    title:   "Справка"
    color:   "#ffffff"

    Rectangle
    {
        anchors.fill: parent

        Text
        {
            text: "Программа представляет собой аналог простого калькулятора Windows.\n\n" +
                  "Вычисления выполняются слева направо, без приоритета операций. Поддерживаемые операции: \"+\", \"-\", \"/\", \"*\".\n\n" +
                  "Исключения и ошибки при работе программы выводятся на Экран консоли.\n" +
                  "Калькулятор поддерживает два режима:\n" +
                  " - Встроенный режим — вычисления производятся с помощью внутреннего метода класса.\n" +
                  " - Внешний режим — вычисления делегируются внешней динамической библиотеке с функцией.\n" +
                  "Смена режима осуществляется по кнопке FUNC/DLL.\n\n" +
                  "Изменение задержки вычисления осуществляется кнопками ▲/▼, либо прокруткой колесика мыши на области элемента.\n\n" +
                  "Ввод выражения осуществляется нажатием на кнопки, либо же клавишами 0-9 или NumPad"

            horizontalAlignment: Text.AlignLeft
            verticalAlignment:   Text.AlignTop
            wrapMode:            Text.WordWrap

            font.pixelSize: 16
            padding:        10
        }
    }

}
