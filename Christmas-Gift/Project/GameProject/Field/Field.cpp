#include"Field.h"
#include"FieldFloor.h"
#include"FieldWall.h"
#include"FieldObject/Door.h"
#include"Field/FieldObject/Switch.h"
#include"Field/FieldObject/Closet.h"
#include"Light/Light.h"
#include"../Navigation/NavManager.h"
#include"../Navigation/NavNode.h"
#include"../ObjectBase/ObjectBase.h"

Field* Field::ms_instance = nullptr;

//�R���X�g���N�^
Field::Field(const char* Nodefile,const char* Lightfile,const char* Doorfile,const char* Switchfile,const char* Closetfile)
	:ObjectBase(ETaskTag::eField,true)
{
	ms_instance = this;

	//�R���W�������f���̎擾
	m_colModel = GET_RESOURCE("MapCol", CModel);
	m_colWallModel = GET_RESOURCE("WallMapCol", CModel);
	
	//���ƕǂ̃N���X���쐬
	m_floor = new FieldFloor();
	m_wall = new FieldWall();

	m_lightNo = 2;
	//�o�H�T���p�̃m�[�h���쐬
	CreateNavNodes(Nodefile);
	//���C�g�쐬
	CreateLights(Lightfile);
	//�h�A�쐬
	CreateDoors(Doorfile);
	//�X�C�b�`�쐬
	CreateSwitchs(Switchfile);
	//�N���[�[�b�g�쐬
	CreateCloset(Closetfile);
}

Field::~Field()
{
	if (ms_instance == this)
	{
		ms_instance = nullptr;
	}
}

//�o�H�T���p�̃m�[�h���쐬
void Field::CreateNavNodes(const char* file)
{
	FILE* fp = NULL;

	//�f�[�^���e�L�X�g�ǂݍ��݂ŃI�[�v��
	fopen_s(&fp, file, "r");
	//�J���̂Ɏ��s
	if (!fp)return;

	char buf[256] = "";

	//�t�@�C���̖����܂ŌJ��Ԃ�
	while (!feof(fp))
	{
		//��s�Â�buf�Ɋi�[
		fgets(buf, 256, fp);

		//�m�[�h�̍��W
		CVector3D Pos(0.0f, 0.0f, 0.0f);

		sscanf_s(buf, "%f %f %f", &Pos.x, &Pos.y, &Pos.z);

		//�m�[�h����
		new NavNode(Pos);
	}
	fclose(fp);
}

//�X�e�[�W�Ƀ��C�g���쐬
void Field::CreateLights(const char* file)
{
	FILE* fp = NULL;

	//�f�[�^���e�L�X�g�ǂݍ��݂ŃI�[�v��
	fopen_s(&fp, file, "r");
	//�J���̂Ɏ��s
	if (!fp)return;

	char buf[256] = "";

	//�t�@�C���̖����܂ŌJ��Ԃ�
	while (!feof(fp))
	{
		//��s�Â�buf�Ɋi�[
		fgets(buf, 256, fp);

		CVector3D Pos(0.0f, 0.0f, 0.0f);		//���C�g�̍��W
		int No = 0;								//���C�g�i���o�[
		float Range = 0.0f;						//���̑傫��
		int IsShine = 0;						//�����Ă��邩�̔��f
		bool Shine = true;				

		sscanf_s(buf, "%f %f %f %d %f %d", &Pos.x, &Pos.y, &Pos.z, &No, &Range, &IsShine);

		//�l��0�Ȃ�d�C�͂��Ă��Ȃ�
		if (IsShine == 0) Shine = false;

		//���C�g����
		new Light(Pos,m_lightNo,No,Range, CLight::eLight_Point,Shine);

		m_lightNo++;
	}
	fclose(fp);

}

