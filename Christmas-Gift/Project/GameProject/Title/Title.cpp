#include"Title.h"
#include"Ranking/Ranking.h"
#include"Title/Snow.h"
#include"GameScene/GameScene.h"
#include"GameScene/GameData.h"

#define MAXSELECT 3			//���[�h�Z���N�g��
#define MAXSTAGE  3			//�X�e�[�W��
#define MAXMANUAL 2			//��������


//�R���X�g���N�^
Title::Title()
	:Task(ETaskTag::eScene,true)
	,m_select(1)
	,m_step(0)
	,m_fuwafuwa(0.0f)
{
	//�t�F�[�h�C�����s
	GameData::StartFadeIn = true;

	//�T�E���h�Đ�
	SOUND("BGM_TitleOP")->Volume(0.2);
	SOUND("BGM_TitleOP")->Play(true);

	m_BackGroundTitle = COPY_RESOURCE("BackGroundTitle", CImage);
	m_TitleText = COPY_RESOURCE("GameTitleText", CImage);
	m_StartText = COPY_RESOURCE("StartText", CImage);
	m_ManualText = COPY_RESOURCE("ManualText", CImage);
	m_RankingText = COPY_RESOURCE("GameRankingText", CImage);
	m_Manual1 = COPY_RESOURCE("Manual1", CImage);
	m_Manual2 = COPY_RESOURCE("Manual2", CImage);
	m_Stage1 = COPY_RESOURCE("Stage1", CImage);
	m_Stage2 = COPY_RESOURCE("Stage2", CImage);
	m_Stage3 = COPY_RESOURCE("Stage3", CImage);
}

//�f�X�g���N�^
Title::~Title()
{
	switch (m_step)
	{
	case 0:
		switch (m_select)
		{
		case 3:
			//�����L���O�N��
			new Ranking("TextData/RankingData.txt");
			break;
		}
		break;
	case 10:
		switch (m_select)
		{
		case 1:
			//�X�e�[�W1
			new GameScene(m_select);
			break;
		case 2:
			//�X�e�[�W2
			new GameScene(m_select);
			break;
		case 3:
			//�X�e�[�W3
			new GameScene(m_select);
			break;
		}
		break;
	}
}

//�ӂ�ӂ퓮������
void Title::FuwaFuwa()
{
	m_TitleText.SetPos(500, 300);
	m_TitleText.Draw();

	m_fuwafuwa += 0.02f;

	//�I�΂ꂽ������sin�J�[�u�łӂ�ӂ퓮��
	switch (m_select)
	{
		//�X�^�[�g����
	case 1:
		m_StartText.SetPos(200, 750 - abs(sin(m_fuwafuwa)) * 64);
		m_StartText.Draw();
		m_ManualText.SetPos(855, 750);
		m_ManualText.Draw();
		m_RankingText.SetPos(1270, 750);
		m_RankingText.Draw();
		break;
		//����������
	case 2:
		m_StartText.SetPos(200, 750);
		m_StartText.Draw();
		m_ManualText.SetPos(855, 750 - abs(sin(m_fuwafuwa)) * 64);
		m_ManualText.Draw();
		m_RankingText.SetPos(1270, 750);
		m_RankingText.Draw();
		break;
		//�����L���O����
	case 3:
		m_StartText.SetPos(200, 750);
		m_StartText.Draw();
		m_ManualText.SetPos(855, 750);
		m_ManualText.Draw();
		m_RankingText.SetPos(1270, 750 - abs(sin(m_fuwafuwa)) * 64);
		m_RankingText.Draw();
		break;
	}

	FONT_T()->Draw(100, 980, 1, 0, 0, "A���� �� �F D���� �� �F �X�y�[�X���� ����");
}

//���[�h�Z���N�g����
void Title::ModeChenge()
{
	//�X�y�[�X�{�^��
	if (PUSH(CInput::eButton5))
	{
		SOUND("SE_Click")->Volume(0.3);
		SOUND("SE_Click")->Play();
		switch (m_select)
		{
		case 1:
			m_step = m_select * 10;
			m_select = 1;
			break;
		case 2:
			m_step = m_select * 10;
			m_select = 1;
			break;
		case 3:
			SOUND("BGM_TitleOP")->Stop();
			GameData::StartFadeOut = true;
			break;
		}
	}

	//A�L�[����
	if (PUSH(CInput::eLeft) && m_select > 1)
	{
		SOUND("SE_Select")->Volume(0.3);
		SOUND("SE_Select")->Play();
		m_select--;
		//�ӂ�ӂ�\�����Z�b�g
		m_fuwafuwa = 0;
	}
	//D�L�[����
	if (PUSH(CInput::eRight) && m_select < MAXSELECT)
	{
		SOUND("SE_Select")->Volume(0.3);
		SOUND("SE_Select")->Play();
		m_select++;
		//�ӂ�ӂ�\�����Z�b�g
		m_fuwafuwa = 0;
	}
}

