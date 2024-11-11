#include "GameOver.h"
#include"Title/Title.h"
#include"GameScene/GameScene.h"

//�R���X�g���N�^
GameOver::GameOver(int stage)
	:Task(ETaskTag::eResult,true)
{
	GameData::StartFadeIn = true;
	GameData::GameStart = false;

	//���݂̃X�e�[�W
	m_stage = stage;

	//�Q�[���I�[�o�[�T�E���h�Đ�
	SOUND("SE_GameOver")->Play();

	m_BuckGraund = COPY_RESOURCE("OverBackGround", CImage);
	m_TitleText = COPY_RESOURCE("TitleText", CImage);
	m_ReStartText = COPY_RESOURCE("ReStartText", CImage);
}

//�f�X�g���N�^
GameOver::~GameOver()
{
	switch (m_select)
	{
		//�^�C�g��
	case 0:
		new Title();
		break;
		//���g���C
	case 1:
		new GameScene(m_stage);
		break;
	}
}

//�����\������
void GameOver::HighLight()
{
	m_High = m_High + 0.1f;
	switch (m_select)
	{
		//�^�C�g����������
	case 0:
		m_TitleText.SetPos(674.0f, 540.0f);
		m_TitleText.SetSize(512.0f + 40.0f * cos(m_High), 128.0f + 10.0f * cos(m_High));
		m_TitleText.Draw();
		m_ReStartText.SetPos(674.0f, 720.0f);
		m_ReStartText.Draw();
		break;
		//���g���C��������
	case 1:
		m_TitleText.SetPos(674.0f, 540.0f);
		m_TitleText.Draw();
		m_ReStartText.SetPos(674.0f, 720.0f);
		m_ReStartText.SetSize(512.0f + 40.0f * cos(m_High), 128.0f + 10.0f * cos(m_High));
		m_ReStartText.Draw();
	}
}

//�X�V����
void GameOver::Update()
{
	//�t�F�[�h�C���E�t�F�[�h�A�E�g���͎��s���Ȃ�
	if (GameData::StartFadeIn) return;
	if (GameData::StartFadeOut) return;

	//�X�y�[�X�L�[�Ō���
	if (PUSH(CInput::eButton1))
	{
		SOUND("SE_Click")->Volume(GameData::volume);
		SOUND("SE_Click")->Play();
		GameData::StartFadeOut = true;
	}

	//W�L�[����
	if (PUSH(CInput::eUp) && m_select > 0)
	{
		SOUND("SE_Select")->Volume(GameData::volume);
		SOUND("SE_Select")->Play();
		m_select--;
		m_High = 0.0f;	//�������Z�b�g
	}
	//S�L�[����
	if (PUSH(CInput::eDown) && m_select < 1)
	{
		SOUND("SE_Select")->Volume(GameData::volume);
		SOUND("SE_Select")->Play();
		m_select++;
		m_High = 0.0f;	//�������Z�b�g
	}
}

//�`�揈��
void GameOver::Draw()
{
	//�w�i�`��
	m_BuckGraund.Draw();
	//�����\���`��
	HighLight();
}
