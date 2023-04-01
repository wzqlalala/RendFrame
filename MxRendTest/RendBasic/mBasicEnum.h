#pragma once

namespace MBasicFunction
{
	/*
	* 分析模块
	*/
	enum AnalysisModule
	{
		//隐式
		ImplicitAnalysis,

		//显式
		ExplicitAnalysis,

		//电磁
		MagnetAnalysis,

		//多物理场分析
		MultiPhysicAnalysis,

		//多物理场分析-子场
		MultiPhysicAnalysisChild,

		//可压流体传热
		FluidCompressibleAnalysis,

		//不可压流体传热
		FluidIncompressibleAnalysis,

		//压电分析模板
		PiezoelectricAnalysis,

		//未知模块
		UnknownAnalysis,

		//通用模块zdh加
		Common,

	};
	enum SigSenderTree
	{
		//隐式
		BrowserTree,
		//显式
		ExBrowserTree,
		//流体传热
		FluidTree,
		//流体传热1
		FluidTree1,
		//多物理场树
		MultiFieldTree,
		//模板树
		TemplateTree,
	};
	/*
	* 输出信息的类型
	*/
	enum OutputMessageType
	{
		//正常
		NormalMessage = 0,

		//警告
		WarningMessage,

		//错误
		ErrorMessage,

		//Python
		PythonCommand,
	};

	/*
	* 前处理模型树的节点类型
	*/
	enum BrowserTreeType
	{
		//父节点
		TreeRootItem,

		//模型父节点
		ModelParentItem,

		//装配模型父节点
		AssemblyParentItem,
		AssemblyMeshItem,
		//装配模型子节点
		AssemblyChildItem,
		//装配模型几何子节点
		AssemblyGeoChildItem,
		AssemblyNoneGeoChildItem,
		//装配模型网格子节点
		AssemblyMeshChildItem,
		AssemblyNoneMeshChildItem,
		//Part节点
		PartTreeItem,

		//独立模型父节点
		IndependParentItem,

		//独立模型子节点
		IndependChildItem,

		//激活模型父节点（多物理场单场用）
		ActivateParentItem,

		//激活模型子节点（多物理场单场用）
		ActivateChildItem,

		//未激活模型父节点（多物理场单场用）
		InactivateParentItem,

		//未激活模型子节点（多物理场单场用）
		InactivateChildItem,

		//连接父节点
		ConnectorParentItem,

		//连接子节点
		ConnectorChildItem,

		//材料父节点
		MaterialParentItem,

		//材料子节点
		MaterialChildItem,

		//截面属性父节点
		PropertyParentItem,

		//截面属性子节点
		PropertyChildItem,

		//自由模型父节点
		CustomParentItem,

		//自由模型子节点
		CustomChildItem,

		//分析工况父节点
		CaseParentItem,

		//分析工况子节点
		CaseChildItem,

		//分析工况下的载荷父节点
		CaseLoadParentItem,

		//分析工况下的载荷子节点
		CaseLoadChildItem,

		//分析工况下的边界父节点
		CaseBCParentItem,

		//分析工况下的边界子节点
		CaseBCChildItem,

		//载荷父节点
		LoadParentItem,

		//载荷子节点
		LoadChildItem,

		//边界条件父节点
		BCParentItem,

		//边界条件子节点
		BCChildItem,

		//初始条件(显式)父节点
		INITParentItem,
		//初始条件(显式)子节点
		INITChildItem,
		//接触父节点
		ContactParentItem,

		//接触子节点
		ContactChildItem,

		//接触属性父节点
		ContactPropParentItem,

		//接触属性子节点
		ContactPropChildItem,

		//输出父节点
		OutputParentItem,

		//输出子节点
		OutputChildItem,

		//优化任务父节点
		OptimizeParentItem,

		//优化任务子节点
		OptimizeChildItem,

		//设计域父节点
		DesignDomainParentItem,

		//设计域子节点
		DesignDomainChildItem,

		//优化响应父节点
		OptiResponseParentItem,

		//优化响应子节点
		OptiResponseChildItem,

		//优化约束父节点
		OptiConstraintParentItem,

		//优化约束子节点
		OptiConstraintChildItem,

		//优化目标父节点
		OptiObjectiveParentItem,

		//优化目标子节点
		OptiObjectiveChildItem,

		//计算任务父节点
		AnalysisParentItem,

		//计算任务子节点
		AnalysisChildItem,

		//集合父节点
		SetParentItem,

		//集合子节点
		SetChildItem,

		//面父节点
		SurfaceParentItem,

		//面子节点
		SurfaceChildItem,

