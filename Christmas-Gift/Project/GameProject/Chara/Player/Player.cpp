#include"Player.h"
#include"Camera/Camera.h"
#include"Chara/Enemy/Enemy.h"
#include"Field/Field.h"
#include"Field/FieldObject/Closet.h"
#include"Filta/Filta.h"
#include"Field/FieldObject/GimmickObjectBase.h"
#include"Camera/Camera.h"
#include"Light/Light.h"
#include"Field/FieldObject/Switch.h"
#include"UI/SleepLife.h"
#include"UI/ToolTips.h"
#include"GameScene/GameScene.h"
#include"Navigation/NavNode.h"
#include"Navigation/NavManager.h"

//�}�N��
#define JUMP 0.2f			//�W�����v��
#define WALK_SPEED 0.04f	//�ʏ�X�s�[�h
#define DOWN_SPEED 0.02f	//���Ⴊ�݃X�s�[�h
#define RUN_SPEED 0.07f		//����X�s�[�h
#define HEIGHT 1.9f			//����
#define RAD 0.4f			//���a


//�R���X�g���N�^
Player::Player(const CVector3D& pos,const CVector3D& rot, const CVector3D& scale)
	:CharaBase(ETaskTag::ePlayer, true)
	, mp_switch(nullptr)
	, m_tooltips(nullptr)
	, key_ang(0.0f)				
	, m_hide(false)											
	, m_state(eState_Idle)		
{
	//�v���C���[���f���ǂݍ���
	m_model = COPY_RESOURCE("Player", CModelA3M);

	m_pos = pos;
	m_scale = scale;

	//�����蔻��̐ݒ�
	m_height = HEIGHT;
	m_rad = RAD;

	//�v���C���[�Ɠ��������̃J�����쐬
	m_camera = new Camera(rot);

	//�v���C���[�ʒu�Ɍo�H�T���p�̃m�[�h���쐬
	m_navNode = new NavNode
	(
		m_pos + CVector3D(0.0f, 1.5f, 0.0f),
		NavNode::NodeType::Destination
	);
	//�m�[�h�̃J���[�I��(��)
	m_navNode->SetNodeColor(CVector3D(1.0f, 0.25f, 0.25f));
}

//�f�X�g���N�^
Player::~Player()
{
}

//�ʏ���
void Player::StateIdle()
{
	m_camera->m_state = m_camera->eState_Idle;
	//�X�y�[�X�{�^������
	if (PUSH(CInput::eButton1) && m_isGround)
	{
		//���n�t���OOFF
		m_isGround = false;
		//�W�����v
		m_vec.y += JUMP;
	}

	//�R���g���[���{�^������
	if (PUSH(CInput::eButton3))
	{
		//���Ⴊ�݃A�j���[�V����
		m_model.ChangeAnimation((int)AnimId::Crouch);
		//���Ⴊ�ݏ�ԂɈڍs
		m_state = eState_Squat;
	}

}

//���Ⴊ�ݏ��
void Player::StateSquat()
{
	m_camera->m_state = m_camera->eState_Idle;
	//���Ⴊ�݃X�s�[�h����
	m_movespeed = DOWN_SPEED;

	//�R���g���[���{�^��
	if (PUSH(CInput::eButton3))
	{
		//�����A�j���[�V����
		m_model.ChangeAnimation((int)AnimId::Stand);
		//�ʏ��Ԃֈڍs
		m_state = eState_Idle;
	}
}

//�n�C�h���
void Player::StateClosetIn()
{
	m_camera->m_state = m_camera->eState_ClosetIn;
	//�{�^�����͂ő��n�C�h���������Ȃ��悤��
	static int count;
	count++;
	//�N���[�[�b�g�̒��S�Ɉړ�
	m_pos = m_Closet_pos;
	if (PUSH(CInput::eMouseL) && count >1)
	{
		SOUND("SE_DoorClose")->Volume(0.3);
		SOUND("SE_DoorClose")->Play();
		//�J�E���g�̏�����
		count = 0;
		//���̈ʒu�֖߂�
		m_pos = m_copy_pos;
		//�n�C�h����
		m_hide = false;
		m_state = eState_Idle;
	}
}

