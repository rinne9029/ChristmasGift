#include"GameData.h"

int GameData::Stage = GameData::A_1;		//�����X�^�[�g�̉�
bool GameData::ChengeStage = false;			//�}�b�v�ړ��t���O
bool GameData::isGift = false;				//�v���[���g�ݒu�t���O

bool GameData::GameClearCheck = false;
bool GameData::GameOverCheck = false;
bool GameData::StartFadeIn = false;
bool GameData::StartFadeOut = false;
bool GameData::GameStart = false;
bool GameData::isPauseGame = false;

float GameData::second = 0;					//�^�C�}�[

float GameData::FacePosition = 340;			//��UI�z�u