#pragma once
namespace MViewBasic
{

	/*
	* 拾取模式,add,reduce,replace
	*/
	enum class PickFuntion
	{
		//添加拾取
		AddPick,

		//减少拾取
		ReducePick,

		//替换拾取
		ReplacePick,
	};

	/*
	* 拾取筛选器:全部,节点,点,梁,三角形,四边形,四面体,三菱柱,六面体,不拾取,单元线,单元面，网格调整，网格偏置，四棱锥
	*/
	enum class PickFilter
	{
		//拾取任何网格（只针对网格和节点，不对网格线和网格面）
		PickAny,

		//拾取点
		PickPoint,

		//拾取一维网格
		Pick1DMesh,

		//拾取二维网格
		Pick2DMesh,

		//拾取三角形
		PickTri,

		//拾取四边形
		PickQuad,

		//拾取四面体
		PickTet,

		//拾取四棱锥
		PickPyramid,

		//拾取三菱柱
		PickWedge,

		//拾取六面体
		PickHex,

		//不选择模式
		PickNothing,

		//拾取节点
		PickNode,

		//拾取节点（保留拾取顺序）
		PickNodeOrder,

		//拾取节点(通过路径)
		PickNodePath,

		//拾取任意网格
		PickAnyMesh,

		//拾取单元线
		PickMeshLine,

		//拾取单元面
		PickMeshFace,

		//拾取节点通过部件
		PickNodeByPart,

		//拾取任意网格通过部件
		PickAnyMeshByPart,

		//拾取单元线通过部件
		PickMeshLineByPart,

		//拾取单元面通过部件
		PickMeshFaceByPart,

		//拾取节点通过线
		PickNodeByLine,

		//拾取单元通过线
		PickMeshByLine,

		//拾取单元线通过线
		PickMeshLineByLine,

		//拾取节点通过面
		PickNodeByFace,

		//拾取单元通过面
		PickMeshByFace,

		//拾取单元面通过面
		PickMeshFaceByFace,

		//拾取节点通过线角度（不区分1维单元或者单元线，内部自动判断）
		PickNodeByLineAngle,

		//拾取节点通过面角度（不区分2维单元或者单元面，内部自动判断）
		PickNodeByFaceAngle,

		//拾取1维单元通过角度
		Pick1DMeshByAngle,

		//拾取2维单元通过角度
		Pick2DMeshByAngle,

		//拾取单元线通过角度
		PickMeshLineByAngle,

		//拾取单元面通过角度
		PickMeshFaceByAngle,

		//拾取网格部件
		PickMeshPart,

		//网格剖分时候点击调整的数字
		PickAdjustNum,

		//网格剖分时候点击偏置的数字
		PickBiasNum,

		//拾取几何体
		PickGeoSolid,

		//拾取几何面
		PickGeoFace,

		//拾取几何线
		PickGeoLine,

		//拾取几何点
		PickGeoPoint,

		//拾取几何体通过部件
		PickGeoSolidByPart,

		//拾取几何面通过部件
		PickGeoFaceByPart,

		//拾取几何线通过部件
		PickGeoLineByPart,

		//拾取几何点通过部件
		PickGeoPointByPart,

		//拾取几何部件
		PickGeoPart,

		//拾取几何线上的任意点
		pickVertexOnGeoLine,

		//拾取几何面上的任意点
		pickVertexOnGeoFace,

		//拾取屏幕上的任意点
		pickVertexOnScreen,

		//拾取后处理上的任意点
		PickAnyPositionValue,
	};

	//拾取对象
	enum PickObject
	{
		//几何
		Geometry,

		//网格
		Mesh,

		//其他
		Other,
	};

	enum class ViewOperateMode
	{
		NoViewOperate,

		CameraOperate,

		PickOperate,
	};

	enum class CameraOperateMode
	{
		NoCameraOperate,

		Rotate,

		Translate,

		Zoom,

	};

	//拾取模式
	enum class PickMode
	{
		//不拾取
		NoPick,

		//拖拽
		DragPick,

		//点选
		SoloPick,

		//框选
		MultiplyPick,



		/***************点选模式*****************/
		//


		/***************拖拽模式*****************/

		//拖拽最小值线条
		//DragMinLine,

		//拖拽最大值线条
		//DragMaxLine,
	};

	enum class MultiplyPickMode
	{
		/**************框选模式***********/
		//矩形框选（角点）
		QuadPick,

		//圆形框选(圆心和半径)
		RoundPick,

