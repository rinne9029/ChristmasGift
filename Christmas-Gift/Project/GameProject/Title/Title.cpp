#include"Title.h"
#include"GameScene/GameScene.h"
#include"GameScene/GameData.h"

//コンストラクタ
Title::Title()
	:Task(ETaskTag::eScene,true)
	,m_select(0)
	,m_fuwafuwa(0.0f)
{
	//フェードイン実行
	GameData::StartFadeIn = true;

	SOUND("BGM_TitleOP")->Play(true);

	m_TitleText = COPY_RESOURCE("GameTitleText", CImage);
	m_StartText = COPY_RESOURCE("StartText", CImage);
	m_ManualText = COPY_RESOURCE("ManualText", CImage);
	m_RankingText = COPY_RESOURCE("GameRankingText", CImage);
}

//デストラクタ
Title::~Title()
{
	switch (m_select)
	{
	case 0:
		//ゲームシーン起動
		new GameScene();
		break;
	case 1:
		//説明書
		//仮
		new GameScene();
		break;
	case 2:
		//ランキング
		//仮
		new GameScene();
		break;
	}
}

//ふわふわ動く文字
void Title::FuwaFuwa()
{
	m_fuwafuwa += 0.02f;

	//選ばれた文字がsinカーブでふわふわ動く
	switch (m_select)
	{
		//スタート文字
	case 0:
		m_StartText.SetPos(200, 750 - abs(sin(m_fuwafuwa)) * 64);
		m_StartText.Draw();
		m_ManualText.SetPos(855, 750);
		m_ManualText.Draw();
		m_RankingText.SetPos(1270, 750);
		m_RankingText.Draw();
		break;
		//説明書文字
	case 1:
		m_StartText.SetPos(200, 750);
		m_StartText.Draw();
		m_ManualText.SetPos(855, 750 - abs(sin(m_fuwafuwa)) * 64);
		m_ManualText.Draw();
		m_RankingText.SetPos(1270, 750);
		m_RankingText.Draw();
		break;
		//ランキング文字
	case 2:
		m_StartText.SetPos(200, 750);
		m_StartText.Draw();
		m_ManualText.SetPos(855, 750);
		m_ManualText.Draw();
		m_RankingText.SetPos(1270, 750 - abs(sin(m_fuwafuwa)) * 64);
		m_RankingText.Draw();
		break;
	}
}

//更新処理
void Title::Update()
{
	if (GameData::StartFadeIn) return;
	if (GameData::StartFadeOut) return;

	//スペースボタン
	if (PUSH(CInput::eButton5))		
	{
		SOUND("BGM_TitleOP")->Stop();
		SOUND("SE_Click")->Volume(0.5);
		SOUND("SE_Click")->Play();
		//フェードアウト実行
		GameData::StartFadeOut = true;
	}

	//Aキー入力
	if (PUSH(CInput::eLeft) && m_select > 0)
	{
		SOUND("SE_Select")->Volume(0.5);
		SOUND("SE_Select")->Play();
		m_select--;
		//ふわふわ表示リセット
		m_fuwafuwa = 0;
	}
	//Dキー入力
	if (PUSH(CInput::eRight) && m_select < 2)
	{
		SOUND("SE_Select")->Volume(0.5);
		SOUND("SE_Select")->Play();
		m_select++;
		//ふわふわ表示リセット
		m_fuwafuwa = 0;
	}
}

//2D描画処理
void Title::Draw()
{
	m_TitleText.SetPos(500, 300);
	m_TitleText.Draw();
	//ふわふわ表示描画
	FuwaFuwa();	
}