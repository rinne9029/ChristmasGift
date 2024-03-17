#include"Title.h"
#include"Ranking/Ranking.h"
#include"Title/Snow.h"
#include"GameScene/GameScene.h"
#include"GameScene/GameData.h"

#define MAXSELECT 3			//モードセレクト数
#define MAXSTAGE  3			//ステージ数
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
	SOUND("BGM_TitleOP")->Play(true);

	m_BackGroundTitle = COPY_RESOURCE("BackGroundTitle", CImage);
	m_TitleText = COPY_RESOURCE("GameTitleText", CImage);
	m_StartText = COPY_RESOURCE("StartText", CImage);
	m_ManualText = COPY_RESOURCE("ManualText", CImage);
	m_RankingText = COPY_RESOURCE("GameRankingText", CImage);
	m_Manual1 = COPY_RESOURCE("Manual1", CImage);
	m_Manual2 = COPY_RESOURCE("Manual2", CImage);
	m_Stage1 = COPY_RESOURCE("Stage1", CImage);
	m_Stage2 = COPY_RESOURCE("Stage2", CImage);
	m_Stage3 = COPY_RESOURCE("Stage3", CImage);
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
			new GameScene();
			break;
		case 2:
			//ステージ2
			new GameScene();
			break;
		case 3:
			//ステージ3
			new GameScene();
			break;
		}
		break;
	}
}

//ふわふわ動く文字
void Title::FuwaFuwa()
{
	m_TitleText.SetPos(500, 300);
	m_TitleText.Draw();

	m_fuwafuwa += 0.02f;

	//選ばれた文字がsinカーブでふわふわ動く
	switch (m_select)
	{
		//スタート文字
	case 1:
		m_StartText.SetPos(200, 750 - abs(sin(m_fuwafuwa)) * 64);
		m_StartText.Draw();
		m_ManualText.SetPos(855, 750);
		m_ManualText.Draw();
		m_RankingText.SetPos(1270, 750);
		m_RankingText.Draw();
		break;
		//説明書文字
	case 2:
		m_StartText.SetPos(200, 750);
		m_StartText.Draw();
		m_ManualText.SetPos(855, 750 - abs(sin(m_fuwafuwa)) * 64);
		m_ManualText.Draw();
		m_RankingText.SetPos(1270, 750);
		m_RankingText.Draw();
		break;
		//ランキング文字
	case 3:
		m_StartText.SetPos(200, 750);
		m_StartText.Draw();
		m_ManualText.SetPos(855, 750);
		m_ManualText.Draw();
		m_RankingText.SetPos(1270, 750 - abs(sin(m_fuwafuwa)) * 64);
		m_RankingText.Draw();
		break;
	}

	FONT_T()->Draw(100, 980, 1, 0, 0, "A入力 ← ： D入力 → ： スペース入力 決定");
}

//モードセレクト処理
void Title::ModeChenge()
{
	//スペースボタン
	if (PUSH(CInput::eButton5))
	{
		SOUND("SE_Click")->Volume(0.5);
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
		SOUND("SE_Select")->Volume(0.5);
		SOUND("SE_Select")->Play();
		m_select--;
		//ふわふわ表示リセット
		m_fuwafuwa = 0;
	}
	//Dキー入力
	if (PUSH(CInput::eRight) && m_select < MAXSELECT)
	{
		SOUND("SE_Select")->Volume(0.5);
		SOUND("SE_Select")->Play();
		m_select++;
		//ふわふわ表示リセット
		m_fuwafuwa = 0;
	}
}

//説明モード処理
void Title::ManualMode()
{
	//Sキー入力
	if (PUSH(CInput::eDown))
	{
		SOUND("SE_Click")->Volume(0.5);
		SOUND("SE_Click")->Play();
		//モードセレクトに戻る
		m_step = 0;
		m_select = 1;
	}

	//Aキー入力
	//次の説明
	if (PUSH(CInput::eLeft) && m_select > 1)
	{
		SOUND("SE_Select")->Volume(0.5);
		SOUND("SE_Select")->Play();
		m_select--;
	}
	//Dキー入力
	//前の説明に戻る
	if (PUSH(CInput::eRight) && m_select < MAXMANUAL)
	{
		SOUND("SE_Select")->Volume(0.5);
		SOUND("SE_Select")->Play();
		m_select++;
	}
}

void Title::ManualDraw()
{
	m_Manual1.SetPos(320 - 1920 * (m_select - 1), 180);
	m_Manual1.Draw();
	m_Manual2.SetPos(2240 - 1920 * (m_select - 1), 180);
	m_Manual2.Draw();
	FONT_T()->Draw(100, 980, 1, 0, 0, "A入力 ← ： D入力 → ： S入力 戻る");
	FONT_T()->Draw(1000, 880, 1, 0, 0, "%d/2", m_select);
}

void Title::StageSelecte()
{
	//スペースボタン
	if (PUSH(CInput::eButton5))
	{
		//現状ステージ１までしか遊べない
		if (m_select < 2)
		{
			SOUND("SE_Click")->Volume(0.5);
			SOUND("SE_Click")->Play();
			SOUND("BGM_TitleOP")->Stop();
			GameData::StartFadeOut = true;
		}
	}

	//Sキー入力
	if (PUSH(CInput::eDown))
	{
		SOUND("SE_Click")->Volume(0.5);
		SOUND("SE_Click")->Play();
		//モードセレクトに戻る
		m_step = 0;
		m_select = 1;
	}

	//Aキー入力
	//次のステージ
	if (PUSH(CInput::eLeft) && m_select > 1)
	{
		SOUND("SE_Select")->Volume(0.5);
		SOUND("SE_Select")->Play();
		m_select--;
	}
	//Dキー入力
	//前のステージ
	if (PUSH(CInput::eRight) && m_select < MAXSTAGE)
	{
		SOUND("SE_Select")->Volume(0.5);
		SOUND("SE_Select")->Play();
		m_select++;
	}
}

void Title::StageDraw()
{
	m_Stage1.SetPos(320 - 1920 * (m_select - 1), 0);
	m_Stage1.Draw();
	m_Stage2.SetPos(2240 - 1920 * (m_select - 1), 0);
	m_Stage2.Draw();
	m_Stage3.SetPos(4160 - 1920 * (m_select - 1), 0);
	m_Stage3.Draw();
	FONT_T()->Draw(100, 910, 1, 0, 0, "SPACEキーでスタート");
	FONT_T()->Draw(100, 980, 1, 0, 0, "A入力 ← ： D入力 → ： S入力 戻る");
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