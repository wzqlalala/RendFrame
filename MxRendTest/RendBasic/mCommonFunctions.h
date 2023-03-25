#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

#include "rendbasic_global.h"

//MBasicFunction
#include "mBasicEnum.h"

//QT
#include <QObject>
#include <QPushButton>
#include <QColor>
#include <QListWidget>
#include <QVector3D>
#include <QTreeWidgetItem>
#include <set>
#include <QTableWidget>
#include <QHeaderView>
#include <QPair>
#include <QPoint>
#include <QHash>
#include <QStackedWidget>

namespace MBasicFunction
{
	class RENDBASIC_EXPORT mCommonFunctions : public QObject
	{
	public:
		/*
		* 设置颜色按钮的色块
		*/
		static void setButtonColor(QPushButton* button, const QColor& color);

		/*
		* 对传入的列表过滤，返回过滤后的列表（使用正则表达式匹配的过滤）
		*/
		static QStringList filterList(const QStringList& list1, const QString& filter);

		/*
		* 普通过滤，包含目标字符即可
		*/
		static QStringList filterList1(const QStringList& list1, const QString& filter);

		/*
		* 清空QListWidget
		*/
		static void clearStackedWidget(QStackedWidget* stackedwidge);

		/*
		* 清空QListWidget
		*/
		static void clearListWidget(QListWidget* listwidget);

		/*
		* 获取QListWidget的选中项
		*/
		static QStringList getSelectedList(QListWidget* listWidget);

		/*
		* 获取QListWidget的未选中项
		*/
		static QStringList getUnselectedList(QListWidget* listWidget);

		/*
		* 获取QListWidget选中项的索引
		*/
		static QList<int> getSelectedIndexes(QListWidget* listwidget);

		/*
		* 获取QListWidget选中项的名称
		*/
		static QString getSelectedString(QListWidget* listwidget);

		/*
		* 获取QListWidget所有Checked项的名称
		*/
		static QStringList getCheckedItemNames(QListWidget* listwidget);

		/*
		* 设置QListWidget所有项是否选中
		*/
		static void setListWidgetItemSelected(QListWidget* listwidget, bool isselect);

		/*
		* 设置QListWidget部分选中
		*/
		static void setListWidgetItemSelected(QListWidget* listwidget, const QStringList& names);

		/*
		* 设置QListWidget某些项Checked
		*/
		static void setListWidgetItemChecked(QListWidget* listwidget, const QStringList& names);

		/*
		* 创建有复选框的QListWidget
		*/
		static void addCheckableListWidget(QListWidget* listwidget, const QStringList& texts);

		/*
		* 设置QListWidget所有已选中的项未选中
		*/
		static void setListWidgetUnchecked(QListWidget* listwidget);

		/*
		* 设置QListWidget所有项未选中
		*/
		static void setListWidgetUnchecked1(QListWidget* listwidget);

		/*
		* 设置QListWidget最多只有一项选中
		*/
		static void setListWidgetItemState(QListWidget* listwidget, QListWidgetItem* item);

		/*
		* 根据单元类型获取单元包含的节点个数
		*/
		static int elementNodeCount(const QString& type);
		static int elementNodeCount(ElementType elementType);
		/*
		* 根据单元枚举类型获取单元大类
		*/
		static QString getElementMainType(ElementType elementType);

		/*
		* 将ID列表转化成字符串
		* 参数type表示ID类型：节点(0)或单元(1)
		*/
		static QString transformListToString(const std::set<int> ids, int type);

		/*
		* 将ID列表转化成字符串（混合类型）
		*/
		static QString transformListToString(const std::set<int> nids, const std::set<int> eids);


		/*
		* 将ID字符串转化成列表
		* 返回值为ID类型：节点(0)或单元(1)
		* 参数ids为字符串解析的ID列表
		*/
		static int transformStringToList(const QString& idstring, QList<int>& ids);

		/*
		* 将ID字符串转化成列表
		* 返回值为ID类型：节点(0)或单元(1)
		* 参数ids为字符串解析的ID列表
		*/
		static int transformStringToList(const QString& idstring, std::set<int>& ids);

