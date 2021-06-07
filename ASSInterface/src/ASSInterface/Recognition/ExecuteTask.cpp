#include "hzpch.h"
#include "ExecuteTask.h"
#include "Platform/Innovatrics/InnoTask.h"

namespace ASSInterface {
	Ref<ExecuteTask> ExecuteTask::CreateInnoTask(int channel)
	{
		return CreateRef<InnoTask>(channel);
	}
}