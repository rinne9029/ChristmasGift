#include "GameOver.h"
#include"Title/Title.h"
#include"GameScene/GameScene.h"

//コンストラクタ
GameOver::GameOver(int stage)
	:Task(ETaskTag::eResult,true)
{
	GameData::StartFadeIn = true;
	GameData::GameStart = false;

	//現在のステージ
	m_stage = stage;

	//ゲームオーバーサウンド再生
	SOUND("SE_GameOver")->Play();

	m_BuckGraund = COPY_RESOURCE("OverBackGround", CImage);
	m_TitleText = COPY_RESOURCE("TitleText", CImage);
	m_ReStartText = COPY_RESOURCE("ReStartText", CImage);
}

//デストラクタ
GameOver::~GameOver()
{
	switch (m_select)
	{
		//タイトル
	case 0:
		new Title();
		break;
		//リトライ
	case 1:
		new GameScene(m_stage);
		break;
	}
}

//強調表示処理
void GameOver::HighLight()
{
	m_High = m_High + 0.1;
	switch (m_select)
	{
		//タイトル文字強調
	case 0:
		m_TitleText.SetPos(674, 540);
		m_TitleText.SetSize(512 + 40 * cos(m_High), 128 + 10 * cos(m_High));
		m_TitleText.Draw();
		m_ReStartText.SetPos(674, 720);
		m_ReStartText.Draw();
		break;
		//リトライ文字強調
	case 1:
		m_TitleText.SetPos(674, 540);
		m_TitleText.Draw();
		m_ReStartText.SetPos(674, 720);
		m_ReStartText.SetSize(512 + 40 * cos(m_High), 128 + 10 * cos(m_High));
		m_ReStartText.Draw();
	}
}

//更新処理
void GameOver::Update()
{
	//フェードイン・フェードアウト中は実行しない
	if (GameData::StartFadeIn) return;
	if (GameData::StartFadeOut) return;

	//スペースキーで決定
	if (PUSH(CInput::eButton1))
	{
		SOUND("SE_Click")->Volume(0.3);
		SOUND("SE_Click")->Play();
		GameData::StartFadeOut = true;
	}

	//Wキー入力
	if (PUSH(CInput::eUp) && m_select > 0)
	{
		SOUND("SE_Select")->Volume(0.3);
		SOUND("SE_Select")->Play();
		m_select--;
		m_High = 0;	//強調リセット
	}
	//Sキー入力
	if (PUSH(CInput::eDown) && m_select < 1)
	{
		SOUND("SE_Select")->Volume(0.3);
		SOUND("SE_Select")->Play();
		m_select++;
		m_High = 0;	//強調リセット
	}
}

//描画処理
void GameOver::Draw()
{
	//背景描画
	m_BuckGraund.Draw();
	//強調表示描画
	HighLight();
}
