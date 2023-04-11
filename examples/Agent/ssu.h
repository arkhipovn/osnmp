#ifndef SSU_H
#define SSU_H

#define MONITORING_CARD_SLOT     18

#define NUMBER_OF_GENERATOR_CARD_INPUTS     18
#define NUMBER_OF_INPUT_CARD_INPUTS         4
#define NUMBER_OF_OUTPUT_CARD_INPUTS        20
#define NUMBER_OF_OUTPUT_CARD_OUTPUTS       20
#define NUMBER_OF_GENERATOR_CARD            2


namespace SSU {
    enum CardType {
        EmptySlot = 0,
        Input   = 1,      // 1
        OutputL,    // 2
        OutputH,    // 3
        Generator,  // 4
        Monitoring, // 5
        Stub,
        GNSS,
        OutputMultiplier    // 8
    };

    enum SSM {      // !! ???
        NONE = 0,
        PRC = 2,
        SSU_T = 4,
        SSU_L = 8,
        SEC = 11,
        DNU = 15
    };

    enum SsmMode {
        ssmModeAUTO = 0,   // 0
        ssmModeMANUAL,     // 1
        ssmModeOFF         // 2
    };

    enum SignalType {
        SignalTypeOff = 0,
        SignalType10MHz,
        SignalType2048KHz,
        SignalTypeE1
    };

    enum OutSignalType {
        oSignalTypeOff = 0,
        //SignalType10MHz,
        oSignalType2048KHz,
        oSignalTypeE1
    };

    // StatusThreshold
    enum StatusThreshold {
        StatusThresholdNone = 0,
        StatusThresholdLow,
        StatusThresholdHigh,
        None11
    };

    enum GeneratorState {
        Warmup = 0,
        Freerun,
        Tracking,
        //Adjustment,
        Holdover,
        Adjustment = 6,
        Error = 0xFF
    };

    enum GeneratorError {
        GeneratorFailure            = 1,
        DDSFailure                  = 2,
        MeasurementSystemFailure    = 4,
        ControlLoopFailure          = 8,
        PriorityTableError          = 16
    };

    enum GeneratorMode {
        GModeAuto,
        GModeManual,
        GModeForcedHoldover
    };

    enum InputSignalState {
        StatusDisqualification = 1,
        LossOfSignal = 2,
        SSMDisqualification = 4,
        LossOfFrame = 8,
        LossOfMultiFrame = 16,
        RecoveryDelay = 32
    };

    enum CardMode {
        Passive = 0,
        Active,
    };
}

#endif // SSU_H
