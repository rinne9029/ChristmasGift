#include"Title.h"
#include"Ranking/Ranking.h"
#include"Title/Snow.h"
#include"GameScene/GameScene.h"

#define MAX_SELECT 3			//モードセレクト数
#define MAX_STAGE  2			//ステージ数
#define MAX_MANUAL 2			//説明枚数
#define CHANGE_IMAGE_TIME 0.5f	//ステージ画像が下に落ちるまでの時間


//コンストラクタ
Title::Title()
	:Task(ETaskTag::eScene,true)
	,m_state(eState_Title)
	,m_select(1)
	,m_alpha(1.0f)
	,m_elapsedtimeY(0.0f)
	,m_volume(3)
{
	//フェードイン実行
	GameData::StartFadeIn = true;

	//サウンド再生
	SOUND("BGM_TitleOP")->Volume(GameData::volume);
	SOUND("BGM_TitleOP")->Play(true);
	

	//画像読み込み
	m_backgroundtitleimage = COPY_RESOURCE("BackGroundTitle", CImage);
	m_titlerogoimage = COPY_RESOURCE("GameTitleRogo", CImage);
	m_stageimage1 = COPY_RESOURCE("Stage1", CImage);
	m_stageimage2 = COPY_RESOURCE("Stage2", CImage);
	m_collectionimage1 = COPY_RESOURCE("Stage2", CImage);
}

//デストラクタ
Title::~Title()
{
	if (m_state == eState_Stage)
	{
		new GameScene(m_select);
	}
}

//タイトル中の更新処理
void Title::TitleUpdate()
{
	//透明度
	static float a;
	a+=1.0f * 2.0f * CFPS::GetDeltaTime();
	m_alpha = 1.0f - 0.4f * abs(sin(a));

	//スペースボタン
	if (PUSH(CInput::eButton1))
	{
		SOUND("SE_Click")->Volume(GameData::volume);	//音量変更
		SOUND("SE_Click")->Play();			//再生(ループ無し)

		switch (m_select)
		{
		case 1:
			//ステージセレクト画面に移動
			m_state = eState_Stage;
			break;
		case 2:
			//メニュー画面に移動
			m_state = eState_Menu;
			break;
		case 3:
			//コレクション画面に移動
			m_state = eState_Collection;
			break;
		}
	}

	//Aキー入力
	if (PUSH(CInput::eLeft) && m_select > 1)
	{
		SOUND("SE_Select")->Volume(GameData::volume);	//音量設定
		SOUND("SE_Select")->Play();			//再生
		m_select--;
	}
	//Dキー入力
	if (PUSH(CInput::eRight) && m_select < MAX_SELECT)
	{
		SOUND("SE_Select")->Volume(GameData::volume);	//音量設定
		SOUND("SE_Select")->Play();			//再生
		m_select++;
	}

}

//タイトル中の描画処理
void Title::TitleDraw()
{
	//ロゴ描画処理
	m_titlerogoimage.SetPos(610.0f, 50.0f);
	m_titlerogoimage.SetSize(700.0f, 700.0f);
	m_titlerogoimage.Draw();

	//選択中の文字に背景カラーをつける
	Utility::DrawQuad(CVector2D(250.0f + 540 * (m_select - 1), 760.0f), CVector2D(380.0f, 170.0f), CVector4D(1.0f, 0.3f, 0.0f, m_alpha));

	//文字描画処理
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
	)->Draw(870, 880, 0.0f, 0.0f, 0.0f, "Menu");

	CREATE_FONT_F
	(
		"name",
		"C:\\Windows\\Fonts\\BOD_CI.TTF",
		128
	)->Draw(1350, 880, 0.0f, 0.0f, 0.0f, "Collection");
}

void Title::StageSelectUpdate()
{
	//スペースボタン
	if (PUSH(CInput::eButton1))
	{
		SOUND("SE_Click")->Volume(GameData::volume);
		SOUND("SE_Click")->Play();
		SOUND("BGM_TitleOP")->Stop();
		GameData::StartFadeOut = true;
	}

	//Sキー入力
	if (PUSH(CInput::eDown))
	{
		SOUND("SE_Click")->Volume(GameData::volume);
		SOUND("SE_Click")->Play();
		//タイトル画面セレクトに戻る
		m_state = eState_Title;
		m_select = 1;
		//経過時間リセット
		m_elapsedtimeY = 0.0f;
		m_imageposY = 0.0f;
	}

	//Aキー入力
	//次のステージ
	if (PUSH(CInput::eLeft) && m_select > 1)
	{
		SOUND("SE_Select")->Volume(GameData::volume);
		SOUND("SE_Select")->Play();
		m_select--;
	}
	//Dキー入力
	//前のステージ
	if (PUSH(CInput::eRight) && m_select < MAX_STAGE)
	{
		SOUND("SE_Select")->Volume(GameData::volume);
		SOUND("SE_Select")->Play();
		m_select++;
	}
}