//�X�V����
void Player::Update()
{
	//�X�C�b�`
	if (!mp_switch)mp_switch = dynamic_cast<Switch*>(TaskManager::FindObject(ETaskTag::eFieldObject));

	//�J�������_
	//�L�[���͂���ĂȂ���
	//�J�����̊p�x�ŃL�����N�^�[�̐��ʊp�x�����܂�
	m_rot.y = Utility::NormalizeAngle(m_camera->m_rot.y + key_ang);


	//�t�F�[�h�C���E�t�F�[�h�A�E�g���͏��������Ȃ�
	if (GameData::StartFadeIn || GameData::StartFadeOut)return;

	//�L�[�����x�N�g�������Z�b�g
	CVector3D key_dir = CVector3D(0, 0, 0);

	//�n�C�h���̓L�[���͂��󂯕t���Ȃ�
	if (m_state != eState_ClosetIn)
	{
		//���͂����L�[�����̃x�N�g����ݒ肷��
		if (HOLD(CInput::eUp)) key_dir.z = 1;
		if (HOLD(CInput::eDown))key_dir.z = -1;
		if (HOLD(CInput::eLeft))key_dir.x = 1;
		if (HOLD(CInput::eRight))key_dir.x = -1;
	

		//�L�[���͂��ꂽ��
		if (key_dir.LengthSq() > 0.1)
		{
			//�L�[�̕����x�N�g�����p�x�ɋt�Z����
			key_ang = atan2(key_dir.x, key_dir.z);

			//�����x�N�g��
			CVector3D dir(sin(m_rot.y), 0, cos(m_rot.y));

			//�ړ�
			m_pos += dir * m_movespeed;

			//�ʏ��ԂȂ�
			if (m_state == eState_Idle)
			{
				//�ʏ�������[�V����
				m_model.ChangeAnimation((int)AnimId::Walk);

				//�V�t�g�L�[����
				if (HOLD(CInput::eButton2))
				{
					//�_�b�V���ړ�
					m_movespeed = RUN_SPEED;
					//����������ԂȂ�
					if (SleepLife::m_REM) GameData::FacePosition -= 1.0f;
					//�m������������ԂȂ�
					else GameData::FacePosition -= 0.5f;

					//���艹���I������܂�
					if (SOUND("SE_Run")->CheckEnd())
					{
						//��������~
						SOUND("SE_Walk")->Stop();
						//���艹2�{��
						SOUND("SE_Run")->Pitch(2);

						SOUND("SE_Run")->GetOffset();
						//���艹�Đ�
						SOUND("SE_Run")->Play();
					}
				}
				else
				{
					m_movespeed = WALK_SPEED;

					//�������I������܂�
					if (SOUND("SE_Walk")->CheckEnd())
					{
						//���艹��~
						SOUND("SE_Run")->Stop();
						//������2�{��
						SOUND("SE_Walk")->Pitch(2);
						//�������Đ�
						SOUND("SE_Walk")->Play();
					}
				}
				
			}
			else
			{
				//���Ⴊ�ݕ���
				m_model.ChangeAnimation((int)AnimId::Crouchwalk);
			}
		}
		else
		{
			//�ҋ@�A�j���[�V����
			if (m_state == eState_Idle)
			{
				//�ʏ�ҋ@
				m_model.ChangeAnimation((int)AnimId::Idle);
				//��������~
				SOUND("SE_Walk")->Stop();
				SOUND("SE_Run")->Stop();
			}	
			else
			{
				//���Ⴊ�ݑҋ@
				m_model.ChangeAnimation((int)AnimId::Crouchidle);
				SOUND("SE_Walk")->Stop();
				SOUND("SE_Run")->Stop();
			}
		}
		Shot();
	}
	//�x�[�X�N���X�̍X�V
	if(m_state != eState_ClosetIn)
	CharaBase::Update();

	//�X�e�[�g��Ԃɂ���ԕω�
	switch (m_state) {
		//�ʏ탂�[�h
	case eState_Idle:
		StateIdle();
		break;
		//���Ⴊ�݃��[�h
	case eState_Squat:
		StateSquat();
		break;
		//�n�C�h���[�h
	case eState_ClosetIn:
		StateClosetIn();
		break;
	}

	if (m_state != eState_Squat)
	{
		m_navNode->m_pos = m_pos + CVector3D(0, 1.5, 0);
	}
	else
	{
		m_navNode->m_pos = m_pos + CVector3D(0, 0.5, 0);
	}

	NavNode* node = NavManager::Instance()->GetNearNavNode(m_navNode);

	//�A�j���[�V�����X�V
	m_model.UpdateAnimation();


	//�v���C���[�J�v�Z���̕\��
	//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(1, 0, 0, 1));
}

