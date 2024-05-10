#include"Enemy.h"
#include"Field/Field.h"
#include"../Player/Player.h"
#include"Navigation/NavNode.h"
#include"Navigation/NavManager.h"
#include"Debug/DebugProfiler.h"
#include"Debug/DebugPrint.h"
#include"EnemyManager.h"

#define MOVE_RANGE_MIN -50.0f
#define MOVE_RANGE_MAX 50.0f

//�ʏ��Ԃ̎���
#define IDLE_TIME 3.0f

//��]���x
#define ROTATE_SPEED 6.0f

Enemy::Enemy(const CVector3D& pos,const CVector3D& dir, const CVector3D& scale,int model)
	:CharaBase(ETaskTag::eEnemy,true)
	, mp_player(nullptr)
	, m_moveDir(0.0f, 0.0f, 0.0f)
	, m_rot_target(0.0f, 0.0f, 0.0f)
	, m_movePos(0.0f, 0.0f, 0.0f)
	, m_moveNode(nullptr)
	, m_elapsedTime(0.0f)
	, m_isvigilance(false)
	, m_state(eState_Idle)
	, m_modelno(model)
	, m_warning(0)
{
	//�G�̊Ǘ��N���X�̃��X�g�Ɏ��g��ǉ�
	EnemyManager::Instance()->Add(this);

	switch (m_modelno)
	{
	case 0:
		m_model = COPY_RESOURCE("Father", CModelA3M);
		break;
	case 1:
		m_model = COPY_RESOURCE("Mother", CModelA3M);
	}
		
	

	m_pos = pos;			//�������W
	m_scale = scale;		//���f���傫��

	m_rad = 0.3f;			//���a
	m_height = 1.8f;		//����

	m_dir = dir;			//��������
	m_moveDir = m_dir;

	//�G�̈ʒu�Ɍo�H�T���p�̃m�[�h���쐬
	m_navNode = new NavNode
	(
		m_pos + CVector3D(0.0f, 1.5f, 0.0f),
		NavNode::NodeType::Destination
	);
	m_navNode->SetNodeColor(CVector3D(0.125f, 0.25f, 1.0f));
}

//�f�X�g���N�^
Enemy::~Enemy()
{
	//�G�̊Ǘ��N���X�̃��X�g���玩�g����菜��
	EnemyManager::Instance()->Remove(this);
	NavManager::Instance()->RemoveNode(m_lostNode);
	//BGM�̒�~
	SOUND("BGM_Chase")->Stop();
}

//�ҋ@��Ԃ̏���
void Enemy::StateIdle()
{
	//�A�j���[�V�����ύX
	m_model.ChangeAnimation((int)AnimId::Idle);

	//�����@�E�A���A���݂ɍĐ�
	if (m_model.GetAnimationFrame() == walk_se_frame[walk_se_idx]) {
		SOUND("SE_WalkMono")->Play3D(m_pos, CVector3D::zero, false);
		walk_se_idx = (walk_se_idx + 1) % 2;
	}

	//�f�o�b�O�p:����̃J���[
	//View_Color();

	m_vec *= 0;

	//�ҋ@����
	if (m_elapsedTime < IDLE_TIME)
	{
		//BGM���ǂ�ǂ񉺂���
		//SOUND("BGM_Chase")->Volume();
		m_elapsedTime += CFPS::GetDeltaTime();
	}
	else
	{
		//������ꂽ���ɉ�������
		SOUND("BGM_Chase")->Stop();
		m_elapsedTime = 0.0f;

		//���ɒT������m�[�h���擾
		SearchNode* node = EnemyManager::Instance()->GetNextSearchNode();
		if (node != nullptr)
		{
			//�T���m�[�h�Ɉ�ԋ߂��m�[�h���ړ��ړI�n�Ƃ���
			m_searchNode = node;
			m_searchNode->enemy = this;
			m_moveNode = NavManager::Instance()->GetNearNavNode(m_searchNode->pos);
		}
		//�T������m�[�h�����݂��Ȃ������ꍇ
		else
		{
			//�ړ��ʒu�������_���ŋ��߂�
			m_movePos.x = Utility::Rand(MOVE_RANGE_MIN, MOVE_RANGE_MAX);
			m_movePos.z = Utility::Rand(MOVE_RANGE_MIN, MOVE_RANGE_MAX);
			m_moveNode = NavManager::Instance()->GetNearNavNode(m_movePos);
		}

		//�ړ���Ԃֈȍ~
		m_state = eState_Move;
	}


	//�v���C���[���T�m�͈͂ɓ�������x�����
	if (IsEyeFoundPlayer() && !mp_player->m_hide) {
		//�x�����
		m_state = eState_Look;
	}
}

