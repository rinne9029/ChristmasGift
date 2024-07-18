#include "Pause.h"

#define MAXSELECT 2		//�Z���N�g�̐�

void Pause::PauseMenu1()
{
	Utility::DrawQuad(CVector2D(855.0f, 500.0f + 100.0f * select), CVector2D(210.0f, 62.0f), CVector4D(1.0f, 0.0f, 0.0f, 0.4f));

	FONT_T()->Draw(864, 650, 0.0f, 1.0f, 0.0f, "������@");
	FONT_T()->Draw(864, 750, 0.0f, 1.0f, 0.0f, "�^�C�g��");

	if(PUSH(CInput::eUp) && select > 1)
	{
		//�Z���N�g��0�ȉ��ɂȂ�Ȃ�
		select--;
		SOUND("SE_Select")->Volume(0.3f);
		SOUND("SE_Select")->Play();
		
	}

	if (PUSH(CInput::eDown) && select < MAXSELECT)
	{
		//�Z���N�g��MAXSELECT�ȏ�ɂȂ�Ȃ�
		select++;
		SOUND("SE_Select")->Volume(0.3f);
		SOUND("SE_Select")->Play();
	}

	if (PUSH(CInput::eButton1))
	{
		SOUND("SE_Click")->Volume(0.3f);
		SOUND("SE_Click")->Play();
		switch (select)
		{
		case 1:
			//������@�\��
			step++;
			break;
		case 2:
			//�|�[�Y��ʒ�~
			GameData::isPauseGame = false;
			GameData::GameStart = false;
			//�t�F�[�h�C���X�^�[�g
			GameData::StartFadeOut = true;
			break;
		}
	}
}

void Pause::PauseMenu2()
{
	if (PUSH(CInput::eButton1))
	{
		SOUND("SE_Click")->Volume(0.3f);
		SOUND("SE_Click")->Play();
		//��O�̉�ʂɖ߂�
		step--;
	}
	m_Manual.SetPos(320.0f,90.0f);
	m_Manual.Draw();

	Utility::DrawQuad(CVector2D(900.0f, 850.0f), CVector2D(115.0f, 60.0f), CVector4D(1.0f, 0.0f, 0.0f, 0.4f));
	
	FONT_T()->Draw(912, 900, 1.0f, 0.0f, 0.0f, "�߂�");

}

Pause::Pause()
	:Task(ETaskTag::eUI,true)
	,select(1)
	,step(1)
{
	m_Manual = COPY_RESOURCE("UI_Manual", CImage);
}

void Pause::Draw()
{

	//�|�[�Y�t���O���I�t�̎��͏��������Ȃ�
	if (!GameData::isPauseGame)
	{
		//�l�����Z�b�g
		select = 1;
		step = 1;
		return;
	}

	//��ʑS�̂𔖂�����
	Utility::DrawQuad(CVector2D(0.0f, 0.0f), CVector2D(1920.0f, 1080.0f), CVector4D(1.0f, 1.0f, 1.0f, 0.4f));

	//�|�[�Y���1�̑���ƃe�L�X�g�\��
	switch (step)
	{
	case 1:
		PauseMenu1();
		break;
	case 2:
		PauseMenu2();
		break;
	}
	
	
}


