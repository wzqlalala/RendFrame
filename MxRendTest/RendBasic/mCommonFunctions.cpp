#include "mCommonFunctions.h"
//#include "mOutputMessage.h"
//#include "mGlobalSignals.h"
#include "mBasicEnum.h"
#include<math.h>
//QT
#include <QPixmap>
#include <QRegExp>
#include <QRegExpValidator>
#include <QApplication>
#include <QClipboard>

namespace MBasicFunction
{
	QStringList mCommonFunctions::_elementStringList
	{ "None","SENSOR","PRETENSIONER","MASS1","ELEMENTSPH","SLIPRING","RETRACTOR","SEATBELT","D2R2","D2R3","D3R2","D3R3","D2B2","D2B3","D3B2","D3B3","PIPE","ELAS1","SPRING1","SPRING2","SPRING3","GAP","CBUSH","TS2","DB2","HB2","BS2","DISCRETE","PST3","PST6","PST3-1","PST3-2","PST3-3","TRIA3","C0","PET3","PET6","PET3-1","PET3-2","PET3-3","PET3H","PET6H","AST3","AST6","AST3H",
		"AST6H","CPDSG","DSG","SST3","SST6","CPDSG-3","MBT3","MBT6","SPT3","PSQ4","PSQ8","PSQ4R","PSQ8R","PSQ4I","PSQ8I","PEQ4","PEQ8","PEQ4R","PEQ8R","PEQ4H","PEQ8H","PEQ4I",
		"PEQ8I","ASQ4","ASQ8","ASQ4R","ASQ8R","ASQ4H","ASQ8H","ASQ4I","ASQ8I","QPH","Q4R","MITC4","SSQ4","SSQ8","BT","HL","CY","BTSHELL","MBQ4","MBQ4R","MBQ8","SPQ4","D3T4","D3T10","D3T4E","D3T4-1","D3T4-2","D3T4-3","D3T10M", "D3T10E","D3P5","D3P6","D3P6E",
		"D3P6H","D3P15","D3P15H","D3P15E","D3S6R","SOLID","D3H8","D3H20","D3H8R","D3H20R","D3H8H","D3H20H","D3H8I","D3H20I","D3H8E","D3H20E","D3S8R","EMD3T4","EMD3T4-3","EMD3P6","EMD3H8" };

	QHash<QString, QString> mCommonFunctions::_outputStringList
	{
		{"displacement","位移"},{"disp","位移"},{"d","位移"},{"stress","节点应力"},{"s","节点应力"},{"strain","节点应变"},{"pe","节点应变"}, {"estress","单元应力"},{"es","单元应力"},
		{"estrain","单元应变"},{"epe","单元应变"},{"simple_estress","基于单元磨平的节点应力"},{"simple_es","基于单元磨平的节点应力"},{"simple_estrain","基于单元磨平的节点应变"},{"simple_epe","基于单元磨平的节点应变"},
		{"frequency","频率"},{"fre","频率"},{"peeq","节点塑性应变"},{"epeeq","单元塑性应变"},
		{"temperature","温度"},{"temp","温度"},{"t","温度"},{"temperaturegradient","温度梯度"},{"tempgrad","温度梯度"},{"tg","温度梯度"},{"etemperaturegradient","单元温度梯度"},{"etempgrad","单元温度梯度"},{"etg","单元温度梯度"},
		{"heatflux","热流密度"},{"hf","热流密度"},{"eheatflux","单元热流密度"},{"ehf","单元热流密度"},
		{"psd_disp","位移PSD"},{"psd_stress","节点应力PSD"}, { "psd_strain","节点应变PSD" },{ "rms_vonmisesstress","节点等效米赛斯应力RMSPSD" },
		{"epot","电势" },{"edisp","电位移" },{"efield","电场强度" },{"echarge","电荷" },{"load_vector","载荷矢量"}
	};

	QHash<QString, QString> mCommonFunctions::_outputStringList1
	{
		{"位移","displacement"},{"节点应力","stress"},{"节点应变","strain"}, {"单元应力","estress"},
		{"单元应变","estrain"},{"基于单元磨平的节点应力","simple_estress"},{"基于单元磨平的节点应变","simple_estrain"},
		{"频率","frequency"},{"节点塑性应变","peeq"},{"单元塑性应变","epeeq"},
		{"温度","temperature"},{"温度梯度","temperaturegradient"},{"单元温度梯度","etemperaturegradient"},
		{"热流密度","heatflux"},{"单元热流密度","eheatflux"},
		{"位移PSD","psd_disp"},{"节点应力PSD","psd_stress"}, { "节点应变PSD","psd_strain"},{"节点等效米赛斯应力RMSPSD","rms_vonmisesstress" },
		{"单元应力PSD","psd_estress"}, { "单元应变PSD","psd_estrain"},{"单元等效米赛斯应力RMSPSD","rms_vonmisesestress" },
		{"电势","EPOT" },{"电位移","EDISP" },{"电场强度","EFIELD" },{"电荷","ECHARGE" },{"载荷矢量","load_vector"}
	};

	QHash<QString, QString> mCommonFunctions::_textOutputStringList
	{
		{"displacement","位移"},{"disp","位移"},{"d","位移"},{"stress","应力"},{"s","应力"},{"strain","应变"},{"pe","应变"}, {"force","载荷"},{"f","载荷"}
	};

	QHash<QString, QString> mCommonFunctions::_textOutputStringList1
	{
		{"位移","displacement"},{"应力","stress"},{"应变","strain"}, {"载荷","force"}
	};

	void mCommonFunctions::setButtonColor(QPushButton* button, const QColor& color)
	{

		if (button == nullptr)
		{
			return;
		}
		QPixmap cpix(QSize(50, 40));
		cpix.fill(color);
		button->setIcon(cpix);
		button->setText(color.name());
	}

	QStringList mCommonFunctions::filterList(const QStringList& list1, const QString& filter)
	{
		QStringList list2;

		//判断filter是否为空
		if (filter.isEmpty())
		{
			return list2;
		}

		//判断filter是否为*
		if (filter == "*")
		{
			return list1;
		}

		QString reStr = filter;
		//判断filter开头是否有*，若没有，则在开头添加^
		if (filter.at(0) != "*")
		{
			reStr = "^" + reStr;
		}
		//判断filter结尾是否有*，若没有，则在开头添加$
		if (filter.at(filter.size() - 1) != "*")
		{
			reStr = reStr + "$";
		}
		//将所有*替换成(\S)+
		while (reStr.contains("*"))
		{
			reStr = reStr.replace("*", "(\\S)+");
		}
		
		QRegExp re(reStr);
		QRegExpValidator v(re, 0);

		//开始过滤
		int pos = 0;
		QValidator::State vstate;
		const int count1 = list1.size();
		for (int i = 0; i < count1; ++i)
		{
			QString str = list1.at(i);
			vstate = v.validate(str, pos);

			if (vstate == QValidator::Acceptable)
			{
				list2.append(str);
			}
		}
		return list2;
	}

	QStringList mCommonFunctions::filterList1(const QStringList& list1, const QString& filter)
	{
		if (filter.isEmpty())
		{
			return list1;
		}
		QStringList filterList;
		const int nlist = list1.size();
		for (int i = 0; i < nlist; ++i)
		{
			QString tepStr = list1.at(i);
			if (tepStr.contains(filter,Qt::CaseInsensitive))
			{
				filterList.append(tepStr);
			}
		}
		return filterList;
	}

	void mCommonFunctions::clearStackedWidget(QStackedWidget * stackedwidget)
	{
		int count = stackedwidget->count();
		for (int i = 0; i < count; ++i)
		{
			QWidget *w = stackedwidget->widget(i);
			//stackedwidget->removeWidget(w);
			if (w != nullptr)
			{
				delete w;
				w = nullptr;
			}
		}
		delete stackedwidget;
		stackedwidget = new QStackedWidget;
	}

	void mCommonFunctions::clearListWidget(QListWidget* listwidget)
	{
		if (listwidget == nullptr)
		{
			return;
		}

		const int nrow = listwidget->count();
		for (int i = 0; i < nrow; ++i)
		{
			QListWidgetItem* tepItem = listwidget->item(i);
			if (tepItem != nullptr)
			{
				delete tepItem;
				tepItem = nullptr;
			}
		}
		listwidget->clear();
	}

	QStringList mCommonFunctions::getSelectedList(QListWidget* listWidget)
	{
		QStringList selectedList;
		if (listWidget == nullptr)
		{
			return selectedList;
		}
		
		const int nitem = listWidget->count();
		for (int i = 0; i < nitem; ++i)
		{
			QListWidgetItem* tepItem = listWidget->item(i);
			if (tepItem != nullptr)
			{
				if (tepItem->isSelected())
				{
					QString itemname = tepItem->text();
					selectedList.append(itemname);
				}
			}
		}
		return selectedList;
	}

	QStringList mCommonFunctions::getUnselectedList(QListWidget * listWidget)
	{
		QStringList unselectedList;
		if (listWidget == nullptr)
		{
			return unselectedList;
		}

		const int nitem = listWidget->count();
		for (int i = 0; i < nitem; ++i)
		{
			QListWidgetItem* tepItem = listWidget->item(i);
			if (tepItem != nullptr)
			{
				if (!tepItem->isSelected())
				{
					QString itemname = tepItem->text();
					unselectedList.append(itemname);
				}
			}
		}
		return unselectedList;
	}

	QList<int> mCommonFunctions::getSelectedIndexes(QListWidget* listwidget)
	{
		QList<int> lists;
		if (listwidget == nullptr)
		{
			return lists;
		}

		const int nitem = listwidget->count();
		for (int i = 0; i < nitem; ++i)
		{
			QListWidgetItem* tepItem = listwidget->item(i);
			if (tepItem != nullptr)
			{
				if (tepItem->isSelected())
				{
					lists.append(i);
				}
			}
		}
		return lists;
	}

	QString mCommonFunctions::getSelectedString(QListWidget* listwidget)
	{
		if (listwidget == nullptr)
		{
			return QString();
		}
		if (listwidget->count() == 0)
		{
			return QString();
		}
		//获取项
		QListWidgetItem* item = nullptr;
		QList<QListWidgetItem*> items = listwidget->selectedItems();
		if (items.size() == 0)
		{
			//返回列表中第一项的名称
			item = listwidget->item(0);
		}
		else
		{
			//返回选中的第一项名称
			item = items.at(0);
		}
		//返回名称
		if (item != nullptr)
		{
			return item->text();
		}
		else
		{
			return QString();
		}
	}

