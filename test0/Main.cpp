#include "DxLib.h"
#include <stdlib.h>

int Key = -1 ;

// �m�[�c�̍ő吔
#define MAX_NOTES 100

//��ʃT�C�Y
#define MAX_DISPLAY_SIZE_X 960
#define MAX_DISPLAY_SIZE_Y 540


int Notes[ MAX_NOTES ] ;	// �m�[�c�����݂��邩�A�t���O
double NotesX[ MAX_NOTES ] ,NotesY[ MAX_NOTES ] ;	// �m�[�c�̈ʒu
double Hit[ MAX_NOTES ];//����Ώۂ̃m�[�c

int PlayerX , PlayerY ;	// �v���C���[�̈ʒu
int PlayerSizeX,PlayerSizeY;
int NotesSizeX,NotesSizeY;

static int mStartTime;      //����J�n����
static int mCount;          //�J�E���^
static float mFps;          //fps
static const int N = 60;	//���ς����T���v����
static const int FPS = 60;	//�ݒ肵��FPS
int GameHp = 50;
float FpsDelayCnt;

int cnt = 0;//�N��������̃J�E���g

int Frame=0;//��ʑJ��

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

int GameCnt;//�Q�[���J�E���g

int MusicTime=30;//�e�X�g�p�@

int NotesPattern[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};//�e�X�g�p
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
int selectStageNumber;
int selectStageDif;
int selectMode;
int selectModeY[3] = {145, 260, 365};
int selectChangeCount = 0;
int stageBackH[2];
int selectModeH[3];
int selectCursorH;