//�x�����
void Enemy::StateLook()
{
	//�A�j���[�V�����ύX
	m_model.ChangeAnimation((int)AnimId::Idle);

	m_vec *= 0;
	//�v���C���[�Ƃ̋���
	CVector3D vec = mp_player->m_pos - m_pos;
	m_dir = vec;
	float Length = vec.Length();

	//�v���C���[�Ƃ̋����ɉ����Ĕ����l�̏㏸���ύX
	//�����ɒǂ�������
	if (Length < 5 && IsLookPlayer())
	{
		m_warning = 3;
		//�ǂ�������
		m_state = eState_Chase;
	}
	//�x���l�㏸�F��
	else if (Length < 10 && IsLookPlayer())
	{
		m_warning += CFPS::GetDeltaTime();
		if (m_warning > 3 && IsLookPlayer())
		{
			m_state = eState_Chase;
		}
	}
	//�x���l�㏸�F��
	else if(IsLookPlayer())
	{
		m_warning += CFPS::GetDeltaTime() * 0.5;
		if (m_warning > 3)
		{
			m_state = eState_Chase;
		}
	}
	else
	{
		m_warning -= CFPS::GetDeltaTime();
		if (m_warning < 0)
		{
			m_warning = 0;
			m_state = eState_Idle;
		}
	}


}

void Enemy::StateMove()
{
	//�A�j���[�V�����ύX
	m_model.ChangeAnimation((int)AnimId::Walk);

	//�f�o�b�O�p:����̃J���[
	//View_Color();

	if (m_moveNode != nullptr)
	{
		//���݈ʒu����ړI�n�̃m�[�h�܂ł̌o�H�T�����s��
		NavManager* navMgr = NavManager::Instance();
		m_nextNode = navMgr->Navigate(m_navNode, m_moveNode);

		//���Ɉړ����ׂ��m�[�h�����݂���΁A
		if (m_nextNode != nullptr)
		{
			//���Ɉړ����ׂ��m�[�h�֌����Ĉړ�
			if (MoveTo(m_nextNode->GetPos()))
			{
				//�ړ����I���΁A�ړI�n�̃m�[�h��
				//���Ɉړ����ׂ��m�[�h��null�ɂ���
				m_moveNode = nullptr;
				m_nextNode = nullptr;
				//�T���m�[�h�����݂��Ȃ��ꍇ�́A���̂܂ܑҋ@��Ԃ�
				if (m_searchNode == nullptr)
				{
					m_state = eState_Idle;
				}
			}
		}
	}
	else
	{
		//�T���m�[�h�܂ňړ�
		if (MoveTo(m_searchNode->pos))
		{
			//�ړ����I���΁A
			m_searchNode->prob = 0.0f;
			m_searchNode->enemy = nullptr;
			m_searchNode = nullptr;
			m_state = eState_Idle;
		}
	}

	//�v���C���[���T�m�͈͂ɓ�������x�����
	if (IsEyeFoundPlayer() && !mp_player->m_hide) {
		//�x�����
		m_state = eState_Look;
		//�ړ��������I�����邽�߁A�ړ��֘A�̊e�m�[�h��������
		m_moveNode = nullptr;
		m_nextNode = nullptr;
		//�T�����̃m�[�h�����݂���΁A�T����Ԃ�����
		if (m_searchNode != nullptr)
		{
			m_searchNode->enemy = nullptr;
			m_searchNode = nullptr;
		}
	}
}

