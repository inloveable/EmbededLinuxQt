import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0

import CppCore 1.0
Rectangle {
    id: rectangle

    color:"#3F51B5"

    property string transmitType:"USB"


    Rectangle{
        width:50
        height:50

        color:"transparent"

        Image{
            id:image
            source:"qrc:/images/icons/Back.png"
            anchors.fill: parent
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
                returnSignal()
            }
        }
    }


    signal returnSignal();
    Rectangle{
        id:stack
        color:"#303F9F"
        width: parent.width
        height: parent.height/4*3
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 0
        anchors.bottomMargin: 0

        Loader{
            id:loader
            anchors.fill: parent
        }
    }

    Component.onCompleted: {
        loader.sourceComponent=usb
    }

    ToolBar{
        id: toolBar

        width: parent.width-10

        height:75
        anchors.bottom: stack.top
        anchors.horizontalCenterOffset: 0
        anchors.bottomMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter

        background: Rectangle{
            // opacity:0.5
            color:"#757575"

            anchors.fill: parent

            border.width: 2
            border.color: "#BDBDBD"
            radius:5
        }

        RowLayout {
            id: rowLayout
            anchors.fill: parent
            MyButton {
                id: toolButton
                text: qsTr("USB传输")
                fontColor:"#ffffff"
                Layout.bottomMargin: 15
                Layout.topMargin: 15
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true
                height:parent.height

                hoverColor: "#448AE0"
                originColor: "#448AFF"

                onClicked:{
                    transmitType="USB"
                    loader.sourceComponent=usb
                }

            }

            MyButton {
                id: toolButton1
                text: qsTr("WIFI传输")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                height:parent.height
                Layout.bottomMargin: 15
                Layout.topMargin: 15
                fontColor:"#ffffff"
                hoverColor: "#448AE0"
                originColor: "#448AFF"
                Layout.fillHeight: true
                onClicked:{
                    transmitType="WIFI"
                    loader.sourceComponent=wifi
                }

            }
        }
    }

    Component{
        id:usb

        Rectangle{
            anchors.fill: parent

            color:"transparent"

            Rectangle{
                id:infoArea
                width:parent.width
                height:parent.height-groupBox.height
                anchors.bottom: parent.bottom

                anchors.left: groupBox.left
                color:"#C5CAE9"
                  Text{
                      id:demonstrate
                      anchors.top: parent.top
                      anchors.left:parent.left
                      color:"#212121"
                      width:parent.width-transferButton.width
                      height:infoArea.height
                      font.pixelSize: 30
                      font.bold: true
                      text:"结果：已完成文件传输\n      未成功导出文件,请检查WIFI连接是否异常\n      未成功导出文件,请检查文件是否存在";
                  }

                MyButton{
                    id:transferButton
                    width:100
                    height:50
                    fontColor:"#ffffff"
                    hoverColor: "#448AE0"
                    originColor: "#448AFF"
                    text:qsTr("传输")
                    anchors.right: parent.right
                    anchors.top: parent.top

                    onClicked: {
                        Service.exportData(modelCombo.currentValue,modelCombo.currentFileType);
                    }
                }


            }

            GroupBox {
                id: groupBox
                width:parent.width
                height:parent.height*0.75
                anchors.top:parent.top
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
                            text:"U盘状态:"
                            horizontalAlignment: Text.AlignRight
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 15
                            font.bold: true
                            width:parent.width/2
                            height:parent.height
                            color:"#ffffff"
                        }
                        Text{
                            text:Service.usbOnline?qsTr("已连接"):qsTr("未连接")
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 15
                            font.bold: true
                            color:"#ffffff"
                            width:parent.width/2
                            height:parent.height
                        }
                    }
                    Row{
                        width:parent.width
                        height:column.height/column.children.length
                        Text{
                            text:"文件选择:"
                            horizontalAlignment: Text.AlignRight
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 15
                            font.bold: true
                            width:parent.width/2
                            height:parent.height
                            color:"#ffffff"
                        }
                        Rectangle{
                            width:parent.width/2
                            height:parent.height
                            color:"transparent"
                            ComboBox{
                                id:modelCombo
                                background:Rectangle{
                                    color:"transparent"
                                    anchors.fill: parent
                                }

                                property int currentFileIndex:-1
                                property int currentFileType:0

                                model:Service.getExportInfos()
                                font.pixelSize: 20
                                font.bold:true
                                contentItem:Text {
                                       //anchors.fill: parent

                                       text: parent.displayText
                                       font: parent.font
                                       color:"white"
                                       verticalAlignment: Text.AlignVCenter
                                       horizontalAlignment: Text.AlignLeft
                                       elide: Text.ElideRight

                                  }



                                onCurrentValueChanged: {
                                    //comboBoxIndexChanged(root.index,currentValue)
                                    ///currentFileType=model.data(modelCombo.currentIndex,type)
                                    //currentFileIndex=model.data(modelCombo.currentIndex,index)

                                    if(currentText.slice(-2)===qsTr("工程")){
                                        currentFileType=1;
                                    }else if(currentText.slice(-1)===qsTr("模型")){
                                        currentFileType=0;
                                    }
                                }

                                textRole: "label"
                                valueRole:"index"
                                anchors.fill:parent
                            }
                        }

                    }
                }

            }
        }
    }

    Component{
        id:wifi

        Rectangle{
            anchors.fill: parent

            color:"transparent"

            Rectangle{
                id:infoArea
                width:parent.width
                height:parent.height-groupBox.height
                anchors.bottom: parent.bottom

                anchors.left: groupBox.left
                color:"#C5CAE9"
                  Text{
                      id:demonstrate
                      anchors.top: parent.top
                      anchors.left:parent.left
                      color:"#212121"
                      width:parent.width-transferButton.width
                      height:infoArea.height
                      font.pixelSize: 30
                      font.bold: true
                      text:"结果：已完成文件传输\n      未成功导出文件,请检查WIFI连接是否异常\n      未成功导出文件,请检查文件是否存在";
                  }

                MyButton{
                    id:transferButton
                    width:100
                    height:50
                    fontColor:"#ffffff"
                    hoverColor: "#448AE0"
                    originColor: "#448AFF"
                    text:qsTr("传输")
                    anchors.right: parent.right
                    anchors.top: parent.top

                    onClicked:{
                        if(transmitType==="USB"){

                        }


                    }
                }


            }

            GroupBox {
                id: groupBox
                width:parent.width
                height:parent.height*0.75
                anchors.top:parent.top
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
                            text:"4G状态:"
                            horizontalAlignment: Text.AlignRight
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 15
                            font.bold: true
                            width:parent.width/2
                            height:parent.height
                            color:"#ffffff"
                        }
                        Text{
                            text:"未连接"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 15
                            font.bold: true
                            color:"#ffffff"
                            width:parent.width/2
                            height:parent.height
                        }
                    }
                    Row{
                        width:parent.width
                        height:column.height/column.children.length
                        Text{
                            text:"WIFI连接:"
                            horizontalAlignment: Text.AlignRight
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 15
                            font.bold: true
                            width:parent.width/2
                            height:parent.height
                            color:"#ffffff"
                        }
                        Text{
                            text:"土壤无核密度仪"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 15
                            font.bold: true
                            color:"#ffffff"
                            width:parent.width/2
                            height:parent.height
                        }
                    }

                    Row{
                        width:parent.width
                        height:column.height/column.children.length
                        Text{
                            text:"WIFI状态:"
                            horizontalAlignment: Text.AlignRight
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 15
                            font.bold: true
                            width:parent.width/2
                            height:parent.height
                            color:"#ffffff"
                        }
                        Text{
                            text:"土壤无核密度仪"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 15
                            font.bold: true
                            color:"#ffffff"
                            width:parent.width/2
                            height:parent.height
                        }
                    }


                    Row{
                        width:parent.width
                        height:column.height/column.children.length
                        Text{
                            text:"文件选择:"
                            horizontalAlignment: Text.AlignRight
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 15
                            font.bold: true
                            width:parent.width/2
                            height:parent.height
                            color:"#ffffff"
                        }
                        Text{
                            text:"土壤无核密度仪"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 15
                            font.bold: true
                            color:"#ffffff"
                            width:parent.width/2
                            height:parent.height
                        }
                    }
                }
            }

        }
    }


}
