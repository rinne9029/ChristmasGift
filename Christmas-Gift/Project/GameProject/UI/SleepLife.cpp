#include"SleepLife.h"
#include"Chara/Player/Player.h"
#include"Filta/Filta.h"

bool SleepLife::m_REM = true;

//�R���X�g���N�^
SleepLife::SleepLife()
	:Task(ETaskTag::eUI, true)
	,m_ChengeFace(0)
{
	m_SleepMeter = COPY_RESOURCE("SleepMeter", CImage);
	m_FaceRelief = COPY_RESOURCE("Relief", CImage);
	m_FaceUsually = COPY_RESOURCE("Usually", CImage);
	m_FaceWorry = COPY_RESOURCE("Worry", CImage);
	m_REMText = COPY_RESOURCE("REMsleep", CImage);
	m_NREMText = COPY_RESOURCE("NREMsleep", CImage);

	//0�`20�b�����_��
	m_ChengeSleep = 300 - rand() % 20;
}

//�X�V����
void SleepLife::Update()
{	
	//�����l�̉�
	if(GameData::FacePosition <340)
	GameData::FacePosition += 0.05f;

	//�q���̐������o���i�Q�[���I�[�o�[�j
	if (GameData::FacePosition > 40) return;
	//�Q�[���I�[�o�[
	GameData::GameOverCheck = true;
	//�t�F�[�h�A�E�g���s
	GameData::StartFadeOut = true;
}

//2D�`�揈��
void SleepLife::Draw()
{
	//���₷�����邽�߂̔w�i
	Utility::DrawQuad(CVector2D(30, 60), CVector2D(380, 140), CVector4D(0.7, 0.7, 0.7, 0.5));

	//�����l���[�^�[
	m_SleepMeter.SetPos(CVector2D(80, 100));
	m_SleepMeter.Draw();

	//�����l�\��
	switch (m_ChengeFace)
	{
	case 0:
		//���v
		m_FaceRelief.SetPos(GameData::FacePosition, 90);
		m_FaceRelief.SetSize(70, 70);
		m_FaceRelief.Draw();
		if (GameData::FacePosition < 270)	m_ChengeFace++;
		break;
	case 1:
		//����
		m_FaceUsually.SetPos(GameData::FacePosition, 90);
		m_FaceUsually.SetSize(70, 70);
		m_FaceUsually.Draw();
		if (GameData::FacePosition < 120)	m_ChengeFace++;
		if (GameData::FacePosition > 270)	m_ChengeFace--;
		break;
	case 2:
		//��΂�
		m_FaceWorry.SetPos(GameData::FacePosition, 90);
		m_FaceWorry.SetSize(70, 70);
		m_FaceWorry.Draw();
		if (GameData::FacePosition > 120)	m_ChengeFace--;
		break;
	}
	
	//������������
	m_REMText.SetPos(156, 166);
	m_REMText.SetSize(128, 32);

	//�m��������������
	m_NREMText.SetPos(92, 166);
	m_NREMText.SetSize(256, 32);

	//������Ԃ̕ω�
	if (GameData::second <= m_ChengeSleep)
	{
		m_REM = !m_REM;
		m_ChengeSleep = m_ChengeSleep - rand() % 100;
	}

	//����������ԂȂ�
	if (m_REM)	m_REMText.Draw();
	//�m������������ԂȂ�
	else		m_NREMText.Draw();
}