//�ǐՏ�Ԃ̏���
void Enemy::StateChase()
{
	//�A�j���[�V�����ύX
	m_model.ChangeAnimation((int)AnimId::Run);

	//�ǐՒ���BGM�̉��ʂ����Z�b�g
	SOUND("BGM_Chase")->Volume(0.3);
	if (SOUND("BGM_Chase")->CheckEnd())
	{
		//�ǐՒ���BGM�Đ��i���[�v����j
		SOUND("BGM_Chase")->Play(true);
	}
	

	//�f�o�b�O�p:����̃J���[
	//View_Color();

	//�e�m�[�h�̍��W���擾
	NavNode* playerNode = mp_player->GetNavNode();
	NavNode* enemyNode = m_navNode;
	CVector3D playerNodePos = playerNode->GetPos();
	CVector3D enemyNodePos = enemyNode->GetPos();
	//���݈ʒu����v���C���[�̈ʒu�܂ł̋��������߂�
	CVector3D vec = playerNodePos - enemyNodePos;
	float dist = vec.Length();

	NavManager* navMgr = NavManager::Instance();

	//�v���C���[�̈ʒu�܂Ŏ������ʂ��Ă��邩����
	CVector3D hitPos, hitNormal;
	bool isHit = Field::CollisionRay(enemyNodePos, playerNodePos, &hitPos, &hitNormal);
	//�v���C���[�̈ʒu�܂Ń��C���΂��āA�����ɏՓ˂�����A
	if (isHit)
	{
		//�v���C���[�܂ł̋��� > �Փ˒n�_�܂ł̋��� �Ȃ�΁A
		//���g�ƃv���C���[�܂ł̊ԂɎՕ��������݂���
		float hitDist = (hitPos - enemyNodePos).Length();
		isHit = dist > hitDist;
	}

	//�v���C���[�̈ʒu�܂ŎՕ��������݂��Ȃ���΁A
	if (!isHit)
	{
		//���݈ʒu����v���C���[�̈ʒu�܂ł̌o�H�T�����s��
		m_nextNode = navMgr->Navigate(enemyNode, playerNode);

		//�����������̍ŏI�ʒu�ɔz�u����m�[�h������
		if (m_lostNode == nullptr)
		{
			m_lostNode = new NavNode
			(
				playerNodePos,
				NavNode::NodeType::Destination
			);
			m_lostNode->SetNodeColor(CVector3D(1.0f, 0.5f, 0.25f));
		}
		//�v���C���[�̍ŏI�ʒu�p�̃m�[�h�����ɑ��݂���Ȃ�΁A
		//�v���C���[�̍ŏI�ʒu���X�V
		else
		{
			m_lostNode->SetPos(playerNodePos);
		}
	}
	//�v���C���[�Ƃ̊ԂɎՕ��������݂���
	else
	{
		//���M�ƃv���C���[�̃m�[�h�̐ڑ���؂�
		enemyNode->RemoveConnect(playerNode);

		//���݈ʒu����v���C���[�̍ŏI�ʒu�܂ł̌o�H�T�����s��
		m_nextNode = navMgr->Navigate(enemyNode, m_lostNode);

		//��������Ԃֈڍs
		m_state = eState_Lost;
		return;
	}

	//���Ɉړ����ׂ��m�[�h�����݂���ꍇ�́A
	//���̃m�[�h�̈ʒu�܂ňړ�
	if (m_nextNode != nullptr)
	{
		MoveTo(m_nextNode->GetPos());
	}
	//���Ɉړ����ׂ��m�[�h�����݂��Ȃ��ꍇ�́A
	//�ҋ@��Ԃֈڍs
	else
	{
		m_state = eState_Idle;
	}
	
}

void Enemy::StateLost()
{
	// �ړI�n�����݂���
	if (m_nextNode != nullptr)
	{
		// ���������ꍇ�́A����͈͂𖳎����āA
		// �v���C���[�܂ł̎������ʂ邩�ǂ����Ŕ��肷��
		if (IsLookPlayer() && !mp_player->m_hide)
		{
			//�ǐՏ�Ԃֈڍs
			m_state = eState_Chase;
		}
		// �v���C���[�������̒ʂ�Ȃ��Ƃ���ɂ���
		else
		{
			//�ړI�n�܂ňړ�
			if (MoveTo(m_nextNode->GetPos()))
			{
				if (m_nextNode != m_lostNode)
				{
					NavManager* navMgr = NavManager::Instance();
					m_nextNode = navMgr->Navigate(m_nextNode, m_lostNode);
				}
				else
				{
					delete m_lostNode;
					m_lostNode = nullptr;
					m_nextNode = nullptr;
				}
			}
		}
	}
	//�ړI�n�܂ňړ����I���΁A
	else
	{
		m_warning = 0;
		//�ҋ@��Ԃֈڍs
		m_state = eState_Idle;
	}
}


