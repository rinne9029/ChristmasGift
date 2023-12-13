#pragma once

class ObjectBase : public Task
{
private:

	//ETaskTag m_tag;			//オブジェクトごとの種類名
	

	//CVector3D m_pos;		//オブジェクト座標
	//CVector3D m_rot;		//オブジェクト回転値
	//CVector3D m_at;			//オブジェクト注視点
	//CVector3D m_vec;		//オブジェクト移動ベクトル
	//CVector3D m_dir;		//オブジェクト前方向ベクトル
	//////int m_kill;

	//////当たり判定用
	//////球、カプセル用
	//float m_rad;			//オブジェクト半径
	//float m_height;			//オブジェクト高さ
	//CVector3D m_lineS;		//オブジェクト線分始点
	//CVector3D m_lineE;		//オブジェクト線分終点

	//////箱形用
	CVector3D m_size;		//大きさ
	//COBB m_obb;				//箱形

public:
	CModelObj m_model;		//オブジェクトモデル
	//コンストラクタ
	ObjectBase(ETaskTag tag,bool remove);
	//デストラクタ
	virtual ~ObjectBase();

	/*ETaskTag GetTag() {
		return m_tag;
	}*/
};