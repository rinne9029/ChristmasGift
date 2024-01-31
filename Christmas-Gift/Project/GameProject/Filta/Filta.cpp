#include"Filta.h"
#include"Chara/Player/Player.h"
#include"GameScene/GameData.h"

#define FADETIME 90.0

//コンストラクタ
Filta::Filta()
	:Task(ETaskTag::eFilta,false)
	, m_alpha(0)
	, m_time(0)
{
	m_filta = COPY_RESOURCE("Filta", CImage);
}

//フェードイン処理
void Filta::Fadein()
{
	m_time -= 3.0f;
	//フェードイン中
	if (m_alpha > 0)
	{
		m_alpha = m_time / FADETIME;
	}
	//フェードイン完了
	else
	{
		//フェードインスタートフラグリセット
		GameData::StartFadeIn = false;
	}
}

//フェードアウト処理
void Filta::Fadeout()
{
	m_time += 3.0f;
	//フェードアウト中
	if (m_alpha < 1)
	{
		m_alpha = m_time / FADETIME;
	}
	//フェードアウト完了
	else
	{
		//フェードアウトスタートフラグリセット
		GameData::StartFadeOut = false;
		//フィルター以外の全タスク削除
		TaskManager::KillALL();
	}
}

//更新処理
void Filta::Update()
{
	//フェードインスタートOn
	if (GameData::StartFadeIn) 
	{
		Fadein();
	}
		
	//フェードアウトスタートOn
	if (GameData::StartFadeOut) 
	{
		Fadeout();
	}
		
}

//2D描画処理
void Filta::Draw()
{
	//フィルター描画
	m_filta.SetColor(1, 1, 1, m_alpha);
	m_filta.Draw();
}