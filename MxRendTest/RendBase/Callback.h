#pragma once
#include "../core/export.h"
namespace mxr
{
	class MXR_EXPORT Callback
	{

	public:
		Callback() {}
		virtual Callback* asCallback() { return this; }
		virtual const Callback* asCallback() const { return this; }
	protected:
		virtual ~Callback() {}

	};
}