//�`�揈��
void Player::Render()
{
	m_model.SetPos(m_pos);		//���W
	m_model.SetRot(m_rot);		//��]�l
	m_model.SetScale(m_scale);	//�傫��
	//�ȈՓI����
	//��l�̎����f����`�悵�Ȃ�
	//m_model.Render();

	if (m_state == eState_ClosetIn) return;
	//���C�̕`��i�f�o�b�O�j
	//Utility::DrawLine(m_lS, m_lE, CVector4D(1, 0, 0, 1), 10000);
	//�I�u�W�F�N�g��I������J�[�\��
	Utility::DrawQuad(CVector2D(954, 510), CVector2D(12, 12), CVector4D(0, 1, 0, 1));
}

//�Փˏ���
void Player::Collision(Task* t)
{
	CharaBase::Collision(t);

	switch (t->GetTag())
	{
	case ETaskTag::eEnemy:
	{
		//�v���C���[�ƓG�̔���
		CVector3D c1, dir1, c2, dir2;
		float dist;
		if (CCollision::CollisionCapsule(m_lineS, m_lineE, m_rad,
			t->m_lineS, t->m_lineE, t->m_rad,
			&dist, &c1, &dir1, &c2, &dir2))
		{
				//�Q�[���I�[�o
				GameData::GameOverCheck = true;
				//�t�F�[�h�A�E�g���s
				GameData::StartFadeOut = true;
		}
	}
	break;
	case ETaskTag::eDoor:
	{
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(t->m_CollisionObb, m_lineS, m_lineE, m_rad, &axis, &dist))
		{
			if (axis.y > 0.5f)
			{
				//�ʂ������->�n�ʂɓ�������
				//�d�͗������x��0�ɖ߂�
				if (m_vec.y < 0)
					m_vec.y = 0;
			}
			//�����߂�
			float s = m_rad - dist;
			m_pos += axis * s;
		}
	}
	break;
	}
}

