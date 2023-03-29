#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")

//MViewBasic
#include "mMeshViewEnum.h"

//MBasicFunction
#include "mBasicEnum.h"
#include "mBasicStructor.h"

#include <QVector4D>
#include <QVector>

using namespace MViewBasic;
using namespace MBasicFunction;
namespace MPostRend
{
	class RENDVIEW_EXPORT mPostRendStatus
	{

	public:
		//��Ⱦģʽ
		PostMode _postMode = OneFrame;

		//�Ƿ���ʾ����ǰ��ͼ
		bool _isShowInitialShape{false};

		//�ڵ��������
		NodeOrElement _nodeOrElement = PostNode;

		//��ʾģʽ
		ShowFuntion _showFunction = ElementFace;

		//ʰȡģʽ
		MViewBasic::PickFilter _pickFilter;

		//ʰȡ�ĵ�Ԫ���͹�����
		std::set<int> _pickElementTypeFilter;

		//��ѡ���ǵ�ѡ
		MViewBasic::PickSoloOrMutiply _pickSoloOrMutiply;

		//��Ⱦ��Χ��0-ȫ��������Χ���Զ��壩��1-��Ⱦ������Χ��
		int _rangeType = 0;

		//�Ƿ���ɢ
		bool _isdispersed = true;

		//�Ƿ�Ȳ�
		bool _isEquivariance = true;

		//�Ƿ���Ⱦ��ɫ��
		bool _isShowPostColorTable = false;

		//������ɫ�����ִ�С
		float _postColorTableFontSize;

		//������ɫ��λ���и߶ȱ���
		float _postColorTableRatio;

		//�Ƿ�������
		bool _lightIsOn = true;

		//����λ�ø������λ��
		bool _lightIsDependOnCamera = true;

		//���ղ���
		PostLight _postLight;

		//��Ĵ�С
		int _pointSize = 1;

		//�߿�
		float _lineWidth = 1.0;

		//�������ߵ���ɫ
		QVector4D _faceLineColor = QVector4D(0.0f, 0.0f, 0.0f, 1.0f);

		//��ƽ��Ĳ���
		QVector<QVector4D> _cuttingPlanes;

		//�Ƿ�ʵʱ��ʾƽ��
		bool _isShowTransparentPlane = true;

		//�Ƿ�ֻ��ʾ���е��棬����ʾģ��
		bool _isOnlyShowCuttingPlane = false;

	};
}