void Title::StageSelectDraw()
{	
	//経過時間
	if (m_elapsedtimeY < CHANGE_IMAGE_TIME)
	{
		m_elapsedtimeY += CFPS::GetDeltaTime();
	}
	else
	{
		m_elapsedtimeY == CHANGE_IMAGE_TIME;
	}
	//画面出現後ステージ1画像を上から下に移動させる
	m_imageposY = 1080 * (m_elapsedtimeY / CHANGE_IMAGE_TIME);

	//ステージ画像1
	m_stageimage1.SetSize(CVector2D(1600, 900));
	m_stageimage1.SetPos(160.0f - 1920 * (m_select - 1), 90.0f - 1080.0f + m_imageposY);
	m_stageimage1.Draw();

	//ステージ画像2
	m_stageimage2.SetSize(CVector2D(1600, 900));
	m_stageimage2.SetPos(2080.0f - 1920 * (m_select - 1), 90.0f - 1080.0f + m_imageposY);
	m_stageimage2.Draw();

}

void Title::TitleMenuUpdate()
{
	//Sキー入力
	if (PUSH(CInput::eDown))
	{
		SOUND("SE_Click")->Volume(GameData::volume);
		SOUND("SE_Click")->Play();
		SOUND("BGM_TitleOP")->Stop();
		//タイトルセレクトに戻る
		m_state = eState_Title;
		m_select = 1;
		SOUND("BGM_TitleOP")->Volume(GameData::volume);
		SOUND("BGM_TitleOP")->Play(true);
	}

	//Aキー入力
	//音量を下げる
	if (PUSH(CInput::eLeft) && m_volume > 0)
	{
		m_volume--;
		GameData::volume = m_volume * 0.2f;
		SOUND("SE_Select")->Volume(GameData::volume);
		SOUND("SE_Select")->Play();
	}
	//Dキー入力
	//音量を上げる
	if (PUSH(CInput::eRight) && m_volume < 5)
	{
		m_volume++;
		GameData::volume = m_volume * 0.2f;
		SOUND("SE_Select")->Volume(GameData::volume);
		SOUND("SE_Select")->Play();
	}
}

void Title::TitleMenuDraw()
{
	//選択中の文字に背景カラーをつける
	Utility::DrawQuad(CVector2D(0.0f, 0.0f), CVector2D(1920.0f, 1080.0f), CVector4D(1.0f, 1.0f, 1.0f, 0.4f));

	CREATE_FONT_F
	(
		"name",
		"C:\\Windows\\Fonts\\BOD_CI.TTF",
		128
	)->Draw(350, 540, 0.0f, 0.0f, 0.0f, "Sound");

}

void Title::CollectionUpdate()
{
}

void Title::CollectionDraw()
{
	//コレクション未取得
	m_collectionimage1.SetColor(0, 0, 0, 1);
	m_collectionimage2.SetColor(0, 0, 0, 1);
	m_collectionimage3.SetColor(0, 0, 0, 1);
	//コレクション取得
	m_collectionimage1.SetColor(1, 1, 1, 1);
	m_collectionimage2.SetColor(1, 1, 1, 1);
	m_collectionimage3.SetColor(1, 1, 1, 1);
	//描画処理
	m_collectionimage1.Draw();
	m_collectionimage2.Draw();
	m_collectionimage3.Draw();
}

//更新処理
void Title::Update()
{
	if (GameData::StartFadeIn) return;
	if (GameData::StartFadeOut) return;

	switch (m_state)
	{
	case eState_Title:
		TitleUpdate();
		break;
	case eState_Stage:
		StageSelectUpdate();
		break;
	case eState_Menu:
		TitleMenuUpdate();
		break;
	case eState_Collection:
		CollectionUpdate();
		break;
	}
}

//2D描画処理
void Title::Draw()
{
	m_backgroundtitleimage.Draw();


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

	switch (m_state)
	{
	case eState_Title:
		TitleDraw();
		break;
	case eState_Stage:
		StageSelectDraw();
		break;
	case eState_Menu:
		TitleMenuDraw();
		break;
	case eState_Collection:
		CollectionDraw();
		break;
	}
	
}