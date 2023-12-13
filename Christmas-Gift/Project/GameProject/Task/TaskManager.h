#pragma once
#include<list>

class TaskManager
{
private:
	//TaskManagerのインスタンス
	static TaskManager* ms_instance;

	//Taskのリスト
	using TaskList = std::list<Task*>;
	//生成されているTaskのリスト
	static TaskList m_taskList;

public:
	~TaskManager();
	//TaskManagerのインスタンスを取得
	static TaskManager* Instance();

	//TaskManagerのインスタンスを破壊
	static void ClearInstance();

	//リストにタスクを追加
	void AddTask(Task* task);

	//リストからタスクを取り除く
	void RemoveTask(Task* task);

	//リストから全てのタスクを取り除く
	static void KillALL();

	//更新処理
	void Update();

	//3D描画処理
	void Render();

	//2D描画処理
	void Draw();

	static Task* FindObject(ETaskTag tag);

	static std::vector<Task*> FindObjects(const ETaskTag tag);

	static void Collision();

	
};