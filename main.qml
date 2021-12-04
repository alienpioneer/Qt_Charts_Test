import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtCharts 2.1

Window {
    id: window
    width: 1000
    height: 480
    visible: true
    color: "#404040"
    title: qsTr("Data Plotter")

    Connections {
        target: controller
        ignoreUnknownSignals: true

        onUpdateValue :
        {
            addPoint( controller.value )
        }

    }

    TextField {
        id: maxYtextField
        x: 87
        y: 27
        width: 70
        height: 28
        selectByMouse: true
        text: "500"

        Label {
            id: maxYLabel
            x: -72
            y: 0
            width: 53
            height: 28
            color: "#000000"
            text: qsTr("yMax")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 8
        }

        onAccepted: yAxis.max = text.valueOf()
    }

    TextField {
        id: maxXtextField
        x: 87
        y: 74
        width: 70
        height: 28
        selectByMouse: true
        text: "100"

        Label {
            id: maxXLabel
            x: -72
            y: 0
            width: 53
            height: 28
            color: "#000000"
            text: qsTr("xWidth")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 8
        }

        onAccepted: xAxis.max = text.valueOf()
    }

    TextField {
        id: sPeriodTextField
        x: 87
        y: 119
        width: 70
        height: 28
        selectByMouse: true
        text: "2"

        Label {
            id: sPeriodLabel
            x: -72
            y: 0
            width: 53
            height: 28
            color: "#000000"
            text: qsTr("sPeriod")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 8
        }

    }

    Rectangle {
        id: plotRectangle
        x: 0
        y: 0
        width: 740
        height: 426
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.rightMargin: 30
        anchors.leftMargin: 193
        anchors.bottomMargin: 27
        anchors.topMargin: 27
        color: "#6e6e6e"

        property int maxThreshold: Math.round(parseInt(maxXtextField.text) * 0.90)
        property int xMinOffset: Math.round(parseInt(maxXtextField.text) * 0.25)
        property int xMaxOffset: Math.round(parseInt(maxXtextField.text) * 0.90)

        ChartView {
            id: plotView
            anchors.fill: parent
            theme: ChartView.ChartThemeDark
            animationOptions: ChartView.NoAnimation

            ValueAxis {
                id: yAxis
                min: 0
                max: parseInt(maxYtextField.text)
            }

            ValueAxis {
                id: xAxis
                min: 0
                max: parseInt(maxXtextField.text)
            }

            LineSeries {
                id : rawDataSeries
                name: "Raw Data"
                color: "#d80028"
                width: 2
                axisX: xAxis
                axisY: yAxis
            }

            LineSeries {
                id : logDataSeries
                name: "Log Data"
                color: "#f9ff00"
                width: 2
                axisX: xAxis
                axisY: yAxis
            }
        }
    }

    function addPoint(value)
    {
        // Calculate X position
        var xVal = 0
        if(rawDataSeries.count > 0)
            xVal = ( rawDataSeries.at(rawDataSeries.count-1).x + parseInt(sPeriodTextField.text) )

        // Resize instantaneous max Y value
        if (value > yAxis.max)
        {
            yAxis.max = value + Math.round(value*0.3)
        }

        rawDataSeries.append(xVal, value)
        logDataSeries.append(xVal, Math.log10(value) * 10/0.25)
        console.log("Plotting Value", xVal, value, Math.log10(value) * 10/0.25)

        if ( xVal > plotRectangle.maxThreshold )
        {
            xAxis.min = xAxis.min + plotRectangle.xMinOffset
            xAxis.max = xAxis.min + parseInt(maxXtextField.text)
            //console.log("New axis", xAxis.min, xAxis.max)

            // Calculate new threshold value
            plotRectangle.maxThreshold =  xAxis.min + plotRectangle.xMaxOffset
            //console.log( "New maxThreshold", plotRectangle.maxThreshold )

            //Remove invisible points
            rawDataSeries.removePoints(0, getSeriesLastHiddenIndex(rawDataSeries, xAxis.min)-1)
            logDataSeries.removePoints(0, getSeriesLastHiddenIndex(rawDataSeries, xAxis.min)-1)
            //console.log( "New count", rawDataSeries.count )

            // Resize yMAx
            yAxis.max = findSeriesMax(rawDataSeries) + Math.round(findSeriesMax(rawDataSeries)*0.3)
            //console.log("New Y max", yAxis.max)
        }
    }

    //need to find the last visible index since there is no guarantee that all the data is received
    function getSeriesLastHiddenIndex(series, xAxisMin)
    {
        for (var index = 0; index < series.count ; index++)
        {
            if( series.at(index).x > xAxisMin )
                return index;
        }
    }

    function findSeriesMax(series)
    {
        var max = 0;
        for (var index = 0; index < series.count ; index++)
        {
            if( series.at(index).y > max )
                max = series.at(index).y;
        }
        return max;
    }
}
