#include"SleepLife.h"
#include"Chara/Player/Player.h"
#include"Filta/Filta.h"

bool SleepLife::m_REM = true;

//コンストラクタ
SleepLife::SleepLife()
	:Task(ETaskTag::eUI, true)
	,m_ChengeFace(0)
{
	m_SleepMeter = COPY_RESOURCE("SleepMeter", CImage);
	m_FaceRelief = COPY_RESOURCE("Relief", CImage);
	m_FaceUsually = COPY_RESOURCE("Usually", CImage);
	m_FaceWorry = COPY_RESOURCE("Worry", CImage);
	m_REMText = COPY_RESOURCE("REMsleep", CImage);
	m_NREMText = COPY_RESOURCE("NREMsleep", CImage);

	//0～20秒ランダム
	m_ChengeSleep = 300 - rand() % 20;
}

//更新処理
void SleepLife::Update()
{	
	//睡眠値の回復
	if(GameData::FacePosition <340)
	GameData::FacePosition += 0.05f;

	//子供の睡眠が覚醒（ゲームオーバー）
	if (GameData::FacePosition > 40) return;
	//ゲームオーバー
	GameData::GameOverCheck = true;
	//フェードアウト実行
	GameData::StartFadeOut = true;
}

//2D描画処理
void SleepLife::Draw()
{
	//見やすくするための背景
	Utility::DrawQuad(CVector2D(30, 60), CVector2D(380, 140), CVector4D(0.7, 0.7, 0.7, 0.5));

	//睡眠値メーター
	m_SleepMeter.SetPos(CVector2D(80, 100));
	m_SleepMeter.Draw();

	//睡眠値表情
	switch (m_ChengeFace)
	{
	case 0:
		//大丈夫
		m_FaceRelief.SetPos(GameData::FacePosition, 90);
		m_FaceRelief.SetSize(70, 70);
		m_FaceRelief.Draw();
		if (GameData::FacePosition < 270)	m_ChengeFace++;
		break;
	case 1:
		//普通
		m_FaceUsually.SetPos(GameData::FacePosition, 90);
		m_FaceUsually.SetSize(70, 70);
		m_FaceUsually.Draw();
		if (GameData::FacePosition < 120)	m_ChengeFace++;
		if (GameData::FacePosition > 270)	m_ChengeFace--;
		break;
	case 2:
		//やばい
		m_FaceWorry.SetPos(GameData::FacePosition, 90);
		m_FaceWorry.SetSize(70, 70);
		m_FaceWorry.Draw();
		if (GameData::FacePosition > 120)	m_ChengeFace--;
		break;
	}
	
	//レム睡眠文字
	m_REMText.SetPos(156, 166);
	m_REMText.SetSize(128, 32);

	//ノンレム睡眠文字
	m_NREMText.SetPos(92, 166);
	m_NREMText.SetSize(256, 32);

	//睡眠状態の変化
	if (GameData::second <= m_ChengeSleep)
	{
		m_REM = !m_REM;
		m_ChengeSleep = m_ChengeSleep - rand() % 100;
	}

	//レム睡眠状態なら
	if (m_REM)	m_REMText.Draw();
	//ノンレム睡眠状態なら
	else		m_NREMText.Draw();
}