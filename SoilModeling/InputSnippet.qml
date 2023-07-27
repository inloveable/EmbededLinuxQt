import QtQuick 2.15

Rectangle{
    id: rectangle2


    property alias tipText:text2.text
    property alias inputText:textInput.text

    signal inputFinished();

    color:"transparent"

    Rectangle{
        width: parent.width
        height: parent.height/2
        color:"transparent"
        border.width: 1
        border.color: "white"
        Text {
            id: text2
            anchors.fill: parent
            text: qsTr("模型名称")
            font.pixelSize: 20
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: true
        }
    }

    Rectangle{
        width: parent.width
        height: parent.height/2
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        color:"transparent"
        border.width: 1
        border.color: "white"
        TextInput {
            id: textInput

            anchors.fill: parent
            text: qsTr("ABC")

            font.pixelSize: 20
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: true

            onEditingFinished: {
                inputFinished()
            }

        }
    }




}
