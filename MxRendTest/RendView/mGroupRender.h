#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

#include <QObject>
#include <QHash>
#include <QVector3D>

#include "Array.h"
#include "Drawable.h"
#include "Group.h"

//namespace mxr
//{
//	class Shader;
//	class Texture;
//	class Uniform;
//	class Drawable;
//	class StateSet;
//	class Group;
//	class Geode;
//	class Texture;
//}
namespace MBaseRend
{
	class mGroupBaseRender
	{
	public:
		mGroupBaseRender(std::shared_ptr<mxr::Group> parent):_parent(parent) 
		{ 
			_drawable = MakeAsset<mxr::Drawable>();		
			_parent->addChild(_drawable);
		};
		~mGroupBaseRender() { _parent->removeChild(_drawable); };
		std::shared_ptr<mxr::Drawable> getDrawable() { return _drawable; }
		void setStateSet(std::shared_ptr<mxr::StateSet> state) { _drawable->setStateSet(state); };
	protected:
		std::shared_ptr<mxr::Drawable> _drawable;
		std::shared_ptr<mxr::Group> _parent;
	};

	template<class T0>
	class mGroupRender1 : public mGroupBaseRender
	{
	public:

		mGroupRender1(std::shared_ptr<mxr::Group> parent):mGroupBaseRender(parent)
		{
			_vertex0 = MakeAsset<T0>();
			_drawable->setVertexAttribArray(0, _vertex0);
		};

		std::shared_ptr<T0>  _vertex0;
	};

	template<class T0, class T1>
	class mGroupRender2 : public mGroupBaseRender
	{
	public:

		mGroupRender2(std::shared_ptr<mxr::Group> parent) :mGroupBaseRender(parent)
		{
			_vertex0 = MakeAsset<T0>();
			_vertex1 = MakeAsset<T1>();
			_drawable->setVertexAttribArray(0, _vertex0);
			_drawable->setVertexAttribArray(1, _vertex1);
		};

		std::shared_ptr<T0>  _vertex0;
		std::shared_ptr<T1>  _vertex1;
	};

	template<class T0, class T1, class T2>
	class mGroupRender3: public mGroupBaseRender
	{
	public:

		mGroupRender3(std::shared_ptr<mxr::Group> parent) :mGroupBaseRender(parent)
		{
			_vertex0 = MakeAsset<T0>();
			_vertex1 = MakeAsset<T1>();
			_vertex2 = MakeAsset<T2>();
			_drawable->setVertexAttribArray(0, _vertex0);
			_drawable->setVertexAttribArray(1, _vertex1);
			_drawable->setVertexAttribArray(2, _vertex2);
		};

		std::shared_ptr<T0>  _vertex0;
		std::shared_ptr<T1>  _vertex1;
		std::shared_ptr<T2>  _vertex2;
	};

	template<class T0, class T1, class T2, class T3>
	class mGroupRender4 : public mGroupBaseRender
	{
	public:

		mGroupRender4(std::shared_ptr<mxr::Group> parent) :mGroupBaseRender(parent)
		{
			_vertex0 = MakeAsset<T0>();
			_vertex1 = MakeAsset<T1>();
			_vertex2 = MakeAsset<T2>();
			_vertex3 = MakeAsset<T3>();
			_drawable->setVertexAttribArray(0, _vertex0);
			_drawable->setVertexAttribArray(1, _vertex1);
			_drawable->setVertexAttribArray(2, _vertex2);
			_drawable->setVertexAttribArray(3, _vertex3);
		};

		std::shared_ptr<T0>  _vertex0;
		std::shared_ptr<T1>  _vertex1;
		std::shared_ptr<T2>  _vertex2;
		std::shared_ptr<T3>  _vertex3;
	};

	template<class T0, class T1, class T2, class T3, class T4>
	class mGroupRender5 : public mGroupBaseRender
	{
	public:

		mGroupRender5(std::shared_ptr<mxr::Group> parent) :mGroupBaseRender(parent)
		{
			_vertex0 = MakeAsset<T0>();
			_vertex1 = MakeAsset<T1>();
			_vertex2 = MakeAsset<T2>();
			_vertex3 = MakeAsset<T3>();
			_vertex4 = MakeAsset<T4>();
			_drawable->setVertexAttribArray(0, _vertex0);
			_drawable->setVertexAttribArray(1, _vertex1);
			_drawable->setVertexAttribArray(2, _vertex2);
			_drawable->setVertexAttribArray(3, _vertex3);
			_drawable->setVertexAttribArray(4, _vertex4);
		};

