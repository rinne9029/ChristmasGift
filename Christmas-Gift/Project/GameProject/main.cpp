#include"Title/Title.h"			//タイトル
#include"Filta/Filta.h"			//フィルター
#include"Task/TaskManager.h"	
#include"Debug/DebugPrint.h"
#include"Debug/DebugProfiler.h"

//--------------------------------------------
//グローバル変数領域
//-------------------------------------------

//ナビノード描画処理フラグ:デバッグ用
bool g_isRenderDebug = false;

//ロード処理用変数
int count = 0;
int i = 0;
CImage g_Loading;
CImage g_LoadingText;

void MainLoop(void) {
	//--------------------------------------------------------------
	//ゲーム中の動きはここに書く
	//ゲーム中はこの関数_を1秒間に60回呼び出している
	//--------------------------------------------------------------

	//Qボタンでナビノード描画切り替え:デバッグ用
	/*if (PUSH(CInput::eButton11))		
	{
		g_isRenderDebug = !g_isRenderDebug;
	}*/

	//全タスクの更新
	DebugProfiler::StartTimer("Update");	//タスクの更新処理計測スタート
	TaskManager::Instance()->Update();
	DebugProfiler::EndTimer("Update");		//タスクの更新処理計測終了

	//全タスクの3D描画
	DebugProfiler::StartTimer("Render");	//タスク3D描画処理計測スタート
	//ディファードレンダリング
	//ライトの数の変更はCLight.h CLight::LIGHT_MAXとCShader.cpp　413行目LIGHT_MAXの値を設定する
	CRendaring::GetInstance()->Render([]() {
		TaskManager::Instance()->Render();
		});
	DebugProfiler::EndTimer("Render");		//タスクの3D描画処理計測終了

	//全タスクの2D描画
	DebugProfiler::StartTimer("Update");	//タスクの2D描画処理計測スタート
	TaskManager::Instance()->Draw();
	DebugProfiler::EndTimer("Update");		//タスクの2D描画処理計測終了

	//全タスクの判定処理
	DebugProfiler::StartTimer("Collision");	//タスクの判定処理計測スタート
	TaskManager::Instance()->Collision();
	DebugProfiler::EndTimer("Collision");	//タスクの判定処理計測終了

	//ロード終了チェック
	if (CLoadThread::GetInstance()->CheckEnd())
	{
		//ロード外は何もしない
	}
	//ロード中の処理
	else
	{
		count++;
		if (count >= 5)
		{
			i++;
			count = 0;
			if (i >= 8) i = 0;
		}
		g_Loading.SetSize(128, 128);
		g_Loading.SetPos(1700, 900);
		g_Loading.SetRect(128 * i, 0, 128 * i + 128, 128);
		g_Loading.Draw();
		
		//ローディング文字表示
		g_LoadingText.SetPos(850, 900);
		g_LoadingText.Draw();
	}

	//float lineWidth = 1.0f;

	//世界の軸を表示
	/*Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(100, 0, 0), CVector4D(1, 0, 0, 1), lineWidth);
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 100, 0), CVector4D(0, 1, 0, 1), lineWidth);
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 0, 100), CVector4D(0, 0, 1, 1), lineWidth);

	for (int i = -5; i <= 5; i++) {
		Utility::DrawLine(CVector3D(-5, 0, i * 1.0f), CVector3D(5, 0, i * 1.0f), CVector4D(0.2, 0.2, 0.2, 1), lineWidth);
		Utility::DrawLine(CVector3D(i * 1.0f, 0, -5), CVector3D(i * 1.0f, 0, 5), CVector4D(0.2, 0.2, 0.2, 1), lineWidth);
	}*/

	//計測結果の描画
	//DebugProfiler::Print();

}
void Init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);//ブレンドの有効化
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//固定シェーダー用
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_ALPHA_TEST);

	CFPS::SetFPS(60);
	//フレーム制御初期化
	CFPS::Init();
	//ボタンの設定
	CInput::Init();
	CInput::SetButton(0, CInput::eButton1, 'Z');
	CInput::SetButton(0, CInput::eButton2, 'X');
	CInput::SetButton(0, CInput::eButton3, 'C');
	CInput::SetButton(0, CInput::eButton4, 'V');
	CInput::SetButton(0, CInput::eButton5, VK_SPACE);
	CInput::SetButton(0, CInput::eButton6, VK_SHIFT);
	CInput::SetButton(0, CInput::eButton7, VK_CONTROL);
	CInput::SetButton(0, CInput::eButton8, 'F');
	CInput::SetButton(0, CInput::eButton9, 'E');
	CInput::SetButton(0, CInput::eButton10, VK_RETURN);
	CInput::SetButton(0, CInput::eButton11, 'Q');
	CInput::SetButton(0, CInput::eButton12, 'K');	//ノード配置確認：デバッグ用
	CInput::SetButton(0, CInput::eUp, 'W');
	CInput::SetButton(0, CInput::eDown, 'S');
	CInput::SetButton(0, CInput::eLeft, 'A');
	CInput::SetButton(0, CInput::eRight, 'D');
	CInput::SetButton(0, CInput::eMouseL, VK_LBUTTON);
	CInput::SetButton(0, CInput::eMouseR, VK_RBUTTON);
	CInput::SetButton(0, CInput::eMouseC, VK_MBUTTON);

	//マウス非表示
	CInput::ShowCursor(false);
	CInput::SetMouseInside(true);
	CInput::Update();
	CInput::Update();


	//ライト設定
	CLight::SetType(0, CLight::eLight_Direction);
	CLight::SetPos(0, CVector3D(0, 200, 200));
	CLight::SetDir(0, CVector3D(-1, -2, 1).GetNormalize());
	//基本は暗く
	CLight::SetColor(0, CVector3D(0.1f, 0.1f, 0.1f), CVector3D(0.1f, 0.1f, 0.1f));

	//CLight::SetColor(0, CVector3D(0.2f, 0.2f, 0.2f), CVector3D(0.8f, 0.8f, 0.8f));

	CLight::SetFogParam(CVector4D(1, 1, 1, 1), 700, 800);

	//カメラ初期化
	CCamera::GetCamera()->LookAt(CVector3D(0, 5, 0),
		CVector3D(0, 0, 0),
		CVector3D(0.0, 1.0, 0.0));

	SetCurrentDirectory("data");


	CShader::GetInstance("StaticMesh");
	CShader::GetInstance("SkinMesh");
	CSound::GetInstance();

	//-----------------------------------------------------
	//初期化の命令を書く
	//ゲーム起動時に一度だけ呼ばれる
	//-----------------------------------------------------
	//ディファードレンダリング用
	//影描画機能を生成		描画範囲　光源の高さ 解像度
	//CShadow::CreateInscance(40.0f, 20.0f, 8192, 8192);
	//ポストエフェクトを生成		画面解像度,被写界深度オフセット
	CRendaring::CreatInstance(SCREEN_WIDTH, SCREEN_HEIGHT, -0.05);

	//ローディングマーク読み込み
	ADD_RESOURCE("NowLoading", CImage::CreateImage("UI/NowLoading.png"));
	//ローディングテキスト読み込み
	ADD_RESOURCE("NowLoading_Text", CImage::CreateImage("UI/NowLoading_Text.png"));
	g_Loading = COPY_RESOURCE("NowLoading", CImage);
	g_LoadingText = COPY_RESOURCE("NowLoading_Text", CImage);

	//ロード中に実行する処理
	CLoadThread::GetInstance()->LoadStart([]()
		{
			//プレイヤーモデル読み込み
			ADD_RESOURCE("Player", CModel::CreateModel("Charactor/Santa/Santa.a3m"));

			//敵モデル（家族）読み込み
			ADD_RESOURCE("Father", CModel::CreateModel("Charactor/Enemy/father.a3m"));
			
			//タンス読み込み
			ADD_RESOURCE("Closet", CModel::CreateModel("object/tansu2.obj",1,1,1));
			//プレゼントボックス読み込み
			ADD_RESOURCE("PresentBox", CModel::CreateModel("object/presentbox.obj", 1, 1, 1));
			//電話機読み込み
			ADD_RESOURCE("Telephone", CModel::CreateModel("object/telephone.obj",1,1,1));
			//電気スイッチ読み込み
			ADD_RESOURCE("Switch", CModel::CreateModel("object/switch.obj",1,1,1));
			//ドア読み込み
			ADD_RESOURCE("Door", CModel::CreateModel("object/door.obj", 1, 1, 1));
			
			//タイトル画像読み込み
			ADD_RESOURCE("StartText", CImage::CreateImage("Title/StartText.png"));
			ADD_RESOURCE("ManualText", CImage::CreateImage("Title/ManualText.png"));
			ADD_RESOURCE("RankingText", CImage::CreateImage("Title/RankingText.png"));

			//リザルト画像読み込み
			ADD_RESOURCE("ClearBackGround", CImage::CreateImage("Result/ClearBackGround.png"));
			ADD_RESOURCE("OverBackGround", CImage::CreateImage("Result/OverBackGround.png"));
			ADD_RESOURCE("TitleText", CImage::CreateImage("Result/TitleText.png"));
			ADD_RESOURCE("RankingText", CImage::CreateImage("Result/RankingText.png"));
			ADD_RESOURCE("ReStartText", CImage::CreateImage("Result/ReStartText.png"));

			//フィルター画像読み込み
			ADD_RESOURCE("Filta", CImage::CreateImage("Filta/Filta.png"));

			//ツールチップ用画像読み込み
			ADD_RESOURCE("MouseL", CImage::CreateImage("UI/MouseL.png"));
			ADD_RESOURCE("Text", CImage::CreateImage("UI/Text.png"));
			//UI画像読み込み
			ADD_RESOURCE("REMsleep", CImage::CreateImage("UI/REMsleep.png"));
			ADD_RESOURCE("NREMsleep", CImage::CreateImage("UI/NREMsleep.png"));

			//ステージ読み込み
			//描画ステージ
			ADD_RESOURCE("Map", CModel::CreateModel("Field/Field/debugstage1yuka.obj", 7, 7, 7));
			ADD_RESOURCE("WallMap", CModel::CreateModel("Field/Field/debugstage1kabe.obj", 7, 7, 7));
			//当たり判定ステージ
			ADD_RESOURCE("MapCol", CModel::CreateModel("Field/Field/debugstage1col.obj", 7, 7, 7));
			ADD_RESOURCE("WallMapCol", CModel::CreateModel("Field/Field/debugstage1col.obj", 7, 7, 7));
			
			new Filta();
			new Title();
		});

}


