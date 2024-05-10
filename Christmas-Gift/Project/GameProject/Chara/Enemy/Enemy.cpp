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

//通常状態の時間
#define IDLE_TIME 3.0f

//回転速度
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
	//敵の管理クラスのリストに自身を追加
	EnemyManager::Instance()->Add(this);

	switch (m_modelno)
	{
	case 0:
		m_model = COPY_RESOURCE("Father", CModelA3M);
		break;
	case 1:
		m_model = COPY_RESOURCE("Mother", CModelA3M);
	}
		
	

	m_pos = pos;			//初期座標
	m_scale = scale;		//モデル大きさ

	m_rad = 0.3f;			//半径
	m_height = 1.8f;		//高さ

	m_dir = dir;			//初期方向
	m_moveDir = m_dir;

	//敵の位置に経路探索用のノードを作成
	m_navNode = new NavNode
	(
		m_pos + CVector3D(0.0f, 1.5f, 0.0f),
		NavNode::NodeType::Destination
	);
	m_navNode->SetNodeColor(CVector3D(0.125f, 0.25f, 1.0f));
}

//デストラクタ
Enemy::~Enemy()
{
	//敵の管理クラスのリストから自身を取り除く
	EnemyManager::Instance()->Remove(this);
	NavManager::Instance()->RemoveNode(m_lostNode);
	//BGMの停止
	SOUND("BGM_Chase")->Stop();
}

//待機状態の処理
void Enemy::StateIdle()
{
	//アニメーション変更
	m_model.ChangeAnimation((int)AnimId::Idle);

	//足音　右、左、交互に再生
	if (m_model.GetAnimationFrame() == walk_se_frame[walk_se_idx]) {
		SOUND("SE_WalkMono")->Play3D(m_pos, CVector3D::zero, false);
		walk_se_idx = (walk_se_idx + 1) % 2;
	}

	//デバッグ用:視野のカラー
	//View_Color();

	m_vec *= 0;

	//待機時間
	if (m_elapsedTime < IDLE_TIME)
	{
		//BGMをどんどん下げる
		//SOUND("BGM_Chase")->Volume();
		m_elapsedTime += CFPS::GetDeltaTime();
	}
	else
	{
		//逃げられた時に音を消す
		SOUND("BGM_Chase")->Stop();
		m_elapsedTime = 0.0f;

		//次に探索するノードを取得
		SearchNode* node = EnemyManager::Instance()->GetNextSearchNode();
		if (node != nullptr)
		{
			//探索ノードに一番近いノードを移動目的地とする
			m_searchNode = node;
			m_searchNode->enemy = this;
			m_moveNode = NavManager::Instance()->GetNearNavNode(m_searchNode->pos);
		}
		//探索するノードが存在しなかった場合
		else
		{
			//移動位置をランダムで求める
			m_movePos.x = Utility::Rand(MOVE_RANGE_MIN, MOVE_RANGE_MAX);
			m_movePos.z = Utility::Rand(MOVE_RANGE_MIN, MOVE_RANGE_MAX);
			m_moveNode = NavManager::Instance()->GetNearNavNode(m_movePos);
		}

		//移動状態へ以降
		m_state = eState_Move;
	}


	//プレイヤーが探知範囲に入ったら警戒状態
	if (IsEyeFoundPlayer() && !mp_player->m_hide) {
		//警戒状態
		m_state = eState_Look;
	}
}