void Enemy::View_Color()
{
	switch (m_state)
	{
		//�ʏ��Ԃ͗ΐF
	case eState_Idle:
		color = CVector4D(0.0f, 1.0f, 0.0f, 1.0f);
		break;
		//�v���C���[�ǔ����͐ԐF
	case eState_Chase:
		color = CVector4D(1.0f, 0.0f, 0.0f, 1.0f);
		break;
		//�ړ����͉��F
	case eState_Move:
		color = CVector4D(1.0f, 1.0f, 0.0f, 1.0f);
		break;
		//�v���C���[����������
	case eState_Lost:
		color = CVector4D(1.0f, 1.0f, 0.0f, 1.0f);
		break;
	}
}

bool Enemy::MoveTo(const CVector3D& target)
{
	float moveSpeed = 2.0f;

	CVector3D vec = target - m_pos;
	vec.y = 0.0f;
	m_moveDir = vec.GetNormalize();

	//�ړI�n�܂ł̋������ړ����x���傫���Ȃ��
	float dist = vec.Length();
	float speed = moveSpeed * CFPS::GetDeltaTime();
	if (dist > speed)
	{
		CVector3D moveVec = m_moveDir * moveSpeed;
		m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);
	}
	//�������ړ����x���Z���̂ł���΁A
	else
	{
		CVector3D moveVec = m_moveDir * dist;
		m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);
		//�ړ��I��
		return true;
	}

	//�ړ��p��
	return false;
}

//�v���C���[�T�m�̃t���O->����
bool Enemy::IsEyeFoundPlayer()
{	
	//����̊p�x
	eye_ang = DtoR(60);
	//����̋���
	eye_length = 10;
	//�v���C���[�̍��W
	CVector3D player_pos = mp_player->m_pos;

	//m_dir = CVector3D(sin(m_rot.y), 0, cos(m_rot.y));
	//�G����v���C���[�܂ł̃x�N�g�������߂�
	CVector3D vec = player_pos - m_pos;
	//���߂��x�N�g���ƓG�̐��ʕ����̃x�N�g������ςƂ���
	//�p�x(cos��)�����߂�
	float eye_dot = CVector3D::Dot(m_dir, vec.GetNormalize());
	//�v���C���[�ƓG�̎���p�x�Ŕ͈͓������肷��
	if (eye_dot < cosf(eye_ang))
		return false;

	//�v���C���[�ƓG�̎��싗���Ŕ͈͓������肷��
	if (vec.Length() > eye_length)
		return false;

	//�Ō�Ɏ��M����v���C���[�܂ł̊Ԃ�
	//�Օ������Ȃ������肷��
	if (!IsLookPlayer()) return false;
	
	return true;
}

bool Enemy::IsLookPlayer() const
{
	NavNode* playernodepos = mp_player->GetNavNode();	//�v���C���[�̃m�[�h
	NavNode* enemynodepos = m_navNode;					//�G�l�~�[�̃m�[�h
	CVector3D playerPos = playernodepos->GetPos();		//�v���C���[�̃m�[�h�̍��W
	CVector3D enemyPos = enemynodepos->GetPos();		//�G�l�~�[�̃m�[�h�̍��W
	CVector3D vec = playerPos - enemyPos;		//�v���C���[�ւ̕����x�N�g��
	// ���݈ʒu����v���C���[�܂ł̋��������߂�
	float dist = vec.Length();

	// �v���C���[�̈ʒu�܂ł̃��C�ƕǂƂ̏Փ˔�����s��
	CVector3D start = enemyPos;
	CVector3D end = playerPos;
	//�������l�����Ȃ�
	/*start.y = 1.0f;
	end.y = 1.0f;*/
	CVector3D hitPos, hitNormal;
	if (Field::CollisionRay(start, end, &hitPos, &hitNormal))
	{
		float hitDist = (hitPos - start).Length();
		if (dist > hitDist) return false;
	}

	// �ǂƏՓ˂��Ă��Ȃ��������́A
	// �Փˈʒu���v���C���[��艜�̈ʒu�ł���Ȃ�΁A
	// �������ʂ��Ă���̂ŁA�v���C���[����������
	return true;
}

