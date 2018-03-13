#ifndef __Detector_H
#define __Detector_H


#define DT_OutTimer 25


enum ValueUnit {
    R_Unknown = 0,//
    R_CPS = 1,
    R_uSvPh,
    R_uGyPh,
};


enum DetectorEN {
    DT_Disabled,
    DT_Enabled,
};


enum DetectorTtpes {
    GammaRay,
    X_Ray,
    AlphaRay,
    BetaRay,
    NeutronRay,
};


typedef struct Detector_{
    unsigned short ID;  //   
    unsigned char  type;//
    unsigned char  Temperature;
    unsigned char  unit;//    
    unsigned char  NOP;
    unsigned short Status;
    unsigned long  Value0;//HZ
    float  Value1;//CPS
    float  Value2;//uSv/h
    float  Value3;//uGy/h
    float  Accumulation1;//uSv
    float  Accumulation2;//uGy
    float  Alarm1;
    float  Alarm2;
    float  Sensitivity1;
    float  Sensitivity2;
    float  DeadTime;
    float  Background;
}Detector;


typedef struct DetectorAbout_{
    unsigned char  ID;     //    
    unsigned char  Enabled;//
    unsigned char  AlramOutEnabled;
    unsigned char  unit;
    unsigned char  Msg[10];
}DetectorAbout;



//typedef struct Detector_  DT;

extern unsigned char DT_Connection_CNT;
extern unsigned char DT_Status;//


extern void init_Detector(void);

extern void RequestDetectorData(void);
extern void USART0_RECEIVE(void);






#endif
