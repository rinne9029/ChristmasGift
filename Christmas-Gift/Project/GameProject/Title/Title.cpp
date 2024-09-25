#include"Title.h"
#include"Ranking/Ranking.h"
#include"Title/Snow.h"
#include"GameScene/GameScene.h"

#define MAXSELECT 3			//モードセレクト数
#define MAXSTAGE  2			//ステージ数
#define MAXMANUAL 2			//説明枚数


//コンストラクタ
Title::Title()
	:Task(ETaskTag::eScene,true)
	,m_select(1)
	,m_step(0)
	,m_fuwafuwa(0.0f)
{
	//フェードイン実行
	GameData::StartFadeIn = true;

	//サウンド再生
	SOUND("BGM_TitleOP")->Volume(0.2f);
	SOUND("BGM_TitleOP")->Play(true);

	m_BackGroundTitle = COPY_RESOURCE("BackGroundTitle", CImage);
	m_TitleRogo = COPY_RESOURCE("GameTitleRogo", CImage);
	m_Manual1 = COPY_RESOURCE("Manual1", CImage);
	m_Manual2 = COPY_RESOURCE("Manual2", CImage);
	m_Stage1 = COPY_RESOURCE("Stage1", CImage);
	m_Stage2 = COPY_RESOURCE("Stage2", CImage);
}

//デストラクタ
Title::~Title()
{
	switch (m_step)
	{
	case 0:
		switch (m_select)
		{
		case 3:
			//ランキング起動
			new Ranking("TextData/RankingData.txt");
			break;
		}
		break;
	case 10:
		switch (m_select)
		{
		case 1:
			//ステージ1
			new GameScene(m_select);
			break;
		case 2:
			//ステージ2
			new GameScene(m_select);
			break;
		}
		break;
	}
}

//ふわふわ動く文字
void Title::FuwaFuwa()
{
	m_TitleRogo.SetPos(610.0f, 50.0f);
	m_TitleRogo.SetSize(700.0f, 700.0f);
	m_TitleRogo.Draw();

	m_fuwafuwa += 0.02f;

	//文字を見やすくするための帯
	Utility::DrawQuad(CVector2D(0.0f, 710.0f), CVector2D(1920.0f, 220.0f), CVector4D(1.0f, 1.0f, 1.0f, 0.4f));

	//選ばれた文字がsinカーブでふわふわ動く
	switch (m_select)
	{
		//スタート文字
	case 1:
		CREATE_FONT_F
		(
			"name",
			"C:\\Windows\\Fonts\\BOD_CI.TTF",
			128
		)->Draw(350, 880 - abs(sin(m_fuwafuwa)) * 64, 0.0f, 0.0f, 0.0f, "Start");

		CREATE_FONT_F
		(
			"name",
			"C:\\Windows\\Fonts\\BOD_CI.TTF",
			128
		)->Draw(870, 880, 0.0f, 0.0f, 0.0f, "Manual");
		
		CREATE_FONT_F
		(
			"name",
			"C:\\Windows\\Fonts\\BOD_CI.TTF",
			128
		)->Draw(1442, 880, 0.0f, 0.0f, 0.0f, "Ranking");
		break;
		//説明書文字
	case 2:
		CREATE_FONT_F
		(
			"name",
			"C:\\Windows\\Fonts\\BOD_CI.TTF",
			128
		)->Draw(350, 880, 0.0f, 0.0f, 0.0f, "Start");
		CREATE_FONT_F
		(
			"name",
			"C:\\Windows\\Fonts\\BOD_CI.TTF",
			128
		)->Draw(870, 880 - abs(sin(m_fuwafuwa)) * 64, 0.0f, 0.0f, 0.0f, "Manual");
		CREATE_FONT_F
		(
			"name",
			"C:\\Windows\\Fonts\\BOD_CI.TTF",
			128
		)->Draw(1442, 880, 0.0f, 0.0f, 0.0f, "Ranking");
		break;
		//ランキング文字
	case 3:
		CREATE_FONT_F
		(
			"name",
			"C:\\Windows\\Fonts\\BOD_CI.TTF",
			128
		)->Draw(350, 880, 0.0f, 0.0f, 0.0f, "Start");
		CREATE_FONT_F
		(
			"name",
			"C:\\Windows\\Fonts\\BOD_CI.TTF",
			128
		)->Draw(870, 880, 0.0f, 0.0f, 0.0f, "Manual");
		CREATE_FONT_F
		(
			"name",
			"C:\\Windows\\Fonts\\BOD_CI.TTF",
			128
		)->Draw(1442, 880 - abs(sin(m_fuwafuwa)) * 64, 0.0f, 0.0f, 0.0f, "Ranking");
		break;
	}

	Utility::DrawQuad(CVector2D(0.0f, 980.0f), CVector2D(1920.0f, 56.0f), CVector4D(1.0f, 1.0f, 1.0f, 0.4f));
	FONT_T()->Draw(100, 1030, 0.0f, 0.0f, 0.0f, "A入力 ← ： D入力 → ： スペース入力 決定");

	
}

