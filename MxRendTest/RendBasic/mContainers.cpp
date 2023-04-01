#include "mContainers.h"
namespace MBasicFunction
{
	 //存储输出信息的容器
	 QStringList _outputMessages;

	 //存储Python命令行的容器
	 QStringList _pythonCommands;

	 //全局Part ID
	 int _globalPartId = 0;

	 //全局Solid ID
	 int _globalSolidId = 0;

	 //全局Face ID
	 int _globalFaceId = 100000000;

	 //全局Line（Edge） ID
	 int _globalLineId = 100000000;

	 //全局Point（Vertex） ID
	 int _globalPointId = 0;

	 //全局Node ID
	 int _globalNodeId = 0;

	 //全局Mesh ID
	 int _globalMeshId = 0;

	 //全局MeshLine ID
	 int _globalMeshLineId = 100000000;

	 //全局MeshFace ID
	 int _globalMeshFaceId = 100000000;


	 //初始化所有的全局ID
	 void initialGlobalID()
	 {
		 _globalPartId = 0;
		 _globalSolidId = 0;
		 _globalFaceId = 100000000;
		 _globalLineId = 100000000;
		 _globalPointId = 0;
		 _globalNodeId = 0;
		 _globalMeshId = 0;
		 _globalMeshLineId = 100000000;
		 _globalMeshFaceId = 100000000;
	 }
}