//import related modules
import QtQuick 6.7
import QtQuick.Controls 6.7
import QtQuick.Layouts 6.7

//window containing the application
ApplicationWindow {

    visible: true

    //title of the application
    title: qsTr("TheLab")
    color: "#c8c4c4"
    width: 1920
    height: 720

    //menu containing two menu items
    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered")
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit()
            }
        }
    }

    //Content Area

    //a button in the middle of the content area
    RowLayout {
        id: rowLayout
        anchors.fill: parent
        
        ListView {
            id: listview
        
            width: 200; height: 200
            snapMode: ListView.SnapOneItem
            orientation: ListView.Horizontal
            highlightRangeMode: ListView.StrictlyEnforceRange
        
            model: CalendarModel {
                from: new Date(2000, 0, 1)
                to: new Date(2032, 11, 31)
            }
        
            delegate: MonthGrid {
                width: listview.width
                height: listview.height
        
                month: model.month
                year: model.year
                locale: Qt.locale("en_US")
            }
        
            ScrollIndicator.horizontal: ScrollIndicator { }
        }

        Column {
            id: column
            width: 195
            height: 480
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            Rectangle {
                id: rectangle
                width: 200
                height: 40
                color: "#3ce6ef"
            }
        }
    }
}
