#include "Drawable.h"
#include "StateSet.h"
#include "NodeVisitor.h"
#include "appInstance.h"

namespace mxr
{
	Drawable::Drawable()
	{
		_arrayListSize = 16;
		_vertexAttribList.resize(_arrayListSize);
		_command.instanceCount = 1;
		_command.baseInstance = 0;

		_elemnetcommand.instanceCount = 1;
		_elemnetcommand.baseInstance = 0;
	}

	Drawable::~Drawable()
	{
		if (ApplicationInstance::GetInstance().getApplicationNum() == 0)
		{
			return;
		}
		if (_nodevisitor)
		{
			_nodevisitor->RemoveDrawableAttribute(this);
		}
	}


	bool Drawable::empty()
	{
		if (_vertexAttribList.size() == 0)
		{
			return true;
		}
		if (getVertexCount() == 0)
		{
			return true;
		}
		return false;
	}

	void Drawable::setVertexAttribArray(unsigned int index, asset_ref<Array> array)
	{
		_vertexAttribList[index] = array;
	}

	Array * Drawable::getVertexAttribArray(unsigned int index)
	{
		return _vertexAttribList[index].get();
	}

	void Drawable::setVertexAttribArrayList(const ArrayList & arrayList)
	{
		_vertexAttribList = arrayList;
	}

	bool Drawable::CompareForm(Drawable * other)
	{
		ArrayList& _list = other->getVertexAttribArrayList();
		if (!_indexAttribute && other->getIndexAttribute())
		{
			return false;
		}
		if (_indexAttribute && !other->getIndexAttribute())
		{
			return false;
		}
		for (int i = 0; i < _arrayListSize; i++)
		{
			if (_vertexAttribList[i] && !_list[i])
			{
				return false;
			}
			if (!_vertexAttribList[i] && _list[i])
			{
				return false;
			}
			if (!_vertexAttribList[i] && !_list[i])
			{
				continue;
			}
			if (_vertexAttribList[i]->CompareForm(_list[i].get()))
			{
				continue;
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	void Drawable::setStateSet(asset_ref<StateSet> stateset)
	{
		//if (_stateset)
		//{
		//	if (_nodevisitor)
		//	{
		//		_nodevisitor->RemoveDrawableAttribute(this);
		//	}
		//}
		_stateset = stateset;
	}


	void Drawable::accept(asset_ref<NodeVisitor> nv)
	{
		_nodevisitor = nv;
		nv->accept(this);
	}

	void Drawable::Draw()
	{


	}



}