import QtQuick 2.15
import QtQuick.Layouts 1.0
import "./"

Item{
    id:root


    required property ListModel itemInfo;

    property var widthRate:[4,4,6,4,4];

    function getWidth(cindex){
        let count=0;

        for(var i=0;i<widthRate.length;++i){
            count+=widthRate[i];
        }
        let totalWidth=root.width
        let slice=totalWidth/count
        return slice*widthRate[cindex];
    }
    Row{
        //QSerialPort
        //第二第三层合并//模型优化改到第三页
       anchors.fill: parent
       spacing:0
        Repeater{
            id:repeater
            anchors.fill: parent
            model:itemInfo
            delegate:MyHeaderItem{
              currentIndex:index
              width:getWidth(index)
              height:parent.height
              text:label
              //x:index*width
            }
        }


    }
}


