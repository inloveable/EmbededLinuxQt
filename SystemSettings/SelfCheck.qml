import QtQuick 2.15
import QtQuick.Controls 2.0
import CppCore 1.0
import "../"
Rectangle {
    id: rectangle
    color:"#70A9FA"


    property bool selfChecking:false

    MyButton {
        id: button
        width: 200
        height: 60

        originColor:"#9FA9FA"
        hoverColor:"#6E7BFA"

        text: qsTr("自检")
        anchors.left: groupBox.left
        anchors.bottom: groupBox.top
        anchors.bottomMargin:15

        onClicked:{
            Service.selfCheck();
            selfCheckTimer.start()
        }

        fontSize:15

    }


    Connections{
        target:Service

        function onSelfCheckFinished(sucess){
            selfCheckTimer.stop()
            if(!sucess){
               Service.messageBox(qsTr("自检失败"));
               return;
            }
            progressBar.value=100

            Service.messageBox(qsTr("自检成功"));
            sensorState.text=sucess?qsTr("在线"):qsTr("断线")


        }
    }

    Timer{
        id:selfCheckTimer
        interval:50
        repeat:true
        onTriggered: {
            if(progressBar.value===99){
                selfCheckTimer.stop()
            }

            progressBar.value+=0.5
        }
    }

    ProgressBar {
        id: progressBar
        width: groupBox.width-button.width
        height: 30
        anchors.left: button.right
        anchors.top: button.top
        anchors.topMargin: button.height/2-height/2
        anchors.leftMargin: 14
        value: 0

        from:0
        to:100


    }

    GroupBox {
        id: groupBox
        width: parent.width/3*2
        height: parent.height/5*4

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin:10
        label:Rectangle{
            width:groupBox.width/7
            height:groupBox.height/9

            x: 50


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
        title: qsTr("自检结果")

        Column {
            id: column
            anchors.fill: parent


            property int fontSize:13

            Row{
                width:parent.width
                height:column.height/column.children.length
                Text{
                    text:qsTr("温度传感器:")
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: column.fontSize
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
                Text{
                    id:sensorState
                    text:qsTr("土壤无核密度仪")
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                     font.pointSize: column.fontSize
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
            }
            Row{
                width:parent.width
                height:column.height/column.children.length
                Text{
                    text:qsTr("产品型号:")
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                     font.pointSize: column.fontSize
                    font.bold: true
                    width:parent.width/2

                    height:parent.height
                }
                Text{
                    text:"EDG-25"
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: column.fontSize
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
            }
            Row{
                width:parent.width
                height:column.height/column.children.length
                Text{
                    text:qsTr("版本编号:")
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                          font.pointSize: column.fontSize
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
                Text{
                    text:"土壤无核密度仪"
                    verticalAlignment: Text.AlignVCenter
                             font.pointSize: column.fontSize
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
            }

            Row{
                width:parent.width
                height:column.height/column.children.length
                Text{
                    text:qsTr("生产厂家:")
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                              font.pointSize: column.fontSize
                    font.bold: true
                    width:parent.width/2
                   height:parent.height
                }
                Text{
                    text:"土壤无核密度仪"
                    verticalAlignment: Text.AlignVCenter
                           font.pointSize: column.fontSize
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
            }
            Row{
                width:parent.width
                height:column.height/column.children.length
                Text{
                    text:qsTr("咨询电话:")
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                                        font.pointSize: column.fontSize
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
                Text{
                    text:"土壤无核密度仪"
                    verticalAlignment: Text.AlignVCenter
                                        font.pointSize: column.fontSize
                    font.bold: true
                    width:parent.width/2
                    height:parent.height
                }
            }

        }
    }
}