		//多边形框选（多段线）
		PolygonPick,
	};

	/*
	* 显示模式：边界线模式，透视线框模式，网格面，网格面和线
	*/
	enum ShowFuntion
	{
		WireEdge,
		WireFrame,
		ElementFace,
		SmoothShaded,
	};

	/*
	 * 背景色模式：纯色，从左到右渐变，从上到下渐变
	 */
	enum BackGroundMode
	{
		PureColor,
		GradientL_R,
		GradientT_B,
	};

	/*
	 * 云图显示模式，节点还是网格
	 */
	enum NodeOrElement
	{
		//节点
		PostNode,

		//单元
		PostElement,
	};

	/*
	* 缩放视角（以物体的视角，以人的视角）
	*/
	enum ScalePerspectice
	{
		Model,
		Eye,
	};

	/*
	* 缩放类型（放大，缩小）
	*/
	enum ScaleDirection		//缩放类型
	{
		Zoom_in,		//放大
		Zoom_out,		//缩小
	};

	/*
	* 旋转类型（XY平面旋转，绕Z旋转）
	*/
	enum RotateType
	{
		Rotate_XY,
		Rotate_Z,
	};

	/*
	* 观察视角
	*/
	enum Perspective
	{
		Front,			//前视
		Rear,			//后视
		Top,			//上视
		Bottom,			//下视
		Left,			//左视
		Right,			//右视
		ISO1,			//标准1asiAlgo_AAG
		ISO2,			//标准2
		ISO3,			//标准3
		ISO4,			//标准4

	};


	/*
	* 坐标轴类型
	*/
	enum AxisType
	{
		Local_Rectangular,		//局部直角坐标系Rigid
		Local_Cylinder,			//局部直角坐标系
		Local_Sphere,			//局部球坐标系
	};


	/*
	* 更新连接渲染数据类型
	*/
	enum UpdateConnectDataType
	{

		//单个连接操作
		//刚性连接
		Append_Rigid_Single,
		Delete_Rigid_Single,
		Hide_Rigid_Single,
		Show_Rigid_Single,
		Highlight_Rigid_Single,
		NoHighlight_Rigid_Single,

		//柔性连接
		Append_Flex_Single,
		Delete_Flex_Single,
		Hide_Flex_Single,
		Show_Flex_Single,
		Highlight_Flex_Single,
		NoHighlight_Flex_Single,

		//MPC连接
		Append_MPC_Single,
		Delete_MPC_Single,
		Hide_MPC_Single,
		Show_MPC_Single,
		Highlight_MPC_Single,
		NoHighlight_MPC_Single,

		//面面绑定
		Append_FaceTie_Single,
		Delete_FaceTie_Single,
		Hide_FaceTie_Single,
		Show_FaceTie_Single,
		Highlight_FaceTie_Single,
		NoHighlight_FaceTie_Single,

		//电势耦合
		Append_PotentialCoupling_Single,
		Delete_PotentialCoupling_Single,
		Hide_PotentialCoupling_Single,
		Show_PotentialCoupling_Single,
		Highlight_PotentialCoupling_Single,
		NoHighlight_PotentialCoupling_Single,

		//点焊
		Append_Spotweld_Single,
		Delete_Spotweld_Single,
		Hide_Spotweld_Single,
		Show_Spotweld_Single,
		Highlight_Spotweld_Single,
		NoHighlight_Spotweld_Single,

		//节点刚体
		Append_Mpc1_Single,
		Delete_Mpc1_Single,
		Hide_Mpc1_Single,
		Show_Mpc1_Single,
		Highlight_Mpc1_Single,
		NoHighlight_Mpc1_Single,

		//所有连接操作
		Append_AllConnect,
		Delete_AllConnect,
		Hide_AllConnect,
		Show_AllConnect,
		Highlight_AllConnect,
		NoHighlight_AllConnect,
	};

	/*
	* 更新刚性墙渲染数据类型
	*/
	enum UpdateRigidWallDataType
	{

		//单个刚性墙操作
		//无限大平面
		Append_Planar_Single,
		Delete_Planar_Single,
		Hide_Planar_Single,
		Show_Planar_Single,
		Highlight_Planar_Single,
		NoHighlight_Planar_Single,	

		//有限大平面
		Append_Flat_Single,
		Delete_Flat_Single,
		Hide_Flat_Single,
		Show_Flat_Single,
		Highlight_Flat_Single,
		NoHighlight_Flat_Single,

