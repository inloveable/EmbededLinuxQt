import QtQuick 2.7

Item {

    // ----- Public Properties ----- //

    property bool selected: false
    property bool collapsed: false
    property alias icon: imgIcon.source

    property alias title: lbTitle.text
    property alias color: rectShadow.color
    property int index: 0

    property alias font: lbTitle.font

    signal itemSelected(string index)



    id: root
    width: parent.width
    height: _getHeight()
    z: parent.children.length - index

    Behavior on height { NumberAnimation { duration: 750; easing.type: Easing.OutExpo } }



    Rectangle {
        id: rectShadow
        anchors.fill: parent
    }

    Item {
        id: container
        width: parent.width
        height: root.collapsed ? lbTitle.contentHeight : ((imgIcon.visible ? imgIcon.height : 0) + lbTitle.contentHeight)
        anchors.centerIn: parent

        Image {
            id: imgIcon
            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
            }
            visible: !root.collapsed || !(status === Image.Null)

            scale: root.collapsed ? 0 : 1
            fillMode:Qt.KeepAspectRatio
            height:root.height/3

            Behavior on scale { NumberAnimation { duration: 200 } }
        }

        Text {
            id: lbTitle
            anchors {
                top: root.collapsed ? parent.top : imgIcon.bottom
                horizontalCenter: parent.horizontalCenter

                topMargin: root.collapsed?0:10
            }
            font.pointSize: 16
            visible: !(scale === 0)
            color: "white"
            scale: root.collapsed && !root.selected ? 0 : 1

            Behavior on scale { NumberAnimation { duration: 200 } }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.selected = !root.selected;
            root.parent.toggle();
            itemSelected(lbTitle.text)
        }
    }

    // ----- Private Functions ----- //

    function _getHeight() {
        if (!selected && collapsed) {
            return 0;
        }
        else if (selected && collapsed) {
            return root.parent.collapsedHeight;
        }

        return parent.height / parent.children.length;
    }
}