void Release()
{
	TaskManager::ClearInstance();
	CLoadThread::ClearInstance();
	CSound::ClearInstance();
	CResourceManager::ClearInstance();
}

static void ResizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);

	//画面解像度変動
	CCamera::GetCamera()->SetSize((float)w, (float)h);
	//画面解像度固定
	//CCamera::GetCamera()->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	CCamera::GetCamera()->Viewport(0, 0, w, h);
	CCamera::GetCurrent()->Perspective(DtoR(45.0), (float)w / (float)h, 1.0, 10000.0);
	
	glfwGetWindowPos(window, &GL::window_x, &GL::window_y);
	GL::UpdateWindowRect(GL::window_x, GL::window_y, w, h);
	CInput::UpdateClipCursor(true);

}
static void WheelCallback(GLFWwindow* _window, double _offsetx, double _offsety) {
	CInput::AddMouseWheel((int)_offsety);

}
static void PosCallback(GLFWwindow* _window, int x, int y) {
	GL::window_x = x;
	GL::window_y = y;
	GL::UpdateWindosRect(x, y, GL::window_width, GL::window_height);
	CInput::UpdateClipCursor(true);

}
static void FocusCallback(GLFWwindow* _window, int f) {
	CInput::UpdateClipCursor(f);
	GL::focus = f;
}