		//长方体刚性墙
		Append_Cuboid_Single,
		Delete_Cuboid_Single,
		Hide_Cuboid_Single,
		Show_Cuboid_Single,
		Highlight_Cuboid_Single,
		NoHighlight_Cuboid_Single,

		//圆柱体刚性墙
		Append_Cylinder_Single,
		Delete_Cylinder_Single,
		Hide_Cylinder_Single,
		Show_Cylinder_Single,
		Highlight_Cylinder_Single,
		NoHighlight_Cylinder_Single,

		//球体刚性墙
		Append_Sphere_Single,
		Delete_Sphere_Single,
		Hide_Sphere_Single,
		Show_Sphere_Single,
		Highlight_Sphere_Single,
		NoHighlight_Sphere_Single,

		//箭头操作
		Append_RigidWallArrow_Single,
		Delete_RigidWallArrow_Single,
		Hide_RigidWallArrow_Single,
		Show_RigidWallArrow_Single,
		Highlight_RigidWallArrow_Single,
		NoHighlight_RigidWallArrow_Single,


		//所有刚性墙操作
		Append_AllRigidWall,
		Delete_AllRigidWall,
		Hide_AllRigidWall,
		Show_AllRigidWall,
		Highlight_AllRigidWall,
		NoHighlight_AllRigidWall,
	};
	/*
	* 更新盒子渲染数据类型
	*/
	enum UpdateBoxDataType
	{
		//常规盒子
		Append_NormalBox_Single,
		Delete_NormalBox_Single,
		Hide_NormalBox_Single,
		Show_NormalBox_Single,
		Highlight_NormalBox_Single,
		NoHighlight_NormalBox_Single,

		//指定盒子
		Append_SpecialBox_Single,
		Delete_SpecialBox_Single,
		Hide_SpecialBox_Single,
		Show_SpecialBox_Single,
		Highlight_SpecialBox_Single,
		NoHighlight_SpecialBox_Single,

		//所有刚性墙操作
		Append_AllBox,
		Delete_AllBox,
		Hide_AllBox,
		Show_AllBox,
		Highlight_AllBox,
		NoHighlight_AllBox,
	};	

	/*
	* 更新刚体连接渲染数据类型
	*/
	enum UpdateRigidConnectDataType
	{
		//单个连接操作

		//刚体RCSpring连接
		Append_RCSpring_Single,
		Delete_RCSpring_Single,
		Hide_RCSpring_Single,
		Show_RCSpring_Single,
		Highlight_RCSpring_Single,
		NoHighlight_RCSpring_Single,

		//刚体RCPrismatic连接
		Append_RCPrismatic_Single,
		Delete_RCPrismatic_Single,
		Hide_RCPrismatic_Single,
		Show_RCPrismatic_Single,
		Highlight_RCPrismatic_Single,
		NoHighlight_RCPrismatic_Single,

		//刚体RCRevolute连接
		Append_RCRevolute_Single,
		Delete_RCRevolute_Single,
		Hide_RCRevolute_Single,
		Show_RCRevolute_Single,
		Highlight_RCRevolute_Single,
		NoHighlight_RCRevolute_Single,

		//刚体RCSpherical连接
		Append_RCSpherical_Single,
		Delete_RCSpherical_Single,
		Hide_RCSpherical_Single,
		Show_RCSpherical_Single,
		Highlight_RCSpherical_Single,
		NoHighlight_RCSpherical_Single,

		//刚体RCCylindrical连接
		Append_RCCylindrical_Single,
		Delete_RCCylindrical_Single,
		Hide_RCCylindrical_Single,
		Show_RCCylindrical_Single,
		Highlight_RCCylindrical_Single,
		NoHighlight_RCCylindrical_Single,

		//刚体RCPlanar连接
		Append_RCPlanar_Single,
		Delete_RCPlanar_Single,
		Hide_RCPlanar_Single,
		Show_RCPlanar_Single,
		Highlight_RCPlanar_Single,
		NoHighlight_RCPlanar_Single,

		//刚体RCDamper连接
		Append_RCDamper_Single,
		Delete_RCDamper_Single,
		Hide_RCDamper_Single,
		Show_RCDamper_Single,
		Highlight_RCDamper_Single,
		NoHighlight_RCDamper_Single,

