#include "Pause.h"

#define MAXSELECT 2		//�Z���N�g�̐�

void Pause::PauseMenu1()
{
	Utility::DrawQuad(CVector2D(855, 500 + 100 * select), CVector2D(210, 62), CVector4D(1, 0, 0, 0.4));

	FONT_T()->Draw(864, 650, 0, 1, 0, "������@");
	FONT_T()->Draw(864, 750, 0, 1, 0, "�^�C�g��");

	if(PUSH(CInput::eUp) && select > 1)
	{
		//�Z���N�g��0�ȉ��ɂȂ�Ȃ�
		select--;
		SOUND("SE_Select")->Volume(0.3);
		SOUND("SE_Select")->Play();
		
	}

	if (PUSH(CInput::eDown) && select < MAXSELECT)
	{
		//�Z���N�g��MAXSELECT�ȏ�ɂȂ�Ȃ�
		select++;
		SOUND("SE_Select")->Volume(0.3);
		SOUND("SE_Select")->Play();
	}

	if (PUSH(CInput::eButton1))
	{
		SOUND("SE_Click")->Volume(0.3);
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
		SOUND("SE_Click")->Volume(0.3);
		SOUND("SE_Click")->Play();
		//��O�̉�ʂɖ߂�
		step--;
	}
	m_Manual.SetPos(320,90);
	m_Manual.Draw();

	Utility::DrawQuad(CVector2D(900, 850), CVector2D(115, 60), CVector4D(1, 0, 0, 0.4));
	
	FONT_T()->Draw(912, 900, 1, 0, 0, "�߂�");

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
	Utility::DrawQuad(CVector2D(0, 0), CVector2D(1920, 1080), CVector4D(1, 1, 1, 0.4));

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


