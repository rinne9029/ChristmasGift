#include "Pause.h"

#define MAXSELECT 2		//セレクトの数

void Pause::PauseMenu1()
{
	Utility::DrawQuad(CVector2D(855, 500 + 100 * select), CVector2D(210, 62), CVector4D(1, 0, 0, 0.4));

	FONT_T()->Draw(864, 650, 0, 1, 0, "操作方法");
	FONT_T()->Draw(864, 750, 0, 1, 0, "タイトル");

	if(PUSH(CInput::eUp) && select > 1)
	{
		//セレクトは0以下にならない
		select--;
		SOUND("SE_Select")->Volume(0.3);
		SOUND("SE_Select")->Play();
		
	}

	if (PUSH(CInput::eDown) && select < MAXSELECT)
	{
		//セレクトはMAXSELECT以上にならない
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
		SOUND("SE_Click")->Volume(0.3);
		SOUND("SE_Click")->Play();
		//一つ前の画面に戻る
		step--;
	}
	m_Manual.SetPos(320,90);
	m_Manual.Draw();

	Utility::DrawQuad(CVector2D(900, 850), CVector2D(115, 60), CVector4D(1, 0, 0, 0.4));
	
	FONT_T()->Draw(912, 900, 1, 0, 0, "戻る");

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
	Utility::DrawQuad(CVector2D(0, 0), CVector2D(1920, 1080), CVector4D(1, 1, 1, 0.4));

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


