import QtQuick 2.15
import QtQuick.Window 2.15



Window {
    width: 1920
    height: 900

    visible: true
    title: "ScreenUi"

    MainUi {
        id: mainScreen
        width:parent.width
        height:parent.height
    }

    Connections{
        target:mainScreen
        function onInvokeMainUi(type1){
            console.log("invoke ui type"+type1);
            if(type1===qsTr("系统设置")){
                subDialog.source="qrc:/SystemSettings.qml"
                mainScreen.z=-1;
                subDialog.active=true;
            }
        }
    }

    Loader{
        id:subDialog

        width:parent.width
        height:parent.height

        onLoaded: {
              // 当项加载完成后，连接返回信号
              subDialog.item.returnSignal.connect(function() {
                 console.log("Result: ");
                  subDialog.active=false;
                  mainScreen.z=1;
              });
           }
    }

}
