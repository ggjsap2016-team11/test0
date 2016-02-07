#include "DxLib.h"

int GTitleHandle;
int GTitleBgHandle;
int GTitleStartHandle;
int GTitleStartPushHandle;
int TitleChangeFlg;
int TitleChangeCount;
int TitleSelectCount;
int StaffRollCount;
int OpeiningCount;
int Opening[4];

void TitleDraw() {
	DrawGraph( 0 , 0, GTitleBgHandle, TRUE);
	DrawGraph( 0 , 0, GTitleHandle, TRUE);
	if (TitleChangeFlg == 0) {
	    DrawGraph( 600, 375, GTitleStartHandle, TRUE);
	} else {
		if (TitleChangeCount % 8 >= 5) {
		    DrawGraph( 600, 375, GTitleStartPushHandle, TRUE);
		}
	}
}


int TitleUpdate(int startPushHandle, int sceneFrame) {

	int Pad = GetJoypadInputState( DX_INPUT_KEY_PAD1);
    if (TitleSelectCount <= 0 && TitleChangeFlg == 0 && (Pad & PAD_INPUT_4) ) {  
		PlaySoundMem(startPushHandle, DX_PLAYTYPE_BACK);
		TitleChangeFlg = 1;
	}

	if (TitleSelectCount <= 0 && (Pad & PAD_INPUT_L)) {
		StaffRollCount = 0;
		sceneFrame = 6;
		TitleSelectCount = 20;
	}

	if (TitleChangeFlg == 1) {
		if (TitleChangeCount >= 120) {
			sceneFrame = 1;
			TitleChangeCount = 0;
			TitleChangeFlg = 0;
			TitleSelectCount = 20;
		} else {
			TitleChangeCount++;
		}
	}

	if (sceneFrame == 0 && TitleSelectCount > 0) {
		TitleSelectCount--;
	}

	return sceneFrame;

}

int OpeningUpdate(int sceneFrame) {
    int Pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
    if(OpeiningCount >= 660 || Pad & PAD_INPUT_4 ){
		sceneFrame = 0;
		TitleChangeCount = 0;
		TitleChangeFlg = 0;
		TitleSelectCount = 20;
    }
	return sceneFrame;
}

void OpeningDraw() {
	OpeiningCount++;
	if (OpeiningCount >= 1 && OpeiningCount != 0) {
		int OpeiningCountFrame = (OpeiningCount / 120); 
		if ((OpeiningCount / 120) > 3) {
			OpeiningCountFrame = 3;
		}
		DrawGraph(0, 0, Opening[OpeiningCountFrame], TRUE);
	} else {
		DrawGraph(0, 0, Opening[0], TRUE);
	}
}

