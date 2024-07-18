#include "Snow.h"

//コンストラクタ
Snow::Snow()
	:Task(ETaskTag::eScene,true)
{
	m_snow = COPY_RESOURCE("Snow", CImage);
	float wide = rand() % 1920;				//X座標をランダム
	m_snowPos = CVector2D(wide, -20.0f);	//初期座標
	m_snowAng = 0;							//初期回転値
}

//更新処理
void Snow::Update()
{
	//毎フレーム回転
	m_snowAng += 0.1f;
	//雪の落下
	m_snowPos.y += 2.0f;
	//落下地点が1200を超えたら削除
	if (m_snowPos.y > 1200.0f) Kill();
}

//描画処理
void Snow::Draw()
{
	m_snow.SetSize(64.0f, 64.0f);
	m_snow.SetCenter(32.0f, 32.0f);
	m_snow.SetPos(m_snowPos);
	m_snow.SetAng(m_snowAng);
	m_snow.Draw();
}
