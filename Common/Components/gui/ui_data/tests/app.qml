import QtQuick
import Constants

Item {
    id: appRootId

    width: Constants.width
    height: Constants.height

    // ------------ External interface to the backend -------------

    // Current vehicle driving speed
    property int speed: 0

    // Current time
    property string formattedTime: "12:00"

    // Battery
    property int batteryHoursLeft: 0
    property int batteryPercentage: 0

    // Fork status
    property int weight: 0 // in KG
    property int maxWeight: 1000 // in KG
    property int forkHeight: 0 // in cm
    property int maxForkHeight: 200 // in cm

    // Telltales
    property bool telltales_turnLeft: true
    property bool telltales_turnRight: true
    property bool telltales_abs: true
    property bool telltales_batteryWarning: true
    property bool telltales_beams: true
    property bool telltales_engine: true
    property bool telltales_neutral: true
    property bool telltales_warning: true

    // Gear indicator
    // -1 -> Reverse
    // 0 -> Neutral
    // 1 -> Forward
    property int gearState: 0

    // Tire pressure
    //
    // Status:
    // 0 -> Normal
    // 1 -> Warning
    // 2 -> Alert
    //
    // Pressure in PSI
    property int frontLeftTirePressure: 0
    property int frontRightTirePressure: 0
    property int rearLeftTirePressure: 0
    property int rearRightTirePressure: 0
    property int frontLeftTireStatus: 0
    property int frontRightTireStatus: 0
    property int rearLeftTireStatus: 0
    property int rearRightTireStatus: 0

    // Engine status
    property real engineTemperature: 50 // In Celsius, valid range is between 50 and 130

    // Hydraulics
    // All "Fill" properties are a percentage (0-100)
    property bool hydraulicStatusIsGood: true
    property int hydraulicTempFill: 0
    property int hydraulicPressureFill: 0
    property int hydraulicFluidLevelFill: 0
    property int hydraulicFlowRateFill: 0

    // Events for GUI navigation
    signal navigateLeft
    signal navigateRight
    signal navigateUp
    signal navigateDown

    // --------------------------------------------------------------

    Fonts { }

    Screen01 {
        id: screenO1Id
        batteryHoursRemaining: appRootId.batteryHoursLeft
        batteryPercentage: appRootId.batteryPercentage
        speedValue: appRootId.speed
        timeValue: appRootId.formattedTime
        weightValue: appRootId.weight
        maxWeightValue: appRootId.maxWeight
        heightValue: appRootId.forkHeight
        maxHeightValue: appRootId.maxForkHeight
        telltales_turnleftVisible: appRootId.telltales_turnLeft
        telltales_turnrightVisible: appRootId.telltales_turnRight
        telltales_absVisible: appRootId.telltales_abs
        telltales_batterywarningVisible: appRootId.telltales_batteryWarning
        telltales_beamsVisible: appRootId.telltales_beams
        telltales_engineVisible: appRootId.telltales_engine
        telltales_neutralVisible: appRootId.telltales_neutral
        telltales_warningVisible: appRootId.telltales_warning
        gearIndicatorState: {
            if (appRootId.gearState < 0) return "Reverse"
            else if (appRootId.gearState > 0) return "Forward"
            else return "Neutral"
        }
        frontLeftTirePressure: appRootId.frontLeftTirePressure
        frontLeftTireStatus: appRootId.frontLeftTireStatus
        frontRightTirePressure: appRootId.frontRightTirePressure
        frontRightTireStatus: appRootId.frontRightTireStatus
        rearLeftTirePressure: appRootId.rearLeftTirePressure
        rearLeftTireStatus: appRootId.rearLeftTireStatus
        rearRightTirePressure: appRootId.rearRightTirePressure
        rearRightTireStatus: appRootId.rearRightTireStatus
        vehicleStatusEngineTemperature: appRootId.engineTemperature
        vehicleStatusHydraulicStatusIsGood: appRootId.hydraulicStatusIsGood
        vehicleStatusHydraulicTempFill: appRootId.hydraulicTempFill
        vehicleStatusHydraulicPressureFill: appRootId.hydraulicPressureFill
        vehicleStatusHydraulicFluidLevelFill: appRootId.hydraulicFluidLevelFill
        vehicleStatusHydraulicFlowRateFill: appRootId.hydraulicFlowRateFill

        Connections {
            target: appRootId
            function onNavigateLeft() {
                screenO1Id.currentViewIndex = Math.min(Math.max(screenO1Id.currentViewIndex - 1, 0), screenO1Id.viewCount - 1)
            }
            function onNavigateRight() {
                screenO1Id.currentViewIndex = Math.min(Math.max(screenO1Id.currentViewIndex + 1, 0), screenO1Id.viewCount - 1)
            }
            function onNavigateDown() {
                if (screenO1Id.currentViewIndex === 2)
                    screenO1Id.currentCameraIndex = Math.min(Math.max(screenO1Id.currentCameraIndex + 1, 0), screenO1Id.cameraCount - 1)
            }
            function onNavigateUp() {
                if (screenO1Id.currentViewIndex === 2)
                    screenO1Id.currentCameraIndex = Math.min(Math.max(screenO1Id.currentCameraIndex - 1, 0), screenO1Id.cameraCount - 1)
            }
        }
    }

    focus: true
    Keys.onPressed: (event) => {
        if (event.key === Qt.Key_Left)
            appRootId.navigateLeft()
        else if (event.key === Qt.Key_Right)
            appRootId.navigateRight()
        else if (event.key === Qt.Key_Up)
            appRootId.navigateUp()
        else if (event.key === Qt.Key_Down)
            appRootId.navigateDown()
    }
}