		//曲线父节点
		CurveParentItem,

		//曲线子节点
		CurveChildItem,
		//向量父节点
		VectorParentItem,

		//向量子节点
		VectorChildItem,

		//盒子父节点
		BoxParentItem,

		//盒子子节点
		BoxChildItem,

		//铺层材料父节点
		PlyParentItem,

		//铺层材料子节点
		PlyChildItem,

		//坐标系父节点
		DatumParentItem,

		//坐标系子节点
		DatumChildItem,

		//刚体父节点
		RigidBodyParentItem,

		//刚体子节点
		RigidBodyChildItem,

		//刚体约束父节点
		RigidConstraintParentItem,

		//刚体约束子节点
		RigidConstraintChildItem,

		//刚体连接父节点
		RigidConnectParentItem,

		//刚体连接子节点
		RigidConnectChildItem,

		//参考点父节点
		RPParentItem,

		//参考点子节点
		RPChildItem,

		//刚性墙父节点
		RigidWallParentItem,

		//刚性墙子节点
		RigidWallChildItem,

		//安全带父节点
		AirBageParentItem,

		//安全带子节点
		AirBageChildItem,

		//流体-通用设置父节点
		FluidCommonSettingParentItem,

		//流体-通用设置子节点
		FluidCommonSettingChildItem,

		//流体-物理模型父节点
		FluidModelParentItem,

		//流体-物理模型子节点
		FluidModelChildItem,

		//流体-材料父节点
		FluidMatParentItem,

		//流体-材料子节点
		FluidMatChildItem,

		//流体-求解域父节点
		FluidDomainParentItem,

		//流体-求解域子节点
		FluidDomainChildItem,

		//流体-边界条件父节点
		FluidBCParentItem,

		//流体-边界条件子节点
		FluidBCChildItem,

		//流体-边界条件孙子节点
		FluidBCChildSonItem,

		//流体-离散格式设置父节点
		FluidDiscreteParentItem,

		//流体-初始设置父节点
		FluidInitParentItem,

		//流体-初始设置子节点
		FluidInitChildItem,

		//流体-监控设置父节点
		FluidMonitorParentItem,

		//流体-监控设置子节点
		FluidMonitorChildItem,

		//流体-监控设置孙子节点
		FluidMonitorChildSonItem,

		//流体-求解控制父节点
		FluidSolveParentItem,

		//流体-求解控制子节点
		FluidSolveChildItem,
	};

	/*
	* 后处理模型树节点类型
	*/
	enum PostTreeType
	{
		//根节点
		PostRootItem,

		//三维视图子节点
		Post3DViewItem,

		//二维视图子节点
		Post2DViewItem,

		//三维视图子节点下的零件节点
		Post3DViewPartItem,

		//刚性墙节点
		RigidWallItem,
	};

	/*
	* 网格形状
	*/
	enum MeshType
	{
		//无节点（特殊单元）
		MeshNone = 0,

		//点
		MeshPoint,

		//线
		MeshBeam,

		//三角形
		MeshTri,

		//四边形
		MeshQuad,

		//四面体
		MeshTet,

		//三菱柱
		MeshWedge,

		//六面体
		MeshHex,

		//四棱锥
		MeshPyramid,
	};

	/*
	 * 单元类型
	 */
	enum ElementType
	{
		/*
		* 无单元
		*/
		NoneElement,

		/*
		* 显隐式单元
		*/

		//传感器单元(无节点)
		Sensor,

		//张紧器单元（无节点）
		Pretensioner,

		//质量单元（一个节点）
		Mass1,

		//质量单元（一个节点）
		ElementSPH,

		//滑轮单元（一个节点）
		Slipring,

		//卷收器单元（一个节点）
		Retractor,

		//安全带单元（线）
		SeatBelt,

		//一阶二维杆单元
		D2R2,

		//二阶二维杆单元
		D2R3,

		//一阶三维杆单元
		D3R2,

		//二阶三维杆单元
		D3R3,

		//一阶二维梁单元
		D2B2,

		//二阶二维梁单元
		D2B3,

		//一阶三维梁单元
		D3B2,

		//二阶三维梁单元
		D3B3,

		//一阶三维管单元
		Pipe,

		//弹簧单元
		ELAS1,

		//弹簧单元
		SPRING1,

		//弹簧单元(一个节点)
		SPRING2,

		//弹簧单元
		SPRING3,

		//弹簧单元
		GAP,

		//弹簧单元
		CBUSH,

		//可拉可压杆单元
		TS2,

		//离散梁
		DB2,

		//积分梁
		HB2,

		//常截面梁
		BS2,

		//离散单元
		Discrete,