//警戒状態
void Enemy::StateLook()
{
	//アニメーション変更
	m_model.ChangeAnimation((int)AnimId::Idle);

	m_vec *= 0;
	//プレイヤーとの距離
	CVector3D vec = mp_player->m_pos - m_pos;
	m_dir = vec;
	float Length = vec.Length();

	//プレイヤーとの距離に応じて発見値の上昇率変更
	//即座に追いかける
	if (Length < 5 && IsLookPlayer())
	{
		m_warning = 3;
		//追いかける
		m_state = eState_Chase;
	}
	//警戒値上昇：大
	else if (Length < 10 && IsLookPlayer())
	{
		m_warning += CFPS::GetDeltaTime();
		if (m_warning > 3 && IsLookPlayer())
		{
			m_state = eState_Chase;
		}
	}
	//警戒値上昇：小
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
	//アニメーション変更
	m_model.ChangeAnimation((int)AnimId::Walk);

	//デバッグ用:視野のカラー
	//View_Color();

	if (m_moveNode != nullptr)
	{
		//現在位置から目的地のノードまでの経路探索を行う
		NavManager* navMgr = NavManager::Instance();
		m_nextNode = navMgr->Navigate(m_navNode, m_moveNode);

		//次に移動すべきノードが存在すれば、
		if (m_nextNode != nullptr)
		{
			//次に移動すべきノードへ向けて移動
			if (MoveTo(m_nextNode->GetPos()))
			{
				//移動が終われば、目的地のノードと
				//次に移動すべきノードをnullにする
				m_moveNode = nullptr;
				m_nextNode = nullptr;
				//探索ノードが存在しない場合は、そのまま待機状態へ
				if (m_searchNode == nullptr)
				{
					m_state = eState_Idle;
				}
			}
		}
	}
	else
	{
		//探索ノードまで移動
		if (MoveTo(m_searchNode->pos))
		{
			//移動が終われば、
			m_searchNode->prob = 0.0f;
			m_searchNode->enemy = nullptr;
			m_searchNode = nullptr;
			m_state = eState_Idle;
		}
	}

	//プレイヤーが探知範囲に入ったら警戒状態
	if (IsEyeFoundPlayer() && !mp_player->m_hide) {
		//警戒状態
		m_state = eState_Look;
		//移動を強制終了するため、移動関連の各ノードを初期化
		m_moveNode = nullptr;
		m_nextNode = nullptr;
		//探索中のノードが存在すれば、探索状態を解除
		if (m_searchNode != nullptr)
		{
			m_searchNode->enemy = nullptr;
			m_searchNode = nullptr;
		}
	}
}

//追跡状態の処理
void Enemy::StateChase()
{
	//アニメーション変更
	m_model.ChangeAnimation((int)AnimId::Run);

	//追跡中のBGMの音量をリセット
	SOUND("BGM_Chase")->Volume(0.3);
	if (SOUND("BGM_Chase")->CheckEnd())
	{
		//追跡中のBGM再生（ループあり）
		SOUND("BGM_Chase")->Play(true);
	}
	

	//デバッグ用:視野のカラー
	//View_Color();

	//各ノードの座標を取得
	NavNode* playerNode = mp_player->GetNavNode();
	NavNode* enemyNode = m_navNode;
	CVector3D playerNodePos = playerNode->GetPos();
	CVector3D enemyNodePos = enemyNode->GetPos();
	//現在位置からプレイヤーの位置までの距離を求める
	CVector3D vec = playerNodePos - enemyNodePos;
	float dist = vec.Length();

	NavManager* navMgr = NavManager::Instance();

	//プレイヤーの位置まで視線が通っているか判定
	CVector3D hitPos, hitNormal;
	bool isHit = Field::CollisionRay(enemyNodePos, playerNodePos, &hitPos, &hitNormal);
	//プレイヤーの位置までレイを飛ばして、何かに衝突したら、
	if (isHit)
	{
		//プレイヤーまでの距離 > 衝突地点までの距離 ならば、
		//自身とプレイヤーまでの間に遮蔽物が存在する
		float hitDist = (hitPos - enemyNodePos).Length();
		isHit = dist > hitDist;
	}

	//プレイヤーの位置まで遮蔽物が存在しなければ、
	if (!isHit)
	{
		//現在位置からプレイヤーの位置までの経路探索を行う
		m_nextNode = navMgr->Navigate(enemyNode, playerNode);

		//見失った時の最終位置に配置するノードを準備
		if (m_lostNode == nullptr)
		{
			m_lostNode = new NavNode
			(
				playerNodePos,
				NavNode::NodeType::Destination
			);
			m_lostNode->SetNodeColor(CVector3D(1.0f, 0.5f, 0.25f));
		}
		//プレイヤーの最終位置用のノードが既に存在するならば、
		//プレイヤーの最終位置を更新
		else
		{
			m_lostNode->SetPos(playerNodePos);
		}
	}
	//プレイヤーとの間に遮蔽物が存在する
	else
	{
		//自信とプレイヤーのノードの接続を切る
		enemyNode->RemoveConnect(playerNode);

		//現在位置からプレイヤーの最終位置までの経路探索を行う
		m_nextNode = navMgr->Navigate(enemyNode, m_lostNode);

		//見失い状態へ移行
		m_state = eState_Lost;
		return;
	}

	//次に移動すべきノードが存在する場合は、
	//そのノードの位置まで移動
	if (m_nextNode != nullptr)
	{
		MoveTo(m_nextNode->GetPos());
	}
	//次に移動すべきノードが存在しない場合は、
	//待機状態へ移行
	else
	{
		m_state = eState_Idle;
	}
	
}

