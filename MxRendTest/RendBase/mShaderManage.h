#pragma once


#include <memory>
#include <qvector.h>
#include <qmap.h>
#include "asset.h"

namespace mxr
{
	class Shader;
	class MXR_EXPORT mShaderManage
	{
	public:
		static mShaderManage* GetInstance();
		std::shared_ptr<mxr::Shader> GetShader(QString pathname);

		void deleteAllShaders();

	protected:
		mShaderManage();

		~mShaderManage();
		QMap<QString, std::shared_ptr<mxr::Shader> > _shaders;

	};

}