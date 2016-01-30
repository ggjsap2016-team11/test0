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

float FpsDelayCnt;

int cnt = 0;//�N��������̃J�E���g

int Frame=0;//��ʑJ��

int SHandle;
int GHandle[2];
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
	GHandle[0] = LoadGraph("�ҋ@1.png");
	GHandle[1] = LoadGraph("�ҋ@2.png");
	BackHandle = LoadGraph("�X�e�[�W�w�i1.png");
	GTitleHandle = LoadGraph("gametitle.png");
	GTitleBgHandle = LoadGraph("gametitle_bg.png");
	GTitleStartHandle = LoadGraph("gametitle_start_default.png");
	GTitleStartPushHandle = LoadGraph("gametitle_start_push.png");
	StageSelectHanele = LoadGraph("stageSelect_base.png");
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
	int FirstFlag = TRUE;
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
	DrawGraph(0, 0, BackHandle, TRUE);
	//�v���C���[�`��
	if (cnt % 60 ==0) {
		GDrawFlag = 0;
	} else if(cnt % 30 == 0) {
		GDrawFlag = 1;
	} else {

	}
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
			DrawBox( NotesX[j] + FpsDelayCnt*10 , NotesY[j] ,NotesX[j] + NotesSizeX+FpsDelayCnt*10 , NotesY[j] + NotesSizeY , GetColor( 255 , 255 , 255 ) , TRUE ) ;
		
			if((CheckHitKey(KEY_INPUT_SPACE) != 0) && ( PlayerX < NotesX[j] ) && ( PlayerX + PlayerSizeX > NotesX[j] )){

				//����`��
				DrawLine( PlayerX , 0 , PlayerX , MAX_DISPLAY_SIZE_Y , GetColor( 255 , 255 , 255 ) ) ;
				DrawLine( PlayerX+PlayerSizeX , 0 , PlayerX+PlayerSizeX , MAX_DISPLAY_SIZE_Y , GetColor( 255 , 255 , 255 ) ) ;

				//�v���C���[�`��
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


	//�E����@�e�X�g�p
	DrawLine( PlayerX+PlayerSizeX , 0 , PlayerX+PlayerSizeX , MAX_DISPLAY_SIZE_Y , GetColor( 255 , 0 , 0 ) ) ;

	//���E��
	DrawLine( 0 , 340 , MAX_DISPLAY_SIZE_X , 340 , GetColor( 255 , 255 , 255 ) ) ;


	

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

void StageSelectUpdate() {
    int Pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
    if( Pad & PAD_INPUT_4 ){
		PlayIni();//������
		SetNotes();//���ʃZ�b�g
		Frame=2;//�Q�[����ʂɑJ��                    
    }
}

void StageSelectDraw() {
	DrawGraph(0, 0, StageSelectHanele, TRUE);
}




void Game(){
	
	static boolean Flag=true;

	if(Flag){
		PlaySoundMem( SHandle , DX_PLAYTYPE_BACK ,FALSE) ;
		Flag=FALSE;
	}
	

	if( Key & PAD_INPUT_RIGHT ) PlayerX += 3 ;	// �E�������Ă�����E�ɐi��
	if( Key & PAD_INPUT_LEFT ) PlayerX -= 3 ;	// ���������Ă����獶�ɐi��
	
	Move();
	Draw();//�`��
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
				Game();
				GameCnt++;
				break;
			case 3:
				DrawFormatString(MAX_DISPLAY_SIZE_X/2-50,MAX_DISPLAY_SIZE_Y/2,GetColor( 255 , 255 , 255 ),"���U���g���"); 
				break;
			case 4:
				DrawFormatString(MAX_DISPLAY_SIZE_X/2-50,MAX_DISPLAY_SIZE_Y/2,GetColor( 255 , 255 , 255 ),"�Q�[���I�[�o�[���"); 
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