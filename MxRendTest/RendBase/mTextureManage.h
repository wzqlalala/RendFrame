#pragma once


#include <memory>
#include <qvector.h>
#include <qmap.h>
#include "asset.h"

namespace mxr
{
	class Texture;
	class MXR_EXPORT mTextureManage
	{
	public:
		static mTextureManage* GetInstance();
		std::shared_ptr<mxr::Texture> GetTexture(QString pathname, GLuint levels = 0);
		void deleteAllTextures();
	protected:
		mTextureManage();
		~mTextureManage();
		QMap<QString, std::shared_ptr<mxr::Texture> > _textures;

	};

}