import QtQuick 2.15
import QtQuick.Controls 2.0
import "../"
Rectangle {
    id: rectangle
    color:"#70A9FA"

    MyButton {
        id: button
        width: 100
        height: 30

        originColor:"#9FA9FA"
        hoverColor:"#6E7BFA"

        text: qsTr("自检")
        anchors.left: groupBox.left
        anchors.top: parent.top

        anchors.topMargin: 83
    }

    ProgressBar {
        id: progressBar
        width: groupBox.width-button.width
        height: 30
        anchors.left: button.right
        anchors.top: button.top
        anchors.topMargin: 0
        anchors.leftMargin: 14
        value: 0.5
    }

    GroupBox {
        id: groupBox
        width: parent.width/3*2
        height: parent.height/3*2

        anchors.centerIn: parent

        title: qsTr("自检结果")

        Column {
            id: column
            anchors.fill: parent
            spacing:10
            Row{
                width:parent.width
                height:column.height/column.children.length
                Text{
                    text:"温度传感器:"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
                Text{
                    text:"土壤无核密度仪"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
            }
            Row{
                width:parent.width
                height:column.height/column.children.length
                Text{
                    text:"产品型号:"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2

                    height:parent.height
                }
                Text{
                    text:"EDG-25"
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
            }
            Row{
                width:parent.width
                height:column.height/column.children.length
                Text{
                    text:"版本编号:"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
                Text{
                    text:"土壤无核密度仪"
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
            }

            Row{
                width:parent.width
                height:column.height/column.children.length
                Text{
                    text:"生产厂家:"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2
                   height:parent.height
                }
                Text{
                    text:"土壤无核密度仪"
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
            }
            Row{
                width:parent.width
                height:column.height/column.children.length
                Text{
                    text:"咨询电话:"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
                Text{
                    text:"土壤无核密度仪"
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
            }

        }
    }
}