bool Update(){
	if( mCount == 0 ){ //1�t���[���ڂȂ玞�����L��
		mStartTime = GetNowCount();
	}
	if( mCount == N ){ //60�t���[���ڂȂ畽�ς��v�Z����
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
	int tookTime = GetNowCount() - mStartTime;	//������������
	int waitTime = mCount*1000/FPS - tookTime;	//�҂ׂ�����
	if( waitTime > 0 ){
		Sleep(waitTime);	//�ҋ@
	}
}

void Load(){
	SHandle = LoadSoundMem("10MIN_120BPM_44100_16bit.wav");

	GHandle[0] = LoadGraph("Image/PlayerWait1.png");//�ҋ@1
	GHandle[1] = LoadGraph("Image/PlayerWait2.png");//�ҋ@2

	GHandle[2] = LoadGraph("Image/ActionUp_Just.png");//��just
	GHandle[3] = LoadGraph("Image/ActionUp_Near.png");//��near
	GHandle[4] = LoadGraph("Image/ActionRight_Just.png");//�Ejust
	GHandle[5] = LoadGraph("Image/ActionRight_Near.png");//�Enear
	GHandle[6] = LoadGraph("Image/ActionDown_Just.png");//��just
	GHandle[7] = LoadGraph("Image/ActionDown_Near.png");//��near
	GHandle[8] = LoadGraph("Image/ActionLeft_Just.png");//��just
	GHandle[9] = LoadGraph("Image/ActionLeft_Near.png");//��near
	GHandle[10] = LoadGraph("Image/miss.png");//miss

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

	CurSor[0] = LoadGraph("Image/c_left.png");
	CurSor[1] = LoadGraph("Image/c_right.png");
	CurSor[2] = LoadGraph("Image/c_down.png");
	CurSor[3] = LoadGraph("Image/c_up.png");

	stageBackH[0] = LoadGraph("Image/BackImage_Stage1.png");
	stageBackH[1] = LoadGraph("Image/BackImage_Stage2.png");
	selectCursorH = LoadGraph("Image/cursor.png");

	selectModeH[0] = LoadGraph("Image/stage1.png");
	selectModeH[1] = LoadGraph("Image/stage2.png");
	selectModeH[2] = LoadGraph("Image/stage0.png");


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
	// �v���C���[�̏����ʒu���Z�b�g
	PlayerX = 100 ;
	PlayerY = 75 ;

	PlayerSizeX = 200;
	PlayerSizeY = 250;

	// �m�[�c�̑��݂�����������
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
	//�m�[�c����
	for(int k=0;k<sizeof NotesPattern/sizeof(int);k++){

		if(NotesPattern[k]==1){

			// �g���Ă��Ȃ��m�[�c�f�[�^��T��
			for( n = 0 ; n < MAX_NOTES ; n ++ ){
				if( Notes[ n ] == 0 ) break ;
			}
			// �����g���Ă��Ȃ��m�[�c�f�[�^����������m�[�c���o��
			if( n != MAX_NOTES ){
				// �V���b�g�̈ʒu��ݒ�
				NotesX[ n ] = MAX_DISPLAY_SIZE_X-60 + k*(10*FPS)+NotesSizeX ;
				NotesY[ n ] = 400 ;

				// �m�[�c�f�[�^���g�p���ɃZ�b�g
				Notes[ n ] = 1 ;

				if(FirstFlag){
					Hit[ n ] = 2;//����m�[�c
					FirstFlag = FALSE;
				}else{
					Hit[ n ] = 1;
				}
			}
		}	
	}
}

void Move(){
	// �m�[�c�̈ړ�����
	int j;
	for( j = 0 ; j < MAX_NOTES ; j ++ ){
		// �m�[�c�f�[�^��������������X�L�b�v
		if( Notes[ j ] == 0 ) continue ;

		// ���Ɉړ�
		NotesX[ j ] -= 10 ;

		// ��ʊO�ɏo�Ă����疳���ɂ���
		if( NotesX[ j ] < -50 ) Notes[ j ] = 0 ;
	}
}

void Draw(){
	// �w�i�w�i
	DrawGraph(0, 0, stageBackH[selectStageDif], TRUE);
	DrawGraph(0, 0, StageBottom, TRUE);
	//�v���C���[�`��
	
	DrawGraph(PlayerX, PlayerY, GHandle[GDrawFlag], TRUE);

	//fps��\��
	DrawFormatString(MAX_DISPLAY_SIZE_X-80,MAX_DISPLAY_SIZE_Y-20,GetColor( 255 , 255 , 255 ),"FPS %.1f",mFps); 
	if(mFps<60 ){
		FpsDelayCnt-=(60.0f-mFps) / 60.0f;
	}else if(mFps>60){
		FpsDelayCnt-=(60.0f-mFps) / 60.0f;
	}
	DrawFormatString(MAX_DISPLAY_SIZE_X-180,MAX_DISPLAY_SIZE_Y-60,GetColor( 255 , 255 , 255 ),"FPSD %f",FpsDelayCnt); 

	// �m�[�c��`�悷��
	for(int j = 0 ; j < MAX_NOTES ; j ++ ){
		// �m�[�c�f�[�^���L���Ȏ��̂ݕ`��
		if( Notes[j] == 1 ) {
			DrawGraph( NotesX[j] + FpsDelayCnt*10 , NotesY[j] , CurSor[j % 4] , TRUE);
		
			if((CheckHitKey(KEY_INPUT_SPACE) != 0) && ( PlayerX < NotesX[j] ) && ( PlayerX + PlayerSizeX > NotesX[j] )){


			}else if(CheckHitKey(KEY_INPUT_SPACE) != 0 && ( PlayerX > NotesX[j]+NotesSizeX+50 ) && ( PlayerX + PlayerSizeX < NotesX[j] +50)){
			}
		}else{
		}
	}


	//�E����@�e�X�g�p
	DrawLine( PlayerX+PlayerSizeX , 0 , PlayerX+PlayerSizeX , MAX_DISPLAY_SIZE_Y , GetColor( 255 , 0 , 0 ) ) ;

	//�o�ߎ��Ԃ�\��
	DrawFormatString(MAX_DISPLAY_SIZE_X-50,0,GetColor( 255 , 255 , 255 ),"%d �b",GameCnt/60); 

	//�Ȃ̌��݈ʒu
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
    int Pad = GetJoypadInputState( DX_INPUT_KEY_PAD1 ) ;        //���͏�Ԃ�Pad�Ɋi�[
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

void StageSelectKey() {
	int Pad = GetJoypadInputState( DX_INPUT_KEY_PAD1 ) ;        //���͏�Ԃ�Pad�Ɋi�[

	if (selectChangeCount == 0 && Pad & PAD_INPUT_UP) { 
		selectMode--;
		if (selectMode < 0) {
			selectMode = 0;
		}

		selectChangeCount = 30;
	}
	if( Pad & PAD_INPUT_RIGHT){
	}
	if(selectChangeCount == 0 && Pad & PAD_INPUT_DOWN){
		selectMode++;
		if (selectMode > 2) {
			selectMode = 2;
		}
		selectChangeCount = 30;

	}
	
	if( Pad & PAD_INPUT_LEFT){
	}

	selectChangeCount--;
	if (selectChangeCount < 0) {
		selectChangeCount = 0;
	}


	if ( Pad & PAD_INPUT_4) {
		switch(selectMode) {
			case 0:
				Frame = 2;
				selectStageDif = 0;
				break;
			case 1:
				selectStageDif = 1;
				Frame =2;
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
}


void CharMove(){
	int Pad = GetJoypadInputState( DX_INPUT_KEY_PAD1 ) ;        //���͏�Ԃ�Pad�Ɋi�[
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
		PlayIni();//������
		SetNotes();//���ʃZ�b�g
		GameHp = 50;
		danceeVal = 0;
		danceScore = 0;
		Frame=3;//�Q�[����ʂɑJ��                    
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
	Draw();//�`��
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

	ChangeWindowMode( TRUE ) ;//�E�B���h�E�Y���[�h
	SetGraphMode( MAX_DISPLAY_SIZE_X , MAX_DISPLAY_SIZE_Y , 16 ) ;//960*540
	if( DxLib_Init() == -1 )	// �c�w���C�u��������������
	{
		 return -1;				// �G���[���N�����璼���ɏI��
	}

	TitleChangeFlg = 0;
	TitleChangeCount = 0;

	// �`����ʂ𗠉�ʂɃZ�b�g
	SetDrawScreen( DX_SCREEN_BACK ) ;

	//�Q�[���N�����̎��Ԃ��L�^
	tick = GetTickCount();

	Load();//�摜���ǂݍ���

	// ���[�v
	while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 ){

		Update();//�X�V

		// ��ʂ�����������
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
		
		ScreenFlip() ;// ����ʂ̓��e��\��ʂɔ��f������

		////BGM�Ƃ̓���
		//WaitTime = tick + (1000.0f/FPS*cnt) - GetTickCount();
		//if(WaitTime > 0){
		//	Sleep(WaitTime);
		//}
	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;					// �\�t�g�̏I��
}