//�h�A���쐬
void Field::CreateDoors(const char* file)
{
	FILE* fp = NULL;

	//�f�[�^���e�L�X�g�ǂݍ��݂ŃI�[�v��
	fopen_s(&fp, file, "r");
	//�J���̂Ɏ��s
	if (!fp)return;
	
	char buf[256] = "";

	//�t�@�C���̖����܂ŌJ��Ԃ�
	while (!feof(fp))
	{
		//��s�Â�buf�Ɋi�[
		fgets(buf, 256, fp);

		//�h�A�̍��W
		CVector3D Pos(0.0f, 0.0f, 0.0f);
		//�h�A�̌���
		float Rot;

		sscanf_s(buf,"%f %f %f %f", &Pos.x, &Pos.y, &Pos.z, &Rot);

		//�h�A����
		new Door(Pos,CVector3D(0.0f,DtoR(Rot),0.0f), CVector3D(1.0f, 1.0f, 1.0f), CVector3D(1.0f, 2.0f, 0.3f));
	}
	fclose(fp);
}

//�X�C�b�`���쐬
void Field::CreateSwitchs(const char* file)
{
	FILE* fp = NULL;

	//�f�[�^���e�L�X�g�ǂݍ��݂ŃI�[�v��
	fopen_s(&fp, file, "r");
	//�J���̂Ɏ��s
	if (!fp)return;

	char buf[256] = "";

	//�t�@�C���̖����܂ŌJ��Ԃ�
	while (!feof(fp))
	{
		//��s�Â�buf�Ɋi�[
		fgets(buf, 256, fp);

		//�X�C�b�`�̍��W
		CVector3D Pos(0.0f, 0.0f, 0.0f);
		//�X�C�b�`�̌���
		float Rot;
		//�X�C�b�`�̔ԍ�
		float No;

		sscanf_s(buf, "%f %f %f %f %f", &Pos.x, &Pos.y, &Pos.z, &Rot, &No);

		//�X�C�b�`����
		new Switch(Pos, CVector3D(0.0f, DtoR(Rot), 0.0f), CVector3D(1.0f, 1.0f, 1.0f),No);
	}
	fclose(fp);
}

//�N���[�[�b�g���쐬
void Field::CreateCloset(const char* file)
{
	FILE* fp = NULL;

	//�f�[�^���e�L�X�g�ǂݍ��݂ŃI�[�v��
	fopen_s(&fp, file, "r");
	//�J���̂Ɏ��s
	if (!fp)return;

	char buf[256] = "";

	//�t�@�C���̖����܂ŌJ��Ԃ�
	while (!feof(fp))
	{
		//��s�Â�buf�Ɋi�[
		fgets(buf, 256, fp);

		//�N���[�[�b�g�̍��W
		CVector3D Pos(0.0f, 0.0f, 0.0f);
		//�N���[�[�b�g�̌���
		float Rot;

		sscanf_s(buf, "%f %f %f %f", &Pos.x, &Pos.y, &Pos.z, &Rot);

		//�N���[�[�b�g����
		new Closet(Pos, CVector3D(0.0f, DtoR(Rot), 0.0f), CVector3D(1.0f, 0.7f, 1.0f),2);
	}
	fclose(fp);
}

//�t�B�[���h�̃R���W�������擾
CModel* Field::GetColModel() const
{
	return m_colModel;
}
CModel* Field::GetColWallModel() const
{
	return m_colWallModel;
}

// �w�肵�����C�ƕǂ̃R���W�����Ƃ̃q�b�g����
bool Field::CollisionRay(const CVector3D& start, const CVector3D& end, CVector3D* outHitPos, CVector3D* outHitNormal)
{
	// �t�B�[���h���쐬����Ă��Ȃ���΁A�X���[
	Field* f = ms_instance;
	if (f == nullptr) return false;

	// �R���W�������f�����쐬����Ă��Ȃ���΁A�X���[
	CModel* col = f->GetColWallModel();
	if (col == nullptr) return false;

	// �R���W���������݂���΁A���C�Ƃ̃q�b�g����̌��ʂ�Ԃ�
	return col->CollisionRay(outHitPos, outHitNormal, start, end);
	
}