		//一阶三角形平面应变单元
		PST3,

		//二阶三角形平面应变单元
		PST6,

		//一阶三角形节点积分平面应变单元
		PST3_1,

		//一阶三角形稳定节点积分平面应变单元
		PST3_2,

		//一阶三角形边光滑平面应变单元
		PST3_3,

		//三角形壳单元TRIA3
		TRIA3,

		//三角形壳单元C0
		C0,

		//一阶三角形平面应力单元(常规)
		PET3,

		//二阶三角形平面应力单元(常规)
		PET6,

		//一阶三角形节点积分平面应变单元
		PET3_1,

		//一阶三角形稳定节点积分平面应变单元
		PET3_2,

		//一阶三角形边光滑平面应变单元
		PET3_3,

		//一阶三角形平面应力单元(杂交元H)
		PET3H,

		//二阶三角形平面应力单元(杂交元H)
		PET6H,

		//一阶三角形轴对称单元(常规)
		AST3,

		//二阶三角形轴对称单元(常规)
		AST6,

		//一阶三角形轴对称单元(杂交元H)
		AST3H,

		//二阶三角形轴对称单元(杂交元H)
		AST6H,

		//三角形壳单元CPDSG
		CPDSG,

		//三角形壳单元CPDSG
		DSG,

		//三角形壳单元SST3
		SST3,

		//三角形壳单元SST6
		SST6,

		//三角形边光滑壳单元
		CPDSG_3,

		//一阶三角形膜单元
		MBT3,

		//二阶三角形膜单元
		MBT6,

		//三角形剪力板单元
		SPT3,

		//一阶四边形平面应变单元(常规)
		PSQ4,

		//二阶四边形平面应变单元(常规)
		PSQ8,

		//一阶四边形平面应变单元(缩减积分R)
		PSQ4R,

		//二阶四边形平面应变单元(缩减积分R)
		PSQ8R,

		//一阶四边形平面应变单元(非协调元I)
		PSQ4I,

		//二阶四边形平面应变单元(非协调元I)
		PSQ8I,

		//一阶四边形平面应力单元(常规)
		PEQ4,

		//二阶四边形平面应力单元(常规)
		PEQ8,

		//一阶四边形平面应力单元(缩减积分R)
		PEQ4R,

		//二阶四边形平面应力单元(缩减积分R)
		PEQ8R,

		//一阶四边形平面应力单元(杂交元H)
		PEQ4H,

		//二阶四边形平面应力单元(杂交元H)
		PEQ8H,

		//一阶四边形平面应力单元(非协调元I)
		PEQ4I,

		//二阶四边形平面应力单元(非协调元I)
		PEQ8I,

		//一阶四边形轴对称单元(常规)
		ASQ4,

		//二阶四边形轴对称单元(常规)
		ASQ8,

		//一阶四边形轴对称单元(缩减积分R)
		ASQ4R,

		//二阶四边形轴对称单元(缩减积分R)
		ASQ8R,

		//一阶四边形轴对称单元(杂交元H)
		ASQ4H,

		//二阶四边形轴对称单元(杂交元H)
		ASQ8H,

		//一阶四边形轴对称单元 (非协调元I)
		ASQ4I,

		//二阶四边形轴对称单元 (非协调元I)
		ASQ8I,

		//四边形壳单元QPH
		QPH,

		//四边形壳单元Q4R
		Q4R,

		//四边形壳单元MITC4
		MITC4,

		//四边形壳单元SSQ4
		SSQ4,

		//四边形壳单元SSQ8
		SSQ8,

		//四边形壳单元BT
		BT,

		//四边形壳单元HL
		HL,

		//四边形壳单元CY
		CY_,

		//四边形壳单元BTSHELL
		BTShell,

		//一阶四边形膜单元
		MBQ4,

		//一阶四边形膜单元(缩减积分)
		MBQ4R,

		//二阶四边形膜单元
		MBQ8,

		//四边形剪力板单元
		SPQ4,

		//一阶四面体单元
		D3T4,

		//二阶四面体单元
		D3T10,

		//一阶四面体压电单元
		D3T4E,

		//一阶节点积分四面体单元
		D3T4_1,

		//一阶稳定节点积分四面体单元
		D3T4_2,

		//一阶边光滑四面体单元
		D3T4_3,

		//二阶四面体修正格式
		D3T10M,

		//二阶四面体压电单元
		D3T10E,

		//一阶四棱锥单元
		D3P5,

		//一阶三棱柱单元(常规)
		D3P6,

		//一阶三棱柱单元(杂交元H)
		D3P6H,

