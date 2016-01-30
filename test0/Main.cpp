#include "DxLib.h"
#include <stdlib.h>

int Key = -1 ;

// ノーツの最大数
#define MAX_NOTES 100

//画面サイズ
#define MAX_DISPLAY_SIZE_X 960
#define MAX_DISPLAY_SIZE_Y 540


int Notes[ MAX_NOTES ] ;	// ノーツが存在するか、フラグ
double NotesX[ MAX_NOTES ] ,NotesY[ MAX_NOTES ] ;	// ノーツの位置
double Hit[ MAX_NOTES ];//判定対象のノーツ

int PlayerX , PlayerY ;	// プレイヤーの位置
int PlayerSizeX,PlayerSizeY;
int NotesSizeX,NotesSizeY;

static int mStartTime;      //測定開始時刻
static int mCount;          //カウンタ
static float mFps;          //fps
static const int N = 60;	//平均を取るサンプル数
static const int FPS = 60;	//設定したFPS
int GameHp = 50;
float FpsDelayCnt;

int cnt = 0;//起動時からのカウント

int Frame=0;//画面遷移

int SHandle;
int GHandle[11];
int GDrawFlag = 0;
int BackHandle;


int GTitleHandle;
int GTitleBgHandle;
int GTitleStartHandle;
int GTitleStartPushHandle;
int StageSelectHanele;
int TitleChangeFlg;
int TitleChangeCount;
int PrevScene;
int WaitTime;

int GameCnt;//ゲームカウント

int MusicTime=30;//テスト用　

int NotesPattern[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};//テスト用
int Opening[4];
int OpeiningCount;
int StageBottom;
int YourDieH;
int GameOverCount;
int danceeVal;
int danceScore;
int danceEvalH[3];
int danceEvalScoreH[3];
int CurSor[4];

