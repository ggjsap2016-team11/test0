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

float FpsDelayCnt;

int cnt = 0;//起動時からのカウント

int Frame=0;//画面遷移

int SHandle;
int GHandle;
int GTitleHandle;


int WaitTime;

int GameCnt;//ゲームカウント

int MusicTime=30;//テスト用　

int NotesPattern[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};//テスト用

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
	GHandle = LoadGraph("待機1.png");
	GTitleHandle = LoadGraph("gametitle.png");
}

void Ini(){
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
	int FirstFlag = TRUE;
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

	//プレイヤー描画
	DrawGraph( PlayerX ,  PlayerY , GHandle , TRUE ) ;


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
			DrawBox( NotesX[j] + FpsDelayCnt*10 , NotesY[j] ,NotesX[j] + NotesSizeX+FpsDelayCnt*10 , NotesY[j] + NotesSizeY , GetColor( 255 , 255 , 255 ) , TRUE ) ;
		
			if((CheckHitKey(KEY_INPUT_SPACE) != 0) && ( PlayerX < NotesX[j] ) && ( PlayerX + PlayerSizeX > NotesX[j] )){

				//判定描画
				DrawLine( PlayerX , 0 , PlayerX , MAX_DISPLAY_SIZE_Y , GetColor( 255 , 255 , 255 ) ) ;
				DrawLine( PlayerX+PlayerSizeX , 0 , PlayerX+PlayerSizeX , MAX_DISPLAY_SIZE_Y , GetColor( 255 , 255 , 255 ) ) ;

				//プレイヤー描画
				//DrawBox( PlayerX , PlayerY , PlayerX + PlayerSizeX , PlayerY + PlayerSizeY, GetColor( 0 , 0 , 255 ) , TRUE ) ;
			}else if(CheckHitKey(KEY_INPUT_SPACE) != 0 && ( PlayerX > NotesX[j]+NotesSizeX+50 ) && ( PlayerX + PlayerSizeX < NotesX[j] +50)){
				//DrawBox( PlayerX , PlayerY , PlayerX + PlayerSizeX , PlayerY + PlayerSizeY, GetColor( 255 , 0 , 0 ) , TRUE ) ;
			}
		}else{
			//DrawBox( PlayerX , PlayerY , PlayerX + PlayerSizeX , PlayerY + PlayerSizeY, GetColor( 255 , 255 , 255 ) , TRUE ) ;
		}
	}

	/*for(int j = 0 ; j < MAX_NOTES ; j ++ ){
		if(PlayerX+PlayerSizeX == Hit[j]){
			Hit[j]
			DrawLine( PlayerX+PlayerSizeX , 0 , PlayerX+PlayerSizeX , MAX_DISPLAY_SIZE_Y , GetColor( 0 , 0 , 255 ) ) ;
		}*/


	//右判定　テスト用
	DrawLine( PlayerX+PlayerSizeX , 0 , PlayerX+PlayerSizeX , MAX_DISPLAY_SIZE_Y , GetColor( 255 , 0 , 0 ) ) ;

	//境界線
	DrawLine( 0 , 340 , MAX_DISPLAY_SIZE_X , 340 , GetColor( 255 , 255 , 255 ) ) ;


	

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

void Game(){
	
	static boolean Flag=true;

	if(Flag){
		PlaySoundMem( SHandle , DX_PLAYTYPE_BACK ,FALSE) ;
		Flag=FALSE;
	}
	

	if( Key & PAD_INPUT_RIGHT ) PlayerX += 3 ;	// 右を押していたら右に進む
	if( Key & PAD_INPUT_LEFT ) PlayerX -= 3 ;	// 左を押していたら左に進む
	
	Move();
	Draw();//描画
}



int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){
	DWORD tick;

	ChangeWindowMode( TRUE ) ;//ウィンドウズモード
	SetGraphMode( MAX_DISPLAY_SIZE_X , MAX_DISPLAY_SIZE_Y , 16 ) ;//960*540
	if( DxLib_Init() == -1 )	// ＤＸライブラリ初期化処理
	{
		 return -1;				// エラーが起きたら直ちに終了
	}

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
		
		switch(Frame){
			case 0:
				DrawGraph( 0 ,0, GTitleHandle, TRUE);
				if( CheckHitKey( KEY_INPUT_RETURN ) != 0) {
					Ini();//初期化
					SetNotes();//譜面セット
					Frame=1;//ゲーム画面に遷移
				}
				break;
			case 1:
				Game();
				GameCnt++;
				break;
			case 2:
				DrawFormatString(MAX_DISPLAY_SIZE_X/2-50,MAX_DISPLAY_SIZE_Y/2,GetColor( 255 , 255 , 255 ),"リザルト画面"); 
				break;
			case 3:
				DrawFormatString(MAX_DISPLAY_SIZE_X/2-50,MAX_DISPLAY_SIZE_Y/2,GetColor( 255 , 255 , 255 ),"ゲームオーバー画面"); 
				break;
		}
		
		ScreenFlip() ;// 裏画面の内容を表画面に反映させる

		////BGMとの同期
		//WaitTime = tick + (1000.0f/FPS*cnt) - GetTickCount();
		//if(WaitTime > 0){
		//	Sleep(WaitTime);
		//}
	}

	DxLib_End() ;				// ＤＸライブラリ使用の終了処理

	return 0 ;					// ソフトの終了
}