		/*
		* 将ID字符串转换成列表
		* 参数idstring为ID字符串
		* 参数nids为节点ID列表，参数eids为单元ID列表
		*/
		static bool transformStringToList(const QString& idstring, std::set<int>& nids, std::set<int>& eids);

		/*
		* 将ID字符串转化为列表
		* 形如1,2,5-10
		*/
		static bool transformMeshStringToSet(const QString& idstring, std::set<int>& mids);

		/*
		* 将Set<int>类型转化成Qlist<int>类型
		*/
		static bool transformSetToList(const std::set<int> id_set, QList<int>& id_list);
		/*
		 将Qlist<int>类型转化成Set<int>类型
		*/
		static bool transformListToSet(const QList<int> id_list, std::set<int>& id_set);
		/*
		* 将QVector<int>转换为std::set<int>
		*/
		static std::set<int> transformQVectorToSet(const QVector<int> vec);

		/*
		* 将QStringList转换成QString，用逗号分割
		*/
		static QString transformQStringListToQString(const QStringList& strlist);

		/*
		* 通过三点计算角度，p3为角点
		*/
		static double getThreePointAngle(const QVector3D p1, const QVector3D p2, const QVector3D p3);

		/*
		* 获取两点的中点坐标
		*/
		static QVector3D getCenterPointCoord(const QVector3D p1, const QVector3D p2);

		/*
		* 获取三角形的形心
		*/
		static QVector3D getCenterPointCoord(const QVector3D p1, const QVector3D p2, const QVector3D p3);

		/*
		* 获取四边形的形心
		*/
		static QVector3D getCenterPointCoord(const QVector3D p1, const QVector3D p2, const QVector3D p3, const QVector3D p4);

		/*
		* 获取两点沿轴向收缩后坐标
		* 参数p1和p2分别为收缩前的两点坐标
		* 参数val为收缩系数，范围为0-1，0表示不收缩，1表示同时收缩到中点
		*/
		static void getShrinkPosition(QVector3D& p1, QVector3D& p2, double val);

		/*
		 * 将枚举的单元类型转化为字符串
		 */
		static QString getElementString(ElementType elementType);

		/*
		 * 将字符串的单元类型转化为枚举
		*/
		static ElementType getElementType(QString elementstring);

		/*
		* 根据场类型来确定默认单元类型(按照单场的类型判断，多物理场在外部调用循环单场)
		*/
		static QHash<MeshType,ElementType> getDefaultElementType(AnalysisModule analysisModule,int order = 1);

		/*
		* 根据场类型和网格类型来确定默认单元类型(按照单场的类型判断，多物理场在外部调用循环单场)
		*/
		static ElementType getDefaultElementType(AnalysisModule analysisModule, MeshType meshType, int order = 1);

		/*
		* 根据低阶单元类型获取高阶单元类型
		*/
		static ElementType getHighOrderElementType(ElementType elementType);

		/*
		* 根据高阶单元类型获取低阶单元类型
		*/
		static ElementType getLowOrderElementType(ElementType elementType);

		/*
		 * 判断某单元类型是一阶还是二阶，并且也判断其拓扑形状
		 */
		static QString getElementInfo(ElementType elementType, int &order);

		/*
		* 设置某父节点下的所有子节点选中状态
		*/
		static void setChildrenItemState(QTreeWidgetItem* pitem, bool ischeck);

		/*
		* 根据名称查找QListWidget中的项，并设置该项选中
		*/
		static QListWidgetItem* getListWidgetItem(QListWidget* widget, const QString& name);

		/*
		* 获取QListWidget选中项中的第一个
		*/
		static QListWidgetItem* getFirstSelectedItem(QListWidget* widget);

		/*
		* QVector3D 类型转换为 QString类型
		*/
		static QString transformVector3DToString(const QVector3D pos);
		/*
		* QVector(QVector3D) 类型转换为 QString类型
		*/
		static QString transformVector_Vector3DToString(const QVector<QVector3D> pos);

