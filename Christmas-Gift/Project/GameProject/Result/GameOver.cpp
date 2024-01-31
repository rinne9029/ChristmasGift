#include "GameOver.h"
#include"Title/Title.h"
#include"GameScene/GameData.h"
#include"GameScene/GameScene.h"

GameOver::GameOver()
	:Task(ETaskTag::eResult,true)
{
	GameData::StartFadeIn = true;

	m_BuckGraund = COPY_RESOURCE("OverBackGround", CImage);
	m_TitleText = COPY_RESOURCE("TitleText", CImage);
	m_ReStartText = COPY_RESOURCE("ReStartText", CImage);
}

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
		new GameScene();
		break;
	}
}

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

void GameOver::Update()
{
	//スペースキーで決定
	if (PUSH(CInput::eButton5))
	{
		GameData::StartFadeOut = true;
	}

	//Wキー入力
	if (PUSH(CInput::eUp) && m_select > 0)
	{
		m_select--;
		m_High = 0;	//強調リセット
	}
	//Sキー入力
	if (PUSH(CInput::eDown) && m_select < 1)
	{
		m_select++;
		m_High = 0;	//強調リセット
	}
}

void GameOver::Draw()
{
	//背景描画
	m_BuckGraund.Draw();
	//強調表示描画
	HighLight();
}
