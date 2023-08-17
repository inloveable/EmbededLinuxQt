import QtQuick 2.15
import QtQuick.Controls 2.0
import CppCore 1.0
Rectangle{
    color: "#1b39be"
    GroupBox {
        id: groupBox
     width:parent.width/3*2
     height:parent.height/3*2
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        title: qsTr("系统信息")

        label:Rectangle{
            width:groupBox.width/7
            height:groupBox.height/9
            color:"transparent"
            Text{
                color: "#ffffff"
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 13
                font.bold: true
              text:groupBox.title
            }
        }

        Column {
            id: column
            anchors.fill: parent
            Row{
                width:parent.width
                height:column.height/column.children.length
                Text{
                    text:"电流: "
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
                Text{
                    text:"--"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                    color: "#1b39be"
                }
            }
            Row{
                width:parent.width
                height:column.height/column.children.length
                Text{
                    text:"电压: "
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2

                    height:parent.height
                }
                Text{
                    text:"--"
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
                    text:"幅值: "
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
                Text{
                    text:"--"
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
                    text:"相角: "
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2
                   height:parent.height
                }
                Text{
                    text:"--"
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
                    text:"温度: "
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
                Text{
                    text:"--"
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
                    text:"电池: "
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
                Text{
                    text:Service.batteryVal+" %"
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
            }

        }
    }
    Rectangle{
        width:300
        height:50

        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom

        color:"transparent"
        Text{

            id:timeLabel
            anchors.fill: parent
            font.pointSize: 13
            color:"white"

        }

        Connections{
            target:Service

            function onSendTime(time,val){
                timeLabel.text=time;
            }
        }
    }
}
