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
	if(GameData::FacePosition <340.0f)
	GameData::FacePosition += 0.05f;

	//�q���̐������o���i�Q�[���I�[�o�[�j
	if (GameData::FacePosition > 40.0f) return;
	//�Q�[���I�[�o�[
	GameData::GameOverCheck = true;
	//�t�F�[�h�A�E�g���s
	GameData::StartFadeOut = true;
}

//2D�`�揈��
void SleepLife::Draw()
{
	//���₷�����邽�߂̔w�i
	Utility::DrawQuad(CVector2D(30.0f, 60.0f), CVector2D(380.0f, 140.0f), CVector4D(0.7f, 0.7f, 0.7f, 0.5f));

	//�����l���[�^�[
	m_SleepMeter.SetPos(CVector2D(80.0f, 100.0f));
	m_SleepMeter.Draw();

	//�����l�\��
	switch (m_ChengeFace)
	{
	case 0:
		//���v
		m_FaceRelief.SetPos(GameData::FacePosition, 90.0f);
		m_FaceRelief.SetSize(70.0f, 70.0f);
		m_FaceRelief.Draw();
		if (GameData::FacePosition < 270.0f)	m_ChengeFace++;
		break;
	case 1:
		//����
		m_FaceUsually.SetPos(GameData::FacePosition, 90.0f);
		m_FaceUsually.SetSize(70.0f, 70.0f);
		m_FaceUsually.Draw();
		if (GameData::FacePosition < 120.0f)	m_ChengeFace++;
		if (GameData::FacePosition > 270.0f)	m_ChengeFace--;
		break;
	case 2:
		//��΂�
		m_FaceWorry.SetPos(GameData::FacePosition, 90.0f);
		m_FaceWorry.SetSize(70.0f, 70.0f);
		m_FaceWorry.Draw();
		if (GameData::FacePosition > 120.0f)	m_ChengeFace--;
		break;
	}
	
	//������������
	m_REMText.SetPos(156.0f, 166.0f);
	m_REMText.SetSize(128.0f, 32.0f);

	//�m��������������
	m_NREMText.SetPos(92.0f, 166.0f);
	m_NREMText.SetSize(256.0f, 32.0f);

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