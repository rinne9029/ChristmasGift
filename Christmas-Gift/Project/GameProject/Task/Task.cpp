
//コンストラクタ
Task::Task(ETaskTag tag,bool Remove)
	: m_tag(tag)
	, m_isKill(false)
	, m_remove(Remove)
{
	TaskManager::Instance()->AddTask(this);
}

//デストラクタ
Task::~Task()
{
	TaskManager::Instance()->RemoveTask(this);
}

//タスクの種類を取得
ETaskTag Task::GetTag() const
{
	return m_tag;
}

void Task::Collision(Task* t)
{
}

//削除フラグ取得
bool Task::IsKill() const
{
	return m_isKill;
}

//タスクを削除
void Task::Kill()
{
	m_isKill = true;
}

//更新処理
void Task::Update()
{

}

//モデル描画処理
void Task::Render()
{

}

