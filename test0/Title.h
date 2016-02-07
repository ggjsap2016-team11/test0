
extern int GTitleHandle;
extern int GTitleBgHandle;
extern int GTitleStartHandle;
extern int GTitleStartPushHandle;
extern int TitleChangeFlg;
extern int TitleChangeCount;
extern int TitleSelectCount;
extern int StaffRollCount;
extern int OpeiningCount;
extern int Opening[4];


extern int OpeningUpdate(int sceneFrame);
extern void OpeningDraw();
extern void TitleDraw();
extern int TitleUpdate(int startPushHandle, int sceneFrame);