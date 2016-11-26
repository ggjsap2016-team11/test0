#include "DxLib.h"
#include <stdlib.h>
#include "Title.h"
#include "resource.h"

int Key = -1;

// ノーツの最大数
#define MAX_NOTES 300

//画面サイズ
#define MAX_DISPLAY_SIZE_X 960
#define MAX_DISPLAY_SIZE_Y 540


// ノーツが存在するか、フラグ
int Notes[MAX_NOTES];	
// ノーツの位置
double NotesX[MAX_NOTES] ,NotesY[MAX_NOTES];
//判定対象のノーツ
double Hit[ MAX_NOTES ];

// プレイヤーの位置
int PlayerX , PlayerY;	
int PlayerSizeX,PlayerSizeY;
int NotesSizeX,NotesSizeY;

static int mStartTime;      //測定開始時刻
static int mCount;          //カウンタ
static float mFps;          //fps
static const int N = 60;	//平均を取るサンプル数
static const int FPS = 60;	//設定したFPS
static int GameHp = 50;
const int GameLineColor = GetColor(0 ,150 ,0 ); 

//起動時からのカウント
int cnt = 0;

//画面遷移
int Frame=2;


int GHandle[11];
int GDrawFlag = 0;
int BackHandle;
int StageSelectHanele;
int PrevScene;
int WaitTime;

int GameCnt;//ゲームカウント

int MusicTime[2];

int NotesPattern[][3][121] = {
	{
		{
			1,2,1,0,3,4,3,0,
			1,0,2,0,3,0,4,0,
			4,3,2,1,2,3,1,4,
			0,0,1,3,2,4,0,0,
			1,2,3,4,1,2,3,0,
			1,2,1,0,3,4,3,0,
			1,0,2,0,3,0,4,0,
			4,3,2,1,2,3,1,4,
			0,0,1,3,2,4,0,0,
			1,2,3,4,1,2,3,0,
			1,2,1,0,3,4,3,0,
			1,0,2,0,3,0,4,0,
			4,3,2,1,2,3,1,4,
			0,0,1,3,2,4,0,0,
			1,2,3,4,1,2,3,0
		},
		{
			1,4,4,4,3,4,3,0,
			1,0,2,0,3,0,4,0,
			4,3,2,1,2,3,1,4,
			0,0,1,3,2,4,0,0,
			1,2,3,4,1,2,3,0,
			1,2,1,0,3,4,3,0,
			1,0,2,0,3,0,4,0,
			4,3,2,1,2,3,1,4,
			0,0,1,3,2,4,0,0,
			1,2,3,4,1,2,3,0,
			1,2,1,0,3,4,3,4,
			1,4,2,4,3,0,4,0,
			4,3,2,1,2,3,1,4,
			0,0,1,3,2,4,0,0,
			1,2,3,4,1,2,3,0
		},
		{
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1
		}
	},
	{
		{
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1
		},
		{
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1
		},
		{
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1
		}
	}
};
int JudgePattern[] = 
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0
	};
int StageBottom;
int YourDieH;
int GameOverCount;
int danceeVal;
int danceScore;
int danceEvalH[3];
int danceEvalScoreH[3];
int CurSor[5];
int selectStageNumber;
int selectStageDif;
int selectMode;
int selectStageDifH[3];
int selectStageDifunH[3];
int selectModeY[3] = {145, 260, 365};
int selectChangeCount = 0;
int stageBackH[2];
int selectModeH[3];
int selectCursorH;
int g_judgenumber = 0;
const int Tama_h =40;
const int Tama_w =40;
const int BorderJusts[3] = {20, 10, 5};
const int BorderNears[3] = {40, 60 , 10};
const int BorderMisss[3] = {60, 70, 30};
int DecisionEffect[3];
int EffectFlag = 4;
int EffectSizeHalf = 75;
int EffectTimer =0;
int resultMenu[3];
int resultMenuY[3] = {335, 400, 470};
int resultSelectMenu = 0;
int selectResultCount = 0;
int EffectHandle[3];
int GameHandle[3];
int PowerBarHandle;
int LevelHandle[6];
int RBaseHandle;
int SHandle[2];
int staffRollH[8];
int MHandle[12];
int GamePlayStart=0;
int GamePlayAnim=0;
int GameOverStart=0;
int GameOverAnim=0;
int GameClearStart=0;
int GameClearAnim=0;