		std::shared_ptr<T0>  _vertex0;
		std::shared_ptr<T1>  _vertex1;
		std::shared_ptr<T2>  _vertex2;
		std::shared_ptr<T3>  _vertex3;
		std::shared_ptr<T4>  _vertex4;
	};

	template<class T0, class T1, class T2, class T3, class T4, class T5>
	class mGroupRender6 : public mGroupBaseRender
	{
	public:

		mGroupRender6(std::shared_ptr<mxr::Group> parent) :mGroupBaseRender(parent)
		{
			_vertex0 = MakeAsset<T0>();
			_vertex1 = MakeAsset<T1>();
			_vertex2 = MakeAsset<T2>();
			_vertex3 = MakeAsset<T3>();
			_vertex4 = MakeAsset<T4>();
			_vertex5 = MakeAsset<T5>();
			_drawable->setVertexAttribArray(0, _vertex0);
			_drawable->setVertexAttribArray(1, _vertex1);
			_drawable->setVertexAttribArray(2, _vertex2);
			_drawable->setVertexAttribArray(3, _vertex3);
			_drawable->setVertexAttribArray(4, _vertex4);
			_drawable->setVertexAttribArray(5, _vertex5);
		};

		std::shared_ptr<T0>  _vertex0;
		std::shared_ptr<T1>  _vertex1;
		std::shared_ptr<T2>  _vertex2;
		std::shared_ptr<T3>  _vertex3;
		std::shared_ptr<T4>  _vertex4;
		std::shared_ptr<T5>  _vertex5;
	};

	template<class T0, class T1, class T2, class T3, class T4, class T5, class T6>
	class mGroupRender7 : public mGroupBaseRender
	{
	public:

		mGroupRender7(std::shared_ptr<mxr::Group> parent) :mGroupBaseRender(parent)
		{
			_vertex0 = MakeAsset<T0>();
			_vertex1 = MakeAsset<T1>();
			_vertex2 = MakeAsset<T2>();
			_vertex3 = MakeAsset<T3>();
			_vertex4 = MakeAsset<T4>();
			_vertex5 = MakeAsset<T5>();
			_vertex6 = MakeAsset<T6>();
			_drawable->setVertexAttribArray(0, _vertex0);
			_drawable->setVertexAttribArray(1, _vertex1);
			_drawable->setVertexAttribArray(2, _vertex2);
			_drawable->setVertexAttribArray(3, _vertex3);
			_drawable->setVertexAttribArray(4, _vertex4);
			_drawable->setVertexAttribArray(5, _vertex5);
			_drawable->setVertexAttribArray(6, _vertex6);
		};									

		std::shared_ptr<T0>  _vertex0;
		std::shared_ptr<T1>  _vertex1;
		std::shared_ptr<T2>  _vertex2;
		std::shared_ptr<T3>  _vertex3;
		std::shared_ptr<T4>  _vertex4;
		std::shared_ptr<T5>  _vertex5;
		std::shared_ptr<T6>  _vertex6;
	};

	template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	class mGroupRender8 : public mGroupBaseRender
	{
	public:

		mGroupRender8(std::shared_ptr<mxr::Group> parent) :mGroupBaseRender(parent)
		{
			_vertex0 = MakeAsset<T0>();
			_vertex1 = MakeAsset<T1>();
			_vertex2 = MakeAsset<T2>();
			_vertex3 = MakeAsset<T3>();
			_vertex4 = MakeAsset<T4>();
			_vertex5 = MakeAsset<T5>();
			_vertex6 = MakeAsset<T6>();
			_vertex7 = MakeAsset<T7>();
			_drawable->setVertexAttribArray(0, _vertex0);
			_drawable->setVertexAttribArray(1, _vertex1);
			_drawable->setVertexAttribArray(2, _vertex2);
			_drawable->setVertexAttribArray(3, _vertex3);
			_drawable->setVertexAttribArray(4, _vertex4);
			_drawable->setVertexAttribArray(5, _vertex5);
			_drawable->setVertexAttribArray(6, _vertex6);
			_drawable->setVertexAttribArray(7, _vertex7);
		};

		std::shared_ptr<T0>  _vertex0;
		std::shared_ptr<T1>  _vertex1;
		std::shared_ptr<T2>  _vertex2;
		std::shared_ptr<T3>  _vertex3;
		std::shared_ptr<T4>  _vertex4;
		std::shared_ptr<T5>  _vertex5;
		std::shared_ptr<T6>  _vertex6;
		std::shared_ptr<T7>  _vertex7;
	};
}