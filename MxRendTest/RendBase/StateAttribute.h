#pragma once
#include <qopenglfunctions_4_5_core.h>
#include <qvector4d.h>
#include <qvector2d.h>
#include <qvector3d.h>
#include <qmatrix4x4.h>
#include "asset.h"

namespace mxr
{
	class StateSet;
	class MXR_EXPORT StateAttribute :protected QOpenGLFunctions_4_5_Core
	{
	public:

		enum Type
		{
			UNTYPE,
			BLENDFUNC = GL_BLEND,
			CULLFACE = GL_CULL_FACE,
			DEPTH = GL_DEPTH,
			VIEWPORT,
			CLIP_DISTANCE0,
			CLIP_DISTANCE1,
			CLIP_DISTANCE2,
			CLIP_DISTANCE3,
			CLIP_DISTANCE4,
			CLIP_DISTANCE5,
			CLIP_DISTANCE6,
			CLIP_DISTANCE7,
			POLYGON_MODE,
			POLYGON_OFFSET_LINE,
			POLYGON_OFFSET_FILL,
		};
		virtual void apply() {}
		virtual Type getType() const { return UNTYPE; }
		virtual void setEnable(bool enable) { _enable = enable; }
		virtual bool whichIsBeforeRend(asset_ref<StateAttribute> attribute) { return false; };
	protected:
		bool _enable;
		StateAttribute();
		virtual ~StateAttribute() {};
	};


	class MXR_EXPORT Clear :public StateAttribute
	{
	public:
		Clear();
		Clear(QVector4D clearValue, GLenum clearBit);
		virtual void apply();
	protected:
		QVector4D _clearValue;
		GLbitfield _clearBit;
	};




	class MXR_EXPORT BlendFunc :public StateAttribute
	{
	public:
		BlendFunc();
		BlendFunc(GLenum source, GLenum destination);
		virtual void apply();
		virtual Type getType() const { return BLENDFUNC; }
	protected:
		GLenum _source_factor;
		GLenum _destination_factor;

	};


	class MXR_EXPORT CullFace :public StateAttribute
	{
	public:

		enum Mode
		{
			FRONT = GL_FRONT,
			BACK = GL_BACK,
			FRONT_AND_BACK = GL_FRONT_AND_BACK
		};
		CullFace(Mode mode = BACK) :
			_mode(mode) {}
		virtual ~CullFace();
		virtual void apply();
		virtual Type getType() const { return CULLFACE; }
	protected:
		Mode _mode;
	};



	class MXR_EXPORT Depth :public StateAttribute
	{
	public:
		enum Function
		{
			NEVER = GL_NEVER,
			LESS = GL_LESS,
			EQUAL = GL_EQUAL,
			LEQUAL = GL_LEQUAL,
			GREATER = GL_GREATER,
			NOTEQUAL = GL_NOTEQUAL,
			GEQUAL = GL_GEQUAL,
			ALWAYS = GL_ALWAYS
		};
		Depth(Function func = LESS, double zNear = 0.0, double zFar = 1.0, bool writeMask = true);
		virtual ~Depth();
		inline void setFunction(Function func) { _func = func; }
		inline Function getFunction() const { return _func; }


		inline void setRange(double zNear, double zFar)
		{
			_zNear = zNear;
			_zFar = zFar;
		}

		inline void setZNear(double zNear) { _zNear = zNear; }
		inline double getZNear() const { return _zNear; }

		inline void setZFar(double zFar) { _zFar = zFar; }
		inline double getZFar() const { return _zFar; }

		inline void setWriteMask(bool mask) { _depthWriteMask = mask; }
		inline bool getWriteMask() const { return _depthWriteMask; }

		virtual Type getType() const { return DEPTH; }
		virtual void apply();


	protected:

		Function            _func;
		double              _zNear;
		double              _zFar;
		bool                _depthWriteMask;

	};


	
	class MXR_EXPORT PolygonMode :public StateAttribute
	{
	public:

		enum Mode
		{
			POINT = GL_POINT,
			LINE = GL_LINE,
			FILL = GL_FILL
		};

		enum Face
		{
			FRONT_AND_BACK = GL_FRONT_AND_BACK,
			FRONT = GL_FRONT,
			BACK = GL_BACK
		};
		PolygonMode();
		PolygonMode(Face face, Mode mode);
		virtual ~PolygonMode();
		void setMode(Face face, Mode mode);
		Mode getMode(Face face) const;
		virtual Type getType() const { return POLYGON_MODE; }
		virtual void apply();
		bool whichIsBeforeRend(asset_ref<StateAttribute> attribute) override;
	protected:
		Face _face;
		Mode _mode;
	};