		//所有连接操作
		Append_AllRigidConnect,
		Delete_AllRigidConnect,
		Hide_AllRigidConnect,
		Show_AllRigidConnect,
		Highlight_AllRigidConnect,
		NoHighlight_AllRigidConnect,
	};
	/*
	* 更新接触渲染数据类型
	*/
	enum UpdateContactDataType
	{

		//单个接触操作
		//隐式面面接触
		Append_Contact_Single,
		Delete_Contact_Single,
		Hide_Contact_Single,
		Show_Contact_Single,
		Highlight_Contact_Single,
		NoHighlight_Contact_Single,
		//显示面面接触
		Append_FaceToFace_Single,
		Delete_FaceToFace_Single,
		Hide_FaceToFace_Single,
		Show_FaceToFace_Single,
		Highlight_FaceToFace_Single,
		NoHighlight_FaceToFace_Single,
		//显示点面接触
		Append_PointToFace_Single,
		Delete_PointToFace_Single,
		Hide_PointToFace_Single,
		Show_PointToFace_Single,
		Highlight_PointToFace_Single,
		NoHighlight_PointToFace_Single,
		//所有接触操作
		Append_AllContact,
		Delete_AllContact,
		Hide_AllContact,
		Show_AllContact,
		Highlight_AllContact,
		NoHighlight_AllContact,
		//接触整体高亮
		Edit_Contact_HightLightState,
	};
	/*
	* 更新接触渲染数据类型
	*/
	enum UpdateVectorDataType
	{
		//单个向量操作
		Append_Vector_Single,
		Delete_Vector_Single,
		Hide_Vector_Single,
		Show_Vector_Single,
		Highlight_Vector_Single,
		NoHighlight_Vector_Single,

		//所有向量操作
		Append_AllVector,
		Delete_AllVector,
		Hide_AllVector,
		Show_AllVector,
		Highlight_AllVector,
		NoHighlight_AllVector,
	};
	/*
	* 更新载荷边界渲染数据类型
	*/
	enum UpdateLoadBCDataType
	{
		//单个载荷操作
		//集中力
		Append_Force_Single,
		Delete_Force_Single,
		Hide_Force_Single,
		Show_Force_Single,
		Highlight_Force_Single,
		NoHighlight_Force_Single,

		//离心力
		Append_CentrifugalForce_Single,
		Delete_CentrifugalForce_Single,
		Hide_CentrifugalForce_Single,
		Show_CentrifugalForce_Single,
		Highlight_CentrifugalForce_Single,
		NoHighlight_CentrifugalForce_Single,

		//刚体载荷
		Append_RigidBodyForce_Single,
		Delete_RigidBodyForce_Single,
		Hide_RigidBodyForce_Single,
		Show_RigidBodyForce_Single,
		Highlight_RigidBodyForce_Single,
		NoHighlight_RigidBodyForce_Single,

		//体力
		Append_BodyForce_Single,
		Delete_BodyForce_Single,
		Hide_BodyForce_Single,
		Show_BodyForce_Single,
		Highlight_BodyForce_Single,
		NoHighlight_BodyForce_Single,

		//重力
		Append_Gravity_Single,
		Delete_Gravity_Single,
		Hide_Gravity_Single,
		Show_Gravity_Single,
		Highlight_Gravity_Single,
		NoHighlight_Gravity_Single,

		//均布载荷
		Append_Distribute_Single,
		Delete_Distribute_Single,
		Hide_Distribute_Single,
		Show_Distribute_Single,
		Highlight_Distribute_Single,
		NoHighlight_Distribute_Single,

		//压强
		Append_Pressure_Single,
		Delete_Pressure_Single,
		Hide_Pressure_Single,
		Show_Pressure_Single,
		Highlight_Pressure_Single,
		NoHighlight_Pressure_Single,

		//线载荷
		Append_LineLoad_Single,
		Delete_LineLoad_Single,
		Hide_LineLoad_Single,
		Show_LineLoad_Single,
		Highlight_LineLoad_Single,
		NoHighlight_LineLoad_Single,

		//螺栓预紧力
		Append_Pretension_Single,
		Delete_Pretension_Single,
		Hide_Pretension_Single,
		Show_Pretension_Single,
		Highlight_Pretension_Single,
		NoHighlight_Pretension_Single,

		//热流密度
		Append_Flux_Single,
		Delete_Flux_Single,
		Hide_Flux_Single,
		Show_Flux_Single,
		Highlight_Flux_Single,
		NoHighlight_Flux_Single,


