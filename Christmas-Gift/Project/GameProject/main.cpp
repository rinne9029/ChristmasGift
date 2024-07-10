#include"Title/Title.h"
#include"Filta/Filta.h"
#include"Task/TaskManager.h"
#include"Debug/DebugPrint.h"
#include"Debug/DebugProfiler.h"

//--------------------------------------------
//�O���[�o���ϐ��̈�
//-------------------------------------------

//�i�r�m�[�h�`�揈���t���O
bool g_isRenderDebug = false;

//���[�h�����p�ϐ�
int count = 0;
int i = 0;
CImage g_Loading;
CImage g_LoadingText;
CImage g_LoadingBackGround;


void MainLoop(void) {
	//--------------------------------------------------------------
	//�Q�[�����̓����͂����ɏ���
	//�Q�[�����͂��̊֐�_��1�b�Ԃ�60��Ăяo���Ă���
	//--------------------------------------------------------------

	//Q�{�^���Ńi�r�m�[�h�`��؂�ւ�:�f�o�b�O�p
	/*if (PUSH(CInput::eButton11))		
	{
		g_isRenderDebug = !g_isRenderDebug;
	}*/

	//Tab�{�^���ňꎞ��~
	if (PUSH(CInput::eButton4) && GameData::GameStart)
	{
		//�t�F�[�h�C���E�t�F�[�h�A�E�g���͏��������Ȃ�
		if (GameData::StartFadeIn || GameData::StartFadeOut)
		{
		}
		else
		{
			GameData::isPauseGame = !GameData::isPauseGame;
		}
	}

	
	if (GameData::isPauseGame && GameData::GameStart)
	{
		//�|�[�Y���͍X�V���������Ȃ�
	}
	else
	{
		//�S�^�X�N�̍X�V
		DebugProfiler::StartTimer("Update");	//�^�X�N�̍X�V�����v���X�^�[�g
		TaskManager::Instance()->Update();
		DebugProfiler::EndTimer("Update");		//�^�X�N�̍X�V�����v���I��
	}
		
	
	//�S�^�X�N��3D�`��
	DebugProfiler::StartTimer("Render");	//�^�X�N3D�`�揈���v���X�^�[�g
	//�f�B�t�@�[�h�����_�����O
	//���C�g�̐��̕ύX��CLight.h CLight::LIGHT_MAX��CShader.cpp�@413�s��LIGHT_MAX�̒l��ݒ肷��
	if (GameData::GameStart)
	{
		CRendaring::GetInstance()->Render([]() {
			TaskManager::Instance()->Render();
			});
	}
	else
	{
		TaskManager::Instance()->Render();
	}
	
	DebugProfiler::EndTimer("Render");		//�^�X�N��3D�`�揈���v���I��

	//�S�^�X�N��2D�`��
	DebugProfiler::StartTimer("Update");	//�^�X�N��2D�`�揈���v���X�^�[�g
	TaskManager::Instance()->Draw();
	DebugProfiler::EndTimer("Update");		//�^�X�N��2D�`�揈���v���I��

	//�S�^�X�N�̔��菈��
	DebugProfiler::StartTimer("Collision");	//�^�X�N�̔��菈���v���X�^�[�g
	TaskManager::Instance()->Collision();
	DebugProfiler::EndTimer("Collision");	//�^�X�N�̔��菈���v���I��

	//���[�h�I���`�F�b�N
	if (CLoadThread::GetInstance()->CheckEnd())
	{
		//���[�h�O�͉������Ȃ�
	}
	//���[�h���̏���
	else
	{
		count++;
		if (count >= 5)
		{
			i++;
			count = 0;
			if (i >= 8) i = 0;
		}
		//�w�i�`��
		g_LoadingBackGround.Draw();

		//���[�f�B���O�}�[�N�`��
		g_Loading.SetSize(128, 128);
		g_Loading.SetPos(1360, 476);
		g_Loading.SetRect(128 * i, 0, 128 * i + 128, 128);
		g_Loading.Draw();
		
		//���[�f�B���O�����`��
		g_LoadingText.SetPos(510, 476);
		g_LoadingText.Draw();
	}

	

	//float lineWidth = 1.0f;
	//���E�̎���\��
	/*Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(100, 0, 0), CVector4D(1, 0, 0, 1), lineWidth);
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 100, 0), CVector4D(0, 1, 0, 1), lineWidth);
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 0, 100), CVector4D(0, 0, 1, 1), lineWidth);

	for (int i = -5; i <= 5; i++) {
		Utility::DrawLine(CVector3D(-5, 0, i * 1.0f), CVector3D(5, 0, i * 1.0f), CVector4D(0.2, 0.2, 0.2, 1), lineWidth);
		Utility::DrawLine(CVector3D(i * 1.0f, 0, -5), CVector3D(i * 1.0f, 0, 5), CVector4D(0.2, 0.2, 0.2, 1), lineWidth);
	}*/

	//�v�����ʂ̕`��
	//DebugProfiler::Print();

}
void Init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);//�u�����h�̗L����
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//�Œ�V�F�[�_�[�p
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_ALPHA_TEST);

	CFPS::SetFPS(60);
	//�t���[�����䏉����
	CFPS::Init();
	//�{�^���̐ݒ�
	CInput::Init();
	CInput::SetButton(0, CInput::eUp, 'W');
	CInput::SetButton(0, CInput::eDown, 'S');
	CInput::SetButton(0, CInput::eLeft, 'A');
	CInput::SetButton(0, CInput::eRight, 'D');
	CInput::SetButton(0, CInput::eMouseL, VK_LBUTTON);
	CInput::SetButton(0, CInput::eMouseR, VK_RBUTTON);
	CInput::SetButton(0, CInput::eMouseC, VK_MBUTTON);
	CInput::SetButton(0, CInput::eButton1, VK_SPACE);
	CInput::SetButton(0, CInput::eButton2, VK_SHIFT);
	CInput::SetButton(0, CInput::eButton3, VK_CONTROL);
	CInput::SetButton(0, CInput::eButton4, VK_TAB);		

	CInput::SetButton(0, CInput::eButton5, 'I');
	CInput::SetButton(0, CInput::eButton6, 'U');
	CInput::SetButton(0, CInput::eButton7, 'O');
	CInput::SetButton(0, CInput::eButton8, 'F');
	CInput::SetButton(0, CInput::eButton9, 'E');
	CInput::SetButton(0, CInput::eButton10, VK_RETURN);
	CInput::SetButton(0, CInput::eButton11, VK_PAUSE);
	CInput::SetButton(0, CInput::eButton12, 'K');
	

	//�}�E�X��\��
	CInput::ShowCursor(false);
	CInput::SetMouseInside(true);
	CInput::Update();
	CInput::Update();


	//���C�g�ݒ�
	CLight::SetType(0, CLight::eLight_Direction);
	CLight::SetPos(0, CVector3D(0, 200, 200));
	CLight::SetDir(0, CVector3D(-1, -2, 1).GetNormalize());
	//��{�͈Â�
	CLight::SetColor(0, CVector3D(0.1f, 0.1f, 0.1f), CVector3D(0.1f, 0.1f, 0.1f));

	//CLight::SetColor(0, CVector3D(0.2f, 0.2f, 0.2f), CVector3D(0.8f, 0.8f, 0.8f));

	CLight::SetFogParam(CVector4D(1, 1, 1, 1), 700, 800);

	//�J����������
	CCamera::GetCamera()->LookAt(CVector3D(0, 5, 0),
		CVector3D(0, 0, 0),
		CVector3D(0.0, 1.0, 0.0));

	SetCurrentDirectory("data");


	CShader::GetInstance("StaticMesh");
	CShader::GetInstance("SkinMesh");
	CSound::GetInstance();

	//-----------------------------------------------------
	//�������̖��߂�����
	//�Q�[���N�����Ɉ�x�����Ă΂��
	//-----------------------------------------------------
	//�f�B�t�@�[�h�����_�����O�p
	//�e�`��@�\�𐶐�		�`��͈́@�����̍��� �𑜓x
	//CShadow::CreateInscance(40.0f, 20.0f, 8192, 8192);
	//�|�X�g�G�t�F�N�g�𐶐�		��ʉ𑜓x,��ʊE�[�x�I�t�Z�b�g
	CRendaring::CreatInstance(SCREEN_WIDTH, SCREEN_HEIGHT, -0.05);
	
	

	//���[�f�B���O���w�i�ǂݍ���
	ADD_RESOURCE("BackGround", CImage::CreateImage("Loading/BackGround.png"));
	//���[�f�B���O�}�[�N�ǂݍ���
	ADD_RESOURCE("NowLoading", CImage::CreateImage("Loading/NowLoading.png"));
	//���[�f�B���O�e�L�X�g�ǂݍ���
	ADD_RESOURCE("NowLoading_Text", CImage::CreateImage("Loading/NowLoading_Text.png"));
	
	
	g_Loading = COPY_RESOURCE("NowLoading", CImage);
	g_LoadingText = COPY_RESOURCE("NowLoading_Text", CImage);
	g_LoadingBackGround = COPY_RESOURCE("BackGround", CImage);
	
	
	srand(time(NULL));

	//���[�h���Ɏ��s���鏈��
	CLoadThread::GetInstance()->LoadStart([]()
		{
			//�v���C���[���f���ǂݍ���
			ADD_RESOURCE("Player", CModel::CreateModel("Charactor/Santa/Santa.a3m"));

			//�G���f���i�Ƒ��j�ǂݍ���
			ADD_RESOURCE("Father", CModel::CreateModel("Charactor/Enemy/father.a3m"));
			ADD_RESOURCE("Mother", CModel::CreateModel("Charactor/Mother/Mother.a3m"));
			
			//�t�B�[���h�ɐݒu����I�u�W�F�N�g�ǂݍ���
			//�^���X
			ADD_RESOURCE("Closet", CModel::CreateModel("object/tansu2.obj",1,1,1));
			//�v���[���g�{�b�N�X
			ADD_RESOURCE("PresentBox", CModel::CreateModel("object/presentbox.obj", 1, 1, 1));
			//�d�b�@
			ADD_RESOURCE("Telephone", CModel::CreateModel("object/telephone.obj",1,1,1));
			//�d�C�X�C�b�`
			ADD_RESOURCE("Switch", CModel::CreateModel("object/switch.obj",1,1,1));
			//�h�A
			ADD_RESOURCE("Door", CModel::CreateModel("object/door.obj", 1, 1, 1));
			
			//�X�e�[�W�ǂݍ���
			//�`��X�e�[�W
			ADD_RESOURCE("Map", CModel::CreateModel("Field/Field/debugstage1yuka.obj", 10, 10, 10));
			ADD_RESOURCE("WallMap", CModel::CreateModel("Field/Field/debugstage1kabe.obj", 10, 10, 10));
			//�����蔻��X�e�[�W
			ADD_RESOURCE("MapCol", CModel::CreateModel("Field/Field/debugstage1col.obj", 10, 10, 10));
			ADD_RESOURCE("WallMapCol", CModel::CreateModel("Field/Field/debugstage1col.obj", 10, 10, 10));

			//�^�C�g���֘A�摜�ǂݍ���
			ADD_RESOURCE("BackGroundTitle", CImage::CreateImage("Title/BackGroundTitle.png"));
			ADD_RESOURCE("GameTitleText", CImage::CreateImage("Title/TitleText.png"));
			ADD_RESOURCE("StartText", CImage::CreateImage("Title/StartText.png"));
			ADD_RESOURCE("ManualText", CImage::CreateImage("Title/ManualText.png"));
			ADD_RESOURCE("GameRankingText", CImage::CreateImage("Title/RankingText.png"));
			ADD_RESOURCE("Manual1", CImage::CreateImage("Title/Manual1.png"));
			ADD_RESOURCE("Manual2", CImage::CreateImage("Title/Manual2.png"));
			ADD_RESOURCE("Snow", CImage::CreateImage("Title/snow.png"));
			ADD_RESOURCE("Stage1", CImage::CreateImage("Title/stage1.png"));
			ADD_RESOURCE("Stage2", CImage::CreateImage("Title/stage2.png"));
			ADD_RESOURCE("Stage3", CImage::CreateImage("Title/stage3.png"));

			//���U���g�摜�ǂݍ���
			ADD_RESOURCE("ClearBackGround", CImage::CreateImage("Result/ClearBackGround.png"));
			ADD_RESOURCE("OverBackGround", CImage::CreateImage("Result/OverBackGround.png"));
			ADD_RESOURCE("TitleText", CImage::CreateImage("Result/TitleText.png"));
			ADD_RESOURCE("RankingText", CImage::CreateImage("Result/RankingText.png"));
			ADD_RESOURCE("ReStartText", CImage::CreateImage("Result/ReStartText.png"));

			//�t�B���^�[�摜�ǂݍ���
			ADD_RESOURCE("Filta", CImage::CreateImage("Filta/Filta.png"));

			//�c�[���`�b�v�p�摜�ǂݍ���
			ADD_RESOURCE("MouseL", CImage::CreateImage("UI/MouseL.png"));
			ADD_RESOURCE("Text", CImage::CreateImage("UI/Text.png"));
			//UI�摜�ǂݍ���
			ADD_RESOURCE("SleepMeter", CImage::CreateImage("UI/SleepMeter.png"));
			ADD_RESOURCE("UI_Manual", CImage::CreateImage("UI/Manual.png"));

			//�����l�̕\��
			ADD_RESOURCE("Relief", CImage::CreateImage("UI/Relief.png"));	//���v
			ADD_RESOURCE("Usually", CImage::CreateImage("UI/Usually.png"));	//����
			ADD_RESOURCE("Worry", CImage::CreateImage("UI/Worry.png"));		//��΂�
			//������ԕ���
			ADD_RESOURCE("REMsleep", CImage::CreateImage("UI/REMsleep.png"));	//������������
			ADD_RESOURCE("NREMsleep", CImage::CreateImage("UI/NREMsleep.png"));	//�m��������������

			//�G�t�F�N�g�ǂݍ���
			ADD_RESOURCE("Star", CModel::CreateModel("Effect/star.obj"));	//��
			ADD_RESOURCE("Effect_Gool_Side", CModel::CreateModel("Effect/Gool_Effect_Side.obj"));

			//SE
			SOUND("SE_DoorOpen")->Load("Sound/SE/DoorOpen.wav", 1);
			SOUND("SE_DoorClose")->Load("Sound/SE/DoorClose.wav", 1);
			SOUND("SE_Walk")->Load("Sound/SE/Walk.wav", 1);
			SOUND("SE_Run")->Load("Sound/SE/Run.wav", 1);
			SOUND("SE_Click")->Load("Sound/SE/Click.wav", 1);
			SOUND("SE_Select")->Load("Sound/SE/Select.wav", 1);
			SOUND("SE_Clearfanfare")->Load("Sound/SE/Clearfanfare.wav", 1);
			SOUND("SE_GameOver")->Load("Sound/SE/GameOverSE.wav", 1);

			//���m��������(+�񃋁[�v)
			SOUND("SE_WalkMono")->Load("Sound/SE/WalkMono.wav", 8);
			//���� 5m�܂ŕ�������A5m�ȍ~�̓J�b�g, 1.0������
			SOUND("SE_WalkMono")->SetDistance(5.0f, 5.0f, 1.0f);

			//BGM
			SOUND("BGM_TitleOP")->Load("Sound/BGM/TitleOP.wav", 1);
			SOUND("BGM_Chase")->Load("Sound/BGM/Chase.wav", 2);

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

	//��ʉ𑜓x�ϓ�
	CCamera::GetCamera()->SetSize((float)w, (float)h);
	//��ʉ𑜓x�Œ�
	//CCamera::GetCamera()->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	CCamera::GetCamera()->Viewport(0, 0, w, h);
	CCamera::GetCurrent()->Perspective(DtoR(45.0), (float)w / (float)h, 0.7, 10000.0);
	
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

//�t���X�N���[��?�E�C���h�E���[�h�̐؂�ւ�
//Alt+Enter�Ő؂�ւ���
void CheckFullScreen() {
	static int key_enter = 0;
	int key_enter_buf = key_enter;
	if (key_enter_buf ^ (key_enter = glfwGetKey(GL::window, GLFW_KEY_ENTER)) && key_enter && (glfwGetKey(GL::window, GLFW_KEY_LEFT_ALT) || glfwGetKey(GL::window, GLFW_KEY_RIGHT_ALT))) {
		GL::ChangeFullScreen(!GL::full_screen);
	}
}

int __main(int* argcp, char** argv) {
	// ���������[�N���o
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
		//�e�o�b�t�@�[���N���A
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