//モードセレクト処理
void Title::ModeChenge()
{
	//スペースボタン
	if (PUSH(CInput::eButton1))
	{
		SOUND("SE_Click")->Volume(0.3f);
		SOUND("SE_Click")->Play();
		switch (m_select)
		{
		case 1:
			m_step = m_select * 10;
			m_select = 1;
			break;
		case 2:
			m_step = m_select * 10;
			m_select = 1;
			break;
		case 3:
			SOUND("BGM_TitleOP")->Stop();
			GameData::StartFadeOut = true;
			break;
		}
	}

	//Aキー入力
	if (PUSH(CInput::eLeft) && m_select > 1)
	{
		SOUND("SE_Select")->Volume(0.3f);
		SOUND("SE_Select")->Play();
		m_select--;
		//ふわふわ表示リセット
		m_fuwafuwa = 0.0f;
	}
	//Dキー入力
	if (PUSH(CInput::eRight) && m_select < MAXSELECT)
	{
		SOUND("SE_Select")->Volume(0.3f);
		SOUND("SE_Select")->Play();
		m_select++;
		//ふわふわ表示リセット
		m_fuwafuwa = 0.0f;
	}
}

//説明モード処理
void Title::ManualMode()
{
	//Sキー入力
	if (PUSH(CInput::eDown))
	{
		SOUND("SE_Click")->Volume(0.3);
		SOUND("SE_Click")->Play();
		//モードセレクトに戻る
		m_step = 0;
		m_select = 1;
	}

	//Aキー入力
	//次の説明
	if (PUSH(CInput::eLeft) && m_select > 1)
	{
		SOUND("SE_Select")->Volume(0.3f);
		SOUND("SE_Select")->Play();
		m_select--;
	}
	//Dキー入力
	//前の説明に戻る
	if (PUSH(CInput::eRight) && m_select < MAXMANUAL)
	{
		SOUND("SE_Select")->Volume(0.3f);
		SOUND("SE_Select")->Play();
		m_select++;
	}
}

void Title::ManualDraw()
{
	Utility::DrawQuad(CVector2D(0.0f, 930.0f), CVector2D(1920.0f, 56.0f), CVector4D(1.0f, 1.0f, 1.0f, 0.4f));

	m_Manual1.SetPos(320.0f - 1920.0f * (m_select - 1), 180.0f);
	m_Manual1.Draw();
	m_Manual2.SetPos(2240.0f - 1920.0f * (m_select - 1), 180.0f);
	m_Manual2.Draw();
	FONT_T()->Draw(100, 980, 0.0f, 0.0f, 0.0f, "A入力 ← ： D入力 → ： S入力 戻る");
	FONT_T()->Draw(1000, 880, 0.0f, 0.0f, 0.0f, "%d/2", m_select);
}

void Title::StageSelecte()
{
	//スペースボタン
	if (PUSH(CInput::eButton1))
	{
		SOUND("SE_Click")->Volume(0.3f);
		SOUND("SE_Click")->Play();
		SOUND("BGM_TitleOP")->Stop();
		GameData::StartFadeOut = true;
	}

	//Sキー入力
	if (PUSH(CInput::eDown))
	{
		SOUND("SE_Click")->Volume(0.3f);
		SOUND("SE_Click")->Play();
		//モードセレクトに戻る
		m_step = 0;
		m_select = 1;
	}

	//Aキー入力
	//次のステージ
	if (PUSH(CInput::eLeft) && m_select > 1)
	{
		SOUND("SE_Select")->Volume(0.3f);
		SOUND("SE_Select")->Play();
		m_select--;
	}
	//Dキー入力
	//前のステージ
	if (PUSH(CInput::eRight) && m_select < MAXSTAGE)
	{
		SOUND("SE_Select")->Volume(0.3f);
		SOUND("SE_Select")->Play();
		m_select++;
	}
}

void Title::StageDraw()
{
	Utility::DrawQuad(CVector2D(0.0f, 960.0f), CVector2D(1920.0f, 120.0f), CVector4D(1.0f, 1.0f, 1.0f, 0.6f));
	m_Stage1.SetSize(CVector2D(1600, 900));
	m_Stage1.SetPos(160.0f + 1920.0f * (m_select - 1), 40.0f);
	m_Stage1.Draw();
	m_Stage2.SetSize(CVector2D(1600, 900));
	m_Stage2.SetPos(2080.0f - 1920.0f * (m_select - 1), 40.0f);
	m_Stage2.Draw();
	FONT_T()->Draw(100, 1010, 0.0f, 0.0f, 0.0f, "SPACEキーでスタート");
	FONT_T()->Draw(100, 1060, 0.0f, 0.0f, 0.0f, "A入力 ← ： D入力 → ： S入力 戻る");
}

//更新処理
void Title::Update()
{
	if (GameData::StartFadeIn) return;
	if (GameData::StartFadeOut) return;

	switch (m_step)
	{
	case 0:
		ModeChenge();
		break;
	case 10:
		StageSelecte();
		break;
	case 20:
		ManualMode();
		break;
	}
}

//2D描画処理
void Title::Draw()
{
	m_BackGroundTitle.Draw();


	//雪を描画する時間
	static int RespawnTime = 0;
	RespawnTime++;
	//１秒度に新しい雪を描画する
	if (RespawnTime > 60)
	{
		new Snow();
		new Snow();
		//描画クールタイムをリセット
		RespawnTime = 0;
	}

	//ふわふわ表示描画
	switch (m_step)
	{
	case 0:
		FuwaFuwa();
		break;
	case 10:
		StageDraw();
		break;
	case 20:
		ManualDraw();
		break;
	}
	
}