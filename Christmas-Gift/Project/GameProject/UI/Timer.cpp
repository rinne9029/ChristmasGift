#include "Timer.h"

//�R���X�g���N�^
Timer::Timer()
	:Task(ETaskTag::eUI, true)
{
	m_Image = COPY_RESOURCE("Text", CImage);
	m_count = 0;
}

void Timer::Update()
{
	m_count++;
	//1�b�o�߂ŃJ�E���g�𑝉�
	if (m_count > 60)
	{
		GameData::second--;	//�P�b���Z
		m_count = 0;
	}
}

//2D�`�揈��
void Timer::Draw()
{
	//���₷�����邽�߂̔w�i
	Utility::DrawQuad(CVector2D(1600, 20), CVector2D(1880, 140), CVector4D(0.7, 0.7, 0.7, 0.5));

	

	int cnt = GameData::second;
	//�^�C�}�[�`��؂�ւ�
	for (int i = 0; i < 3; i++, cnt /= 10) {
		int s = cnt % 10;
		m_Image.SetRect(8 * s, 30, 8 * s + 8, 40);
		m_Image.SetSize(64, 64);
		m_Image.SetPos(1792 - 64 * i, 50);
		m_Image.Draw();
	}
}