//���C�̏���
void Player::Shot()
{
	//���肷�郌�C�̋���
	const float range = 1.5f;
	CVector3D dir = CMatrix::MRotation(m_camera->m_rot).GetFront();
	//���C�̎n�_
	CVector3D lineS = m_pos + m_camera->m_pos;

	//���C�̏I�_
	CVector3D lineE = m_pos + m_camera->m_pos + dir * range;

	//�ł��߂��I�u�W�F�N�g�ւ̋���
	float dist = FLT_MAX;
	//���C�Ƃ̏Փ˓_
	CVector3D hit_field_point;
	//�Փ˂����X�e�[�W�I�u�W�F�N�g
	Field* hit_field = nullptr;
	if (Field* f = dynamic_cast<Field*>(TaskManager::FindObject(ETaskTag::eField)))
	{
		//�ڐG�ʂ̖@���i�g��Ȃ��j
		CVector3D n;
		if (f->GetColModel()->CollisionRay(&hit_field_point, &n, lineS, lineE))
		{
			//���ˈʒu����ڐG�ʂւ̋���
			dist = (hit_field_point - lineS).LengthSq();
			//�ڐG�����X�e�[�W���X�V
			hit_field = f;
		}
	}
	//�Փ˂����I�u�W�F�N�g
	GimmickObjectBase* hit_object = nullptr;
	//�S�I�u�W�F�N�g��T��
	auto list = TaskManager::FindObjects(ETaskTag::eFieldObject);
	for (auto t : list)
	{
		if (GimmickObjectBase* o = dynamic_cast<GimmickObjectBase*>(t))
		{
			//���C�Ƃ̏Փ˒n�_
			CVector3D c;
			CVector3D n;
			//�e�̐����ŃI�u�W�F�N�g�Ƃ̔�����s��
			if (o->CollisionRay(lineS, lineE, &c, &n))
			{
				//���ˈʒu����ł��߂��I�u�W�F�N�g�𒲂ׂ�
				float l = (c - lineS).LengthSq();
				if (dist > l)
				{
					dist = l;
					hit_object = o;
				}
			}
		}
		//�ڐG�����I�u�W�F�N�g��������ΒT������߂�
		if (hit_object != nullptr)	break;

	}
	//�ł��߂��I�u�W�F�N�g�ɓ�����
	if (hit_object)
	{
		//�c�[���`�b�v�쐬
		if (m_tooltips == nullptr)m_tooltips = new ToolTips();
		//�������Ă���I�u�W�F�N�g�̃i���o�[�ɉ����ď�����ύX
		switch (hit_object->m_objectno)
		{
		case 0:
		{
			m_tooltips->m_Text = "";
			//���N���b�N�œG���W�߂�
			if (PUSH(CInput::eMouseL))
			{

			}
		}
		break;
		case 1:
		{
			if (mp_switch = dynamic_cast<Switch*>(hit_object))
			{
				auto lightlist = TaskManager::FindObjects(ETaskTag::eFieldLight);
				for (auto t : lightlist)
				{
					if (Light* l = dynamic_cast<Light*>(t))
					{
						if (l->m_islight && mp_switch->m_SwitchNo == l->m_roomNo)
						{
							m_tooltips->m_Text = "�d�C������";
							//���N���b�N�œd�C������
							if (PUSH(CInput::eMouseL))
							{
								l->m_islight = false;
							}
						}
						else if (!l->m_islight && mp_switch->m_SwitchNo == l->m_roomNo)
						{
							m_tooltips->m_Text = "�d�C������";
							if (PUSH(CInput::eMouseL))
							{
								l->m_islight = true;
							}
						}
					}
				}
			}
		}
		break;
		//�N���[�[�b�g
		case 2:
		{
			if (m_state == eState_ClosetIn)
			{
				m_tooltips->m_Text = "�o��";
			}
			else
			{
				m_tooltips->m_Text = "�B���";
			}
			

			
			//���N���b�N�ŉB���
			if (PUSH(CInput::eMouseL) && m_state == eState_Idle)
			{
				//�G�ꂽ�N���[�[�b�g�̍��W��ۑ�
				m_Closet_pos = hit_object->m_pos;

				//�G�ꂽ�N���[�[�b�g�̐��ʃx�N�g����ۑ�
				m_Closet_rot = hit_object->m_rot;

				if (PUSH(CInput::eMouseL))
				{
					//���ݒn�ۑ�
					m_copy_pos = m_pos;
					//�n�C�h��Ԃֈڍs
					m_hide = true;
					m_state = eState_ClosetIn;
					//�����Đ�
					SOUND("SE_DoorOpen")->Volume(0.3);
					SOUND("SE_DoorOpen")->Play();
				}
			}

		}
		break;
		}
	}
	else
	{
		if (m_tooltips != nullptr)
		{
			m_tooltips->m_Iskill = true;
			m_tooltips = nullptr;
		}
	}
	m_lS = lineS;
	m_lE = lineE;
}
