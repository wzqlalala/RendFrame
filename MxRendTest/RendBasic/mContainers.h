#pragma once
#include "rendbasic_global.h"

//QT
#include <QStringList>

namespace MBasicFunction
{
	//存储输出信息的容器
	extern RENDBASIC_EXPORT QStringList _outputMessages;

	//存储Python命令行的容器
	extern RENDBASIC_EXPORT QStringList _pythonCommands;

	//全局Part ID
	extern RENDBASIC_EXPORT int _globalPartId;

	//全局Solid ID
	extern RENDBASIC_EXPORT int _globalSolidId;

	//全局Face ID
	extern RENDBASIC_EXPORT int _globalFaceId;

	//全局Line ID
	extern RENDBASIC_EXPORT int _globalLineId;

	//全局Point ID
	extern RENDBASIC_EXPORT int _globalPointId;

	//全局Node ID
	extern RENDBASIC_EXPORT int _globalNodeId;

	//全局Mesh ID
	extern RENDBASIC_EXPORT int _globalMeshId;

	//全局MeshLine ID
	extern RENDBASIC_EXPORT int _globalMeshLineId;

	//全局MeshFace ID
	extern RENDBASIC_EXPORT int _globalMeshFaceId;

	//初始化所有的全局ID
	extern RENDBASIC_EXPORT void initialGlobalID();
}