#include "GameClear.h"
#include"Title/Title.h"

//�R���X�g���N�^
GameClear::GameClear()
	:Task(ETaskTag::eResult, true)
	, m_select(0)
	, m_High(0.0f)
{
	//�t�F�[�h�C�����s
	GameData::StartFadeIn = true;
	GameData::GameStart = false;

	//�N���A�T�E���h�Đ�
	SOUND("SE_Clearfanfare")->Play();

	m_BuckGraund = COPY_RESOURCE("ClearBackGround", CImage);
	m_TitleText = COPY_RESOURCE("TitleText", CImage);
}

//�f�X�g���N�^
GameClear::~GameClear()
{
	switch (m_select)
	{
		//�^�C�g��
	case 0:
		new Title();
		break;
	}
}

//�n�C���C�g�\������
void GameClear::HighLight()
{
	m_High = m_High + 0.05f;
	m_TitleText.SetPos(674.0f, 540.0f);
	m_TitleText.SetSize(512.0f + 40.0f * cos(m_High), 128.0f + 10.0f * cos(m_High));
	m_TitleText.Draw();
}

//�X�V����
void GameClear::Update()
{
	//�t�F�[�h�C���E�t�F�[�h�A�E�g���͎��s���Ȃ�
	if (GameData::StartFadeIn) return;
	if (GameData::StartFadeOut) return;

	//�X�y�[�X�L�[�Ō���
	if (PUSH(CInput::eButton1))
	{
		SOUND("SE_Click")->Volume(GameData::volume);
		SOUND("SE_Click")->Play();
		//�t�F�[�h�A�E�g���s
		GameData::StartFadeOut = true;
	}
}

//2D�`�揈��
void GameClear::Draw()
{
	//�w�i�`��
	m_BuckGraund.Draw();
	//�����\���`��
	//HighLight();
}
