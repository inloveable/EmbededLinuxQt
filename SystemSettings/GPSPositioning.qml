import QtQuick 2.15
import QtQuick.Controls 2.0
import CppCore 1.0
Rectangle {
    id: rectangle

    color:"#8DC376"
    GroupBox {
        id: groupBox
       width:parent.width/3*2
        height:parent.height/3
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        title: qsTr("系统信息")

        /*
        contentItem:Rectangle{
            border.width: 2
            border.color: "black"
            color:"transparent"
        }*/

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
                    text:"GPS:"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
                Text{
                    text:Service.getGps();
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
            }

        }


    }

    Switch {
        id: switch1

        width:groupBox.width/2
        height:100
        text: qsTr("GPS")
        anchors.left: groupBox.left
        anchors.bottom: groupBox.top
        anchors.bottomMargin: 25
        anchors.leftMargin: 0
    }

    Rectangle{
        width:300
        height:50

        anchors.left: parent.left
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
