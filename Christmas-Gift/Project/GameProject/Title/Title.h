#pragma once

//タイトル画面管理クラス

class Title : public Task {
private:
	enum TitleScene
	{
		eState_Title,
		eState_Stage,
		eState_Menu,
		eState_Collection
	};
	int m_state;					//現在のステート
	int m_select;					//セレクトナンバー
	float m_alpha;					//透明度
	float m_elapsedtimeY;			//Y座標を動かす経過時間	
	float m_imageposY;				//画像のY座標
	float m_volume;					//音量

	CImage m_backgroundtitleimage;	//タイトル背景
	CImage m_titlerogoimage;		//タイトル名テキスト
	CImage m_stageimage1;			//ステージ１
	CImage m_stageimage2;			//ステージ２
	CImage m_collectionimage1;		//コレクション画像1
	CImage m_collectionimage2;		//コレクション画像2
	CImage m_collectionimage3;		//コレクション画像3

	void TitleUpdate();			//タイトル画面の更新処理
	void TitleDraw();			//タイトル画面の描画処理
	void StageSelectUpdate();	//ステージ選択処理
	void StageSelectDraw();		//ステージ選択描画
	void TitleMenuUpdate();		//タイトルメニュー画面の更新処理
	void TitleMenuDraw();		//タイトルメニュー画面の描画処理
	void CollectionUpdate();	//コレクション画面の更新処理
	void CollectionDraw();		//コレクション画面の描画処理
public:
	//コンストラクタ
	Title();
	//デストラクタ
	~Title();
	//更新処理
	void Update();
	//2D描画処理
	void Draw();
};