		//对流换热
		Append_Conv_Single,
		Delete_Conv_Single,
		Hide_Conv_Single,
		Show_Conv_Single,
		Highlight_Conv_Single,
		NoHighlight_Conv_Single,


		//温度载荷
		Append_TempLoad_Single,
		Delete_TempLoad_Single,
		Hide_TempLoad_Single,
		Show_TempLoad_Single,
		Highlight_TempLoad_Single,
		NoHighlight_TempLoad_Single,


		//内部热源
		Append_InterHeat_Single,
		Delete_InterHeat_Single,
		Hide_InterHeat_Single,
		Show_InterHeat_Single,
		Highlight_InterHeat_Single,
		NoHighlight_InterHeat_Single,

		//边电荷源
		Append_LineQSource_Single,
		Delete_LineQSource_Single,
		Hide_LineQSource_Single,
		Show_LineQSource_Single,
		Highlight_LineQSource_Single,
		NoHighlight_LineQSource_Single,

		//边电流源
		Append_LineJSource_Single,
		Delete_LineJSource_Single,
		Hide_LineJSource_Single,
		Show_LineJSource_Single,
		Highlight_LineJSource_Single,
		NoHighlight_LineJSource_Single,

		//节点/体电荷源
		Append_QSource_Single,
		Delete_QSource_Single,
		Hide_QSource_Single,
		Show_QSource_Single,
		Highlight_QSource_Single,
		NoHighlight_QSource_Single,

		//节点电流源
		Append_JSource_Single,
		Delete_JSource_Single,
		Hide_JSource_Single,
		Show_JSource_Single,
		Highlight_JSource_Single,
		NoHighlight_JSource_Single,

		//电荷密度
		Append_QDensity_Single,
		Delete_QDensity_Single,
		Hide_QDensity_Single,
		Show_QDensity_Single,
		Highlight_QDensity_Single,
		NoHighlight_QDensity_Single,

		//面电荷密度
		Append_FaceQDensity_Single,
		Delete_FaceQDensity_Single,
		Hide_FaceQDensity_Single,
		Show_FaceQDensity_Single,
		Highlight_FaceQDensity_Single,
		NoHighlight_FaceQDensity_Single,

		//面电流密度
		Append_FaceJDensity_Single,
		Delete_FaceJDensity_Single,
		Hide_FaceJDensity_Single,
		Show_FaceJDensity_Single,
		Highlight_FaceJDensity_Single,
		NoHighlight_FaceJDensity_Single,

		//体电荷密度
		Append_BodyQDensity_Single,
		Delete_BodyQDensity_Single,
		Hide_BodyQDensity_Single,
		Show_BodyQDensity_Single,
		Highlight_BodyQDensity_Single,
		NoHighlight_BodyQDensity_Single,

		//体电流密度
		Append_BodyJDensity_Single,
		Delete_BodyJDensity_Single,
		Hide_BodyJDensity_Single,
		Show_BodyJDensity_Single,
		Highlight_BodyJDensity_Single,
		NoHighlight_BodyJDensity_Single,

		//磁通密度
		Append_BDensity_Single,
		Delete_BDensity_Single,
		Hide_BDensity_Single,
		Show_BDensity_Single,
		Highlight_BDensity_Single,
		NoHighlight_BDensity_Single,

		//磁通密度
		Append_MagneticCoercivity_Single,
		Delete_MagneticCoercivity_Single,
		Hide_MagneticCoercivity_Single,
		Show_MagneticCoercivity_Single,
		Highlight_MagneticCoercivity_Single,
		NoHighlight_MagneticCoercivity_Single,

		//表面电场强度
		Append_FaceEIntensity_Single,
		Delete_FaceEIntensity_Single,
		Hide_FaceEIntensity_Single,
		Show_FaceEIntensity_Single,
		Highlight_FaceEIntensity_Single,
		NoHighlight_FaceEIntensity_Single,


		//位移/角位移
		Append_Displacement_Single,
		Delete_Displacement_Single,
		Hide_Displacement_Single,
		Show_Displacement_Single,
		Highlight_Displacement_Single,
		NoHighlight_Displacement_Single,

		//位移/角位移
		Append_Velocity_Single,
		Delete_Velocity_Single,
		Hide_Velocity_Single,
		Show_Velocity_Single,
		Highlight_Velocity_Single,
		NoHighlight_Velocity_Single,

		//加速度/角加速度
		Append_Acceleration_Single,
		Delete_Acceleration_Single,
		Hide_Acceleration_Single,
		Show_Acceleration_Single,
		Highlight_Acceleration_Single,
		NoHighlight_Acceleration_Single,