	QStringList mCommonFunctions::getCheckedItemNames(QListWidget * listwidget)
	{
		QStringList names;
		if (listwidget == nullptr)
		{
			return names;
		}
		const int nitem = listwidget->count();
		for (int i = 0; i < nitem; ++i)
		{
			QListWidgetItem* item = listwidget->item(i);
			if (item != nullptr&&item->checkState() == Qt::Checked)
			{
				names.append(item->text());
			}
		}
		return names;
	}

	void mCommonFunctions::setListWidgetItemSelected(QListWidget * listwidget, bool isselect)
	{
		if (listwidget == nullptr)
		{
			return;
		}
		const int n = listwidget->count();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* item = listwidget->item(i);
			if (item != nullptr)
			{
				item->setSelected(isselect);
			}
		}
	}

	void mCommonFunctions::setListWidgetItemSelected(QListWidget * listwidget, const QStringList & names)
	{
		if (listwidget == nullptr)
		{
			return;
		}
		const int n = listwidget->count();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* item = listwidget->item(i);
			if (item != nullptr)
			{
				QString name = item->text();
				if (names.contains(name))
				{
					item->setSelected(true);
				}
				else
				{
					item->setSelected(false);
				}
			}
		}
	}

	void mCommonFunctions::setListWidgetItemChecked(QListWidget * listwidget, const QStringList & names)
	{
		if (listwidget == nullptr)
		{
			return;
		}
		const int nitem = listwidget->count();
		for (int i = 0; i < nitem; ++i)
		{
			QListWidgetItem* item = listwidget->item(i);
			if (item != nullptr)
			{
				if (names.contains(item->text()))
				{
					item->setCheckState(Qt::Checked);
				}
				else
				{
					item->setCheckState(Qt::Unchecked);
				}
			}
		}
	}

	void mCommonFunctions::addCheckableListWidget(QListWidget* listwidget, const QStringList& texts)
	{
		if (listwidget == nullptr)
		{
			return;
		}
		const int nitem = texts.size();
		for (int i = 0; i < nitem; ++i)
		{
			QString txt = texts.at(i);
			QListWidgetItem* item = new QListWidgetItem(txt);
			item->setCheckState(Qt::Unchecked);
			listwidget->addItem(item);
		}
	}

	void mCommonFunctions::setListWidgetUnchecked(QListWidget* listwidget)
	{
		if (listwidget == nullptr)
		{
			return;
		}
		const int nitem = listwidget->count();
		for (int i = 0; i < nitem; ++i)
		{
			QListWidgetItem* item = listwidget->item(i);
			if (item != nullptr&&item->checkState() == Qt::Checked)
			{
				item->setCheckState(Qt::Unchecked);
			}
		}
	}

	void mCommonFunctions::setListWidgetUnchecked1(QListWidget * listwidget)
	{
		if (listwidget == nullptr)
		{
			return;
		}
		const int nitem = listwidget->count();
		for (int i = 0; i < nitem; ++i)
		{
			QListWidgetItem* item = listwidget->item(i);
			if (item != nullptr)
			{
				item->setCheckState(Qt::Unchecked);
			}
		}
	}

	void mCommonFunctions::setListWidgetItemState(QListWidget* listwidget, QListWidgetItem* item)
	{
		if (listwidget == nullptr || item == nullptr)
		{
			return;
		}

		Qt::CheckState cs = item->checkState();
		if (cs == Qt::Checked)
		{
			item->setCheckState(Qt::Unchecked);
		}
		else
		{
			//设置复选框状态
			const int nitem = listwidget->count();
			for (int i = 0; i < nitem; ++i)
			{
				QListWidgetItem* tepItem = listwidget->item(i);
				if (tepItem != nullptr&&tepItem != item)
				{
					tepItem->setCheckState(Qt::Unchecked);
				}
			}
			item->setCheckState(Qt::Checked);
		}
	}

	int mCommonFunctions::elementNodeCount(const QString& type)
	{
		QString typestr = type.toUpper();
		if (typestr == "CPDSG"|| typestr == "SST3"|| typestr == "TRIA3"|| typestr == "PST3" ||		//三节点单元（将常用类型放在最前面，以提高效率）
			typestr == "PET3"|| typestr == "PST3-1"|| typestr == "PST3-2"|| typestr == "PST3-3" ||
			typestr == "PET3-1"|| typestr == "PET3-2"|| typestr == "PET3-3" || typestr=="CPDSG-3" ||
			typestr == "EST" || typestr == "MBT3"|| typestr.contains(QString("PET3"))|| typestr == "DSG"||
			typestr == "C0")
		{
			return 3;
		}
		else if (typestr == "MITC4" || typestr == "PSQ4" || typestr == "PEQ4" || typestr == "D3T4" ||		//四节点单元（将常用类型放在最前面，以提高效率）
			typestr == "SSQ4" || typestr == "QPH" || typestr == "Q4R" || typestr == "SPQ4" ||
			typestr == "MBQ4" || typestr == "MBQ4R" || typestr == "BT" || typestr == "D3T4-1" ||
			typestr == "D3T4-2" || typestr == "D3T4-3"|| typestr == "EMD3T4" || typestr == "EMD3T4-3" || typestr == "BTSHELL" || typestr == "HL" || typestr == "CY" || typestr == "D3T4E")
		{
			return 4;
		}
		else if (typestr == "D3H8" || typestr == "D3H8R" || typestr == "D3H8I" || typestr == "D3H8E" || typestr == "SST8" ||		//八节点单元（将常用类型放在最前面，以提高效率）
			typestr == "PEQ8" || typestr == "PSQ8" || typestr == "D3S8R" || typestr == "EMD3H8" || typestr == "SOLID")
		{
			return 8;
		}
		else if (typestr == "D3P6" || typestr == "PST6" || typestr == "PET6" || typestr == "SST6" ||		//六节点单元（将常用类型放在最前面，以提高效率）
			typestr == "D3S6R" || typestr.contains(QString("D3P6")) || typestr.contains(QString("d3p6")) || typestr == "EMD3P6")
		{
			return 6;
		}
		else if (typestr == "D3R2" || typestr == "D3B2" || typestr == "ELAS1" || typestr == "DB2" ||		//二节点单元（将常用类型放在最前面，以提高效率）
			typestr == "HB2" || typestr == "BS2" || typestr == "DISCRETE" || typestr == "SPRING1" || 
			typestr == "SEATBELT" || typestr == "PIPE"||typestr=="GAP" || typestr == "TS2" || typestr == "SPRING3" || typestr == "CBUSH")
		{
			return 2;
		}
		else if (typestr == "D3P5")																			//五节点单元（将常用类型放在最前面，以提高效率）
		{
			return 5;
		}
		else if (typestr == "MASS1"|| typestr == "SLIPRING"||typestr == "RETRACTOR"||typestr == "ELEMENTSPH" || typestr == "SPRING2")																		//一节点单元（将常用类型放在最前面，以提高效率）
		{
			return 1;
		}
		else if (typestr == "D3T10"|| typestr == "D3T10M" || typestr == "D3T10E")													//十节点单元（将常用类型放在最前面，以提高效率）
		{
			return 10;
		}
		else if (typestr == "D3P15")																		//十五节点单元（将常用类型放在最前面，以提高效率）
		{
			return 15;
		}
		else if (typestr == "D3H20"|| typestr == "D3H20E")																		//二十节点单元（将常用类型放在最前面，以提高效率）
		{
			return 20;
		}
		else
		{
			return 0;
		}
		
	}

	int mCommonFunctions::elementNodeCount(ElementType elementType)
	{
		switch (elementType)
		{
		case MBasicFunction::Mass1:
			return 1;
			break;
		case MBasicFunction::ElementSPH:
			return 1;
			break;
		case MBasicFunction::Slipring:
			return 1;
			break;
		case MBasicFunction::Retractor:
			return 1;
			break;
		case MBasicFunction::SPRING2:
			return 1;
			break;
		case MBasicFunction::SeatBelt:
			return 2;
			break;
		case MBasicFunction::D2R2:
			return 2;
			break;
		case MBasicFunction::D2R3:
			return 3;
			break;
		case MBasicFunction::D3R2:
			return 2;
			break;
		case MBasicFunction::D3R3:
			return 3;
			break;
		case MBasicFunction::D2B2:
			return 2;
			break;
		case MBasicFunction::D2B3:
			return 3;
			break;
		case MBasicFunction::D3B2:
			return 2;
			break;
		case MBasicFunction::D3B3:
			return 3;
			break;
		case MBasicFunction::Pipe:
			return 2;
			break;
		case MBasicFunction::ELAS1:
			return 2;
			break;
		case MBasicFunction::SPRING1:
			return 2;
			break;
		case MBasicFunction::SPRING3:
			return 2;
			break;
		case MBasicFunction::GAP:
			return 2;
			break;
		case MBasicFunction::CBUSH:
			return 2;
			break;
		case MBasicFunction::DB2:
			return 2;
			break;
		case MBasicFunction::HB2:
			return 2;
			break;
		case MBasicFunction::TS2:
			return 2;
			break;
		case MBasicFunction::BS2:
			return 2;
			break;
		case MBasicFunction::Discrete:
			return 2;
			break;
		case MBasicFunction::PST3:
			return 3;
			break;
		case MBasicFunction::PST6:
			return 6;
			break;
		case MBasicFunction::PST3_1:
			return 3;
			break;
		case MBasicFunction::PST3_2:
			return 3;
			break;
		case MBasicFunction::PST3_3:
			return 3;
			break;
		case MBasicFunction::TRIA3:
			return 3;
			break;
		case MBasicFunction::C0:
			return 3;
			break;
		case MBasicFunction::PET3:
			return 3;
			break;
		case MBasicFunction::PET6:
			return 6;
			break;
		case MBasicFunction::PET3_1:
			return 3;
			break;
		case MBasicFunction::PET3_2:
			return 3;
			break;
		case MBasicFunction::PET3_3:
			return 3;
			break;
		case MBasicFunction::PET3H:
			return 3;
			break;
		case MBasicFunction::PET6H:
			return 6;
			break;
		case MBasicFunction::AST3:
			return 3;
			break;
		case MBasicFunction::AST6:
			return 6;
			break;
		case MBasicFunction::AST3H:
			return 3;
			break;
		case MBasicFunction::AST6H:
			return 6;
			break;
		case MBasicFunction::CPDSG:
			return 3;
			break;
		case MBasicFunction::DSG:
			return 3;
			break;
		case MBasicFunction::SST3:
			return 3;
			break;
		case MBasicFunction::SST6:
			return 6;
			break;
		case MBasicFunction::CPDSG_3:
			return 3;
			break;
		case MBasicFunction::MBT3:
			return 3;
			break;
		case MBasicFunction::MBT6:
			return 6;
			break;
		case MBasicFunction::SPT3:
			return 3;
			break;
		case MBasicFunction::PSQ4:
			return 4;
			break;
		case MBasicFunction::PSQ8:
			return 8;
			break;
		case MBasicFunction::PSQ4R:
			return 4;
			break;
		case MBasicFunction::PSQ8R:
			return 8;
			break;
		case MBasicFunction::PSQ4I:
			return 4;
			break;
		case MBasicFunction::PSQ8I:
			return 8;
			break;
		case MBasicFunction::PEQ4:
			return 4;
			break;
		case MBasicFunction::PEQ8:
			return 8;
			break;
		case MBasicFunction::PEQ4R:
			return 4;
			break;
		case MBasicFunction::PEQ8R:
			return 8;
			break;
		case MBasicFunction::PEQ4H:
			return 4;
			break;
		case MBasicFunction::PEQ8H:
			return 8;
			break;
		case MBasicFunction::PEQ4I:
			return 4;
			break;
		case MBasicFunction::PEQ8I:
			return 8;
			break;
		case MBasicFunction::ASQ4:
			return 4;
			break;
		case MBasicFunction::ASQ8:
			return 8;
			break;
		case MBasicFunction::ASQ4R:
			return 4;
			break;
		case MBasicFunction::ASQ8R:
			return 8;
			break;
		case MBasicFunction::ASQ4H:
			return 4;
			break;
		case MBasicFunction::ASQ8H:
			return 8;
			break;
		case MBasicFunction::ASQ4I:
			return 4;
			break;
		case MBasicFunction::ASQ8I:
			return 8;
			break;
		case MBasicFunction::QPH:
			return 4;
			break;
		case MBasicFunction::Q4R:
			return 4;
			break;
		case MBasicFunction::MITC4:
			return 4;
			break;
		case MBasicFunction::SSQ4:
			return 4;
			break;
		case MBasicFunction::SSQ8:
			return 8;
			break;
		case MBasicFunction::BT:
			return 4;
			break;
		case MBasicFunction::HL:
			return 4;
			break;
		case MBasicFunction::CY_:
			return 4;
			break;
		case MBasicFunction::BTShell:
			return 4;
			break;
		case MBasicFunction::MBQ4:
			return 4;
			break;
		case MBasicFunction::MBQ4R:
			return 4;
			break;
		case MBasicFunction::MBQ8:
			return 8;
			break;
		case MBasicFunction::SPQ4:
			return 4;
			break;
		case MBasicFunction::D3T4:
			return 4;
			break;
		case MBasicFunction::D3T10:
			return 10;
			break;
		case MBasicFunction::D3T4E:
			return 4;
			break;
		case MBasicFunction::D3T4_1:
			return 4;
			break;
		case MBasicFunction::D3T4_2:
			return 4;
			break;
		case MBasicFunction::D3T4_3:
			return 4;
			break;
		case MBasicFunction::D3T10M:
			return 10;
			break;
		case MBasicFunction::D3T10E:
			return 10;
			break;
		case MBasicFunction::D3P5:
			return 5;
			break;
		case MBasicFunction::D3P6:
			return 6;
			break;
		case MBasicFunction::D3P6H:
			return 6;
			break;		
		case MBasicFunction::D3P6E:
			return 6;
			break;
		case MBasicFunction::D3P15:
			return 15;
			break;
		case MBasicFunction::D3P15H:
			return 15;
			break;
		case MBasicFunction::D3P15E:
			return 15;
			break;
		case MBasicFunction::D3S6R:
			return 6;
			break;
		case MBasicFunction::SOLID:
			return 8;
			break;
		case MBasicFunction::D3H8:
			return 8;
			break;
		case MBasicFunction::D3H20:
			return 20;
			break;
		case MBasicFunction::D3H8R:
			return 8;
			break;
		case MBasicFunction::D3H20R:
			return 20;
			break;
		case MBasicFunction::D3H8H:
			return 8;
			break;
		case MBasicFunction::D3H20H:
			return 20;
			break;
		case MBasicFunction::D3H8I:
			return 8;
			break;
		case MBasicFunction::D3H20I:
			return 20;
			break;
		case MBasicFunction::D3H8E:
			return 8;
			break;
		case MBasicFunction::D3H20E:
			return 20;
			break;
		case MBasicFunction::D3S8R:
			return 8;
			break;
		case MBasicFunction::EMD3T4:
			return 4;
			break;
		case MBasicFunction::EMD3T4_3:
			return 4;
			break;
		case MBasicFunction::EMD3P6:
			return 6;
			break;
		case MBasicFunction::EMD3H8:
			return 8;
			break;
		default:
			break;
		}
		return 0;
	}

	QString mCommonFunctions::getElementMainType(ElementType elementType)
	{
		if (elementType == D3B2 || elementType == BS2 || elementType == HB2 || elementType == DB2 || elementType == TS2)
		{
			return "梁";
		}
		else if(elementType == Pipe)
		{
			return "管";
		}
		else if (elementType == Discrete)
		{
			return "离散单元";
		}
		else if (elementType == ELAS1 || elementType == SeatBelt)
		{
			return "有参弹簧";
		}
		else if (elementType == SPRING1 || elementType == SPRING2 || elementType == GAP || elementType == CBUSH || elementType == SPRING3)
		{
			return "无参弹簧";
		}
		else if (elementType == Mass1 || elementType == ElementSPH || elementType == Slipring || elementType == Retractor)
		{
			return "质量";
		}
		else if (elementType == CPDSG || elementType == SST3 || elementType == SST6 || elementType == TRIA3 || elementType == MITC4 || elementType == SSQ4 || elementType == SSQ8 || elementType == Q4R || elementType == QPH || elementType == BT || elementType == BTShell||elementType==DSG||elementType == HL || elementType == CY_ || elementType == C0)
		{
			return "壳";
		}
		else if (elementType == D3R2)
		{
			return "杆";
		}
		else if (elementType == PST3|| elementType == PSQ4|| elementType == PST3_2|| elementType == PST3_3)
		{
			return "平面应变";
		}
		else if (elementType == PET3 || elementType == PEQ4||elementType == PET3_2|| elementType == PET3_3)
		{
			return "平面应力";
		}
		else if (elementType == SPQ4)
		{
			return "剪切板";
		}
		else if (elementType == D3T4|| elementType == EMD3T4 ||elementType == D3T10|| elementType == D3T10M || elementType == D3H8 || elementType == SOLID || elementType == D3H8R || elementType == D3H8H || elementType == D3H20||
			elementType == D3H8I || elementType == D3H8E || elementType == D3H20E || elementType == D3P6 || elementType == D3P15 || elementType == D3P5||
			elementType == D3T4_2|| elementType == D3T4_3 || elementType == EMD3T4_3 || elementType == EMD3P6 || elementType == EMD3H8 || elementType == D3T4E || elementType == D3T10E || elementType == D3S8R || elementType == D3S6R)
		{
			return "三维实体";
		}	
		else if(elementType == D3S6R || elementType == D3S8R)
		{
			return "实体壳";
		}
		else if (elementType == MBT3 || elementType == MBQ4 || elementType == MBQ4R)
		{
			return "膜";
		}
		else if (elementType == Sensor || elementType == Pretensioner)
		{
			return "特殊单元";
		}
	}

	QString mCommonFunctions::transformListToString(const std::set<int> ids, int type)
	{
		QString idstring;
		if (ids.size() == 0)
		{
			return idstring;
		}

		//将ID转成QString
		std::set<int>::iterator it;
		for (it = ids.begin(); it != ids.end(); it++)
		{
			int id = *it;
			if (it == ids.begin())
			{
				idstring = QString::number(id);
			}
			else
			{
				idstring += QString(",%1").arg(id);
			}
		}
		//添加类型关键字
		if (type == 0)
		{
			idstring = QString("Node[%1]").arg(idstring);
		}
		else
		{
			idstring = QString("Ele[%1]").arg(idstring);
		}

		return idstring;
	}

	QString mCommonFunctions::transformListToString(const std::set<int> nids, const std::set<int> eids)
	{
		QString nidstr = transformListToString(nids, 0);
		QString eidstr = transformListToString(eids, 1);
		QString idstr;
		if (nids.size() > 0 && eids.size() > 0)
		{
			idstr = QString("%1;%2").arg(nidstr).arg(eidstr);
		}
		else if(nids.size()>0&&eids.size()==0)
		{
			idstr = nidstr;
		}
		else if (nids.size() == 0 && eids.size() > 0)
		{
			idstr = eidstr;
		}
		return idstr;
	}
	bool mCommonFunctions::transformMeshStringToSet(const QString & idstring, std::set<int>& mids)
	{
		QStringList idstrlist = idstring.split(",");
		for (QString idstr : idstrlist)
		{
			if (idstr.contains("-"))
			{
				QStringList conidlist = idstr.split("-");
				if (conidlist.size() == 2)
				{
					int startid = conidlist.at(0).simplified().toInt();
					int endid = conidlist.at(1).simplified().toInt();
					for (int i = startid; i <= endid; ++i)
					{
						mids.insert(i);
					}
				}
			}
			else
			{
				mids.insert(idstr.simplified().toInt());
			}
		}
		return true;
	}
	bool mCommonFunctions::transformSetToList(const std::set<int> id_set, QList<int>& id_list)
	{
		if (id_list.size() != 0)
		{
			return false;
		}
		for (auto it = id_set.begin(); it != id_set.end(); it++)
		{
			id_list.append(*it);
		}
		return true;
	}
	bool mCommonFunctions::transformListToSet(const QList<int> id_list, std::set<int>& id_set)
	{
		if (id_set.size() != 0)
		{
			return false;
		}
		for (auto it = id_list.begin(); it != id_list.end(); it++)
		{
			id_set.insert(*it);
		}
		return true;

	}
	int mCommonFunctions::transformStringToList(const QString& idstring, QList<int>& ids)
	{
		QRegExp nrx("(Node\\[)(.*)(?:\\])");
		QRegExp erx("(Ele\\[)(.*)(?:\\])");

		//判断并提取ID字符串
		QString idStr;
		int idType = -1;
		if (idstring.indexOf(nrx) >= 0)
		{
			QStringList list = nrx.capturedTexts();
			if (list.size() == 3)
			{
				idStr = list.at(2);
				idType = 0;
			}
		}
		else if (idstring.indexOf(erx)>= 0)
		{
			QStringList list = erx.capturedTexts();
			if (list.size() == 3)
			{
				idStr = list.at(2);
				idType = 1;
			}
		}
		else
		{
			//mOutputMessage tepMsg(ErrorMessage, tr("ID列表无效!"));
			//mGlobalSignals::getInstance()->outputMessageSig(tepMsg);
			return -1;
		}

		//分割并存储
		QStringList intIds = idStr.split(",");
		const int idcount = intIds.size();
		for (int i = 0; i < idcount; ++i)
		{
			QString tepId = intIds.at(i).simplified();
			if (!tepId.isEmpty())
			{
				//判断是否有连续ID
				if (tepId.contains(":"))
				{
					QStringList list2 = tepId.split(":");
					if (list2.size() == 2)
					{
						int startId = list2.at(0).toInt();
						int endId = list2.at(1).toInt();
						for (int j = startId; j <= endId; ++j)
						{
							ids.append(j);
						}
					}
				}
				else
				{
					ids.append(tepId.toInt());
				}
			}
		}
		return idType;
	}

	int mCommonFunctions::transformStringToList(const QString& idstring, std::set<int>& ids)
	{
		QRegExp nrx("(Node\\[)(.*)(?:\\])");
		QRegExp erx("(Ele\\[)(.*)(?:\\])");

		//判断并提取ID字符串
		QString idStr;
		int idType = -1;
		if (idstring.indexOf(nrx) >= 0)
		{
			QStringList list = nrx.capturedTexts();
			if (list.size() == 3)
			{
				idStr = list.at(2);
				idType = 0;
			}
		}
		else if (idstring.indexOf(erx) >= 0)
		{
			QStringList list = erx.capturedTexts();
			if (list.size() == 3)
			{
				idStr = list.at(2);
				idType = 1;
			}
		}
		else
		{
			//mOutputMessage tepMsg(ErrorMessage, tr("ID列表无效!"));
			//mGlobalSignals::getInstance()->outputMessageSig(tepMsg);
			return -1;
		}

		//分割并存储
		QStringList intIds = idStr.split(",");
		const int idcount = intIds.size();
		for (int i = 0; i < idcount; ++i)
		{
			QString tepId = intIds.at(i).simplified();
			if (!tepId.isEmpty())
			{
				//判断是否有连续ID
				if (tepId.contains(":"))
				{
					QStringList list2 = tepId.split(":");
					if (list2.size() == 2)
					{
						int startId = list2.at(0).toInt();
						int endId = list2.at(1).toInt();
						for (int j = startId; j <= endId; ++j)
						{
							ids.insert(j);
						}
					}
				}
				else
				{
					ids.insert(tepId.toInt());
				}
			}
		}
		return idType;
	}

	bool mCommonFunctions::transformStringToList(const QString& idstring, std::set<int>& nids, std::set<int>& eids)
	{
		QStringList splitStr = idstring.split(";");
		if (splitStr.size() > 2 || splitStr.size() == 0)
		{
			//mOutputMessage tepMsg(ErrorMessage, tr("ID列表无效!"));
			//mGlobalSignals::getInstance()->outputMessageSig(tepMsg);
			return false;
		}
		else if(splitStr.size()==2)
		{
			QString idstr1 = splitStr.at(0);
			QString idstr2 = splitStr.at(1);
			if (idstr1.left(4) == QString("Node"))
			{
				transformStringToList(idstr1, nids);
				transformStringToList(idstr2, eids);
			}
			else
			{
				transformStringToList(idstr2, nids);
				transformStringToList(idstr1, eids);
			}
			return true;
		}
		else if (splitStr.size() == 1)
		{
			QString idstr = splitStr.at(0);
			if (idstr.left(4) == QString("Node"))
			{
				transformStringToList(idstr, nids);
			}
			else
			{
				transformStringToList(idstr, eids);
			}
			return true;
		}
		else
		{
			//mOutputMessage tepMsg(ErrorMessage, tr("无法解析ID字符串!"));
			//mGlobalSignals::getInstance()->outputMessageSig(tepMsg);
			return false;
		}
	}
	std::set<int> mCommonFunctions::transformQVectorToSet(const QVector<int> vec)
	{
		std::set<int> temp;
		for (int i = 0; i < vec.size(); i++)
		{
			temp.insert(vec[i]);
		}
		return temp;
	}

	QString mCommonFunctions::transformQStringListToQString(const QStringList& strlist)
	{
		QString str;
		const int nlist = strlist.size();
		for (int i = 0; i < nlist; ++i)
		{
			QString cname = strlist.at(i);
			if (i == 0)
			{
				str = cname;
			}
			else
			{
				str = QString("%1,%2").arg(str).arg(cname);
			}
		}
		return str;
	}

	double mCommonFunctions::getThreePointAngle(const QVector3D p1, const QVector3D p2, const QVector3D p3)
	{
		//获取向量1
		QVector3D vec1 = p1 - p3;
		QVector3D vec2 = p2 - p3;
		QVector3D center(0, 0, 0);
		double cos = std::abs(QVector3D::dotProduct(vec1, vec2)) / ((vec1.distanceToPoint(center))*(vec2.distanceToPoint(center)));
		double angle = std::acos(cos);
		//弧度转角度
		return angle * 180.0 / 3.1415926535;
	}

	QVector3D mCommonFunctions::getCenterPointCoord(const QVector3D p1, const QVector3D p2)
	{
		return (p1 + p2) / 2.0;
	}

	QVector3D mCommonFunctions::getCenterPointCoord(const QVector3D p1, const QVector3D p2, const QVector3D p3)
	{
		return (p1 + p2 + p3) / 3.0;
	}

	QVector3D mCommonFunctions::getCenterPointCoord(const QVector3D p1, const QVector3D p2, const QVector3D p3, const QVector3D p4)
	{
		double x = (p1.x() + p2.x() + p3.x() + p4.x()) / 4.0;
		double y = (p1.y() + p2.y() + p3.y() + p4.y()) / 4.0;
		double z = (p1.z() + p2.z() + p3.z() + p4.z()) / 4.0;
		return QVector3D(x, y, z);
	}

	void mCommonFunctions::getShrinkPosition(QVector3D& p1, QVector3D& p2, double val)
	{
		//计算两点距离
		double xminus = p1.x() - p2.x();
		double yminus = p1.y() - p2.y();
		double zminus = p1.z() - p2.z();
		p1.setX(p1.x() - xminus * 0.5*val);
		p1.setY(p1.y() - yminus * 0.5*val);
		p1.setZ(p1.z() - zminus * 0.5*val);
		p2.setX(p2.x() + xminus * 0.5*val);
		p2.setY(p2.y() + yminus * 0.5*val);
		p2.setZ(p2.z() + zminus * 0.5*val);
	}

	QString mCommonFunctions::getElementString(ElementType elementType)
	{
		return _elementStringList.at(elementType);
	}

	ElementType mCommonFunctions::getElementType(QString elementstring)
	{
		ElementType elementtype{Mass1};
		int index = _elementStringList.indexOf(elementstring.toUpper());
		if (index>=0)
		{
			elementtype = (ElementType)index;
		}
		return elementtype;
	}

	QHash<MeshType, ElementType> mCommonFunctions::getDefaultElementType(AnalysisModule analysisModule, int order)
	{
		QHash<MeshType, ElementType> meshType;
		if (order == 1)
		{
			if (analysisModule == ImplicitAnalysis)
			{
				meshType[MeshPoint] = Mass1;
				meshType[MeshBeam] = D3B2;
				meshType[MeshTri] = CPDSG;
				meshType[MeshQuad] = QPH;
				meshType[MeshTet] = D3T4;
				meshType[MeshPyramid] = D3P5;
				meshType[MeshWedge] = D3P6;
				meshType[MeshHex] = D3H8;
			}
			else if (analysisModule == ExplicitAnalysis)
			{
				meshType[MeshPoint] = ElementSPH;
				meshType[MeshBeam] = DB2;
				meshType[MeshTri] = CPDSG;
				meshType[MeshQuad] = BT;
				meshType[MeshTet] = D3T4;
				meshType[MeshPyramid] = D3P5;
				meshType[MeshWedge] = D3P6;
				meshType[MeshHex] = D3H8;
			}
			else if (analysisModule == MagnetAnalysis)
			{
				meshType[MeshPoint] = Mass1;
				meshType[MeshBeam] = D3B2;
				meshType[MeshTri] = CPDSG;
				meshType[MeshQuad] = QPH;
				meshType[MeshTet] = EMD3T4;
				meshType[MeshPyramid] = D3P5;
				meshType[MeshWedge] = EMD3P6;
				meshType[MeshHex] = EMD3H8;
			}
		}
		else
		{
			if (analysisModule == ImplicitAnalysis)
			{
				//meshType[MeshPoint] = Mass1;
				meshType[MeshBeam] = D3B3;
				meshType[MeshTri] = SST6;
				meshType[MeshQuad] = SSQ8;
				meshType[MeshTet] = D3T10;
				//meshType[MeshPyramid] = D3P5;
				meshType[MeshWedge] = D3P15;
				meshType[MeshHex] = D3H20;
			}
			else if (analysisModule == ExplicitAnalysis)
			{
				//meshType[MeshPoint] = ElementSPH;
				meshType[MeshBeam] = DB2;
				meshType[MeshTri] = SST6;
				meshType[MeshQuad] = SSQ8;
				meshType[MeshTet] = D3T10;
				//meshType[MeshPyramid] = D3P5;
				meshType[MeshWedge] = D3P15;
				meshType[MeshHex] = D3H20;
			}
			else if (analysisModule == MagnetAnalysis)
			{
				//meshType[MeshPoint] = Mass1;
				//meshType[MeshBeam] = D3B2;
				//meshType[MeshTri] = CPDSG;
				//meshType[MeshQuad] = QPH;
				//meshType[MeshTet] = EMD3T4;
				//meshType[MeshPyramid] = D3P5;
				//meshType[MeshWedge] = EMD3P6;
				//meshType[MeshHex] = EMD3H8;
			}
		}
		return meshType;
	}

	ElementType mCommonFunctions::getDefaultElementType(AnalysisModule analysisModule, MeshType meshType, int order)
	{
		if (order == 1)
		{
			if (analysisModule == ImplicitAnalysis)
			{
				switch (meshType)
				{
				case MeshPoint:
					return Mass1;
				case MeshBeam:
					return D3B2;
				case MeshTri:
					return CPDSG;
				case MeshQuad:
					return QPH;
				case MeshTet:
					return D3T4;
				case MeshPyramid:
					return D3P5;
				case MeshWedge:
					return D3P6;
				case MeshHex:
					return D3H8;
				default:
					break;
				}
			}
			else if (analysisModule == ExplicitAnalysis)
			{
				switch (meshType)
				{
				case MeshPoint:
					return ElementSPH;
				case MeshBeam:
					return DB2;
				case MeshTri:
					return CPDSG;
				case MeshQuad:
					return BT;
				case MeshTet:
					return D3T4;
				case MeshPyramid:
					return D3P5;
				case MeshWedge:
					return D3P6;
				case MeshHex:
					return D3H8;
				default:
					break;
				}
			}
			else if (analysisModule == MagnetAnalysis)
			{
				switch (meshType)
				{
				case MeshPoint:
					return Mass1;
				case MeshBeam:
					return D3B2;
				case MeshTri:
					return CPDSG;
				case MeshQuad:
					return QPH;
				case MeshTet:
					return EMD3T4;
				case MeshPyramid:
					return D3P5;
				case MeshWedge:
					return EMD3P6;
				case MeshHex:
					return EMD3H8;
				default:
					break;
				}
			}
		}
		else
		{
			if (analysisModule == ImplicitAnalysis)
			{
				switch (meshType)
				{
				//case MeshPoint:
				//	return Mass1;
				case MeshBeam:
					return D3B3;
				case MeshTri:
					return SST6;
				case MeshQuad:
					return SSQ8;
				case MeshTet:
					return D3T10;
				//case MeshPyramid:
				//	return D3P5;
				case MeshWedge:
					return D3P15;
				case MeshHex:
					return D3H20;
				default:
					break;
				}
			}
			else if (analysisModule == ExplicitAnalysis)
			{
				switch (meshType)
				{
				//case MeshPoint:
				//	return ElementSPH;
				//case MeshBeam:
				//	return DB2;
				case MeshTri:
					return SST6;
				case MeshQuad:
					return SSQ8;
				case MeshTet:
					return D3T10;
				//case MeshPyramid:
				//	return D3P5;
				case MeshWedge:
					return D3P15;
				case MeshHex:
					return D3H20;
				default:
					break;
				}
			}
			else if (analysisModule == MagnetAnalysis)
			{
				//switch (meshType)
				//{
				//case MeshPoint:
				//	return Mass1;
				//case MeshBeam:
				//	return D3B2;
				//case MeshTri:
				//	return CPDSG;
				//case MeshQuad:
				//	return QPH;
				//case MeshTet:
				//	return EMD3T4;
				//case MeshPyramid:
				//	return D3P5;
				//case MeshWedge:
				//	return EMD3P6;
				//case MeshHex:
				//	return EMD3H8;
				//default:
				//	break;
				//}
			}
		}
		return ElementType();
	}

	ElementType mCommonFunctions::getHighOrderElementType(ElementType elementType)
	{
		ElementType result = elementType;
		switch (elementType)
		{
		case MBasicFunction::D2R2:result = D2R3;
			break;		
		case MBasicFunction::D3R2:result = D3R3;
			break;
		case MBasicFunction::D2B2:result = D2B3;
		case MBasicFunction::D3B2:
		case MBasicFunction::Pipe:result = D3B3;
			break;
		case MBasicFunction::PST3:
		case MBasicFunction::PST3_1:
		case MBasicFunction::PST3_2:
		case MBasicFunction::PST3_3:
		case MBasicFunction::TRIA3:result = PST6;
			break;
		case MBasicFunction::PET3:
		case MBasicFunction::PET3_1:
		case MBasicFunction::PET3_2:
		case MBasicFunction::PET3_3:result = PET6;
		case MBasicFunction::PET3H:result = PET6H;
			break;
		case MBasicFunction::AST3:result = AST6;
			break;
		case MBasicFunction::AST3H:result = AST6H;
			break;
		case MBasicFunction::CPDSG:result = PST6;
			break;
		case MBasicFunction::DSG:result = PST6;
			break;
		case MBasicFunction::SST3:result = SST6;
			break;
		case MBasicFunction::CPDSG_3:result = SST6;
			break;
		case MBasicFunction::MBT3:result = MBT6;
			break;
		case MBasicFunction::SPT3:result = SST6;
			break;
		case MBasicFunction::PSQ4:result = PSQ8;
			break;
		case MBasicFunction::PSQ4R:result = PSQ8R;
			break;
		case MBasicFunction::PSQ4I:result = PSQ8I;
			break;
		case MBasicFunction::PEQ4:result = PEQ8;
			break;
		case MBasicFunction::PEQ4R:result = PEQ8R;
			break;
		case MBasicFunction::PEQ4H:result = PEQ8H;
			break;
		case MBasicFunction::PEQ4I:result = PEQ8I;
			break;
		case MBasicFunction::ASQ4:result = ASQ8;
			break;
		case MBasicFunction::ASQ4R:result = ASQ8R;
			break;
		case MBasicFunction::ASQ4H:result = ASQ8H;
			break;
		case MBasicFunction::ASQ4I:result = ASQ8I;
			break;
		case MBasicFunction::QPH:
		case MBasicFunction::Q4R:
		case MBasicFunction::MITC4:result = PEQ8;
			break;
		case MBasicFunction::SSQ4:result = SSQ8;
			break;
		case MBasicFunction::BT:result = PEQ8;
			break;
		case MBasicFunction::BTShell:result = PEQ8;
			break;
		case MBasicFunction::MBQ4:result = MBQ8;
			break;
		case MBasicFunction::MBQ4R:result = MBQ8;
			break;
		case MBasicFunction::SPQ4:result = PEQ8;
			break;
		case MBasicFunction::D3T4:
		case MBasicFunction::D3T4_1:
		case MBasicFunction::D3T4_2:
		case MBasicFunction::D3T4_3:
		case MBasicFunction::D3T4E: result = D3T10E;
			break;
		case MBasicFunction::D3T10M:
			break;
		case MBasicFunction::D3P5:result = D3P5;
			break;
		case MBasicFunction::D3P6:result = D3P15;
			break;
		case MBasicFunction::D3P6H:result = D3P15H;
			break;
		case MBasicFunction::D3P6E:result = D3P15E;
			break;
		case MBasicFunction::D3S6R:result = D3S6R;
			break;
		case MBasicFunction::D3H8:result = D3H8R;
			break;
		case MBasicFunction::D3H8R:result = D3H20R;
			break;
		case MBasicFunction::D3H8H:result = D3H20H;
			break;
		case MBasicFunction::D3H8I:result = D3H20I;
			break;
		case MBasicFunction::D3H8E:result = D3H20E;
			break;
		case MBasicFunction::D3S8R:result = D3S8R;
			break;
		case MBasicFunction::EMD3T4:result = EMD3T4;
			break;
		case MBasicFunction::EMD3T4_3:result = EMD3T4_3;
			break;
		case MBasicFunction::EMD3P6:result = EMD3P6;
			break;
		case MBasicFunction::EMD3H8:result = EMD3H8;
			break;
		default:
			break;
		}
		return result;
	}

	ElementType mCommonFunctions::getLowOrderElementType(ElementType elementType)
	{
		ElementType result = elementType;
		switch (elementType)
		{
		case MBasicFunction::D2R3:result = D2R2;
			break;
		case MBasicFunction::D3R3:result = D3R2;
			break;
		case MBasicFunction::D2B3:result = D2B2;
			break;
		case MBasicFunction::D3B3:result = D3B2;
			break;
		case MBasicFunction::PST6:result = PST3;
			break;
		case MBasicFunction::PET6:result = PET3;
			break;
		case MBasicFunction::PET6H:result = PET3H;
			break;
		case MBasicFunction::AST6:result = AST3;
			break;
		case MBasicFunction::AST6H:result = AST3H;
			break;
		case MBasicFunction::SST6:result = SST3;
			break;
		case MBasicFunction::MBT6:result = MBT3;
			break;
		case MBasicFunction::PSQ8:result = PSQ4;
			break;
		case MBasicFunction::PSQ8R:result = PSQ4R;
			break;
		case MBasicFunction::PSQ8I:result = PSQ4I;
			break;
		case MBasicFunction::PEQ8:result = PEQ4;
			break;
		case MBasicFunction::PEQ8R:result = PEQ4R;
			break;
		case MBasicFunction::PEQ8H:result = PEQ4H;
			break;
		case MBasicFunction::PEQ8I:result = PEQ4I;
			break;
		case MBasicFunction::ASQ8:result = ASQ4;
			break;
		case MBasicFunction::ASQ8R:result = ASQ4R;
			break;
		case MBasicFunction::ASQ8H:result = ASQ4H;
			break;
		case MBasicFunction::ASQ8I:result = ASQ4I;
			break;
		case MBasicFunction::SSQ8:result = SSQ4;
			break;
		case MBasicFunction::MBQ8:result = MBQ4;
			break;
		case MBasicFunction::D3T10E:result = D3T4E;
			break;
		case MBasicFunction::D3T10:
		case MBasicFunction::D3T10M:result = D3T4;
			break;
		case MBasicFunction::D3P15:result = D3P6;
			break;
		case MBasicFunction::D3P15H:result = D3P6H;
			break;
		case MBasicFunction::D3P15E:result = D3P6E;
			break;
		case MBasicFunction::D3H20:result = D3H8;
			break;
		case MBasicFunction::D3H20R:result = D3H8R;
			break;
		case MBasicFunction::D3H8H:
			break;
		case MBasicFunction::D3H20H:result = D3H8H;
			break;
		case MBasicFunction::D3H20I:result = D3H8I;
			break;
		case MBasicFunction::D3H20E:result = D3H8E;
			break;
		default:
			break;
		}
		return result;
	}

	QString mCommonFunctions::getElementInfo(ElementType elementType, int& order)
	{
		order = 1;
		if (elementType == Sensor || elementType == Pretensioner)
		{
			return "无网格";
		}
		if (elementType== Mass1 || elementType == ElementSPH || elementType == Slipring || elementType == Retractor || elementType == SPRING2)
		{
			return "点";
		}
		if (elementType== D2R2|| elementType == D3R2|| elementType == D2B2|| elementType == D3B2 || elementType == DB2 || elementType == HB2 || elementType == BS2|| 
			elementType == ELAS1 || elementType == SPRING1 || elementType == SPRING3 || elementType == GAP || elementType == CBUSH || elementType == Discrete || elementType == SeatBelt || elementType == Pipe || elementType == TS2)
		{
			order = 1;
			return "线";
		}
		if (elementType == D2R3 || elementType == D3R3 || elementType == D2B3 || elementType == D3B3)
		{
			order = 2;
			return "线";
		}
		if (elementType == PST3 || elementType == PST3_1 || elementType == PST3_2 || elementType == PST3_3 || elementType == TRIA3  || elementType == PET3 || elementType == PET3H || elementType == AST3 ||
			elementType == PET3_1 || elementType == PET3_2 || elementType == PET3_3 || elementType == AST3H || elementType == CPDSG || elementType == SST3 || 
			elementType == CPDSG_3 || elementType == MBT3|| elementType==SPT3|| elementType == DSG || elementType == C0)
		{
			order = 1;
			return "三角形";
		}
		if (elementType == PST6 || elementType == PET6 || elementType == PET6H || elementType == AST6 || elementType == AST6H || elementType == MBT6 || elementType == SST6)
		{
			order = 2;
			return "三角形";
		}
		if (elementType == PSQ4 || elementType == PSQ4R || elementType == PSQ4I || elementType == PEQ4 || elementType == PEQ4R || elementType == PEQ4H ||
			elementType == PEQ4I || elementType == ASQ4 || elementType == ASQ4R || elementType == ASQ4H || elementType == ASQ4I || elementType == QPH || 
			elementType == Q4R || elementType == MITC4 || elementType == SSQ4 || elementType == BT || elementType == MBQ4 || elementType == MBQ4R || elementType == SPQ4
			|| elementType == HL || elementType == CY_)
		{
			order = 1;
			return "四边形";
		}
		if (elementType == PSQ8 || elementType == PSQ8R || elementType == PSQ8I || elementType == PEQ8 || elementType == PEQ8R || elementType == PEQ8H ||
			elementType == PEQ8I || elementType == ASQ8 || elementType == ASQ8R || elementType == ASQ8H || elementType == ASQ8I || elementType == MBQ8 || elementType == SSQ8)
		{
			order = 2;
			return "四边形";
		}
		if (elementType == D3T4|| elementType == D3T4_1|| elementType == D3T4_2 || elementType == D3T4_3|| elementType == EMD3T4 || elementType == EMD3T4_3 || elementType == D3T4E)
		{
			order = 1;
			return "四面体";
		}
		if (elementType == D3T10|| elementType == D3T10M || elementType == D3T10E)
		{
			order = 2;
			return "四面体";
		}
		if (elementType == D3P5)

		{
			order = 1;
			return "四棱锥";
		}
		if (elementType == D3P6|| elementType == D3P6H || elementType == D3P6E || elementType == D3S6R|| elementType == EMD3P6)
		{
			order = 1;
			return "三棱柱";
		}
		if (elementType == D3P15 ||elementType == D3P15H || elementType == D3P15E)
		{
			order = 2;
			return "三棱柱";
		}
		if (elementType == D3H8 || elementType == SOLID || elementType == D3H8R || elementType == D3H8H || elementType == D3H8I || elementType == D3H8E || elementType == D3S8R || elementType == EMD3H8)
		{
			order = 1;
			return "六面体";
		}
		if (elementType == D3H20 || elementType == D3H20R || elementType == D3H20H || elementType == D3H20I || elementType == D3H20E)
		{
			order = 2;
			return "六面体";
		}
		return "";
	}

	void mCommonFunctions::setChildrenItemState(QTreeWidgetItem* pitem, bool ischeck)
	{
		if (pitem == nullptr)
		{
			return;
		}

		Qt::CheckState cs = Qt::Unchecked;
		if (ischeck)
		{
			cs = Qt::Checked;
		}

		const int ncitem = pitem->childCount();
		for (int i = 0; i < ncitem; ++i)
		{
			QTreeWidgetItem* citem = pitem->child(i);
			if (citem != nullptr)
			{
				citem->setCheckState(0, cs);
			}
		}
	}

	QListWidgetItem* mCommonFunctions::getListWidgetItem(QListWidget* widget, const QString& name)
	{
		if (widget == nullptr)
		{
			return nullptr;
		}
		const int nitem = widget->count();
		for (int i = 0; i < nitem; ++i)
		{
			QListWidgetItem* tepitem = widget->item(i);
			if (tepitem != nullptr)
			{
				if (name == tepitem->text())
				{
					tepitem->setSelected(true);
					return tepitem;
				}
			}
		}
		return nullptr;
	}

	QListWidgetItem* mCommonFunctions::getFirstSelectedItem(QListWidget* widget)
	{
		if (widget == nullptr)
		{
			return nullptr;
		}
		QList<QListWidgetItem*> items = widget->selectedItems();
		if (items.size() == 0)
		{
			return nullptr;
		}
		else
		{
			return items.at(0);
		}
	}

	QString mCommonFunctions::transformVector3DToString(const QVector3D pos)
	{
		QString temp = QString::number(pos.x()) + "," + QString::number(pos.y()) + "," + QString::number(pos.z());
		return temp;
	}


	QVector3D mCommonFunctions::transformStringToVector3D(const QString pos)
	{
		QStringList list = pos.split(",");
		QVector3D temp;
		if (list.size() >= 3)
		{
			temp = QVector3D(list[0].toDouble(), list[1].toDouble(), list[2].toDouble());
		}
		return temp;
	}
	QString mCommonFunctions::transformVector_Vector3DToString(const QVector<QVector3D> pos)
	{
		QString temp;
		temp += "(" + QString::number(pos[0].x()) + "," + QString::number(pos[0].y()) + "," + QString::number(pos[0].z()) + "),";
		temp += "(" + QString::number(pos[pos.size()-1].x()) + "," + QString::number(pos[pos.size() - 1].y()) + "," + QString::number(pos[pos.size() - 1].z()) + ")";
		return temp;

	}
	QVector<QVector3D> mCommonFunctions::transformStringToVector_Vector3D(const QString pos)
	{
		QVector<QVector3D> temp_v;
		QString temp_s;
		for (int i = 0; i < pos.size(); i++)
		{
			if (pos[i] != ")" && pos[i] != "(")
			{
				temp_s.append(pos[i]);
			}
		}
		QStringList list = temp_s.split(",");
		if (list.size() != 6)
		{
			return QVector<QVector3D>{ {0, 0, 0}, { 0,0,0 }};
		}
		temp_v.append(QVector3D(list[0].toDouble(), list[1].toDouble(), list[2].toDouble()));
		temp_v.append(QVector3D(list[3].toDouble(), list[4].toDouble(), list[5].toDouble()));
			
		return temp_v;
	}
	bool mCommonFunctions::LineIntersect3D(const QVector3D &line1P1, const QVector3D &line1P2, const QVector3D &line2P1, const QVector3D &line2P2, QVector3D &intersectP)
	{
		QVector3D v1 = line1P2 - line1P1;
		QVector3D v2 = line2P2 - line2P1;
		//首先确定两条直线是否平行，利用向量点乘结果是否等于0来判断，等于0垂直，等于1则平行。
		if (QVector3D::dotProduct(v1, v2) == 1)
		{
			// 两线平行
			return false;
		}
		//确定两条直线在一个平面内，否则无论如何也无法相交，这个用向量叉乘来判断，即判断向量CA和向量AB叉乘得到的向量是否垂直于向量CD。
		QVector3D startPointSeg = line2P1 - line1P1;
		QVector3D vecS1 = QVector3D::crossProduct(v1, v2);            // 有向面积1
		QVector3D vecS2 = QVector3D::crossProduct(startPointSeg, v2); // 有向面积2
		float num = QVector3D::dotProduct(startPointSeg, vecS1);
		if (num >= 1E-05f || num <= -1E-05f)
		{
			return false;
		}

		// 有向面积比值，利用点乘是因为结果可能是正数或者负数
		if (qFuzzyIsNull(vecS1.lengthSquared())) {
			return false;
		}
		float num2 = QVector3D::dotProduct(vecS2, vecS1) / vecS1.lengthSquared();
		if (num2 > 1 || num < 0) {
			return false;//num2的大小还可以判断是延长线相交还是线段相交
		}
		intersectP = line1P1 + v1 * num2;
		return true;
	}
	
	MeshType mCommonFunctions::transformTypeStrToEnum(const QString& typestr)
	{
		ElementType elementType = getElementType(typestr);
		return transformTypeStrToEnum(elementType);
	}

	MeshType mCommonFunctions::transformTypeStrToEnum(ElementType elementType)
	{
		switch (elementType)
		{
		case Sensor:
		case Pretensioner:
			return MeshNone;
		case Mass1:
		case ElementSPH:
		case Slipring:
		case Retractor:
		case SPRING2:
			return MeshPoint;
		case SeatBelt:
		case D2R2:
		case D3R2:
		case D2B2:
		case D3B2:
		case TS2:
		case DB2:
		case HB2:
		case BS2:
		case ELAS1:
		case SPRING1:
		case SPRING3:
		case GAP:
		case CBUSH:
		case Discrete:
		case D2R3:
		case D3R3:
		case D2B3:
		case D3B3:
		case Pipe:
			return MeshBeam;
		case PST3:
		case PST3_1:
		case PST3_2:
		case PST3_3:
		case TRIA3:
		case C0:
		case PET3:
		case PET3H:
		case AST3:
		case PET3_1:
		case PET3_2:
		case PET3_3:
		case AST3H:
		case CPDSG:
		case DSG:
		case SST3:
		case CPDSG_3:
		case MBT3:
		case SPT3:
		case PST6:
		case PET6:
		case PET6H:
		case AST6H:
		case MBT6:
		case SST6:
			return MeshTri;
		case PSQ4:
		case PSQ4R:
		case PSQ4I:
		case PEQ4:
		case PEQ4R:
		case PEQ4H:
		case PEQ4I:
		case ASQ4:
		case ASQ4R:
		case ASQ4H:
		case ASQ4I:
		case QPH:
		case Q4R:
		case MITC4:
		case SSQ4:
		case BT:
		case HL:
		case CY_:
		case MBQ4R:
		case MBQ4:
		case SPQ4:
		case PSQ8:
		case PSQ8R:
		case PSQ8I:
		case PEQ8:
		case PEQ8R:
		case PEQ8H:
		case PEQ8I:
		case ASQ8:
		case ASQ8R:
		case ASQ8H:
		case ASQ8I:
		case MBQ8:
		case SSQ8:
		case BTShell:
			return MeshQuad;
		case D3T4:
		case EMD3T4:
		case EMD3T4_3:
		case D3T4_1:
		case D3T4_2:
		case D3T4_3:
		case D3T4E:
		case D3T10:
		case D3T10M:
		case D3T10E:
			return MeshTet;
		case D3P5:
			return MeshPyramid;
		case D3P6:
		case EMD3P6:
		case D3P6H:
		case D3P6E:
		case D3S6R:
		case D3P15:
		case D3P15H:
		case D3P15E:
			return MeshWedge;
		case SOLID:
		case D3H8:
		case EMD3H8:
		case D3H8R:
		case D3H8H:
		case D3H8I:
		case D3H8E:
		case D3S8R:
		case D3H20:
		case D3H20R:
		case D3H20H:
		case D3H20I:
		case D3H20E:
			return MeshHex;
		default:
			return MeshPoint;
		}
	}

	QStringList mCommonFunctions::getVariableComponents(const QString& var)
	{
		QStringList compos;
		if (var == QString("Displacement")|| var == QString("Displacement(R)")||
			var==QString("Velocity")||var==QString("Velocity(R)")||
			var == QString("Acceleration") || var == QString("Acceleration(R)")||
			var == QString("Eddy Current Density(J)")|| var == QString("Magnetic Flux Density(B)")||
			var == QString("Lorentz Force(F)")||var==QString("Velocity(E)")||
			var==QString("Temperature Gradient(N)")||var==QString("Heat-flow Density(N)")||
			var==QString("Temperature Gradient(E)")||var==QString("Heat-flow Density(E)")||
			var == QString("Velocity(N)"))
		{
			compos.append(QStringList{ QString("X"),QString("Y"),QString("Z"),QString("Magnitude") });
		}
		else if (var == QString("Nodal Stresses(2D & 3D)")|| var == QString("Element Stresses(2D & 3D)"))
		{
			compos.append(QStringList{ QString("XX"),QString("YY"),QString("ZZ"),QString("YZ"),QString("XZ"),QString("XY"),
				QString("Mises") ,QString("Max.Principal"),QString("Mid.Principal"),QString("Min.Principal"),
				QString("Max.Principal(Abs)"),QString("Max.Shear"), QString("Tresca"),QString("Pressure") });
		}
		else if (var == QString("Beam Stresses(N)")|| var == QString("Beam Stresses(E)"))
		{
			compos.append(QStringList{ QString("Axial"),QString("SAC"),QString("SAD"),QString("SAE"),QString("SAF"),
				QString("SAMAX"),QString("SAMIN"),QString("SBC"),QString("SBD"),QString("SBE"),QString("SBF"),
				QString("SBMIN"),QString("SBMAX") });
		}
		else if (var == QString("Rod Stresses(N)")|| var == QString("Rod Stresses(E)"))
		{
			compos.append(QStringList{ QString("Axial") });
		}
		else if (var == QString("Plane Stresses(N)")|| var == QString("Plane Stresses(E)"))
		{
			compos.append(QStringList{ QString("XX"),QString("YY"),QString("ZZ"),QString("YZ"),QString("XZ"),QString("XY"),
				QString("Mises") ,QString("Max.Principal"),QString("Mid.Principal"),QString("Min.Principal"),
				QString("Max.Principal(Abs)"),QString("Max.Shear"), QString("Tresca"),QString("Pressure") });
		}
		else if (var == QString("Nodal Strains(2D & 3D)")|| var == QString("Element Strains(2D & 3D)"))
		{
			compos.append(QStringList{ QString("XX"),QString("YY"),QString("ZZ"),QString("YZ"),QString("XZ"),QString("XY"),
				QString("Mises") ,QString("Max.Principal"),QString("Mid.Principal"),QString("Min.Principal"),
				QString("Max.Principal(Abs)"),QString("Max.Shear"), QString("Tresca"),QString("Pressure") });
		}
		else if (var == QString("Beam Strains(N)")|| var == QString("Beam Strains(E)"))
		{
			compos.append(QStringList{ QString("Axial"),QString("SAC"),QString("SAD"),QString("SAE"),QString("SAF"),
				QString("SAMAX"),QString("SAMIN"),QString("SBC"),QString("SBD"),QString("SBE"),QString("SBF"),
				QString("SBMIN"),QString("SBMAX") });
		}
		else if (var == QString("Rod Strains(N)")|| var == QString("Rod Strains(E)"))
		{
			compos.append(QStringList{ QString("Axial") });
		}
		else if (var == QString("Plane Strains(N)")|| var == QString("Plane Strains(E)"))
		{
			compos.append(QStringList{ QString("XX"),QString("YY"),QString("ZZ"),QString("YZ"),QString("XZ"),QString("XY"),
				QString("Mises") ,QString("Max.Principal"),QString("Mid.Principal"),QString("Min.Principal"),
				QString("Max.Principal(Abs)"),QString("Max.Shear"), QString("Tresca"),QString("Pressure") });
		}
		return compos;
	}

	int mCommonFunctions::getPostRendType(const QString& var)
	{
		if (var == QString("Element Stresses(2D & 3D)") || var == QString("Beam Stresses(E)") || var == QString("Rod Stresses(E)") ||
			var == QString("Plane Stresses(E)") || var == QString("Element Strains(2D & 3D)") || var == QString("Beam Strains(E)") ||
			var == QString("Rod Strains(E)") || var == QString("Plane Strains(E)")||var==QString("Equivalent Plastic Strains(E)")||
			var== QString("Temperature(E)")||var== QString("Velocity(E)")||var== QString("Pressure(E)")||
			var == QString("Temperature Gradient(E)")|| var == QString("Heat-flow Density(E)"))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	void mCommonFunctions::setTableWidgetStyle(QTableWidget* tw, const QStringList& headers)
	{
		if (tw == nullptr)
		{
			return;
		}
		//初始化表格
		tw->setColumnCount(2);
		tw->setEditTriggers(QAbstractItemView::NoEditTriggers);
		tw->verticalHeader()->setHidden(true);
		tw->resizeColumnsToContents();

		//表头
		tw->setHorizontalHeaderLabels(headers);
		tw->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		tw->horizontalHeader()->setEditTriggers(QAbstractItemView::NoEditTriggers);
		tw->horizontalHeader()->setHighlightSections(false);
		tw->horizontalHeader()->setStyleSheet("QHeaderView:section{background:rgb(218,218,218);}");
	}

	void mCommonFunctions::getTableWidgetDatas(QTableWidget* tw, QList<QPair<double, double>>& datas)
	{
		if (tw == nullptr || tw->columnCount() != 2)
		{
			return;
		}
		const int rowcount = tw->rowCount();
		for (int i = 0; i < rowcount; ++i)
		{
			QTableWidgetItem* item1 = tw->item(i, 0);
			QTableWidgetItem* item2 = tw->item(i, 1);
			if (item1 != nullptr&&item2 != nullptr)
			{
				QPair<double, double> datapair;
				datapair.first = item1->text().toDouble();
				datapair.second = item2->text().toDouble();
				datas.append(datapair);
			}
			else if (item1 == nullptr&&item2 != nullptr)
			{
				QPair<double, double> datapair;
				datapair.first = 0;
				datapair.second = item2->text().toDouble();
				datas.append(datapair);
			}
			else if (item1 != nullptr&&item2 == nullptr)
			{
				QPair<double,double> datapair;
				datapair.first = item1->text().toDouble();
				datapair.second = 0;
				datas.append(datapair);
			}
		}
	}

	void mCommonFunctions::setTableWidgetDatas(QTableWidget* tw, QList<QPair<double, double>>& datas)
	{
		if (tw == nullptr || tw->columnCount() != 2)
		{
			return;
		}
		clearTableWidgetDatas(tw);

		const int rowcount = datas.size();
		tw->setRowCount(rowcount);
		for (int i = 0; i < rowcount; ++i)
		{
			QPair<double, double> pairval = datas.at(i);
			tw->setItem(i, 0, new QTableWidgetItem(QString::number(pairval.first)));
			tw->setItem(i, 1, new QTableWidgetItem(QString::number(pairval.second)));
		}
	}

	void mCommonFunctions::clearTableWidgetDatas(QTableWidget* tw)
	{
		const int count1 = tw->rowCount();
		for (int i = 0; i < count1; ++i)
		{
			QTableWidgetItem* item1 = tw->item(i, 0);
			if (item1 != nullptr)
			{
				delete item1;
			}
			QTableWidgetItem* item2 = tw->item(i, 1);
			if (item2 != nullptr)
			{
				delete item2;
			}
			QTableWidgetItem* item3 = tw->item(i, 2);
			if (item3 != nullptr)
			{
				delete item3;
			}
		}
		tw->setRowCount(0);
	}
	void mCommonFunctions::setWidgetPosByMouse(QWidget * mainWidget, QWidget * popWidget)
	{
		QPoint mousePos = QCursor().pos();//当前鼠标绝对坐标
		QPoint mainPos = mainWidget->mapToGlobal(mainWidget->pos());//获取父窗口的全局坐标
		int mainLeft = mainPos.x();
		int mainTop = mainPos.y();
		int mainRight = mainPos.x() + mainWidget->width();
		int mainBottom = mainPos.y() + mainWidget->height();
		//设置弹窗的初始中心位置在鼠标点击处
		int popLeft = mousePos.x() - popWidget->width() / 2;
		int popRight = mousePos.x() + popWidget->width() / 2;
		int popTop = mousePos.y() - popWidget->height() / 2;
		int popBottom = mousePos.y() + popWidget->height() / 2;
		// 最终要确定的只有左上角的坐标位置: 

		//弹窗左边超出主窗口
		if (popLeft < mainLeft)
		{
			int temp = mainLeft - popLeft;
			popLeft = popLeft + temp;
			popRight = popRight + temp;
		}
		//弹窗上部分超出主窗口上部值
		if (popTop < mainTop)
		{
			int temp = mainTop - popTop;
			popTop = popTop + temp;
			popBottom = popBottom + temp;
		}

		//最终以下方和右边为边界

		//弹窗右侧超出主窗口右侧边界值
		if (popRight > mainRight)
		{
			int temp = popRight - mainRight;
			popLeft = popLeft - temp;
			popRight = popRight - temp;
		}
		//弹窗下侧超出主窗口下侧边界值
		if (popBottom > mainBottom)
		{
			int temp = popBottom - mainBottom;
			popBottom = popBottom - temp;
			popTop = popTop - temp;
		}

			
		popWidget->move(popLeft, popTop);
	}
	QVector3D mCommonFunctions::caculatePlaneRange(float minx, float miny, float minz, float maxx, float maxy, float maxz, QVector3D normal, float ratio)
	{
		QVector3D vertex0{ minx,miny,minz };
		QVector3D vertex1{ maxx,miny,minz };
		QVector3D vertex2{ maxx,maxy,minz };
		QVector3D vertex3{ minx,maxy,minz };
		QVector3D vertex4{ minx,miny,maxz };
		QVector3D vertex5{ maxx,miny,maxz };
		QVector3D vertex6{ maxx,maxy,maxz };
		QVector3D vertex7{ minx,maxy,maxz };
		QVector<QVector3D> vertexs{ vertex0 ,vertex1 ,vertex2,vertex3,vertex4,vertex5, vertex6, vertex7 };

		QVector3D center{ (minx + maxx) / 2.0f,(miny + maxy) / 2.0f, (minz + maxz) / 2.0f };

		float minDistance = 0, maxDistance = 0;
		QVector3D minVertex, maxVertex;
		//求距离
		for (int i = 0; i < 8; i++)
		{
			float distance = vertexs[i].distanceToPlane(center, normal);
			if (distance<minDistance)
			{
				minDistance = distance;
				minVertex = vertexs[i];
			}
			else if (distance > maxDistance)
			{
				maxDistance = distance;
				maxVertex = vertexs[i];
			}
		}

		//求点
		return center + (ratio - 0.5) * (maxDistance - minDistance)*normal.normalized();
	}
	float mCommonFunctions::caculatePlaneRatio(float minx, float miny, float minz, float maxx, float maxy, float maxz, QVector3D normal, QVector3D vertex)
	{
		QVector3D vertex0{ minx,miny,minz };
		QVector3D vertex1{ maxx,miny,minz };
		QVector3D vertex2{ maxx,maxy,minz };
		QVector3D vertex3{ minx,maxy,minz };
		QVector3D vertex4{ minx,miny,maxz };
		QVector3D vertex5{ maxx,miny,maxz };
		QVector3D vertex6{ maxx,maxy,maxz };
		QVector3D vertex7{ minx,maxy,maxz };
		QVector<QVector3D> vertexs{ vertex0 ,vertex1 ,vertex2,vertex3,vertex4,vertex5, vertex6, vertex7 };

		QVector3D center{ (minx + maxx) / 2.0f,(miny + maxy) / 2.0f, (minz + maxz) / 2.0f };

		float minDistance = 0, maxDistance = 0;
		QVector3D minVertex, maxVertex;
		//求距离
		for (int i = 0; i < 8; i++)
		{
			float distance = vertexs[i].distanceToPlane(center, normal);
			if (distance < minDistance)
			{
				minDistance = distance;
				minVertex = vertexs[i];
			}
			else if (distance > maxDistance)
			{
				maxDistance = distance;
				maxVertex = vertexs[i];
			}
		}

		float distance = vertex.distanceToPlane(center, normal);

		return (distance - minDistance) / (maxDistance - minDistance);
	}
	void mCommonFunctions::caculateModelCenterAndRaduis(float minx, float miny, float minz, float maxx, float maxy, float maxz, QVector3D & center, float & radius)
	{
		center = QVector3D((maxx + minx) / 2.0, (maxy+ miny) / 2.0, (maxz + minz) / 2.0);
		radius = sqrt(pow(maxx - minx, 2) + pow(maxy - miny, 2) + pow(maxz - minz, 2));
	}
	double mCommonFunctions::getMeshVolume(QVector<QVector3D> vertexs,MeshType meshType)
	{
		double V = 0;
		QVector3D AB, AC, AD;
		switch (meshType)
		{
		case MBasicFunction::MeshPoint:
			break;
		case MBasicFunction::MeshBeam:
			break;
		case MBasicFunction::MeshTri:
			AB = vertexs[1] - vertexs[0];
			AC = vertexs[2] - vertexs[0];
			V = 1.0 / 2.0 * QVector3D::crossProduct(AB, AC).length();
			break;
		case MBasicFunction::MeshQuad:
			AB = vertexs[1] - vertexs[0];
			AC = vertexs[2] - vertexs[0];
			V = 1.0 / 2.0 * QVector3D::crossProduct(AB, AC).length();

			AD = vertexs[3] - vertexs[0];
			V += 1.0 / 2.0 * QVector3D::crossProduct(AC, AD).length();
			break;
		case MBasicFunction::MeshTet:
			AB = vertexs[1] - vertexs[0];
			AC = vertexs[2] - vertexs[0];
			AD = vertexs[3] - vertexs[0];
			//四面体体积
			V = fabs(1.0 / 6.0 * QVector3D::dotProduct(AB, QVector3D::crossProduct(AC, AD)));
			break;

		case MBasicFunction::MeshWedge:
			break;
		case MBasicFunction::MeshHex:
			break;
		case MBasicFunction::MeshPyramid:
			break;
		default:
			break;
		}
		return V;
	}
	QString mCommonFunctions::transformOutputEToC(QString str)
	{
		return _outputStringList.value(str);
	}
	QString mCommonFunctions::transformOutputCToE(QString str)
	{
		return _outputStringList1.value(str);
	}
	QString mCommonFunctions::transformTextOutputEToC(QString str)
	{
		return _textOutputStringList.value(str);
	}
	QString mCommonFunctions::transformTextOutputCToE(QString str)
	{
		return _textOutputStringList1.value(str);
	}
	QString mCommonFunctions::transformAnalysisEnumToString(AnalysisModule am)
	{
		switch (am)
		{
		case MBasicFunction::ImplicitAnalysis:
			return QString("通用结构分析");
		case MBasicFunction::ExplicitAnalysis:
			return QString("显示动力学分析");
		case MBasicFunction::MagnetAnalysis:
			return QString("电磁场分析");
		case MBasicFunction::MultiPhysicAnalysis:
		case MBasicFunction::MultiPhysicAnalysisChild:
			return QString("多物理场耦合分析");
		case MBasicFunction::FluidCompressibleAnalysis:
			return QString("可压流体分析");
		case MBasicFunction::FluidIncompressibleAnalysis:
			return QString("不可压流体分析");
		case MBasicFunction::PiezoelectricAnalysis:
			return QString("压点传感器分析");
		case MBasicFunction::UnknownAnalysis:
			return QString("未知分析");
		}
	}

	QString mCommonFunctions::getParenthesesContent(const QString & text)
	{
		QRegExp exp("(?:\\()(.*)(?:\\))");
		if (text.indexOf(exp) >= 0)
		{
			QStringList list = exp.capturedTexts();
			if (list.size() == 2)
			{
				return list.at(1);
			}
		}
		return text;
	}

	void mCommonFunctions::copyTableToBoard(QTableWidget * tw)
	{
		if (tw == nullptr)
		{
			return;
		}
		QStringList tablelist;
		const int nrow = tw->rowCount();
		const int ncol = tw->columnCount();
		for (int i = 0; i < nrow; ++i)
		{
			QStringList rowlist;
			for (int j = 0; j < ncol; ++j)
			{
				QTableWidgetItem* item = tw->item(i, j);
				if (item != nullptr)
				{
					rowlist.append(item->text());
				}
			}
			tablelist.append(rowlist.join("\t"));
		}
		QApplication::clipboard()->setText(tablelist.join("\n"));

		//输出信息
		//mOutputMessage msg(NormalMessage, QString("复制成功！"));
		//mGlobalSignals::getInstance()->outputMessageSig(msg);
	}

	void mCommonFunctions::pasteBoardToTable(QTableWidget * tw, bool isone)
	{
		if (tw == nullptr)
		{
			return;
		}
		//获取选中的单元格位置
		int rowi = 0;
		int coli = 0;
		QList<QTableWidgetItem*> items = tw->selectedItems();
		if (items.size() > 0)
		{
			QTableWidgetItem* item = items.at(0);
			if (item != nullptr)
			{
				rowi = tw->row(item);
				coli = tw->column(item);
			}
		}
		//获取表格列数（一般而言，表格的列数不能改变）
		const int tablecol = tw->columnCount();
		//获取剪切板数据
		QString cliptext = QApplication::clipboard()->text();
		//按行分割
		QStringList tablelist = cliptext.split("\n");
		//去除空行
		tablelist.removeAll("");
		//根据选中的单元格位置和剪切板中的数据行数，设置表格的行数
		const int nrow = tablelist.size();
		if (isone)
		{
			tw->setRowCount(1);
		}
		else
		{
			tw->setRowCount(rowi + nrow);
		}
		//从开始按行遍历剪切板的数据
		for (int i = 0; i < nrow; ++i)
		{
			//逐次获取行的数据
			QString rowtext = tablelist.at(i);
			QStringList rowlist = rowtext.split("\t");
			int ncol = rowlist.size();
			//写入表格中
			int datai = 0;
			for (int j = coli; j < tablecol; ++j)
			{
				if (datai >= rowlist.size())
				{
					break;
				}
				QString itemtext = rowlist.at(datai);
				tw->setItem(rowi + i, j, new QTableWidgetItem(itemtext));

				++datai;
			}
		}
	}
}
