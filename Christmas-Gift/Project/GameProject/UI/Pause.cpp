#include "Pause.h"

#define MAXSELECT 2		//セレクトの数

void Pause::PauseMenu1()
{
	Utility::DrawQuad(CVector2D(855.0f, 500.0f + 100.0f * select), CVector2D(210.0f, 62.0f), CVector4D(1.0f, 0.0f, 0.0f, 0.4f));

	FONT_T()->Draw(864, 650, 0.0f, 1.0f, 0.0f, "操作方法");
	FONT_T()->Draw(864, 750, 0.0f, 1.0f, 0.0f, "タイトル");

	if(PUSH(CInput::eUp) && select > 1)
	{
		//セレクトは0以下にならない
		select--;
		SOUND("SE_Select")->Volume(0.3f);
		SOUND("SE_Select")->Play();
		
	}

	if (PUSH(CInput::eDown) && select < MAXSELECT)
	{
		//セレクトはMAXSELECT以上にならない
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
			//操作方法表示
			step++;
			break;
		case 2:
			//ポーズ画面停止
			GameData::isPauseGame = false;
			GameData::GameStart = false;
			//フェードインスタート
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
		//一つ前の画面に戻る
		step--;
	}
	m_Manual.SetPos(320.0f,90.0f);
	m_Manual.Draw();

	Utility::DrawQuad(CVector2D(900.0f, 850.0f), CVector2D(115.0f, 60.0f), CVector4D(1.0f, 0.0f, 0.0f, 0.4f));
	
	FONT_T()->Draw(912, 900, 1.0f, 0.0f, 0.0f, "戻る");

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

	//ポーズフラグがオフの時は処理をしない
	if (!GameData::isPauseGame)
	{
		//値をリセット
		select = 1;
		step = 1;
		return;
	}

	//画面全体を薄く白く
	Utility::DrawQuad(CVector2D(0.0f, 0.0f), CVector2D(1920.0f, 1080.0f), CVector4D(1.0f, 1.0f, 1.0f, 0.4f));

	//ポーズ画面1の操作とテキスト表示
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


