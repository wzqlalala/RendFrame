#pragma once
#include "export.h"


template<typename T> using asset_ref = std::shared_ptr<T>;
template<typename T> using asset_tmp = std::unique_ptr<T>;

template<typename T, typename... Args>
constexpr asset_ref<T> MakeAsset(Args&&... args) 
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
constexpr asset_tmp<T> WrapAsset(Args&&... args) 
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

namespace mxr
{
	class MXR_EXPORT IAsset :protected QOpenGLFunctions_4_5_Core
	{
	protected:
		GLuint id;
	public:
		GLuint ID() const { return this->id; }

	protected:
		IAsset();
		virtual ~IAsset() {};  // ensure derived classes are destructed in order

		IAsset(const IAsset&) = delete;
		IAsset& operator=(const IAsset&) = delete;
		IAsset(IAsset&& other) noexcept;
		IAsset& operator=(IAsset&& other) noexcept;

		virtual void Bind() {}
		virtual void Unbind() {}

		virtual void Bind(GLuint index) {}
		virtual void Unbind(GLuint index) {}
	};

}

