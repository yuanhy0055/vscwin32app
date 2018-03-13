
#ifndef windows_H_
#define windows_H_


typedef struct Desktop_ {
    unsigned char  Update;    //
    void (*window)(void);     //
    unsigned short CNT;       //
}Desktops;


extern Desktops desktops;
extern void (*window_desktop)(void);
extern unsigned char my_key_val;


extern void Window(void);
extern void Desktop_Instantaneous(void);
extern void Desktop_Accumulative(void);
//extern void Desktop_PC(void);
extern void Menu_Main(void);
extern void Desktop_Time(void);

extern void Desktop_Alarm(void);
extern void Desktop_alarmvalue(void);
extern void Desktop_resetalarmvalue(void);

extern void Desktop_Number(void);
extern void Desktop_Workmodel(void);
extern void Desktop_Kedu(void);
extern void Desktop_Historyrecord(void);


extern unsigned char model;

#endif

