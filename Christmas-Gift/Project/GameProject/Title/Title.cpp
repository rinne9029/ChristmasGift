#include"Title.h"
#include"Ranking/Ranking.h"
#include"Title/Snow.h"
#include"GameScene/GameScene.h"

#define MAX_SELECT 3			//���[�h�Z���N�g��
#define MAX_STAGE  2			//�X�e�[�W��
#define MAX_MANUAL 2			//��������
#define CHANGE_IMAGE_TIME 0.5f	//�X�e�[�W�摜�����ɗ�����܂ł̎���


//�R���X�g���N�^
Title::Title()
	:Task(ETaskTag::eScene,true)
	,m_state(eState_Title)
	,m_select(1)
	,m_alpha(1.0f)
	,m_elapsedtimeY(0.0f)
	,m_volume(3)
{
	//�t�F�[�h�C�����s
	GameData::StartFadeIn = true;

	//�T�E���h�Đ�
	SOUND("BGM_TitleOP")->Volume(GameData::volume);
	SOUND("BGM_TitleOP")->Play(true);
	

	//�摜�ǂݍ���
	m_backgroundtitleimage = COPY_RESOURCE("BackGroundTitle", CImage);
	m_titlerogoimage = COPY_RESOURCE("GameTitleRogo", CImage);
	m_stageimage1 = COPY_RESOURCE("Stage1", CImage);
	m_stageimage2 = COPY_RESOURCE("Stage2", CImage);
	m_collectionimage1 = COPY_RESOURCE("Stage2", CImage);
}

//�f�X�g���N�^
Title::~Title()
{
	if (m_state == eState_Stage)
	{
		new GameScene(m_select);
	}
}

//�^�C�g�����̍X�V����
void Title::TitleUpdate()
{
	//�����x
	static float a;
	a+=1.0f * 2.0f * CFPS::GetDeltaTime();
	m_alpha = 1.0f - 0.4f * abs(sin(a));

	//�X�y�[�X�{�^��
	if (PUSH(CInput::eButton1))
	{
		SOUND("SE_Click")->Volume(GameData::volume);	//���ʕύX
		SOUND("SE_Click")->Play();			//�Đ�(���[�v����)

		switch (m_select)
		{
		case 1:
			//�X�e�[�W�Z���N�g��ʂɈړ�
			m_state = eState_Stage;
			break;
		case 2:
			//���j���[��ʂɈړ�
			m_state = eState_Menu;
			break;
		case 3:
			//�R���N�V������ʂɈړ�
			m_state = eState_Collection;
			break;
		}
	}

	//A�L�[����
	if (PUSH(CInput::eLeft) && m_select > 1)
	{
		SOUND("SE_Select")->Volume(GameData::volume);	//���ʐݒ�
		SOUND("SE_Select")->Play();			//�Đ�
		m_select--;
	}
	//D�L�[����
	if (PUSH(CInput::eRight) && m_select < MAX_SELECT)
	{
		SOUND("SE_Select")->Volume(GameData::volume);	//���ʐݒ�
		SOUND("SE_Select")->Play();			//�Đ�
		m_select++;
	}

}

//�^�C�g�����̕`�揈��
void Title::TitleDraw()
{
	//���S�`�揈��
	m_titlerogoimage.SetPos(610.0f, 50.0f);
	m_titlerogoimage.SetSize(700.0f, 700.0f);
	m_titlerogoimage.Draw();

	//�I�𒆂̕����ɔw�i�J���[������
	Utility::DrawQuad(CVector2D(250.0f + 540 * (m_select - 1), 760.0f), CVector2D(380.0f, 170.0f), CVector4D(1.0f, 0.3f, 0.0f, m_alpha));

	//�����`�揈��
	CREATE_FONT_F
	(
		"name",
		"C:\\Windows\\Fonts\\BOD_CI.TTF",
		128
	)->Draw(350, 880, 0.0f, 0.0f, 0.0f, "Start");

	CREATE_FONT_F
	(
		"name",
		"C:\\Windows\\Fonts\\BOD_CI.TTF",
		128
	)->Draw(870, 880, 0.0f, 0.0f, 0.0f, "Menu");

	CREATE_FONT_F
	(
		"name",
		"C:\\Windows\\Fonts\\BOD_CI.TTF",
		128
	)->Draw(1350, 880, 0.0f, 0.0f, 0.0f, "Collection");
}

void Title::StageSelectUpdate()
{
	//�X�y�[�X�{�^��
	if (PUSH(CInput::eButton1))
	{
		SOUND("SE_Click")->Volume(GameData::volume);
		SOUND("SE_Click")->Play();
		SOUND("BGM_TitleOP")->Stop();
		GameData::StartFadeOut = true;
	}

	//S�L�[����
	if (PUSH(CInput::eDown))
	{
		SOUND("SE_Click")->Volume(GameData::volume);
		SOUND("SE_Click")->Play();
		//�^�C�g����ʃZ���N�g�ɖ߂�
		m_state = eState_Title;
		m_select = 1;
		//�o�ߎ��ԃ��Z�b�g
		m_elapsedtimeY = 0.0f;
		m_imageposY = 0.0f;
	}

	//A�L�[����
	//���̃X�e�[�W
	if (PUSH(CInput::eLeft) && m_select > 1)
	{
		SOUND("SE_Select")->Volume(GameData::volume);
		SOUND("SE_Select")->Play();
		m_select--;
	}
	//D�L�[����
	//�O�̃X�e�[�W
	if (PUSH(CInput::eRight) && m_select < MAX_STAGE)
	{
		SOUND("SE_Select")->Volume(GameData::volume);
		SOUND("SE_Select")->Play();
		m_select++;
	}
}

