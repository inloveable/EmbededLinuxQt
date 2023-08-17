import QtQuick 2.15
import QtQuick.Controls 2.15

import "../"
import CppCore 1.0
Rectangle {
    id: rectangle

    width:600
    height:400
    color: "#6a0ebf"

    border.width: 2
    border.color: "#7826a0"


    Rectangle{
        id: rectangle1
        width: parent.width/2
        height: parent.height/2
        color: "#8a44ca"
        radius: 10
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        Switch {
            id: switch1

            Component.onCompleted: {
                console.log("current language:"+Service.currentLanguage)
                checked=(Service.currentLanguage==="ZH")
            }
  
            width: 200
            height: 50
            text: checked?qsTr("中文"):qsTr("英文")

            anchors.left: text1.right
            anchors.top: text1.bottom

            anchors.leftMargin: 23

            onCheckedChanged: {
                if(!checked){
                    Service.requestRetranslate("EN")
                }else{
                    Service.requestRetranslate("ZH")
                }
            }
        }

        Text {
            id: text1

            width: 200
            height: 50
            text: qsTr("系统语言")
            anchors.left: parent.left
            anchors.top: parent.top
            font.pixelSize: 20
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            anchors.leftMargin: 10

            font.bold: true
        }

        Text {
            id: text2

            width: 150
            height: 40
            text: qsTr("系统时间")
            anchors.left: text1.left
            anchors.top: text1.bottom
            font.pixelSize: 15
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            anchors.topMargin: 70
            font.bold: true
        }

        Text {
            id: text3
            width: 214
            height: 39
            text: Service.getTime()
            anchors.left: text2.right
            anchors.top: text2.top
            font.pixelSize: 17
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            anchors.leftMargin: 10
            font.bold: true
        }
    }

    Connections{
        target:Service

        function onSendTime(time,sucess){
            console.log(time)
            if(sucess===true){
               text3.text=time;
            }else{
               Service.messageBox("请求时间失败:"+time);
            }


            text3.text=time;
        }
    }

    MyButton {
        id: button
        width: 150
        height: 50
        opacity: 0.9
        text: qsTr("更新")
        anchors.left: rectangle1.right
        anchors.top: rectangle1.bottom
        anchors.leftMargin: -52
        anchors.topMargin: 26

        hoverColor: "#964DBA"
        originColor:"#714DBA"

        onClicked:{
           Service.getTime();
        }
    }
}