//�T���m�[�h��ݒ�
void Enemy::SetSearchNode(SearchNode* node)
{
	m_searchNode = node;
}

//�T�����̃m�[�h���擾
SearchNode* Enemy::GetSearchNode() const
{
	return m_searchNode;
}


//�X�V����
void Enemy::Update()
{
	//�v���C���[
	if (!mp_player)	mp_player = dynamic_cast<Player*>(TaskManager::FindObject(ETaskTag::ePlayer));
	

	switch (m_state)
	{
	case eState_Idle:
		StateIdle();
		break;
	case eState_Look:
		StateLook();
		break;
	case eState_Move:
		StateMove();
		break;
	case eState_Chase:
		StateChase();
		break;
	case eState_Lost:
		StateLost();
		break;
	}
	
	

	//�A�j���[�V�����X�V
	m_model.UpdateAnimation();

	//�ړ�
	m_pos += m_vec * CFPS::GetDeltaTime();

	//�v���C���[�̌��������X�Ɉړ������֌�����
	m_dir = CVector3D::Sleap(m_dir, m_moveDir, ROTATE_SPEED * CFPS::GetDeltaTime());
	//�v���C���[�̌������f
	m_rot.y = atan2f(m_dir.x, m_dir.z);

	

	//�x�[�X�N���X�̍X�V
	CharaBase::Update();

	//�x���l�̃Q�[�W
	m_WarningGauge = COBB(
		m_pos + CVector3D(0,2,0),
		CVector3D(0, 0, 0),
		CVector3D(0.05, 0.05, 0.05)
	);

	//float lineWidth = 10.0f;

	//����͈͂̕\��
	/*Utility::DrawLine(m_pos + CVector3D(0, 1.0, 0), m_pos + CVector3D(0, 1.0, 0) + m_dir * eye_length, CVector4D(1, 0, 1, 1), lineWidth);
	Utility::DrawLine(m_pos + CVector3D(0, 1.0, 0), m_pos + CVector3D(0, 1.0, 0) + CMatrix::MRotationY(eye_ang) * m_dir * eye_length, CVector4D(0, 1, 1, 1), lineWidth);
	Utility::DrawLine(m_pos + CVector3D(0, 1.0, 0), m_pos + CVector3D(0, 1.0, 0) + CMatrix::MRotationY(-eye_ang) * m_dir * eye_length, CVector4D(0, 1, 1, 1), lineWidth);
	CMatrix m;
	m.LookAt(m_pos + CVector3D(0, 1.5, 0), m_pos + CVector3D(0, 0.1, 0) + m_dir * eye_length, CVector3D(0, 1, 0));
	Utility::DrawSector(m, -eye_ang, eye_ang, eye_length, color);*/

	//�f�o�b�O�p:�J�v�Z������̕\��
	//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(1, 0, 0, 1));
}

//�`�揈��
void Enemy::Render()
{
	m_model.SetScale(m_scale);	//���f���T�C�Y
	m_model.SetPos(m_pos);		//���f�����W
	m_model.SetRot(m_rot);		//���f����]�l
	m_model.Render();			//���f���`��

	Utility::DrawOBB(m_WarningGauge, CVector4D(m_warning/3,(3-m_warning)/3, 0, 1));
}


void Enemy::Collision(Task* t) 
{
	CharaBase::Collision(t);

	switch (t->GetTag())
	{
	case ETaskTag::eEnemy:
	{
		//�G�ƓG�̔���
		CVector3D c1, dir1, c2, dir2;
		float dist;
		if (CCollision::CollisionCapsule(m_lineS, m_lineE, m_rad,
			t->m_lineS, t->m_lineE, t->m_rad,
			&dist, &c1, &dir1, &c2, &dir2)) 
		{
			float s = (m_rad + t->m_rad) - dist;
			m_pos += dir2 * s * 0.5f;
			m_lineS += dir2 * s * 0.5f;
			m_lineE += dir2 * s * 0.5f;
		}
	}
	break;
	}

}