		/*
		*  QString 类型转换为 QVector3D类型
		*/
		static QVector3D transformStringToVector3D(const QString pos);
		/*
		*  QString 类型转换为 QVector(QVector3D)类型
		*/
		static  QVector<QVector3D> transformStringToVector_Vector3D(const QString pos);
		/*
		*  判断三维线段是否相交
		*/
		static bool LineIntersect3D(const QVector3D &line1P1, const QVector3D &line1P2, const QVector3D &line2P1, const QVector3D &line2P2, QVector3D &intersectP);

		/*
		* 根据网格字符串类型获取枚举类型
		*/
		static MeshType transformTypeStrToEnum(const QString& typestr);
		static MeshType transformTypeStrToEnum(ElementType elementType);
		/*
		* 获取变量的分量列表
		*/
		static QStringList getVariableComponents(const QString& var);

		/*
		* 根据变量名获取后处理渲染的类型，0-节点，1-单元
		*/
		static int getPostRendType(const QString& var);

		/*
		* 设置表格样式
		*/
		static void setTableWidgetStyle(QTableWidget* tw, const QStringList& headers);

		/*
		* 获取表格中的XY数据
		*/
		static void getTableWidgetDatas(QTableWidget* tw, QList<QPair<double, double>>& datas);

		/*
		* 设置表格数据
		*/
		static void setTableWidgetDatas(QTableWidget* tw, QList<QPair<double, double>>& datas);

		/*
		* 清空表格
		*/
		static void clearTableWidgetDatas(QTableWidget* tw);

		/*
		* 通过鼠标点击的位置获取弹窗的位置
		*/
		static void setWidgetPosByMouse(QWidget *parnetWidget, QWidget* popWidget);

		/*
		* 通过模型的最值和滑条的比例确定平面的经过的点
		*/
		static QVector3D caculatePlaneRange(float minx,float miny,float minz,float maxx, float maxy, float maxz, QVector3D normal,float ratio);

		/*
		* 通过模型的最值和平面经过的点确定滑条的比例
		*/
		static float caculatePlaneRatio(float minx, float miny, float minz, float maxx, float maxy, float maxz, QVector3D normal, QVector3D vertex);

		/*
		* 通过模型的最值确定模型的中心和最大半径
		*/
		static void caculateModelCenterAndRaduis(float minx, float miny, float minz, float maxx, float maxy, float maxz, QVector3D &center, float &radius);

		/*
		* 获取一个单元的体积
		*/
		static double getMeshVolume(QVector<QVector3D> vertexs, MeshType meshType);

		/*
		* 输出的英文转中文
		*/
		static QString transformOutputEToC(QString str);

		/*
		* 输出的中文转英文
		*/
		static QString transformOutputCToE(QString str);

		/*
		* 文本输出的英文转中文
		*/
		static QString transformTextOutputEToC(QString str);

		/*
		* 文本输出的中文转英文
		*/
		static QString transformTextOutputCToE(QString str);

		/*
		* 将分析类型枚举转换为字符串
		*/
		static QString transformAnalysisEnumToString(AnalysisModule am);

		/*
		* 使用正则表达式提取括号中的内容
		*/
		static QString getParenthesesContent(const QString& text);

		/*
		* 复制表格内容到剪切板
		*/
		static void copyTableToBoard(QTableWidget* tw);

		/*
		* 将剪切板内容粘贴到表格中
		* 参数isone表示是否限制表格仅为一行（材料模块需要）
		*/
		static void pasteBoardToTable(QTableWidget* tw, bool isone);

	private:
		//单元类型：字符串、枚举互转
		static QStringList _elementStringList;

		//输出：字符串英文转中文
		static QHash<QString,QString> _outputStringList;

		//输出：字符串中文转英文
		static QHash<QString, QString> _outputStringList1;

		//文本输出：字符串英文转中文
		static QHash<QString, QString> _textOutputStringList;

		//文本输出：字符串中文转英文
		static QHash<QString, QString> _textOutputStringList1;
	};
}