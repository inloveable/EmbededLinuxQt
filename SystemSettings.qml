import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id:mainWindow

//    height:parent.height

    signal returnSignal();

    MyMenu {
        z:1
        id: cl
        width: parent.width
        height: parent.height
        collapsedHeight: mainWindow.height * 0.1

        property int currentIndex



        MyMenuItem {
            color: "#D77EFC"
            collapsed: cl.collapsed
            index: 0
            title: qsTr("系统设置")
            icon: "qrc:/images/icons/settings.png"

            onItemSelected:function(str) {
               contented.change(str)
            }
        }

        MyMenuItem {
            color: "#FC6F61"
            collapsed: cl.collapsed
            index: 1
            title: qsTr("基本信息")
            icon: "qrc:/images/icons/refresh.png"
            onItemSelected:function(str) {
               contented.change(str)
            }
        }

       MyMenuItem {
            color: "#18A0FA"
            collapsed: cl.collapsed
            index: 2
            title: qsTr("状态检查")
            icon: "qrc:/images/icons/status.png"
            onItemSelected:function(str) {
              contented.change(str)
            }
        }

       MyMenuItem {
            color: "#94C325"
            collapsed: cl.collapsed
            index: 3
            title: qsTr("GPS定位")
            icon: "qrc:/images/icons/gps.png"
            onItemSelected:function(str) {
               contented.change(str)
            }
        }
       MyMenuItem {
            color: "#1b3999"
            collapsed: cl.collapsed
            index: 4
            title: qsTr("实时参数")
            icon: "qrc:/images/icons/realTime.png"
            onItemSelected:function(str) {
               contented.change(str)
            }
        }
       MyMenuItem {
            color: "#2b4f99"
            collapsed: cl.collapsed
            index: 5
            title: qsTr("返回")
            icon: "qrc:/images/icons/return.png"
            onItemSelected:function(str) {
                contented.active=false;
                mainWindow.returnSignal()
            }
        }
    }

    Rectangle{
        id:background
        width:parent.width
        height:parent.height- mainWindow.height * 0.1
        color:cl.collapsed?"gray":"transparent"
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        Behavior on color { ColorAnimation { duration: 200 } }



        Loader{
            id:contented
            //anchors.fill: parent

            property bool clicked: false

            width:parent.width
            height:parent.height

            function change(str){

                if(clicked===true){
                    contented.active=false;
                    clicked=!clicked;
                     return;
                }

                clicked=!clicked;

                console.log(str)
                if(str===qsTr("系统设置")){
                    contented.source="qrc:/SystemSettings/BasicSettings.qml";
                    contented.active=true;

                }else if(str===qsTr("基本信息")){
                    contented.source="qrc:/SystemSettings/BasicInfo.qml";
                    contented.active=true;

                }else if(str===qsTr("状态检查")){
                    contented.source="qrc:/SystemSettings/SelfCheck.qml";
                    contented.active=true;
                }else if(str===qsTr("GPS定位")){
                    contented.source="qrc:/SystemSettings/GPSPositioning.qml";
                    contented.active=true;
                }else if(str===qsTr("实时参数")){
                    contented.source="qrc:/SystemSettings/RealTimeStatus.qml";
                    contented.active=true;
                }
                else{
                    console.log("error ui")
                }

            }
        }
    }
}