	class MXR_EXPORT PolygonOffset :public StateAttribute
	{
	public:
		PolygonOffset();
		PolygonOffset(float factor, float units);
		virtual ~PolygonOffset();
		inline void  setFactor(float factor) { _factor = factor; }
		inline float getFactor() const { return _factor; }
		inline void  setUnits(float units) { _units = units; }
		inline float getUnits() const { return _units; }
		virtual void apply() ;
		bool whichIsBeforeRend(asset_ref<StateAttribute> attribute) override;
	protected:
		float       _factor;
		float       _units;
	};



	class MXR_EXPORT PolygonOffsetLine :public PolygonOffset
	{
	public:
		PolygonOffsetLine() :PolygonOffset() {};
		PolygonOffsetLine(float factor, float units) :PolygonOffset(factor, units) {};
		virtual Type getType() const { return POLYGON_OFFSET_LINE; }
		virtual void apply();


	};

	class MXR_EXPORT PolygonOffsetFill :public PolygonOffset
	{
	public:
		PolygonOffsetFill() :PolygonOffset() {};
		PolygonOffsetFill(float factor, float units) :PolygonOffset(factor, units) {};
		virtual Type getType() const { return POLYGON_OFFSET_FILL; }
		virtual void apply();
		//bool whichIsBeforeRend(asset_ref<StateAttribute> attribute) override;

	};




	class Scissor :public StateAttribute
	{


	};
	
	class MXR_EXPORT ClipDistance :public StateAttribute
	{
	public:
		ClipDistance(){}
		ClipDistance(GLuint distance) { _distance = distance; }
		virtual Type getType() const { return Type(int(CLIP_DISTANCE0) + _distance); }
		virtual void apply();
	protected:
		GLuint _distance;
	};


	class MXR_EXPORT Viewport :public StateAttribute
	{

	public:
		Viewport();
		Viewport(int x, int y, int width, int height) :
			_x(x),
			_y(y),
			_width(width),
			_height(height) {}
		virtual ~Viewport();

		inline void setViewport(int x, int y, int width, int height)
		{
			_x = x;
			_y = y;
			_width = width;
			_height = height;
		}

		virtual Type getType() const { return VIEWPORT; }
		virtual void apply();
	protected:
		int _x;
		int _y;
		int _width;
		int _height;
	};

	class Shader;
	class Uniform;
	class NodeVisitor;
	class MXR_EXPORT UniformCallback
	{
	public:
		virtual void operator()(Uniform* uniform, NodeVisitor* nv) {};

	};


	class MXR_EXPORT Uniform
	{
	public:
		enum Type
		{
			INT,
			FLOAT,
			VEC2,
			VEC3,
			VEC4,
			MAT4
		};
		Uniform() {}
		Uniform(std::string name, int intdata) { _name = name; _intdata = intdata; _type = INT; }
		Uniform(std::string name, float floatdata) { _name = name; _floatdata = floatdata; _type = FLOAT; }
		Uniform(std::string name, QVector2D  vec2data) { _name = name; _vec2data = vec2data; _type = VEC2; }
		Uniform(std::string name, QVector3D  vec3data) { _name = name; _vec3data = vec3data; _type = VEC3; }
		Uniform(std::string name, QVector4D  vec4data) { _name = name; _vec4data = vec4data; _type = VEC4; }
		Uniform(std::string name, QMatrix4x4 mat4data) { _name = name; _mat4data = mat4data; _type = MAT4; }

		void SetName(std::string name) { _name = name; }
		std::string getName() { return _name; }
		void SetData(int intdata);
		void SetData(float floatdata);
		void SetData(QVector2D  vec2data);
		void SetData(QVector3D  vec3data);
		void SetData(QVector4D  vec4data);
		void SetData(QMatrix4x4 mat4data);

		void SetUniform(asset_ref<Shader> shader);

		void SetEnable(bool enable);
		bool getEnable() { return _enable; }

		typedef UniformCallback Callback;
		void setUpdateCallback(asset_ref<UniformCallback> uc) { _updateCallback = uc; };
		UniformCallback* getUpdateCallback() { return _updateCallback.get(); }

	protected:
		Type _type;
		std::string _name;
		int _intdata;
		float _floatdata;
		QVector2D _vec2data;
		QVector3D _vec3data;
		QVector4D _vec4data;
		QMatrix4x4 _mat4data;
		asset_ref<UniformCallback> _updateCallback;
		bool _enable = true;
	};





	



}


