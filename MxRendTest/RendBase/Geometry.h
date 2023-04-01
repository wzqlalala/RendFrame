#pragma once
#include "Drawable.h"
#include "Array.h"
namespace mxr
{
	class MXR_EXPORT Geometry : public Drawable
	{
	public:
		typedef std::vector<asset_ref<Array>>  ArrayList;
		Geometry();
		virtual Geometry* asGeometry() { return this; }
		virtual const Geometry* asGeometry() const { return this; }


	protected:
		Geometry& operator = (const Geometry&) { return *this; }
		virtual ~Geometry();

	};
}