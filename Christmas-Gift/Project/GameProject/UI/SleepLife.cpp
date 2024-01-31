#include"SleepLife.h"
#include"Chara/Player/Player.h"
#include"Filta/Filta.h"
#include"GameScene/GameData.h"

bool SleepLife::m_REM = true;

SleepLife::SleepLife()
	:Task(ETaskTag::eUI, true)
{
	m_REMText = COPY_RESOURCE("REMsleep", CImage);
	m_NREMText = COPY_RESOURCE("NREMsleep", CImage);

	//0～100秒ランダム
	m_ChengeSleep = rand() % 100;
}

//更新処理
void SleepLife::Update()
{
	//子供の睡眠が覚醒（ゲームオーバー）
	if (GameData::BlueSleepSize >= 0) return;

	//ゲームオーバー
	GameData::GameOverCheck = true;
	//フェードアウト実行
	GameData::StartFadeOut = true;
}

//2D描画処理
void SleepLife::Draw()
{
	//赤の睡眠ゲージ描画
	Utility::DrawQuad(CVector2D(20, 250), CVector2D(300, 25), CVector4D(1, 0, 0, 1));
	//青の睡眠ゲージ描画
	Utility::DrawQuad(CVector2D(20, 250), CVector2D(GameData::BlueSleepSize, 25), CVector4D(0, 0, 1, 1));

	//レム睡眠文字
	m_REMText.SetPos(106, 216);
	m_REMText.SetSize(128, 32);

	//ノンレム睡眠文字
	m_NREMText.SetPos(42, 216);
	m_NREMText.SetSize(256, 32);

	if (GameData::second >= m_ChengeSleep)
	{
		m_REM = !m_REM;
		m_ChengeSleep = m_ChengeSleep + rand() % 100;
	}

	//レム睡眠状態なら
	if (m_REM)	m_REMText.Draw();
	//ノンレム睡眠状態なら
	else		m_NREMText.Draw();
}