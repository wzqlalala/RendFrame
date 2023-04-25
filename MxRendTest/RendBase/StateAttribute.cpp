#include "StateAttribute.h"
#include "shader.h"
#include "app.h"

namespace mxr
{
	StateAttribute::StateAttribute()
	{
		//QOpenGLContext *_context = Application::GetInstance()._context;
		//QSurface *_surface = _context->surface();
		//_context->makeCurrent(_surface);
		initializeOpenGLFunctions();
		_enable = false;
	}

	BlendFunc::BlendFunc()
	{
		_source_factor = _destination_factor = 0;
	}

	BlendFunc::BlendFunc(GLenum source, GLenum destination)
	{
		_source_factor = source;
		_destination_factor = destination;
	}

	void BlendFunc::apply()
	{
		glBlendFunc(_source_factor, _destination_factor);
	}

	void CullFace::apply()
	{
		glCullFace(_mode);
	}

	CullFace::~CullFace()
	{

	}

	Depth::Depth(Function func, double zNear, double zFar, bool writeMask)
	{
		_func = func;
		_zNear = zNear;
		_zFar = zFar;
		_depthWriteMask = writeMask;
	}

	void Depth::apply()
	{
		glDepthFunc(_func);
		glDepthRange(_zNear, _zFar);
		glDepthMask(_depthWriteMask);
	}

	Depth::~Depth()
	{
	}

	PolygonMode::PolygonMode()
	{
		_face = FRONT_AND_BACK;
		_mode = FILL;
	}

	PolygonMode::PolygonMode(Face face, Mode mode)
	{
		_face = face;
		_mode = mode;
	}

	PolygonMode::~PolygonMode()
	{

	}

	void PolygonMode::setMode(Face face, Mode mode)
	{
		_face = face;
		_mode = mode;
	}

	PolygonMode::Mode PolygonMode::getMode(Face face) const
	{
		return _mode;
	}

	void PolygonMode::apply()
	{
		glPolygonMode(_face, _mode);
	}

	bool PolygonMode::whichIsBeforeRend(asset_ref<StateAttribute> attribute)
	{
		auto polygonMode = std::dynamic_pointer_cast<PolygonMode>(attribute);
		if (polygonMode)
		{
			return this->_mode < polygonMode->_mode;
		}
		return false;
	}


	
	PolygonOffset::PolygonOffset()
	{
		_factor = _units = 0;
	}


	PolygonOffset::PolygonOffset(float factor, float units)
	{
		_factor = factor;
		_units = units;
	}


	void PolygonOffset::apply()
	{
		glPolygonOffset(_factor, _units);
	}


	PolygonOffset::~PolygonOffset()
	{

	}

	bool PolygonOffset::whichIsBeforeRend(asset_ref<StateAttribute> attribute)
	{
		auto polygonOffset = std::dynamic_pointer_cast<PolygonOffset>(attribute);
		if (polygonOffset)
		{
			if (_factor == polygonOffset->_factor)
			{
				if (_factor < 0)
				{
					return this->_units < polygonOffset->_units;
				}
				else
				{
					return this->_units > polygonOffset->_units;
				}

			}
			return this->_factor > polygonOffset->_factor;
		}
		else
		{
			if (_factor > 0)
			{
				return true;
			}
			return false;
		}
		return false;
	}
	

	void PolygonOffsetLine::apply()
	{
		if (_enable)
		{
			glEnable(GL_POLYGON_OFFSET_LINE);
			glPolygonOffset(_factor, _units);
		}
		else
		{
			glDisable(GL_POLYGON_OFFSET_LINE);
		}

	}


	void PolygonOffsetFill::apply()
	{
		if (_enable)
		{
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(_factor, _units);
		}
		else
		{
			glDisable(GL_POLYGON_OFFSET_FILL);
		}
	}

	//bool PolygonOffsetFill::whichIsBeforeRend(asset_ref<StateAttribute> attribute)
	//{
	//	return false;
	//}

	void ClipDistance::apply()
	{
		if (_enable)
		{
			glEnable(GL_CLIP_DISTANCE0 + _distance);
		}
		else
		{
			glDisable(GL_CLIP_DISTANCE0 + _distance);
		}
	}



	Viewport::Viewport()
	{
	}

	Viewport::~Viewport()
	{
	}

	void Viewport::apply()
	{
		glViewport(_x, _y, _width, _height);
	}

	void Uniform::SetData(int intdata)
	{
		_intdata = intdata;
		_type = INT;
	}

	void Uniform::SetData(float floatdata)
	{
		_floatdata = floatdata;
		_type = FLOAT;
	}

	void Uniform::SetData(QVector2D vec2data)
	{
		_vec2data = vec2data;
		_type = VEC2;
	}

	void Uniform::SetData(QVector3D vec3data)
	{
		_vec3data = vec3data;
		_type = VEC3;
	}

	void Uniform::SetData(QVector4D vec4data)
	{
		_vec4data = vec4data;
		_type = VEC4;
	}

	void Uniform::SetData(QMatrix4x4 mat4data)
	{
		_mat4data = mat4data;
		_type = MAT4;
	}

	void Uniform::SetUniform(asset_ref<Shader> shader)
	{
		switch (_type)
		{
		case Uniform::INT:
			shader->SetUniform(_name.c_str(), _intdata);
			break;
		case Uniform::FLOAT:
			shader->SetUniform(_name.c_str(), _floatdata);
			break;
		case Uniform::VEC2:
			shader->SetUniform(_name.c_str(), _vec2data);
			break;
		case Uniform::VEC3:
			shader->SetUniform(_name.c_str(), _vec3data);
			break;
		case Uniform::VEC4:
			shader->SetUniform(_name.c_str(), _vec4data);
			break;
		case Uniform::MAT4:
			shader->SetUniform(_name.c_str(), _mat4data);
			break;
		default:
			break;
		}

	}

	void Uniform::SetEnable(bool enable)
	{
		_enable = enable;
	}

	Clear::Clear()
	{
		_clearValue = QVector4D(0, 0, 0, 1);
		_clearBit = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	}

	Clear::Clear(QVector4D clearValue, GLenum clearBit)
	{
		_clearValue = clearValue;
		_clearBit = clearBit;
	}

	void Clear::apply()
	{
		glClearColor(_clearValue[0], _clearValue[1], _clearValue[2], _clearValue[3]);
		glClear(_clearBit);
	}

	

}