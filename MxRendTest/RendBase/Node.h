#pragma once
#include <vector>
#include "asset.h"

namespace mxr
{
	class Drawable;
	class Geometry;
	class Group;
	class Switch;
	class Geode;
	class Frode;
	class NodeVisitor;
	class Callback;
	class StateSet;

	class MXR_EXPORT Node
	{
	public:
		typedef std::vector<Group*> ParentList;
		typedef unsigned int NodeMask;
		friend Group;

		virtual Node* asNode() { return this; }
		virtual const Node* asNode() const { return this; }
		virtual Drawable* asDrawable() { return 0; }
		virtual const Drawable* asDrawable() const { return 0; }
		virtual Geometry* asGeometry() { return 0; }
		virtual const Geometry* asGeometry() const { return 0; }
		virtual Group* asGroup() { return 0; }
		virtual const Group* asGroup() const { return 0; }
		virtual Switch* asSwitch() { return 0; }
		virtual const Switch* asSwitch() const { return 0; }
		virtual Geode* asGeode() { return 0; }
		virtual const Geode* asGeode() const { return 0; }
		virtual Frode* asFrode() { return 0; }
		virtual const Frode* asFrode() const { return 0; }
		virtual bool isNeedAccept() { return true; };
		virtual void accept(asset_ref<NodeVisitor> nv) {};


		inline const ParentList& getParents() const { return _parents; }
		inline ParentList getParents() { return _parents; }
		inline Group* getParent(unsigned int i) { return _parents[i]; }
		inline const Group* getParent(unsigned int i) const { return _parents[i]; }
		inline unsigned int getNumParents() const { return static_cast<unsigned int>(_parents.size()); }

		inline void setNodeMask(NodeMask nm) { _nodeMask = nm; }
		inline NodeMask getNodeMask() const { return _nodeMask; }
		virtual void setStateSet(asset_ref<StateSet> stateset);
		//virtual void deleteDrawable();
		asset_ref<StateSet> getOrCreateStateSet();
		inline asset_ref<StateSet> getStateSet() { return _stateset; }
	protected:
		Node();
		virtual ~Node();
		void addParent(Group* parent);
		void removeParent(Group* parent);
		ParentList _parents;
		NodeMask _nodeMask;
		asset_ref<StateSet> _stateset;

	};

}