#include "Snow.h"

//�R���X�g���N�^
Snow::Snow()
	:Task(ETaskTag::eScene,true)
{
	m_snow = COPY_RESOURCE("Snow", CImage);
	float wide = rand() % 1920;				//X���W�������_��
	m_snowPos = CVector2D(wide, -20.0f);	//�������W
	m_snowAng = 0;							//������]�l
}

//�X�V����
void Snow::Update()
{
	//���t���[����]
	m_snowAng += 0.1f;
	//��̗���
	m_snowPos.y += 2.0f;
	//�����n�_��1200�𒴂�����폜
	if (m_snowPos.y > 1200.0f) Kill();
}

//�`�揈��
void Snow::Draw()
{
	m_snow.SetSize(64.0f, 64.0f);
	m_snow.SetCenter(32.0f, 32.0f);
	m_snow.SetPos(m_snowPos);
	m_snow.SetAng(m_snowAng);
	m_snow.Draw();
}
