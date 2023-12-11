#include"Debug/DebugPrint.h"
#include"Debug/DebugProfiler.h"

TaskManager* TaskManager::ms_instance = nullptr;
std::list<Task*> TaskManager::m_taskList;

TaskManager::~TaskManager()
{
	//Taskの削除処理
	TaskList::iterator it = m_taskList.begin();
	while (it != m_taskList.end()) {
		//Taskの状態を取得
		Task* deleteTask = *it;

		//次のタスクへ進行しておく
		it++;

		//削除フラグがたっていたら、タスクを削除
		//（リストからのRemove処理はTaskのデストラクタで呼ばれる）
		delete deleteTask;
	}
	m_taskList.clear();
}

// TaskManagerのインスタンスを取得
TaskManager* TaskManager::Instance()
{
	if (ms_instance == nullptr)
	{
		ms_instance = new TaskManager();
	}
	return ms_instance;
}

// TaskManagerのインスタンスを破棄
void TaskManager::ClearInstance()
{
	if (ms_instance != nullptr)
	{
		delete ms_instance;
		ms_instance = nullptr;
	}
}

// リストにタスクを追加
void TaskManager::AddTask(Task* task)
{
	TaskList::iterator it = m_taskList.begin();
	while (it != m_taskList.end())
	{
		//追加するタスクが検索中のタスクより
		//処理順序が先ならば、その位置に追加
		if (task->GetTag() < (*it)->GetTag())
		{
			m_taskList.insert(it, task);
			return;
		}

		//処理順序が後ならば、次のタスクを検索
		it++;
	}

	//追加するタスクより
	//処理順序が後のタスクが見つからない場合は
	//リストの最後尾に追加
	m_taskList.push_back(task);
}

// リストからタスクを取り除く
void TaskManager::RemoveTask(Task* task)
{
	m_taskList.remove(task);
}

//全てのタスクを削除する
void TaskManager::KillALL() {
	//全ての削除フラグをONにする
	for (auto& t : m_taskList) {
		if (!t->m_remove) return;
		t->Kill();
	}
}

//更新処理
void TaskManager::Update()
{
	//Taskの削除処理
	TaskList::iterator it = m_taskList.begin();
	while (it != m_taskList.end())
	{
		//Taskの状態を取得
		Task* deleteTask = *it;
		bool isDelete = deleteTask->IsKill();

		//次のタスクへ進行しておく
		it++;

		//削除フラグが立っていたら、タスクを削除
		//（リストからのRemove処理はTaskのデストラクタで呼ばれる）
		if (isDelete)
		{
			delete deleteTask;
		}
	}

	//全TaskのUpdate関数を呼び出す
	for (Task* task : m_taskList)
	{
		task->Update();
	}
}

//3D描画処理
void TaskManager::Render()
{
	//全TaskのRender関数を呼び出す
	for (Task* task : m_taskList)
	{
		task->Render();
	}
}

//2D描画処理
void TaskManager::Draw()
{
	//全TaskのDraw関数を呼び出す
	for (Task* task : m_taskList)
	{
		task->Draw();
	}
}

Task* TaskManager::FindObject(ETaskTag tag) {

	for (auto& b : m_taskList) {
		if (b->GetTag() == tag && !b->m_kill) return b;
	}
	return nullptr;
}

//タスクのコリジョン処理
void TaskManager::Collision() {
	//DebugProfiler::StartTimer("コリジョン判定");
	auto itr = m_taskList.begin();
	//末尾まで繰り返す
	while (itr != m_taskList.end()) {
		if (!(*itr)->m_kill) {
			auto ct = itr;
			ct++;
			while (ct != m_taskList.end()) {
				if (!(*ct)->m_kill) {
					(*itr)->Collision(*ct);
					(*ct)->Collision(*itr);
				}
				//次のオブジェクト
				ct++;
			}
		}
		//次のオブジェクト
		itr++;
	}
	//DebugProfiler::EndTimer("コリジョン判定");
}

