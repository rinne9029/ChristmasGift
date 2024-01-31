#include"Title.h"
#include"Filta/Filta.h"
#include"GameScene/GameScene.h"
#include"GameScene/GameData.h"

//コンストラクタ
Title::Title()
	:Task(ETaskTag::eScene,true)
	,m_select(0)
	,m_fuwafuwa(0.0f)
{
	m_StartText = COPY_RESOURCE("StartText", CImage);
	m_ManualText = COPY_RESOURCE("ManualText", CImage);
	m_RankingText = COPY_RESOURCE("RankingText", CImage);
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
		//スタート
	case 0:
		m_StartText.SetPos(200, 750 - abs(sin(m_fuwafuwa)) * 64);
		m_StartText.Draw();
		m_ManualText.SetPos(855, 750);
		m_ManualText.Draw();
		m_RankingText.SetPos(1270, 750);
		m_RankingText.Draw();
		break;
		//説明書
	case 1:
		m_StartText.SetPos(200, 750);
		m_StartText.Draw();
		m_ManualText.SetPos(855, 750 - abs(sin(m_fuwafuwa)) * 64);
		m_ManualText.Draw();
		m_RankingText.SetPos(1270, 750);
		m_RankingText.Draw();
		break;
		//ランキング
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
	//スペースボタン
	if (PUSH(CInput::eButton5))		
	{
		//フェードアウト実行
		GameData::StartFadeOut = true;
	}

	//Aキー入力
	if (PUSH(CInput::eLeft) && m_select > 0)
	{
		m_select--;
		m_fuwafuwa = 0;
	}
	//Dキー入力
	if (PUSH(CInput::eRight) && m_select < 2)
	{
		m_select++;
		m_fuwafuwa = 0;
	}
}

//2D描画処理
void Title::Draw()
{
	FuwaFuwa();	
}