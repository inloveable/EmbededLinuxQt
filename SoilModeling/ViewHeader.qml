import QtQuick 2.15

import "./"
Row{

    //QSerialPort

    //第二第三层合并//模型优化改到第三页

    id:root

    required property ListModel itemInfo;
    readonly property int leftWidth:root.width-root.width/4
    Repeater{
        id:repeater
        anchors.fill: parent
        model:itemInfo
        delegate:MyHeaderItem{
          currentIndex:index
          width:parent.width/repeater.count
          height:parent.height
          text:label
          x:index*width
        }
    }

    /*
    MyHeaderItem{
        currentIndex:0
        width:leftWidth/(root.children.length-1)
        height:parent.height
        text:"编号"
    }

    MyHeaderItem{

        currentIndex:1
        width:leftWidth/(root.children.length-1)
        height:parent.height
        text:"名称"
    }
    MyHeaderItem{

        currentIndex:2

        width:root.width/4
        height:parent.height
        text:"建立时间"
    }
    MyHeaderItem{

        currentIndex:3

        width:leftWidth/(root.children.length-1)
        height:parent.height
        text:"相关系数"
    }
    MyHeaderItem{
        currentIndex:4
        width:leftWidth/(root.children.length-1)
        height:parent.height
        text:"来源"
    }
*/
}