		//初始应力
		Append_IniStress_Single,
		Delete_IniStress_Single,
		Hide_IniStress_Single,
		Show_IniStress_Single,
		Highlight_IniStress_Single,
		NoHighlight_IniStress_Single,

		//虚拟约束
		Append_VirtualConstraints_Single,
		Delete_VirtualConstraints_Single,
		Hide_VirtualConstraints_Single,
		Show_VirtualConstraints_Single,
		Highlight_VirtualConstraints_Single,
		NoHighlight_VirtualConstraints_Single,

		//温度约束
		Append_TempConstraints_Single,
		Delete_TempConstraints_Single,
		Hide_TempConstraints_Single,
		Show_TempConstraints_Single,
		Highlight_TempConstraints_Single,
		NoHighlight_TempConstraints_Single,

		//电势
		Append_PiezoelectricPotential_Single,
		Delete_PiezoelectricPotential_Single,
		Hide_PiezoelectricPotential_Single,
		Show_PiezoelectricPotential_Single,
		Highlight_PiezoelectricPotential_Single,
		NoHighlight_PiezoelectricPotential_Single,

		//磁矢量边界
		Append_MagneticVector_Single,
		Delete_MagneticVector_Single,
		Hide_MagneticVector_Single,
		Show_MagneticVector_Single,
		Highlight_MagneticVector_Single,
		NoHighlight_MagneticVector_Single,

		//电位边界
		Append_PotentialBC_Single,
		Delete_PotentialBC_Single,
		Hide_PotentialBC_Single,
		Show_PotentialBC_Single,
		Highlight_PotentialBC_Single,
		NoHighlight_PotentialBC_Single,

		//速度入口边界
		Append_VelocityEntryBoundary_Single,
		Delete_VelocityEntryBoundary_Single,
		Hide_VelocityEntryBoundary_Single,
		Show_VelocityEntryBoundary_Single,
		Highlight_VelocityEntryBoundary_Single,
		NoHighlight_VelocityEntryBoundary_Single,

		//压力入口边界
		Append_PressureInletBoundary_Single,
		Delete_PressureInletBoundary_Single,
		Hide_PressureInletBoundary_Single,
		Show_PressureInletBoundary_Single,
		Highlight_PressureInletBoundary_Single,
		NoHighlight_PressureInletBoundary_Single,
		//出流边界
		Append_OutflowBoundary_Single,
		Delete_OutflowBoundary_Single,
		Hide_OutflowBoundary_Single,
		Show_OutflowBoundary_Single,
		Highlight_OutflowBoundary_Single,
		NoHighlight_OutflowBoundary_Single,
		//压力出口边界
		Append_PressureOutletBoundary_Single,
		Delete_PressureOutletBoundary_Single,
		Hide_PressureOutletBoundary_Single,
		Show_PressureOutletBoundary_Single,
		Highlight_PressureOutletBoundary_Single,
		NoHighlight_PressureOutletBoundary_Single,
		//璧面边界
		Append_WallBoundary_Single,
		Delete_WallBoundary_Single,
		Hide_WallBoundary_Single,
		Show_WallBoundary_Single,
		Highlight_WallBoundary_Single,
		NoHighlight_WallBoundary_Single,
		//耦合璧面边界
		Append_CoupledWallBoundary_Single,
		Delete_CoupledWallBoundary_Single,
		Hide_CoupledWallBoundary_Single,
		Show_CoupledWallBoundary_Single,
		Highlight_CoupledWallBoundary_Single,
		NoHighlight_CoupledWallBoundary_Single,
		//对称边界
		Append_SymmetricalBoundary_Single,
		Delete_SymmetricalBoundary_Single,
		Hide_SymmetricalBoundary_Single,
		Show_SymmetricalBoundary_Single,
		Highlight_SymmetricalBoundary_Single,
		NoHighlight_SymmetricalBoundary_Single,

		/****可压缩流边界****/
			 //远场条件
			Append_FluidFarFieldBoundary1_Single,
			Delete_FluidFarFieldBoundary1_Single,
			Hide_FluidFarFieldBoundary1_Single,
			Show_FluidFarFieldBoundary1_Single,
			Highlight_FluidFarFieldBoundary1_Single,
			NoHighlight_FluidFarFieldBoundary1_Single,


