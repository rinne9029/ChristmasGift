#include"Filta.h"
#include"Chara/Player/Player.h"

#define FADETIME 1	//�t�F�[�h�C���A�t�F�[�h�A�E�g��������

//�R���X�g���N�^
Filta::Filta()
	:Task(ETaskTag::eFilta,false)
	, m_alpha(1.0)
	, m_time(1)
{
	m_filta = COPY_RESOURCE("Filta", CImage);
}

//�t�F�[�h�C������
void Filta::Fadein()
{
	m_time -= CFPS::GetDeltaTime();
	//�t�F�[�h�C����
	if (m_alpha > 0)
	{
		m_alpha = m_time / FADETIME;
	}
	//�t�F�[�h�C������
	else
	{
		//�t�F�[�h�C���X�^�[�g�t���O���Z�b�g
		GameData::StartFadeIn = false;
	}
}

//�t�F�[�h�A�E�g����
void Filta::Fadeout()
{
	m_time += CFPS::GetDeltaTime();
	//�t�F�[�h�A�E�g��
	if (m_alpha < 1)
	{
		m_alpha = m_time / FADETIME;
	}
	//�t�F�[�h�A�E�g����
	else
	{
		//�t�F�[�h�A�E�g�X�^�[�g�t���O���Z�b�g
		GameData::StartFadeOut = false;
		//�t�B���^�[�ȊO�̑S�^�X�N�폜
		TaskManager::KillALL();
	}
}

//�X�V����
void Filta::Update()
{
	//�t�F�[�h�C���X�^�[�gOn
	if (GameData::StartFadeIn) Fadein();
	//�t�F�[�h�A�E�g�X�^�[�gOn
	if (GameData::StartFadeOut) Fadeout();
}

//2D�`�揈��
void Filta::Draw()
{
	//�t�B���^�[�`��
	m_filta.SetColor(1, 1, 1, m_alpha);
	m_filta.Draw();
}