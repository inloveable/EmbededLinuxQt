import QtQuick 2.15
import QtQuick.Controls 2.7
import QtQuick.VirtualKeyboard 2.15
import QtQuick.VirtualKeyboard.Styles 2.15
import QtQuick.VirtualKeyboard.Settings 2.15

import "../"
import CppCore 1.0
Item {
    SwipeView{
        id:view

        anchors.fill: parent
        currentIndex: 1

        Rectangle{
            id:root

            color:"orange"
            gradient: Gradient
            {
                GradientStop { position: 0.0; color: "#f46b45" }

                GradientStop { position: 1.0; color: "#eea849" }
                orientation: Qt.Horizontal

            }
            Rectangle {
                id: rectangle

                width: parent.width/5*4
                height: parent.height/5*3
                color: "transparent"
                radius: 5
                border.color: "#ffffff"
                anchors.verticalCenter: parent.verticalCenter

                anchors.horizontalCenter: parent.horizontalCenter


                Row {
                    id: row
                    y: 26
                    width: parent.width
                    height: parent.height-rectangle1.height/2-5
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0

                    InputSnippet{
                        id: rectangle2
                        width:parent.width/3
                        height:parent.height

                        tipText:"新建模型"
                        inputText: "ABC"
                    }
                    InputSnippet{
                        id: rectangle3
                        width:parent.width/3
                        height:parent.height

                        tipText:"最大干密度(g/cm^3)"
                        inputText: "ABC"
                    }
                    InputSnippet{
                        id: rectangle4
                        width:parent.width/3
                        height:parent.height

                        tipText:"最佳含水率(%)"
                        inputText: "ABC"
                    }


                }

            }

            Rectangle {
                id: rectangle1


                width: rectangle.width/2
                height: 100
                color: root.color
                anchors.horizontalCenter: rectangle.horizontalCenter
                radius: 5
                border.color: "#ffffff"
                anchors.top: rectangle.top
                anchors.topMargin:-rectangle1.height/2

                Text {
                    id: text1
                    text: qsTr("模型设置")
                    anchors.fill: parent
                    font.pixelSize: 25
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.bold: true
                }


            }
        }

        Rectangle{
            id:page2


            FocusScope{
                anchors.fill: parent

                focus:true

                Rectangle {
                    id: rectangle5
                    y: 29

                    width: parent.width*0.9
                    height: parent.height*0.7
                    color: "transparent"


                    border.width: 1
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 50
                    anchors.horizontalCenter: parent.horizontalCenter
                    border.color: "white"
                    radius:5




                    ListView {
                        id: listView

                        anchors.fill: parent
                        anchors.topMargin: 0
                        header:ViewHeader{
                               width:parent.width
                               height:50

                               widthRate:[1,3,3,2,2,2,2,2]

                               itemInfo:ListModel{

                                   ListElement{
                                       label:"测点"
                                   }
                                   ListElement{
                                       label:"湿密度"
                                   }
                                   ListElement{
                                       label:"含水率"
                                   }
                                   ListElement{
                                       label:"幅值"
                                   }
                                   ListElement{
                                       label:"相角"
                                   }
                                   ListElement{
                                       label:"温度"
                                   }
                                   ListElement{
                                       label:"GPS"
                                   }
                                   ListElement{
                                       label:""
                                   }
                               }
                        }

                        model: Service.getTestPointModel()

                        delegate: AddPointDelegate{
                            id:addPointDele
                                   width:parent.width
                                   height:50

                                   itemIndex: model.index
                                   density: model.density
                                   waterRate: model.waterRate
                                   amplitude:model.ampitude//error spelling
                                   anglePhase: model.phaseAngle
                                   temperature: model.temperature
                                   gps:model.gps
                                   isPairing:model.isPairing

                        }
                    }
                }

                property color buttonColor:"#6441a5"

                MyButton {
                    id: button
                    width:100
                    height:50
                    text: qsTr("增加项")
                    anchors.left: rectangle5.left
                    anchors.bottom: rectangle5.top
                    anchors.bottomMargin: 20
                    anchors.leftMargin: 0

                    originColor: "#6441a5"
                    hoverColor: Qt.lighter(originColor,1.2)
                }

                MyButton {
                    id: button1
                    text: qsTr("删除项")
                    width:100
                    height:50
                    anchors.left: button.right
                    anchors.top: button.top
                    anchors.topMargin: 0
                    anchors.leftMargin: 20

                    originColor: "#6441a5"
                    hoverColor: Qt.lighter(originColor,1.2)
                }
                MyButton {
                    id: button3
                    text: qsTr("编辑完成")
                    width:150
                    height:50
                    anchors.left: button1.right
                    anchors.top: button1.top
                    anchors.topMargin: 0
                    anchors.leftMargin: 20

                    originColor: "#6441a5"
                    hoverColor: Qt.lighter(originColor,1.2)

                    onClicked:{
                        button3.focus=true
                    }
                }
            }

            color:"yellow"

            gradient: Gradient {
                        GradientStop { position: 0.0; color: "#8e0e00" }

                        GradientStop { position: 1.0; color: "#1f1c18" }
                        //start: Qt.point(0, 0)
                        //end: Qt.point(width, height)
                        orientation: Qt.Horizontal

                    }




        }



    }

    PageIndicator{
        id:indicator


        height:50

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom

        count:view.count
        currentIndex:view.currentIndex



        delegate: Rectangle {
            implicitWidth: 16
            implicitHeight: 16

            radius: width / 2
            color: "#21be2b"

            opacity: index === indicator.currentIndex ? 0.95 : pressed ? 0.7 : 0.45

            Behavior on opacity {
                OpacityAnimator {
                    duration: 100
                }
            }
        }


    }
}

/*##^##
Designer {
    D{i:0}D{i:2;invisible:true;locked:true}D{i:11}
}
##^##*/