			//出口边界
			Append_FluidOutflowBoundary1_Single,
			Delete_FluidOutflowBoundary1_Single,
			Hide_FluidOutflowBoundary1_Single,
			Show_FluidOutflowBoundary1_Single,
			Highlight_FluidOutflowBoundary1_Single,
			NoHighlight_FluidOutflowBoundary1_Single,

			//壁面边界
			Append_FluidWallBoundary1_Single,
			Delete_FluidWallBoundary1_Single,
			Hide_FluidWallBoundary1_Single,
			Show_FluidWallBoundary1_Single,
			Highlight_FluidWallBoundary1_Single,
			NoHighlight_FluidWallBoundary1_Single,

			//对称边界
			Append_FluidSymmetricBoundary1_Single,
			Delete_FluidSymmetricBoundary1_Single,
			Hide_FluidSymmetricBoundary1_Single,
			Show_FluidSymmetricBoundary1_Single,
			Highlight_FluidSymmetricBoundary1_Single,
			NoHighlight_FluidSymmetricBoundary1_Single,




			//初始速度
			Append_InitialVelocity_Single,
			Delete_InitialVelocity_Single,
			Hide_InitialVelocity_Single,
			Show_InitialVelocity_Single,
			Highlight_InitialVelocity_Single,
			NoHighlight_InitialVelocity_Single,

			//初始位移
			Append_InitialDisplacement_Single,
			Delete_InitialDisplacement_Single,
			Hide_InitialDisplacement_Single,
			Show_InitialDisplacement_Single,
			Highlight_InitialDisplacement_Single,
			NoHighlight_InitialDisplacement_Single,

			//所有载荷操作
			Append_AllLoad,
			Delete_AllLoad,
			Hide_AllLoad,
			Show_AllLoad,
			Highlight_AllLoad,
			NoHighlight_AllLoad,

			//所有边界操作
			Append_AllBC,
			Delete_AllBC,
			Hide_AllBC,
			Show_AllBC,
			Highlight_AllBC,
			NoHighlight_AllBC,

	};
	/*
	* 更新初始条件渲染数据类型
	*/
	enum UpdateInitialConditionDataType
	{
		Append_InitialVelocityEX_Single,
		Delete_InitialVelocityEX_Single,
		Hide_InitialVelocityEX_Single,
		Show_InitialVelocityEX_Single,
		Highlight_InitialVelocityEX_Single,
		NoHighlight_InitialVelocityEX_Single,

		//所有操作
		Append_AllInitialCondition,
		Delete_AllInitialCondition,
		Hide_AllInitialCondition,
		Show_AllInitialCondition,
		Highlight_AllInitialCondition,
		NoHighlight_AllInitialCondition,

	};
	/*
	* 更新刚体边界渲染数据类型
	*/
	enum UpdateRBCDataType
	{
		//位移/角位移
		Append_RBCDisplacement_Single,
		Delete_RBCDisplacement_Single,
		Hide_RBCDisplacement_Single,
		Show_RBCDisplacement_Single,
		Highlight_RBCDisplacement_Single,
		NoHighlight_RBCDisplacement_Single,

		//所有边界操作
		Append_AllRBC,
		Delete_AllRBC,
		Hide_AllRBC,
		Show_AllRBC,
		Highlight_AllRBC,
		NoHighlight_AllRBC,

	};
	/*
	* 更新文字渲染数据类型
	*/
	enum UpdateFontDataType
	{

		//单个通用文字操作
		Append_CommonFont_Single,
		Delete_CommonFont_Single,
		Hide_CommonFont_Single,
		Show_CommonFont_Single,
		Set_CommonFont_Color,
		Set_CommonFont_Size,
		//所有通用文字操作
		Append_CommonFont_All,
		Delete_CommonFont_All,
		Hide_CommonFont_All,
		Show_CommonFont_All,

		//单个固定文字操作
		Append_FixedFont_Single,
		Delete_FixedFont_Single,
		Hide_FixedFont_Single,
		Show_FixedFont_Single,
		Set_FixedFont_Color,
		Set_FixedFont_Size,
		//所有固定文字操作
		Append_FixedFont_All,
		Delete_FixedFont_All,
		Hide_FixedFont_All,
		Show_FixedFont_All,

		//单个载荷文字操作
		Append_LoadFont_Single,
		Delete_LoadFont_Single,
		Hide_LoadFont_Single,
		Show_LoadFont_Single,
		//所有载荷文字操作
		Append_LoadFont_All,
		Delete_LoadFont_All,
		Hide_LoadFont_All,
		Show_LoadFont_All,

