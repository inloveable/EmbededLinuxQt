import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.0

import CppCore 1.0
import "../"
Rectangle {
    id: rectangle1
    color:"#6a0ebf"

    width:1080
    height:800
    Rectangle {

        color:"transparent"
        radius: 10

        border.width: 1
        border.color: "white"

        id: rectangle
        width: parent.width*2/3
        height: parent.height*2/3
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle {
            id: viewArea

            width: rectangle.width
            height: rectangle.height-groupTitle.height/2-5
            color: "transparent"
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.leftMargin: 0

            AdvancedTableView{
                anchors.fill: parent
            }

        }

    }

    Rectangle {
        id: groupTitle
        y: 56
        width: rectangle.width/2
        height: 50
        color: "#6a0ebf"
        border.width: 1
        anchors.bottom: rectangle.top
        anchors.bottomMargin: -height/2
        border.color: "white"

        anchors.horizontalCenter: rectangle.horizontalCenter
        radius: 10
        Text {
            id: text1
            color: "#ffffff"
            text: qsTr("模型清单")
            anchors.fill: parent
            font.pixelSize: 20
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: true
        }
    }




    RoundButton {
        id: roundButton

        width:100
        height:100

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.rightMargin: 50

        background:Rectangle{
            width:roundButton.icon.width
            height:roundButton.icon.width
            radius:parent.width
            color:Qt.lighter(rectangle1.color,1.5)
            opacity:roundButton.pressed?0.5:0
        }

        icon.source:"qrc:/images/icons/tip.png"
        icon.width: width
        icon.height: height

        onClicked: {
               tip.active=true
        }

    }

    Loader{
        id:tip
        active:false
        sourceComponent: Window{
            id:message
            width:400
            height:300
            Rectangle{
                width:200
                height:200
                anchors.centerIn: parent
                Text{

                    text:"右划删除模型"

                    anchors.fill: parent
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                }
            }



            Component.onCompleted: {
                message.show()
                focus=true
                var timer = Qt.createQmlObject('import QtQuick 2.15; Timer {}', message);
                timer.interval = 1500;
                timer.triggered.connect(function() {
                    tip.active=false
                });
                timer.start();
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