bool Update(){
	if( mCount == 0 ){ 
		//1フレーム目なら時刻を記憶
		mStartTime = GetNowCount();
	}
	if( mCount == N ){ 
		//60フレーム目なら平均を計算する
		int t = GetNowCount();
		mFps = 1000.f/((t-mStartTime)/(float)N);
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	cnt++;
	return true;
}

void Wait(){
	int tookTime = GetNowCount() - mStartTime;
	int waitTime = mCount*1000/FPS - tookTime;
	if( waitTime > 0 ){
		Sleep(waitTime);
	}
}

void Load(){


	GHandle[0] = LoadGraph("Image/PlayerWait1.png");
	GHandle[1] = LoadGraph("Image/PlayerWait2.png");

	GHandle[2] = LoadGraph("Image/ActionUp_Just.png");
	GHandle[3] = LoadGraph("Image/ActionUp_Near.png");
	GHandle[4] = LoadGraph("Image/ActionRight_Just.png");
	GHandle[5] = LoadGraph("Image/ActionRight_Near.png");
	GHandle[6] = LoadGraph("Image/ActionDown_Just.png");
	GHandle[7] = LoadGraph("Image/ActionDown_Near.png");
	GHandle[8] = LoadGraph("Image/ActionLeft_Just.png");
	GHandle[9] = LoadGraph("Image/ActionLeft_Near.png");
	GHandle[10] = LoadGraph("Image/miss.png");

	DecisionEffect[0]= LoadGraph("Image/Effect_Just.png");
	DecisionEffect[1]= LoadGraph("Image/Effect_Near.png");
	DecisionEffect[2]= LoadGraph("Image/Effect_Miss.png");

	BackHandle = LoadGraph("Image/BackImage_Stage1.png");
	StageBottom = LoadGraph("Image/stage_bottom.png");

	GTitleHandle = LoadGraph("Image/TitleLogo.png");
	GTitleBgHandle = LoadGraph("Image/BackImage_Title.png");
	GTitleStartHandle = LoadGraph("Image/start_01.png");
	GTitleStartPushHandle = LoadGraph("Image/start_02.png");
	StageSelectHanele = LoadGraph("Image/stageSelect.png");

	Opening[0] = LoadGraph("Image/Opening1.png");
	Opening[1] = LoadGraph("Image/Opening2.png");
	Opening[2] = LoadGraph("Image/Opening3.png");
	Opening[3] = LoadGraph("Image/Opening4.png");
	YourDieH = LoadGraph("Image/YouDie.png");
	danceEvalH[0] = LoadGraph("Image/BackImage_Result3.png");
	danceEvalH[1] = LoadGraph("Image/BackImage_Result2.png");
	danceEvalH[2] = LoadGraph("Image/BackImage_Result1.png");
	danceEvalScoreH[0] =  LoadGraph("Image/score_bad.png");
	danceEvalScoreH[1] =  LoadGraph("Image/score_normal.png");
	danceEvalScoreH[2] =  LoadGraph("Image/score_good.png");

	CurSor[0] = 0;
	CurSor[1] = LoadGraph("Image/c_up.png");
	CurSor[2] = LoadGraph("Image/c_right.png");
	CurSor[3] = LoadGraph("Image/c_down.png");
	CurSor[4] = LoadGraph("Image/c_left.png");

	stageBackH[0] = LoadGraph("Image/BackImage_Stage1.png");
	stageBackH[1] = LoadGraph("Image/BackImage_Stage2.png");
	selectCursorH = LoadGraph("Image/cursor.png");

	selectModeH[0] = LoadGraph("Image/stage1.png");
	selectModeH[1] = LoadGraph("Image/stage2.png");
	selectModeH[2] = LoadGraph("Image/stage0.png");


	resultMenu[0] = LoadGraph("Image/result_retry.png");
	resultMenu[1] =  LoadGraph("Image/result_stage.png");
	resultMenu[2] = LoadGraph("Image/result_title.png");

	EffectHandle[0] = LoadGraph("Image/Effect_Just.png");
	EffectHandle[1] = LoadGraph("Image/Effect_Miss.png");
	EffectHandle[2] = LoadGraph("Image/Effect_Near.png");

	GameHandle[0] = LoadGraph("Image/gameClear.png");
	GameHandle[1] = LoadGraph("Image/gameOver.png");
	GameHandle[2] = LoadGraph("Image/gameStart.png");
	
	PowerBarHandle = LoadGraph("Image/HitPointBar.png");
	
	LevelHandle[0] = LoadGraph("Image/Effect_Just.png");
	LevelHandle[1] = LoadGraph("Image/result_title.png");
	LevelHandle[2] = LoadGraph("Image/result_title.png");
	LevelHandle[3] = LoadGraph("Image/result_title.png");
	LevelHandle[4] = LoadGraph("Image/result_title.png");
	LevelHandle[5] = LoadGraph("Image/result_title.png");

	selectStageDifH[0] = LoadGraph("Image/level_01.png");
	selectStageDifH[1] = LoadGraph("Image/level_02.png");
	selectStageDifH[2] = LoadGraph("Image/level_03.png");


	selectStageDifunH[0] = LoadGraph("Image/level_01x.png");
	selectStageDifunH[1] = LoadGraph("Image/level_02x.png");
	selectStageDifunH[2] = LoadGraph("Image/level_03x.png");

	RBaseHandle = LoadGraph("Image/result_title.png");

	//曲	
	SHandle[0] = LoadSoundMem("Sound/GGJ_music1BPM120.ogg");
	SHandle[1] = LoadSoundMem("Sound/GGJ_music2completedBPM135.ogg");

	MusicTime[0] = 77;
	MusicTime[1] = 62;

	//効果音
	MHandle[0] = LoadSoundMem("Sound/GGJ_button push.wav");
	MHandle[1] = LoadSoundMem("Sound/GGJ_clear.wav");
	MHandle[2] = LoadSoundMem("Sound/GGJ_finish button.wav");
	MHandle[3] = LoadSoundMem("Sound/GGJ_gudge button down.wav");
	MHandle[4] = LoadSoundMem("Sound/GGJ_gudge button left.wav");
	MHandle[5] = LoadSoundMem("Sound/GGJ_gudge button right.wav");
	MHandle[6] = LoadSoundMem("Sound/GGJ_gudge button up.wav");
	MHandle[7] = LoadSoundMem("Sound/GGJ_hanko.wav");
	MHandle[8] = LoadSoundMem("Sound/GGJ_miss button.wav");
	MHandle[9] = LoadSoundMem("Sound/GGJ_retire.wav");
	MHandle[10] = LoadSoundMem("Sound/GGJ_select button1.wav");

	staffRollH[0] = LoadGraph("Image/staffroll_1.png");
	staffRollH[1] = LoadGraph("Image/staffroll_2.png");
	staffRollH[2] = LoadGraph("Image/staffroll_3.png");
	staffRollH[3] = LoadGraph("Image/staffroll_4.png");
	staffRollH[4] = LoadGraph("Image/staffroll_5.png");
	staffRollH[5] = LoadGraph("Image/staffroll_6.png");
	staffRollH[6] = LoadGraph("Image/staffroll_7.png");
	staffRollH[7] = LoadGraph("Image/staffroll_8.png");


}

//効果音を再生
void PlaySound(int Handle){
	PlaySoundMem(Handle ,DX_PLAYTYPE_BACK);
}

void setDanceEval() {
	if (GameHp > 80) {
		danceeVal = 2;
	} else if (GameHp > 30) {
		danceeVal = 1;
	} else {
		danceeVal = 0;
	}
	PlaySound(MHandle[7]);

}

void SetNotes(){
	//ノーツ発生
	int selectNotesPattern[121];
	int selectNoteLen = sizeof selectNotesPattern/sizeof(int);

	for(int i=0; i < selectNoteLen ;i++){
		selectNotesPattern[i] = NotesPattern[selectStageNumber][selectStageDif][i];
	}

	for (int i = 0; i < selectNoteLen ; i++) {
		// ショットの位置を設定
		NotesX[i] = MAX_DISPLAY_SIZE_X - 60 +  (i * 75);
		NotesY[i] = 400;
			
		// ノーツデータを使用中にセット
		Notes[i] = selectNotesPattern[i];

		JudgePattern[i] = selectNotesPattern[i];

	}
}

void PlayIni(){
	// プレイヤーの初期位置をセット
	PlayerX = 200;
	PlayerY = 75;

	PlayerSizeX = 200;
	PlayerSizeY = 250;

	// ノーツの存在を初期化する
	for(int i = 0 ; i < MAX_NOTES ; i ++ ){
		Notes[i] = 0 ;
		Hit[i] = 0;
	}


	NotesSizeX = 50;
	NotesSizeY = 50;
	
	GameCnt=0;
	GamePlayStart = 0;
	//譜面セット
	SetNotes();
	
}



void Move(){

	if (GamePlayStart == 0) {
		GamePlayAnim++;
	}

	if (GamePlayStart == 0 && GamePlayAnim > 150) {
		GamePlayStart = 1;
		GamePlayAnim = 0;
		g_judgenumber = 0;
		PlaySoundMem(SHandle[selectMode] , DX_PLAYTYPE_BACK);
	}
	
	if (GamePlayStart == 0) {
		return;
	}


	// ノーツの移動処理
	for (int j = 0 ; j < MAX_NOTES ; j ++ ){

		if (NotesX[j] == -3000) {
			continue;
		}

		// 左に移動
		NotesX[ j ] -= 10;

		// 画面外に出ていたら無効にする
		if( NotesX[j] < -10 && -1 < Notes[j] && j < 121){
			NotesX[j] = -3000;
			g_judgenumber++;
			if( 0 < Notes[j] ) {
				GDrawFlag = 10;
				GameHp--;
				PlaySound(MHandle[8]);
			}
			Notes[j] = -1;
		}
	}

	if (GameOverStart == 1) {
		GameOverAnim++;
	}

	if (GameClearStart == 1) {
		GameClearAnim++;
	}

	if (GameOverStart == 1 && GameOverAnim >= 180) {
		GameOverCount = 0;
		setDanceEval();
		Frame=4;
	}

	if (GameClearStart == 1 && GameClearAnim >= 180) {
		setDanceEval();
		Frame =5;
	}

}

void Draw(){
	// 背景背景
	DrawGraph(0, 0, stageBackH[selectStageNumber], TRUE);
	DrawGraph(0, 0, StageBottom, TRUE);

	//プレイヤー描画
	DrawGraph(PlayerX, PlayerY, GHandle[GDrawFlag], TRUE);

	if(EffectFlag != 4 && EffectTimer <=10){
		DrawGraph((PlayerX+PlayerSizeX/2) - EffectSizeHalf,365,DecisionEffect[EffectFlag],TRUE);
		EffectTimer++;
		if(EffectTimer > 10){
			EffectTimer = 0;
			EffectFlag = 4;
		}
	}

	// ノーツを描画する
	for(int j = 0 ; j < MAX_NOTES ; j ++ ){
		// ノーツデータが有効な時のみ描画
		if( 1 <= Notes[j] ) {
			DrawGraph( NotesX[j], NotesY[j] , CurSor[NotesPattern[selectStageNumber][selectStageDif][j]] , TRUE);		
		}
	}

	
	//曲の現在位置
	DrawBox(50 , 0 , MAX_DISPLAY_SIZE_X - 50 , 20 , GameLineColor, FALSE);
	
	float drawBoxCheckValue = (float)(GameCnt/60)/MusicTime[selectMode];
	if(drawBoxCheckValue < 1){
		DrawBox( 50 , 0 , 50 + ( MAX_DISPLAY_SIZE_X - 100 )*( (float)(GameCnt)/(MusicTime[selectMode]*60) ), 20 , GameLineColor, TRUE);
	}else if(drawBoxCheckValue >= 1){
		DrawBox( 50 , 0 , MAX_DISPLAY_SIZE_X - 50, 20 , GameLineColor , TRUE);
	}

	if (GameHp > 0) {
		DrawRotaGraph3( 50, 500, 0, 0, ((float)GameHp / 100), 1.0f, 0, PowerBarHandle, TRUE);
	}

	if (GameOverStart == 1 && GameOverAnim % 5 < 4) {
		DrawGraph(310, 120, GameHandle[1], TRUE);
	}

	if (GameClearStart == 1 && GameClearAnim % 5 < 4) {
		DrawGraph(310, 120, GameHandle[0], TRUE);
	}

	if (GamePlayStart == 0 && GamePlayAnim > 30 && GamePlayAnim % 5 < 4) {
		DrawGraph(310, 120, GameHandle[2], TRUE);
	}

}

void MainGameInit() {
	PlayIni();//初期化
	SetNotes();//譜面セット
	GameHp = 50;
	danceeVal = 0;
	danceScore = 0;
	GamePlayStart = 0;
	GamePlayAnim = 0;
	GameOverStart = 0;
	GameOverAnim = 0;
	GameClearStart = 0;
	GameClearAnim = 0;
	selectStageNumber = selectMode;
	
	//ゲーム画面に遷移
	Frame=3;                    
}



void StageSelectKey() {
	int Pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if (selectChangeCount == 0 && Pad & PAD_INPUT_UP) { 
		PlaySound(MHandle[10]);
		selectMode--;
		if (selectMode < 0) {
			selectMode = 0;
		}

		selectChangeCount = 20;
	}
	if(selectChangeCount == 0 && Pad & PAD_INPUT_RIGHT) {
		PlaySound(MHandle[10]);
		selectStageDif++;
		if (selectStageDif > 2) {
			selectStageDif = 2;
		}

		selectChangeCount = 20;
	}
	if(selectChangeCount == 0 && Pad & PAD_INPUT_DOWN){
		PlaySound(MHandle[10]);
		selectMode++;
		if (selectMode > 2) {
			selectMode = 2;
		}
		selectChangeCount = 20;

	}
	
	if(selectChangeCount == 0 && Pad & PAD_INPUT_LEFT){
		PlaySound(MHandle[10]);
		selectStageDif--;
		if (selectStageDif < 0) {
			selectStageDif = 0;
		}
		selectChangeCount = 20;

	}

	selectChangeCount--;
	if (selectChangeCount < 0) {
		selectChangeCount = 0;
	}

	if (selectChangeCount == 0 && Pad & PAD_INPUT_4) {
		PlaySoundMem( MHandle[0] , DX_PLAYTYPE_BACK ) ;
		switch(selectMode) {
			case 0:
				MainGameInit();
				break;
			case 1:
				selectStageNumber = 1;
				selectStageDif = 1;
				MainGameInit();
				break;
			case 2:
				Frame = 0;
				break;
		}
	}


}

void StageSelectUpdate() {
	StageSelectKey();
}

void StageSelectDraw() {
	DrawGraph(0, 0, StageSelectHanele, TRUE);
	DrawGraph(125, selectModeY[selectMode]+15, selectCursorH, TRUE);
	for (int i = 0; i < 3; i++) {
		DrawGraph(225, selectModeY[i], selectModeH[i], TRUE);
	}

	for (int i = 0; i < 3; i++) {
		if (selectMode == 0 && i == selectStageDif) {
			DrawGraph(675+(i*45), 145, selectStageDifH[i], TRUE);
		} else {
			DrawGraph(675+(i*45), 155, selectStageDifunH[i], TRUE);
		}
		if (selectMode == 1 && i == selectStageDif) {
			DrawGraph(675+(i*45), 255, selectStageDifH[i], TRUE);
		} else {
			DrawGraph(675+(i*45), 265, selectStageDifunH[i], TRUE);
		}
	}

}

void CharacterMove(int ComandNumber,int ObjectNumber)
{
	GDrawFlag = ComandNumber;
	Notes[ObjectNumber] = -1;
	NotesX[ObjectNumber] = -3000;
	g_judgenumber++;
	cnt = 0;

}

void CharacterMiss(int ObjectNumber) {
	CharacterMove(10, ObjectNumber);
	EffectFlag = 2;
	GameHp--;
	PlaySound(MHandle[8]);
}



int getBorderClickValue(int borderGameClick, int BorderJust, int BorderNear) {

	if(borderGameClick < BorderJust && borderGameClick > -BorderJust)
	{
		return 0;
	}
	else if(BorderJust < borderGameClick < BorderNear || -BorderJust > borderGameClick > -BorderNear)
	{
		return 1;

	}
	else
	{
		return 2;
	}			
}



void CheckInputUpJudge(int Pad, int selectNotesPattern, int i, int borderGameClick, int BorderJust, int BorderNear) {
	if ((Pad & PAD_INPUT_UP  || Pad & PAD_INPUT_2) && selectNotesPattern == 1)
	{ 	
		int borderClickType = getBorderClickValue(borderGameClick, BorderJust, BorderNear);		
		if(borderClickType == 0)
		{
			CharacterMove(2, i);		
			EffectFlag = 0;
			GameHp++;
			PlaySound(MHandle[6]);
		}
		else if(borderClickType ==1)
		{
			CharacterMove(3, i);		
			EffectFlag = 1;
			PlaySound(MHandle[6]);
		}
		else if(borderClickType == 2)
		{
			CharacterMiss(i);
		}
	}

}

void CheckInputRightJudge(int Pad, int selectNotesPattern, int i, int borderGameClick, int BorderJust, int BorderNear) {
	if((Pad & PAD_INPUT_RIGHT  || Pad & PAD_INPUT_4) && selectNotesPattern == 2){
		int borderClickType = getBorderClickValue(borderGameClick, BorderJust, BorderNear);		
		if (borderClickType == 0)
		{
			CharacterMove(4, i);		
			EffectFlag = 0;
			GameHp++;
			PlaySoundMem( MHandle[5] , DX_PLAYTYPE_BACK);
		}
		else if(borderClickType == 1)
		{
			CharacterMove(5, i);		
			EffectFlag = 1;
			PlaySoundMem( MHandle[5] , DX_PLAYTYPE_BACK);
		}
		else if(borderClickType == 2)
		{
			CharacterMiss(i);
		}			
	}

}

void CheckInputDownJudge(int Pad, int selectNotesPattern, int i, int borderGameClick, int BorderJust, int BorderNear) {

	if( (Pad & PAD_INPUT_DOWN  || Pad & PAD_INPUT_3) && selectNotesPattern == 3){
		int borderClickType = getBorderClickValue(borderGameClick, BorderJust, BorderNear);
		if (borderClickType == 0)
		{
			CharacterMove(6, i);
			EffectFlag = 0;
			GameHp++;
			PlaySoundMem( MHandle[3] , DX_PLAYTYPE_BACK);
		}
		else if (borderClickType == 1)
		{
			CharacterMove(7, i);		
			EffectFlag = 1;
			PlaySoundMem( MHandle[3], DX_PLAYTYPE_BACK);
		}
		else if(borderClickType == 2)
		{
			CharacterMiss(i);
		}			
	}
}


void CheckInputLeftJudge(int Pad, int selectNotesPattern, int i, int borderGameClick, int BorderJust, int BorderNear) {
	if( Pad & PAD_INPUT_LEFT || Pad & PAD_INPUT_1 && selectNotesPattern == 4){
		int borderClickType = getBorderClickValue(borderGameClick, BorderJust, BorderNear);
		if (borderClickType == 0)
		{
			CharacterMove(8, i);	
			EffectFlag = 0;
			GameHp++;
			PlaySoundMem(MHandle[4], DX_PLAYTYPE_BACK);
		}
		else if(borderClickType == 1)
		{
			CharacterMove(9, i);	
			EffectFlag = 1;
			PlaySoundMem(MHandle[4], DX_PLAYTYPE_BACK);
		}
		else if (borderClickType == 2)
		{
			CharacterMiss(i);
		}			
	}
}


void CharMove(){
	int Pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	
	if (cnt % 60 ==0) {
		GDrawFlag = 0;
		cnt = 0;
	} else if(cnt % 30 == 0) {
		GDrawFlag = 1;
	}
	int BorderJust = BorderJusts[selectStageDif];
	int BorderNear = BorderNears[selectStageDif];
	int BorderMiss = BorderMisss[selectStageDif];
	int selectNotesLen = sizeof NotesPattern[selectStageNumber][selectStageDif]/sizeof(int);
	int selectNotesPattern[121];

	for (int i = 0; i < selectNotesLen; i++) {
		selectNotesPattern[i] = NotesPattern[selectStageNumber][selectStageDif][i];
	}


	for (int i = 0; i < selectNotesLen; i++) {
		if (Notes[i] == -1) {
			continue;
		}
		float playerCenterX = (PlayerX+PlayerSizeX / 2);
		if (playerCenterX + BorderMiss < NotesX[i] ||  NotesX[i] < playerCenterX - BorderMiss) {
			continue;
		}
		int selectJudgePattern = JudgePattern[i];
		float borderGameClick = playerCenterX - (NotesX[i]) - 10;
		switch(selectJudgePattern){
			case 1:
				CheckInputUpJudge(Pad, selectJudgePattern, i, borderGameClick, BorderJust, BorderNear);
				break;
			case 2:
				CheckInputRightJudge(Pad, selectJudgePattern, i, borderGameClick, BorderJust, BorderNear);
				break;
			case 3:
				CheckInputDownJudge(Pad, selectJudgePattern, i, borderGameClick, BorderJust, BorderNear);
				break;
			case 4:
				CheckInputLeftJudge(Pad,selectJudgePattern, i, borderGameClick, BorderJust, BorderNear);
				break;
		}
	}

}


void Game(){	
	Move();
	Draw();
	CharMove();

	if (GameHp <= 0 && GameOverStart == 0) {
		PlaySound(MHandle[9]);
		GameOverStart = 1;
	} else if (0 < GameHp && GameClearStart == 0 && 121 <= g_judgenumber ) {
		GameClearStart = 1;
		PlaySound(MHandle[1]);
	}
	if (GameOverStart == 1 || GameClearStart == 1) {
		g_judgenumber = 0;
	}

}

void GameOverUpdate() {

	GameOverCount++;
	if (GameOverCount > 300) {
		resultSelectMenu = 0;
		selectResultCount = 0;
		setDanceEval();
		Frame=5;
	}


}


void GameOverDraw() {
	
	DrawGraph(0, 0, YourDieH, TRUE);
}

void ResultUpdate() {

	int Pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if (selectResultCount == 0 && Pad & PAD_INPUT_UP) { 
		PlaySound(MHandle[10]);
		resultSelectMenu--;
		if (resultSelectMenu < 0) {
			resultSelectMenu = 0;
		}

		selectResultCount = 20;
	}
	if(selectResultCount == 0 && Pad & PAD_INPUT_DOWN){
		PlaySound(MHandle[10]);
		resultSelectMenu++;
		if (resultSelectMenu > 2) {
			resultSelectMenu = 2;
		}
		selectResultCount = 20;

	}
	
	selectResultCount--;
	if (selectResultCount < 0) {
		selectResultCount = 0;
	}


	if ( Pad & PAD_INPUT_4) {
		switch(resultSelectMenu) {
			case 0:
				MainGameInit();				
				break;
			case 1:
				Frame =1;
				OpeiningCount = 0;
				selectChangeCount = 20;
				break;
			case 2:
				Frame = 0;
				OpeiningCount = 0;
				break;
		}

	}


}

void ResultDraw() {

	if (Frame != 5) {
		return;
	}

	DrawGraph(0, 0, danceEvalH[danceeVal], TRUE);
	DrawGraph(0, 0, danceEvalScoreH[danceeVal], TRUE);
	for (int i = 0; i < 3; i++) {
		int resultX = -30;
		if (resultSelectMenu == i) {
			resultX = 0;
		}
		DrawGraph(resultX, resultMenuY[i], resultMenu[i], TRUE);
	}
}

void StaffRollUpdate() {

	//入力状態をPadに格納
	int Pad = GetJoypadInputState( DX_INPUT_KEY_PAD1);        

	if (Pad & PAD_INPUT_4) { 
		Frame = 0;
		OpeiningCount = 0;
	}
}


void StaffRollDraw() {
	StaffRollCount++;
	if (StaffRollCount >= 1 && StaffRollCount != 0) {
		int StaffRollCountFrame = (StaffRollCount / 120); 
		if (StaffRollCountFrame > 7) {
			StaffRollCountFrame = 7;
		}
		DrawGraph(0, 0, staffRollH[StaffRollCountFrame], TRUE);
	} else {
		DrawGraph(0, 0, staffRollH[0], TRUE);
	}

}

bool checkGameLoop() {
	return ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0;
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){
	DWORD tick;

	ChangeWindowMode(TRUE);
	SetWindowIconID(MAINICON);
	SetMainWindowText("とぶおれ");
	SetGraphMode(MAX_DISPLAY_SIZE_X, MAX_DISPLAY_SIZE_Y, 16);
	SetOutApplicationLogValidFlag(FALSE);
	SetDXArchiveKeyString("puka1203");

	if( DxLib_Init() == -1 )
	{
		 return -1;	
	}

	TitleChangeFlg = 0;
	TitleChangeCount = 0;
	TitleSelectCount = 20;

	// 描画先画面を裏画面にセット
	SetDrawScreen( DX_SCREEN_BACK ) ;

	//ゲーム起動時の時間を記録
	tick = GetTickCount();

	Load();//画像等読み込み

	// ループ
	while(checkGameLoop()){

		//更新
		Update();

		// 画面を初期化する
		ClearDrawScreen();
		switch(Frame){
			case 0:
				TitleDraw();
				Frame = TitleUpdate(MHandle[0], Frame);
				break;
			case 1:
				StageSelectUpdate();
				StageSelectDraw();
				break;
			case 2:
				Frame = OpeningUpdate(Frame);
				OpeningDraw();
				break;
			case 3:
				Game();
				GameCnt++;
				break;
			case 4:
				GameOverUpdate();
				GameOverDraw();
				break;
			case 5:
				ResultUpdate();
				ResultDraw();
				break;
			case 6:
				StaffRollUpdate();
				StaffRollDraw();
				break;
		
		}
		
		ScreenFlip();

	}

	DxLib_End();
	return 0;
}