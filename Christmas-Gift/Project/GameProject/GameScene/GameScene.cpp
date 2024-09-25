#include"GameScene.h"
#include"Chara/Player/Player.h"
#include"Chara/Enemy/Enemy.h"
#include"Camera/Camera.h"
#include"Field/Field.h"
#include"Field/FieldObject/Closet.h"
#include"Field/FieldObject/Telephone.h"
#include"CollisionHitBox/MidPoint.h"
#include"UI/SleepLife.h"
#include"UI/Timer.h"
#include"UI/Pause.h"
#include"Title/Title.h"
#include"UI/TargetText.h"
#include"UI/DangerFilta.h"
#include"Effect/Destination.h"
#include"Result/GameClear.h"
#include"Result/GameOver.h"

#define DANGER_TIME 240.0f

//�R���X�g���N�^
GameScene::GameScene(int stage)
	:Task(ETaskTag::eScene,true)
	,m_dangerfilta(NULL)
{
	//�Q�[���󋵂̃��Z�b�g
	GameData::isGift = false;
	GameData::FacePosition = FACE_POSITION;
	GameData::GameClearCheck = false;
	GameData::GameOverCheck = false;
	GameData::second = GAME_TIME;
	GameData::GameStart = true;

	m_stage = stage;

	//�t�F�[�h�C�����s
	GameData::StartFadeIn = true;

	//�X�e�[�W����
	switch (stage)
	{
	//�X�e�[�W�P����
	case 1:
	{
		//�e�L�X�g�f�[�^�ɂ���ăX�e�[�W���I�u�W�F�N�g���ω�
		new Field
		(
			"TextData/StageNode1.txt",		//�m�[�h
			"TextData/LightData1.txt",		//���C�g
			"TextData/DoorData1.txt",		//�h�A
			"TextData/SwitchData1.txt",		//�X�C�b�`
			"TextData/ClosetData1.txt"		//�N���[�[�b�g
		);

		//�v���C���[����
		new Player
		(
			CVector3D(4.55f, -0.2f, 6.18f),
			CVector3D(0.0f, DtoR(180), 0.0f),
			CVector3D(0.01f, 0.01f, 0.01f)
		);

		//�G�t�F�N�g����
		/*new Destination
		(
			COPY_RESOURCE("Star", CModelObj),
			CVector3D(4.55, 1, 6.18),
			CVector3D(0.1, 0.1, 0.1)
		);*/

		//�G�F���e����
		/*new Enemy
		(
			CVector3D(2.9, 0, 0.351914),
			CVector3D(-1, 0, 0),
			CVector3D(0.01, 0.01, 0.01),
			0
		);*/

		//�v���[���g�ݒu�|�C���g
		new MidPoint
		(
			CVector3D(5.998150f, 9.0f, -1.6f),
			CVector3D(0.0f, 0.0f, 0.0f),
			CVector3D(1.0f, 0.5f, 1.0f)
		);

		
		new SleepLife();			//�����Q�[�W
		new Timer();				//��������
		new TargetText();			//���݂̖ڕW

	}
	break;
	//�X�e�[�W�Q����
	case 2:
	{
		//�e�L�X�g�f�[�^�ɂ���ăX�e�[�W���I�u�W�F�N�g���ω�
		new Field
		(
			"TextData/StageNode1.txt",		//�m�[�h
			"TextData/LightData1.txt",		//���C�g
			"TextData/DoorData1.txt",		//�h�A
			"TextData/SwitchData1.txt",		//�X�C�b�`
			"TextData/ClosetData1.txt"		//�N���[�[�b�g
		);

		//�v���C���[����
		new Player
		(
			CVector3D(4.55f, -0.2f, 6.18f),
			CVector3D(0.0f, DtoR(180), 0.0f),
			CVector3D(0.01f, 0.01f, 0.01f)
		);

		//�v���[���g�ݒu�|�C���g
		new MidPoint
		(
			CVector3D(5.998150f, 9.0f, -1.6f),
			CVector3D(0.0f, 0.0f, 0.0f),
			CVector3D(1.0f, 0.5f, 1.0f)
		);

		new SleepLife();			//�����Q�[�W
		new Timer();				//��������
		new TargetText();			//���݂̖ڕW

		//�G�F���e����
		new Enemy
		(
			CVector3D(2.9, 0, 0.351914),
			CVector3D(-1, 0, 0),
			CVector3D(0.01, 0.01, 0.01),
			0
		);

		////�G�F��e����
		new Enemy
		(
			CVector3D(-2.073762, 9.891998, 1.103694),
			CVector3D(-1, 0, 0),
			CVector3D(0.01, 0.01, 0.01),
			1
		);
	}
	break;
	}
	new Pause();		//�|�[�Y���
}

//�f�X�g���N�^
GameScene::~GameScene()
{
	//�Q�[���N���A�t���OOn
	if (GameData::GameClearCheck)
	{
		SOUND("SE_Walk")->Stop();
		SOUND("SE_Run")->Stop();
		//�N���A�V�[���ڍs
		new GameClear();
	}
	//�Q�[���I�[�o�[�t���OOn
	else if (GameData::GameOverCheck)
	{
		SOUND("SE_Walk")->Stop();
		SOUND("SE_Run")->Stop();
		//�Q�[���I�[�o�[�V�[���ڍs
		new GameOver(m_stage);
	}
	//����ȊO
	else
	{
		SOUND("SE_Walk")->Stop();
		SOUND("SE_Run")->Stop();
		//�Q�[���I�[�o�[�V�[���ڍs
		new Title();
	}
}

//�X�V����
void GameScene::Update()
{
	if (GameData::second < DANGER_TIME && m_dangerfilta == NULL)
	{
		m_dangerfilta = new DangerFilta();		//�댯��Ԃ�`����t�B���^�[
	}
	//5���o�߂ŃQ�[���I�[�o�[
	if (GameData::second == 0.0f)
	{
		//�Q�[���I�[�o�[�t���OOn
		GameData::GameOverCheck = true;
		//�t�F�[�h�A�E�g���s
		GameData::StartFadeOut = true;
	}
}