		//一阶三棱柱单元(压电单元)
		D3P6E,

		//二阶三棱柱单元(常规)
		D3P15,

		//二阶三棱柱单元(杂交元H)
		D3P15H,

		//二阶三棱柱单元(压电单元)
		D3P15E,

		//一阶三棱柱实体壳单元(缩减积分R)
		D3S6R,

		//通用实体单元
		SOLID,

		//一阶六面体单元(常规)
		D3H8,

		//二阶六面体单元(常规)
		D3H20,

		//一阶六面体单元(缩减积分R)
		D3H8R,

		//二阶六面体单元(缩减积分R)
		D3H20R,

		//一阶六面体单元(杂交元H)
		D3H8H,

		//二阶六面体单元(杂交元H)
		D3H20H,

		//一阶六面体单元(非协调元I)
		D3H8I,

		//二阶六面体单元(非协调元I)
		D3H20I,

		//一阶六面体单元(压电单元)
		D3H8E,

		//二阶六面体单元(压电单元)
		D3H20E,

		//一阶六面体实体壳单元(缩减积分R)
		D3S8R,

		/*
		*电磁模块单元
		*/

		//一阶四面体单元
		EMD3T4,

		//一阶边光滑四面体单元
		EMD3T4_3,

		//一阶三棱柱单元(常规)
		EMD3P6,

		//一阶六面体单元(常规)
		EMD3H8
	};

	/*
	* 删除的对象
	*/
	enum DeleteType
	{
		//删除组件
		DeleteAll,

		//删除几何
		DeleteGeo,

		//删除几何
		DeleteMesh
	};

	/*
	 * 二维曲线类型
	 */
	enum PlotType
	{
		//折线图
		LineChart,

		//光滑曲线图
		SmoothChart,

		//阶跃曲线图
		StepChart,

		//分布光滑曲线图（前段为光滑，后端为直线）
		SmoothStepChart,
	};

	/*
	* 网格和几何模型操作枚举
	*/
	enum ModelOperateEnum
	{
		//导入文件操作
		ImportPart,

		//不操作
		OperateNothing,

		//单个几何操作
		AppendOnePart,
		DeleteOnePart,
		HideOnePart,
		ShowOnePart,
		ColorOnePart,
		ReplaceOnePart,
		RenameOnePart,

		//全部几何操作
		DeleteAllPart,
		HideAllPart,
		ShowAllPart,
		ColorAllPart,
	};

	/*
	 * 后处理模型操作枚举
	 */
	enum PostModelOperateEnum
	{
		//导入文件操作
		ImportOperate,

		//不操作
		NoOperate,

		//单个部件操作
		HideOnePartOperate,//隐藏
		ShowOnePartOperate,//显示
		ChangeColorOnePartOperate,//改变颜色
		ColorOnePartOperate,//显示模型颜色
		TransparentOnePartOperate,//显示透明
		TextureOnePartOperate,//显示纹理
		ReplaceOnePartOperate,//替换部件

		//全部部件操作
		HideAllPartOperate,//隐藏所有Isshow
		ShowAllPartOperate,//显示所有Isshow
		ChangeColorAllPartOperate,//改变颜色Material
		ColorAllPartOperate,//显示模型颜色isColor
		TransparentAllPartOperate,//显示透明isColor
		TextureAllPartOperate,//显示纹理isColor
		UpdateVariableValue,//更新变量值value
		UpdateMinMax,//更新最值
		UpdateNormal,//更新法向量Normal
		ShowNodeformation,//显示变形前的图
		HideNodeformation,//隐藏变形前的图
	};

	/*
	* 网格剖分枚举
	*/
	enum MeshingEnum
	{
		//未剖分
		NoMeshing,

		//正在剖分
		Meshing,

		//调整后剖分
		AdjustingMeshing,

		//偏置后剖分
		BiasingMeshing,

		//网格调整中
		Adjusting,

		//节点偏置中
		Biasing,

		//剖分完成
		FinishMeshing,

		//完成调整
		FinishAdjusting,

		//完成偏置
		FinishBiasing,
	};

	//拾取对象
	enum class PickObjectType
	{
		Mesh1D,

		MeshEdge,

		Mesh2D,

		MeshFace,
	};

	enum class GeoLineProperty
	{
		IndependentEdge,//独立边
		EdgeOnFace,//面上的边
		DottedTopology//虚拓扑边
	};

	//后处理渲染模式
	enum PostMode
	{

		No,

		//单帧
		OneFrame,

		//动画
		Animation,

		//单帧线性动画
		OneFrameLinearAnimation,

		//单帧正弦动画
		OneFrameSinAnimation,
	};
}