void Title::StageSelectDraw()
{	
	//�o�ߎ���
	if (m_elapsedtimeY < CHANGE_IMAGE_TIME)
	{
		m_elapsedtimeY += CFPS::GetDeltaTime();
	}
	else
	{
		m_elapsedtimeY == CHANGE_IMAGE_TIME;
	}
	//��ʏo����X�e�[�W1�摜���ォ�牺�Ɉړ�������
	m_imageposY = 1080 * (m_elapsedtimeY / CHANGE_IMAGE_TIME);

	//�X�e�[�W�摜1
	m_stageimage1.SetSize(CVector2D(1600, 900));
	m_stageimage1.SetPos(160.0f - 1920 * (m_select - 1), 90.0f - 1080.0f + m_imageposY);
	m_stageimage1.Draw();

	//�X�e�[�W�摜2
	m_stageimage2.SetSize(CVector2D(1600, 900));
	m_stageimage2.SetPos(2080.0f - 1920 * (m_select - 1), 90.0f - 1080.0f + m_imageposY);
	m_stageimage2.Draw();

}

void Title::TitleMenuUpdate()
{
	//S�L�[����
	if (PUSH(CInput::eDown))
	{
		SOUND("SE_Click")->Volume(GameData::volume);
		SOUND("SE_Click")->Play();
		SOUND("BGM_TitleOP")->Stop();
		//�^�C�g���Z���N�g�ɖ߂�
		m_state = eState_Title;
		m_select = 1;
		SOUND("BGM_TitleOP")->Volume(GameData::volume);
		SOUND("BGM_TitleOP")->Play(true);
	}

	//A�L�[����
	//���ʂ�������
	if (PUSH(CInput::eLeft) && m_volume > 0)
	{
		m_volume--;
		GameData::volume = m_volume * 0.2f;
		SOUND("SE_Select")->Volume(GameData::volume);
		SOUND("SE_Select")->Play();
	}
	//D�L�[����
	//���ʂ��グ��
	if (PUSH(CInput::eRight) && m_volume < 5)
	{
		m_volume++;
		GameData::volume = m_volume * 0.2f;
		SOUND("SE_Select")->Volume(GameData::volume);
		SOUND("SE_Select")->Play();
	}
}

void Title::TitleMenuDraw()
{
	//�I�𒆂̕����ɔw�i�J���[������
	Utility::DrawQuad(CVector2D(0.0f, 0.0f), CVector2D(1920.0f, 1080.0f), CVector4D(1.0f, 1.0f, 1.0f, 0.4f));

	CREATE_FONT_F
	(
		"name",
		"C:\\Windows\\Fonts\\BOD_CI.TTF",
		128
	)->Draw(350, 540, 0.0f, 0.0f, 0.0f, "Sound");

}

void Title::CollectionUpdate()
{
}

void Title::CollectionDraw()
{
	//�R���N�V�������擾
	m_collectionimage1.SetColor(0, 0, 0, 1);
	m_collectionimage2.SetColor(0, 0, 0, 1);
	m_collectionimage3.SetColor(0, 0, 0, 1);
	//�R���N�V�����擾
	m_collectionimage1.SetColor(1, 1, 1, 1);
	m_collectionimage2.SetColor(1, 1, 1, 1);
	m_collectionimage3.SetColor(1, 1, 1, 1);
	//�`�揈��
	m_collectionimage1.Draw();
	m_collectionimage2.Draw();
	m_collectionimage3.Draw();
}

//�X�V����
void Title::Update()
{
	if (GameData::StartFadeIn) return;
	if (GameData::StartFadeOut) return;

	switch (m_state)
	{
	case eState_Title:
		TitleUpdate();
		break;
	case eState_Stage:
		StageSelectUpdate();
		break;
	case eState_Menu:
		TitleMenuUpdate();
		break;
	case eState_Collection:
		CollectionUpdate();
		break;
	}
}

//2D�`�揈��
void Title::Draw()
{
	m_backgroundtitleimage.Draw();


	//���`�悷�鎞��
	static int RespawnTime = 0;
	RespawnTime++;
	//�P�b�x�ɐV�������`�悷��
	if (RespawnTime > 60)
	{
		new Snow();
		new Snow();
		//�`��N�[���^�C�������Z�b�g
		RespawnTime = 0;
	}

	switch (m_state)
	{
	case eState_Title:
		TitleDraw();
		break;
	case eState_Stage:
		StageSelectDraw();
		break;
	case eState_Menu:
		TitleMenuDraw();
		break;
	case eState_Collection:
		CollectionDraw();
		break;
	}
	
}