bool Update(){
	if( mCount == 0 ){ //1フレーム目なら時刻を記憶
		mStartTime = GetNowCount();
	}
	if( mCount == N ){ //60フレーム目なら平均を計算する
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
	int tookTime = GetNowCount() - mStartTime;	//かかった時間
	int waitTime = mCount*1000/FPS - tookTime;	//待つべき時間
	if( waitTime > 0 ){
		Sleep(waitTime);	//待機
	}
}

void Load(){
	SHandle = LoadSoundMem("10MIN_120BPM_44100_16bit.wav");

	GHandle[0] = LoadGraph("Image/PlayerWait1.png");//待機1
	GHandle[1] = LoadGraph("Image/PlayerWait2.png");//待機2

	GHandle[2] = LoadGraph("Image/ActionUp_Just.png");//上just
	GHandle[3] = LoadGraph("Image/ActionUp_Near.png");//上near
	GHandle[4] = LoadGraph("Image/ActionRight_Just.png");//右just
	GHandle[5] = LoadGraph("Image/ActionRight_Near.png");//右near
	GHandle[6] = LoadGraph("Image/ActionDown_Just.png");//下just
	GHandle[7] = LoadGraph("Image/ActionDown_Near.png");//下near
	GHandle[8] = LoadGraph("Image/ActionLeft_Just.png");//左just
	GHandle[9] = LoadGraph("Image/ActionLeft_Near.png");//左near
	GHandle[10] = LoadGraph("Image/miss.png");//miss

	BackHandle = LoadGraph("Image/BackImage_Stage1.png");
	StageBottom = LoadGraph("Image/stage_bottom.png");

	GTitleHandle = LoadGraph("Image/TitleLogo.png");
	GTitleBgHandle = LoadGraph("Image/BackImage_Title.png");
	GTitleStartHandle = LoadGraph("Image/start_01.png");
	GTitleStartPushHandle = LoadGraph("Image/start_02.png");
	StageSelectHanele = LoadGraph("stageSelect_base.png");

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

	CurSor[0] = LoadGraph("Image/c_left.png");
	CurSor[1] = LoadGraph("Image/c_right.png");
	CurSor[2] = LoadGraph("Image/c_down.png");
	CurSor[3] = LoadGraph("Image/c_up.png");


}


void setDanceEval() {
	if (danceScore > 100) {
		danceeVal = 2;
	} else if (danceScore > 50) {
		danceeVal = 1;
	} else {
		danceeVal = 0;
	}

}

void PlayIni(){
	// プレイヤーの初期位置をセット
	PlayerX = 100 ;
	PlayerY = 75 ;

	PlayerSizeX = 200;
	PlayerSizeY = 250;

	// ノーツの存在を初期化する
	for(int i = 0 ; i < MAX_NOTES ; i ++ ){
		Notes[ i ] = 0 ;
		Hit[ i ] = 0;
	}


	NotesSizeX = 50;
	NotesSizeY = 50;
	
	GameCnt=0;

	FpsDelayCnt=0;

	
}

void SetNotes(){
	int n;
	boolean FirstFlag = TRUE;
	//ノーツ発生
	for(int k=0;k<sizeof NotesPattern/sizeof(int);k++){

		if(NotesPattern[k]==1){

			// 使われていないノーツデータを探す
			for( n = 0 ; n < MAX_NOTES ; n ++ ){
				if( Notes[ n ] == 0 ) break ;
			}
			// もし使われていないノーツデータがあったらノーツを出す
			if( n != MAX_NOTES ){
				// ショットの位置を設定
				NotesX[ n ] = MAX_DISPLAY_SIZE_X-60 + k*(10*FPS)+NotesSizeX ;
				NotesY[ n ] = 400 ;

				// ノーツデータを使用中にセット
				Notes[ n ] = 1 ;

				if(FirstFlag){
					Hit[ n ] = 2;//判定ノーツ
					FirstFlag = FALSE;
				}else{
					Hit[ n ] = 1;
				}
			}
		}	
	}
}

void Move(){
	// ノーツの移動処理
	int j;
	for( j = 0 ; j < MAX_NOTES ; j ++ ){
		// ノーツデータが無効だったらスキップ
		if( Notes[ j ] == 0 ) continue ;

		// 左に移動
		NotesX[ j ] -= 10 ;

		// 画面外に出ていたら無効にする
		if( NotesX[ j ] < -50 ) Notes[ j ] = 0 ;
	}
}

void Draw(){
	// 背景背景
	DrawGraph(0, 0, BackHandle, TRUE);
	DrawGraph(0, 0, StageBottom, TRUE);
	//プレイヤー描画
	
	DrawGraph(PlayerX, PlayerY, GHandle[GDrawFlag], TRUE);

	//fpsを表示
	DrawFormatString(MAX_DISPLAY_SIZE_X-80,MAX_DISPLAY_SIZE_Y-20,GetColor( 255 , 255 , 255 ),"FPS %.1f",mFps); 
	if(mFps<60 ){
		FpsDelayCnt-=(60.0f-mFps) / 60.0f;
	}else if(mFps>60){
		FpsDelayCnt-=(60.0f-mFps) / 60.0f;
	}
	DrawFormatString(MAX_DISPLAY_SIZE_X-180,MAX_DISPLAY_SIZE_Y-60,GetColor( 255 , 255 , 255 ),"FPSD %f",FpsDelayCnt); 

	// ノーツを描画する
	for(int j = 0 ; j < MAX_NOTES ; j ++ ){
		// ノーツデータが有効な時のみ描画
		if( Notes[j] == 1 ) {
			DrawGraph( NotesX[j] + FpsDelayCnt*10 , NotesY[j] , CurSor[j % 4] , TRUE);
		
			if((CheckHitKey(KEY_INPUT_SPACE) != 0) && ( PlayerX < NotesX[j] ) && ( PlayerX + PlayerSizeX > NotesX[j] )){


			}else if(CheckHitKey(KEY_INPUT_SPACE) != 0 && ( PlayerX > NotesX[j]+NotesSizeX+50 ) && ( PlayerX + PlayerSizeX < NotesX[j] +50)){
			}
		}else{
		}
	}


	//右判定　テスト用
	DrawLine( PlayerX+PlayerSizeX , 0 , PlayerX+PlayerSizeX , MAX_DISPLAY_SIZE_Y , GetColor( 255 , 0 , 0 ) ) ;

	//経過時間を表示
	DrawFormatString(MAX_DISPLAY_SIZE_X-50,0,GetColor( 255 , 255 , 255 ),"%d 秒",GameCnt/60); 

	//曲の現在位置
	DrawBox( 50 , MAX_DISPLAY_SIZE_Y - 40 , MAX_DISPLAY_SIZE_X - 50 , MAX_DISPLAY_SIZE_Y - 20 , GetColor( 255 , 255 , 255 ) , FALSE) ;
	if((float)(GameCnt/60)/MusicTime < 1){
		DrawBox( 50 , MAX_DISPLAY_SIZE_Y - 40 , 50 + ( MAX_DISPLAY_SIZE_X - 100 )*( (float)(GameCnt)/(MusicTime*60) ) , MAX_DISPLAY_SIZE_Y - 20 , GetColor( 255 , 255 , 255 ) , TRUE) ;
	}else if((float)(GameCnt/60)/MusicTime >= 1){
		DrawBox( 50 , MAX_DISPLAY_SIZE_Y - 40 , MAX_DISPLAY_SIZE_X - 50 , MAX_DISPLAY_SIZE_Y - 20 , GetColor( 0 , 255 , 255 ) , TRUE) ;
	}
}

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


void TitleUpdate() {
    int Pad = GetJoypadInputState( DX_INPUT_KEY_PAD1 ) ;        //入力状態をPadに格納
    if (TitleChangeFlg == 0 && (Pad & PAD_INPUT_4) ) {  
		TitleChangeFlg = 1;
	}

	if (TitleChangeFlg == 1) {
		if (TitleChangeCount >= 120) {
			Frame = 1;
			TitleChangeCount = 0;
		} else {
			TitleChangeCount++;
		}
	}

}

void StageSelectUpdate() {
    int Pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
    if( Pad & PAD_INPUT_4 ){
		OpeiningCount = 0;
		Frame=2;//ゲーム画面に遷移                    
    }
}

void StageSelectDraw() {
	DrawGraph(0, 0, StageSelectHanele, TRUE);
}

void CharMove(){
	int Pad = GetJoypadInputState( DX_INPUT_KEY_PAD1 ) ;        //入力状態をPadに格納
	int num;

	
	if (cnt % 60 ==0) {
		GDrawFlag = 0;
	} else if(cnt % 30 == 0) {
		GDrawFlag = 1;
	}
	if ( Pad & PAD_INPUT_UP  || Pad & PAD_INPUT_2 ) { 
		GDrawFlag=2;
	}else if( Pad & PAD_INPUT_RIGHT  || Pad & PAD_INPUT_4 ){
		GDrawFlag=4;
	}else if( Pad & PAD_INPUT_DOWN  || Pad & PAD_INPUT_3 ){
		GDrawFlag=6;
	}else if( Pad & PAD_INPUT_LEFT || Pad & PAD_INPUT_1 ){
		GDrawFlag=8;
	}

	if (Pad & PAD_INPUT_L) {
		GameHp--;
	}

	if (Pad & PAD_INPUT_R) {
		danceScore++;
		GameHp++;
	}
}

void OpeningUpdate() {
    int Pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
    if(OpeiningCount >= 660 && Pad & PAD_INPUT_4 ){
		PlayIni();//初期化
		SetNotes();//譜面セット
		GameHp = 50;
		danceeVal = 0;
		danceScore = 0;
		Frame=3;//ゲーム画面に遷移                    
    }
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


void Game(){
	
	static boolean Flag=true;

	if(Flag){
		PlaySoundMem( SHandle , DX_PLAYTYPE_BACK ,FALSE) ;
		Flag=FALSE;
	}
	
	Move();
	Draw();//描画
	CharMove();

	if (GameHp <= 0) {
		GameOverCount = 0;
		setDanceEval();
		Frame=4;
	}

}

void GameOverUpdate() {

	GameOverCount++;
	if (GameOverCount > 300) {
	
		Frame=5;
	}
}


void GameOverDraw() {
	
	DrawGraph(0, 0, YourDieH, TRUE);
}

void ResultUpdate() {

}

void ResultDraw() {

	DrawGraph(0, 0, danceEvalH[danceeVal], TRUE);
	DrawGraph(0, 0, danceEvalScoreH[danceeVal], TRUE);
}


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){
	DWORD tick;

	ChangeWindowMode( TRUE ) ;//ウィンドウズモード
	SetGraphMode( MAX_DISPLAY_SIZE_X , MAX_DISPLAY_SIZE_Y , 16 ) ;//960*540
	if( DxLib_Init() == -1 )	// ＤＸライブラリ初期化処理
	{
		 return -1;				// エラーが起きたら直ちに終了
	}

	TitleChangeFlg = 0;
	TitleChangeCount = 0;

	// 描画先画面を裏画面にセット
	SetDrawScreen( DX_SCREEN_BACK ) ;

	//ゲーム起動時の時間を記録
	tick = GetTickCount();

	Load();//画像等読み込み

	// ループ
	while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 ){

		Update();//更新

		// 画面を初期化する
		ClearDrawScreen() ;
		int Pad;
		switch(Frame){
			case 0:
				TitleDraw();
				TitleUpdate();
				break;
			case 1:
				StageSelectUpdate();
				StageSelectDraw();
				break;
			case 2:
				OpeningUpdate();
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
		
		}
		
		ScreenFlip() ;// 裏画面の内容を表画面に反映させる

		////BGMとの同期
		//WaitTime = tick + (1000.0f/FPS*cnt) - GetTickCount();
		//if(WaitTime > 0){
		//	Sleep(WaitTime);
		//}
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}