//�������[�h����
void Title::ManualMode()
{
	//S�L�[����
	if (PUSH(CInput::eDown))
	{
		SOUND("SE_Click")->Volume(0.3);
		SOUND("SE_Click")->Play();
		//���[�h�Z���N�g�ɖ߂�
		m_step = 0;
		m_select = 1;
	}

	//A�L�[����
	//���̐���
	if (PUSH(CInput::eLeft) && m_select > 1)
	{
		SOUND("SE_Select")->Volume(0.3);
		SOUND("SE_Select")->Play();
		m_select--;
	}
	//D�L�[����
	//�O�̐����ɖ߂�
	if (PUSH(CInput::eRight) && m_select < MAXMANUAL)
	{
		SOUND("SE_Select")->Volume(0.3);
		SOUND("SE_Select")->Play();
		m_select++;
	}
}

void Title::ManualDraw()
{
	m_Manual1.SetPos(320 - 1920 * (m_select - 1), 180);
	m_Manual1.Draw();
	m_Manual2.SetPos(2240 - 1920 * (m_select - 1), 180);
	m_Manual2.Draw();
	FONT_T()->Draw(100, 980, 1, 0, 0, "A���� �� �F D���� �� �F S���� �߂�");
	FONT_T()->Draw(1000, 880, 1, 0, 0, "%d/2", m_select);
}

void Title::StageSelecte()
{
	//�X�y�[�X�{�^��
	if (PUSH(CInput::eButton5))
	{
		//����X�e�[�W�P�܂ł����V�ׂȂ�
		if (m_select < 2)
		{
			SOUND("SE_Click")->Volume(0.3);
			SOUND("SE_Click")->Play();
			SOUND("BGM_TitleOP")->Stop();
			GameData::StartFadeOut = true;
		}
	}

	//S�L�[����
	if (PUSH(CInput::eDown))
	{
		SOUND("SE_Click")->Volume(0.3);
		SOUND("SE_Click")->Play();
		//���[�h�Z���N�g�ɖ߂�
		m_step = 0;
		m_select = 1;
	}

	//A�L�[����
	//���̃X�e�[�W
	if (PUSH(CInput::eLeft) && m_select > 1)
	{
		SOUND("SE_Select")->Volume(0.3);
		SOUND("SE_Select")->Play();
		m_select--;
	}
	//D�L�[����
	//�O�̃X�e�[�W
	if (PUSH(CInput::eRight) && m_select < MAXSTAGE)
	{
		SOUND("SE_Select")->Volume(0.3);
		SOUND("SE_Select")->Play();
		m_select++;
	}
}

void Title::StageDraw()
{
	m_Stage1.SetPos(320 - 1920 * (m_select - 1), 0);
	m_Stage1.Draw();
	m_Stage2.SetPos(2240 - 1920 * (m_select - 1), 0);
	m_Stage2.Draw();
	m_Stage3.SetPos(4160 - 1920 * (m_select - 1), 0);
	m_Stage3.Draw();
	FONT_T()->Draw(100, 910, 1, 0, 0, "SPACE�L�[�ŃX�^�[�g");
	FONT_T()->Draw(100, 980, 1, 0, 0, "A���� �� �F D���� �� �F S���� �߂�");
}

//�X�V����
void Title::Update()
{
	if (GameData::StartFadeIn) return;
	if (GameData::StartFadeOut) return;

	switch (m_step)
	{
	case 0:
		ModeChenge();
		break;
	case 10:
		StageSelecte();
		break;
	case 20:
		ManualMode();
		break;
	}
}

//2D�`�揈��
void Title::Draw()
{
	m_BackGroundTitle.Draw();

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

	//�ӂ�ӂ�\���`��
	switch (m_step)
	{
	case 0:
		FuwaFuwa();
		break;
	case 10:
		StageDraw();
		break;
	case 20:
		ManualDraw();
		break;
	}
	
}