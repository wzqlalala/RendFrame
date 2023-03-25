#pragma once
#include "renddata_global.h"

#include <QObject>
#include <QHash>
#include <set>

namespace MDataPost
{
	class mPostOneFrameRendData;
	class RENDDATA_EXPORT mPostAnimationRendData :public QObject
	{
	public:

		/*
		 * 构造函数
		 */
		mPostAnimationRendData();

		/*
		 * 析构函数
		 */
		~mPostAnimationRendData();

		/*
		 * 为动画获取所有帧的id
		 */
		std::set<int> getRendAnimationIds();

		/*
		 * 为动画添加某一帧的数据
		 */
		void appendRendOneFrameData(int id, mPostOneFrameRendData *postOneFrameData);

		/*
		 * 为动画获取某一帧的数据
		 */
		mPostOneFrameRendData* getRendOneFrameData(int id);

		/*
		* 删除所有帧的数据
		*/
		void deleteAnimationRendData();

		/*
		 * 设置动画的渲染范围,真的话则是统一的渲染范围，否的话则把每帧的范围还原为初始值
		 */
		void setAnimationFrameRange(bool isAgreement);

		/*
		* 设置动画的自定义渲染范围
		*/
		void setAnimationCustomFrameRange(float minData, float maxData);

	private:
		//每一帧对应的数据
		QHash<int, mPostOneFrameRendData*> _rendFrameid_RendFrameData;

		//所有帧的数据
		std::set<int> _allRendFrameids;

		//所有帧数据最小值
		float _allMinData;

		//所有帧数据最大值
		float _allMaxData;
	};
}