static void error_callback(int error, const char* description)
{
	printf("Error: %s\n", description);
}

//フルスクリーン?ウインドウモードの切り替え
//Alt+Enterで切り替える
void CheckFullScreen() {
	static int key_enter = 0;
	int key_enter_buf = key_enter;
	if (key_enter_buf ^ (key_enter = glfwGetKey(GL::window, GLFW_KEY_ENTER)) && key_enter && (glfwGetKey(GL::window, GLFW_KEY_LEFT_ALT) || glfwGetKey(GL::window, GLFW_KEY_RIGHT_ALT))) {
		GL::ChangeFullScreen(!GL::full_screen);
	}
}

int __main(int* argcp, char** argv) {
	// メモリリーク検出
	//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	//OpenGL4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) return -1;
	//	glutInit(argcp, argv);

	GL::window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple", nullptr, nullptr);
	glfwGetWindowSize(GL::window, &GL::window_width, &GL::window_height);
	glfwGetWindowPos(GL::window, &GL::window_x, &GL::window_y);

	glfwSetFramebufferSizeCallback(GL::window, ResizeCallback);
	glfwSetScrollCallback(GL::window, WheelCallback);
	glfwSetWindowFocusCallback(GL::window, FocusCallback);
	glfwSetWindowPosCallback(GL::window, PosCallback);
	if (!GL::window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(GL::window);
	glfwSwapInterval(1);
	ResizeCallback(GL::window, SCREEN_WIDTH, SCREEN_HEIGHT);

	GLenum err = glewInit();
	if (err == GLEW_OK) {
		printf("%s\n", glewGetString(GLEW_VERSION));
	}
	else {
		printf("%s\n", glewGetErrorString(err));
		getchar();
		return -1;
	}
	HDC glDc = wglGetCurrentDC();
	GL::hWnd = WindowFromDC(glDc);

	Init();
	while (!glfwWindowShouldClose(GL::window)) {
		CheckFullScreen();


		CInput::Update();
		//各バッファーをクリア
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MainLoop();


		glfwSwapBuffers(GL::window);


		CFPS::Wait();

		char title[32];
		sprintf_s(title, "math fps:%d dt:%.3f", CFPS::GetFPS(), CFPS::GetDeltaTime());
		glfwSetWindowTitle(GL::window, title);

		glfwPollEvents();
		if (glfwGetKey(GL::window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(GL::window, GL_TRUE);
		}

	}

	glfwTerminate();




	Release();
	return 0;
}

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR argv, INT argc)
{
	return __main(&__argc, __argv);
}

int main(int argc, char** argv)
{
	return __main(&argc, argv);
}
