import QtQuick 2.15
import QtQuick.Controls 2.0
Item {
    id:mainWindow

//    height:parent.height

    signal returnSignal();


    BusyIndicator{
        id:indicator
        width:200
        height:200
        anchors.centerIn: parent
        //running:(contented.state===Loader.Loading)?true:false
    }

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
            title: "模型管理"
            icon: "qrc:/images/icons/settings.png"

            onItemSelected:function(str) {
               contented.change(str)
            }
        }

        MyMenuItem {
            color: "#FC6F61"
            collapsed: cl.collapsed
            index: 1
            title: "新建模型"
            icon: "qrc:/images/icons/refresh.png"
            onItemSelected:function(str) {
               contented.change(str)
            }
        }


       MyMenuItem {
            color: "#2b4f99"
            collapsed: cl.collapsed
            index: 5
            title: "返回"
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


            onLoaded: {
                indicator.running=false
            }

            function change(str){

                if(clicked===true){
                    contented.active=false;
                    clicked=!clicked;
                     return;
                }

                clicked=!clicked;

                console.log(str)
                if(str===qsTr("模型管理")){
                    contented.source="qrc:/SoilModeling/ModelList.qml";
                    contented.active=true;

                    indicator.running=true

                }else if(str===qsTr("新建模型")){
                    contented.source="qrc:/SoilModeling/AddNewModel.qml";
                    contented.active=true;
                }
                else{
                    console.log("error ui")
                }

            }
        }
    }

}