void Enemy::StateLost()
{
	// 目的地が存在する
	if (m_nextNode != nullptr)
	{
		// 見失った場合は、視野範囲を無視して、
		// プレイヤーまでの視線が通るかどうかで判定する
		if (IsLookPlayer() && !mp_player->m_hide)
		{
			//追跡状態へ移行
			m_state = eState_Chase;
		}
		// プレイヤーが視線の通らないところにいる
		else
		{
			//目的地まで移動
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
	//目的地まで移動が終われば、
	else
	{
		m_warning = 0;
		//待機状態へ移行
		m_state = eState_Idle;
	}
}


void Enemy::View_Color()
{
	switch (m_state)
	{
		//通常状態は緑色
	case eState_Idle:
		color = CVector4D(0.0f, 1.0f, 0.0f, 1.0f);
		break;
		//プレイヤー追尾時は赤色
	case eState_Chase:
		color = CVector4D(1.0f, 0.0f, 0.0f, 1.0f);
		break;
		//移動中は黄色
	case eState_Move:
		color = CVector4D(1.0f, 1.0f, 0.0f, 1.0f);
		break;
		//プレイヤーを見失った
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

	//目的地までの距離が移動速度より大きいならば
	float dist = vec.Length();
	float speed = moveSpeed * CFPS::GetDeltaTime();
	if (dist > speed)
	{
		CVector3D moveVec = m_moveDir * moveSpeed;
		m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);
	}
	//距離が移動速度より短いのであれば、
	else
	{
		CVector3D moveVec = m_moveDir * dist;
		m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);
		//移動終了
		return true;
	}

	//移動継続
	return false;
}

//プレイヤー探知のフラグ->視野
bool Enemy::IsEyeFoundPlayer()
{	
	//視野の角度
	eye_ang = DtoR(60);
	//視野の距離
	eye_length = 10;
	//プレイヤーの座標
	CVector3D player_pos = mp_player->m_pos;

	//m_dir = CVector3D(sin(m_rot.y), 0, cos(m_rot.y));
	//敵からプレイヤーまでのベクトルを求める
	CVector3D vec = player_pos - m_pos;
	//求めたベクトルと敵の正面方向のベクトルを内積とって
	//角度(cosθ)を求める
	float eye_dot = CVector3D::Dot(m_dir, vec.GetNormalize());
	//プレイヤーと敵の視野角度で範囲内か判定する
	if (eye_dot < cosf(eye_ang))
		return false;

	//プレイヤーと敵の視野距離で範囲内か判定する
	if (vec.Length() > eye_length)
		return false;

	//最後に自信からプレイヤーまでの間に
	//遮蔽物がないか判定する
	if (!IsLookPlayer()) return false;
	
	return true;
}

bool Enemy::IsLookPlayer() const
{
	NavNode* playernodepos = mp_player->GetNavNode();	//プレイヤーのノード
	NavNode* enemynodepos = m_navNode;					//エネミーのノード
	CVector3D playerPos = playernodepos->GetPos();		//プレイヤーのノードの座標
	CVector3D enemyPos = enemynodepos->GetPos();		//エネミーのノードの座標
	CVector3D vec = playerPos - enemyPos;		//プレイヤーへの方向ベクトル
	// 現在位置からプレイヤーまでの距離を求める
	float dist = vec.Length();

	// プレイヤーの位置までのレイと壁との衝突判定を行う
	CVector3D start = enemyPos;
	CVector3D end = playerPos;
	//高さを考慮しない
	/*start.y = 1.0f;
	end.y = 1.0f;*/
	CVector3D hitPos, hitNormal;
	if (Field::CollisionRay(start, end, &hitPos, &hitNormal))
	{
		float hitDist = (hitPos - start).Length();
		if (dist > hitDist) return false;
	}

	// 壁と衝突していないもしくは、
	// 衝突位置がプレイヤーより奥の位置であるならば、
	// 視線が通っているので、プレイヤーが見える状態
	return true;
}

//探索ノードを設定
void Enemy::SetSearchNode(SearchNode* node)
{
	m_searchNode = node;
}

//探索中のノードを取得
SearchNode* Enemy::GetSearchNode() const
{
	return m_searchNode;
}


//更新処理
void Enemy::Update()
{
	//プレイヤー
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
	
	

	//アニメーション更新
	m_model.UpdateAnimation();

	//移動
	m_pos += m_vec * CFPS::GetDeltaTime();

	//プレイヤーの向きを徐々に移動方向へ向ける
	m_dir = CVector3D::Sleap(m_dir, m_moveDir, ROTATE_SPEED * CFPS::GetDeltaTime());
	//プレイヤーの向き反映
	m_rot.y = atan2f(m_dir.x, m_dir.z);

	

	//ベースクラスの更新
	CharaBase::Update();

	//警戒値のゲージ
	m_WarningGauge = COBB(
		m_pos + CVector3D(0,2,0),
		CVector3D(0, 0, 0),
		CVector3D(0.05, 0.05, 0.05)
	);

	//float lineWidth = 10.0f;

	//視野範囲の表示
	/*Utility::DrawLine(m_pos + CVector3D(0, 1.0, 0), m_pos + CVector3D(0, 1.0, 0) + m_dir * eye_length, CVector4D(1, 0, 1, 1), lineWidth);
	Utility::DrawLine(m_pos + CVector3D(0, 1.0, 0), m_pos + CVector3D(0, 1.0, 0) + CMatrix::MRotationY(eye_ang) * m_dir * eye_length, CVector4D(0, 1, 1, 1), lineWidth);
	Utility::DrawLine(m_pos + CVector3D(0, 1.0, 0), m_pos + CVector3D(0, 1.0, 0) + CMatrix::MRotationY(-eye_ang) * m_dir * eye_length, CVector4D(0, 1, 1, 1), lineWidth);
	CMatrix m;
	m.LookAt(m_pos + CVector3D(0, 1.5, 0), m_pos + CVector3D(0, 0.1, 0) + m_dir * eye_length, CVector3D(0, 1, 0));
	Utility::DrawSector(m, -eye_ang, eye_ang, eye_length, color);*/

	//デバッグ用:カプセル判定の表示
	//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(1, 0, 0, 1));
}

//描画処理
void Enemy::Render()
{
	m_model.SetScale(m_scale);	//モデルサイズ
	m_model.SetPos(m_pos);		//モデル座標
	m_model.SetRot(m_rot);		//モデル回転値
	m_model.Render();			//モデル描画

	Utility::DrawOBB(m_WarningGauge, CVector4D(m_warning/3,(3-m_warning)/3, 0, 1));
}


void Enemy::Collision(Task* t) 
{
	CharaBase::Collision(t);

	switch (t->GetTag())
	{
	case ETaskTag::eEnemy:
	{
		//敵と敵の判定
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