		//单个刚性墙文字操作
		Append_RigidWallFont_Single,
		Delete_RigidWallFont_Single,
		Hide_RigidWallFont_Single,
		Show_RigidWallFont_Single,
		//所有刚性墙文字操作
		Append_RigidWallFont_All,
		Delete_RigidWallFont_All,
		Hide_RigidWallFont_All,
		Show_RigidWallFont_All,

		//单个向量文字操作
		Append_VectorFont_Single,
		Delete_VectorFont_Single,
		Hide_VectorFont_Single,
		Show_VectorFont_Single,
		//所有向量文字操作
		Append_VectorFont_All,
		Delete_VectorFont_All,
		Hide_VectorFont_All,
		Show_VectorFont_All,

		//单个初始条件文字操作
		Append_InitialConditionFont_Single,
		Delete_InitialConditionFont_Single,
		Hide_InitialConditionFont_Single,
		Show_InitialConditionFont_Single,
		//所有初始条件文字操作
		Append_InitialConditionFont_All,
		Delete_InitialConditionFont_All,
		Hide_InitialConditionFont_All,
		Show_InitialConditionFont_All,

		//单个边界文字操作
		Append_BCFont_Single,
		Delete_BCFont_Single,
		Hide_BCFont_Single,
		Show_BCFont_Single,
		//所有边界文字操作
		Append_BCFont_All,
		Delete_BCFont_All,
		Hide_BCFont_All,
		Show_BCFont_All,

		//单个连接文字操作
		Append_ConnectFont_Single,
		Delete_ConnectFont_Single,
		Hide_ConnectFont_Single,
		Show_ConnectFont_Single,
		//所有连接文字操作
		Append_ConnectFont_All,
		Delete_ConnectFont_All,
		Hide_ConnectFont_All,
		Show_ConnectFont_All,

		//单个局部坐标系文字操作
		Append_LocalAxisFont_Single,
		Delete_LocalAxisFont_Single,
		Hide_LocalAxisFont_Single,
		Show_LocalAxisFont_Single,
		//所有局部坐标系文字操作
		Append_LocalAxisFont_All,
		Delete_LocalAxisFont_All,
		Hide_LocalAxisFont_All,
		Show_LocalAxisFont_All,

		//后处理
		Initial_Font_Post,
		Append_Font_Post,
		Delete_Font_Post,
		Edit_Font_ShowState_Post,


	};


	/*
	* 更新局部坐标轴渲染数据类型
	*/
	enum UpdateLocalAxisDataType
	{
		//单个局部坐标系操作
		Append_LocalAxis_Single,
		Delete_LocalAxis_Single,
		Hide_LocalAxis_Single,
		Show_LocalAxis_Single,
		Highlight_LocalAxis_Single,
		NoHighlight_LocalAxis_Single,
		//所有局部坐标系操作
		Append_LocalAxis_All,
		Delete_LocalAxis_All,
		Hide_LocalAxis_All,
		Show_LocalAxis_All,
		Highlight_LocalAxis_All,
		NoHighlight_LocalAxis_All,

	};

	/*
	* 更新箭头渲染数据类型
	*/
	enum UpdateArrowDataType
	{
		//单个箭头操作
		Append_Arrow_Single,
		Delete_Arrow_Single,
		Hide_Arrow_Single,
		Show_Arrow_Single,
		Highlight_Arrow_Single,
		NoHighlight_Arrow_Single,
		//所有箭头操作
		Append_Arrow_All,
		Delete_Arrow_All,
		Hide_Arrow_All,
		Show_Arrow_All,
		Highlight_Arrow_All,
		NoHighlight_Arrow_All,

	};
	/*
	* 	图标操作类型（添加、删除、显示、隐藏、高亮、取消高亮）
	*/

	enum  Lable_operate_type
	{
		Append,
		Delete,
		Show,
		Hide,
		Highlight,
		NoHighlight,

		//通用文字用
		SetColor,
		SetSize

	};

	/*
	* 	更新后处理矢量箭头渲染数据类型
	*/
	enum UpdateVecArrowDataType
	{
		//单个箭头操作
		Append_VecArrow_Single,
		Delete_VecArrow_Single,
		Hide_VecArrow_Single,
		Show_VecArrow_Single,
		//所有箭头操作
		Append_VecArrow_All,
		Delete_VecArrow_All,
		Hide_VecArrow_All,
		Show_VecArrow_All,
	};


}
