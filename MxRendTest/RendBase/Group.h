#pragma once

#include "Node.h"
namespace mxr
{
	typedef std::vector< asset_ref<Node> > NodeList;
	class MXR_EXPORT Group : public Node
	{
	public:
		Group();
		virtual ~Group();
		virtual Group* asGroup() { return this; }
		virtual const Group* asGroup() const { return this; }
		virtual bool isNeedAccept() { return _needAccept; };

		inline void setNeedAccept(bool isAccept)
		{
			_needAccept = isAccept;
			for (auto parent : _parents)
			{
				parent->setNeedAccept(true);
			}
		};
		virtual void accept(asset_ref<NodeVisitor> nv);
		virtual bool addChild(asset_ref<Node> child);
		virtual bool insertChild(unsigned int index, asset_ref<Node> child);
		virtual bool removeChild(asset_ref<Node> child);
		virtual void removeAllChild();
		virtual unsigned int getNumChildren() const;
		inline Node* getChild(unsigned  int i) { return _children[i].get(); }
		inline const Node* getChild(unsigned  int i) const { return _children[i].get(); }
		inline bool containsNode(const Node* node) const
		{
			for (NodeList::const_iterator itr = _children.begin(); itr != _children.end(); ++itr)
			{
				if (itr->get() == node) return true;
			}
			return false;
		}

		inline unsigned int getChildIndex(const asset_ref<Node> node) const
		{
			for (unsigned int childNum = 0; childNum < _children.size(); ++childNum)
			{
				if (_children[childNum] == node) return childNum;
			}
			return static_cast<unsigned int>(_children.size()); // node not found.
		}
	protected:
		NodeList _children;

		bool _needAccept = true;
	};
}