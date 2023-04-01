
#include "buffer.h"
#include "app.h"
#include "log.h"
#include <qdebug.h>
#include <qvector2d.h>
namespace mxr
{
	IBuffer::IBuffer() : id(0), _size(0), data_ptr(nullptr),_access(0)
	{
		//QOpenGLContext *_context = Application::GetInstance()._context;
		//QSurface *_surface = _context->surface();
		//_context->makeCurrent(_surface);
		//QOpenGLContext *context = QOpenGLContext::currentContext();
		//qDebug() << "createcontext" << context;
		initializeOpenGLFunctions();
	}

	IBuffer::IBuffer(GLsizeiptr size, const void* data, GLbitfield access) : _size(size), data_ptr(),_access(access)
	{
		//QOpenGLContext *context = QOpenGLContext::currentContext();
		//qDebug() << "createbuffer" << QString::number(long long int(context), 16);
		//Application::GetInstance()._context->makeCurrent(Application::GetInstance()._surface);
		initializeOpenGLFunctions();
		glCreateBuffers(1, &id);
		glNamedBufferStorage(id, size, data, access);  // immutable storage

	}

	IBuffer::~IBuffer()
	{
		glDeleteBuffers(1, &id);  // bound buffers will be unbound, `id = 0` will be ignored
	}

	IBuffer::IBuffer(IBuffer&& other) noexcept : id{ std::exchange(other.id, 0) } {}

	IBuffer& IBuffer::operator=(IBuffer&& other) noexcept
	{
		if (this != &other)
		{
			this->id = std::exchange(other.id, 0);
		}
		return *this;
	}

	void IBuffer::GetData(void* data)
	{
		glGetNamedBufferSubData(id, 0, this->_size, data);
	}

	void IBuffer::GetData(GLintptr offset, GLsizeiptr size, void* data)
	{
		glGetNamedBufferSubData(id, offset, size, data);
	}

	void IBuffer::SetData(const void* data)
	{
		glNamedBufferSubData(id, 0, this->_size, data);
	}

	void IBuffer::SetData(GLintptr offset, GLsizeiptr size, const void* data)
	{
		
		glNamedBufferSubData(id, offset, size, data);
		
	}


	void IBuffer::DeleteData(GLintptr offset, GLsizeiptr size)
	{
		if (offset + size > _size)
		{
			CORE_ERROR("Memory Error");
		}
		GLuint temp_id;
		glCreateBuffers(1, &temp_id);
		glNamedBufferStorage(temp_id, _size - size, nullptr, _access);
		Copy(id, temp_id, 0, 0, offset);
		Copy(id, temp_id, offset+size, offset, _size-(offset + size));
		glDeleteBuffers(1, &id);
		id = temp_id;
		_size = _size - size;
	}


	void IBuffer::Acquire(GLbitfield access)
	{
		if (!this->data_ptr)
		{
			if (data_ptr = glMapNamedBufferRange(id, 0, this->_size, access); !data_ptr)
			{
				CORE_ERROR("Cannot map buffer to the client's memory...");
			}
		}
	}

	void IBuffer::Release()
	{
		if (this->data_ptr)
		{
			this->data_ptr = nullptr;
			if (GLboolean ret = glUnmapNamedBuffer(id); ret == GL_FALSE)
			{
				CORE_ERROR("Corrupted data store contents...");
			}
		}
	}

	void IBuffer::Clear()
	{
		glClearNamedBufferSubData(id, GL_R8UI, 0, this->_size, GL_RED, GL_UNSIGNED_BYTE, NULL);
	}

	void IBuffer::Clear(GLintptr offset, GLsizeiptr size)
	{
		glClearNamedBufferSubData(id, GL_R8UI, offset, size, GL_RED, GL_UNSIGNED_BYTE, NULL);
	}

	void IBuffer::Flush()
	{
		glFlushMappedNamedBufferRange(id, 0, this->_size);
	}

	void IBuffer::Flush(GLintptr offset, GLsizeiptr size)
	{
		glFlushMappedNamedBufferRange(id, offset, size);
	}

	void IBuffer::Invalidate()
	{
		glInvalidateBufferSubData(id, 0, this->_size);
	}

	void IBuffer::Invalidate(GLintptr offset, GLsizeiptr size)
	{
		glInvalidateBufferSubData(id, offset, size);
	}

	void IBuffer::Copy(GLuint fr, GLuint to, GLintptr fr_offset, GLintptr to_offset, GLsizei size)
	{
	
		
		//glCopyNamedBufferSubData(fr, to, fr_offset, to_offset, 12);
		glBindBuffer(GL_COPY_READ_BUFFER, fr);
		glBindBuffer(GL_COPY_WRITE_BUFFER, to);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, fr_offset, to_offset, size);
		glBindBuffer(GL_COPY_READ_BUFFER, 0);
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
		
		//GL_INVALID_VALUE
	}

	void IIndexedBuffer::Reset(GLuint index)
	{
		this->index = index;
		glBindBufferBase(this->target, index, id);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////

	ATC::ATC(GLuint index, GLsizeiptr size, GLbitfield access) : IIndexedBuffer(index, size, access)
	{
		this->target = GL_ATOMIC_COUNTER_BUFFER;
		glBindBufferBase(this->target, index, id);
	}

	SSBO::SSBO(GLuint index, GLsizeiptr size, GLbitfield access) : IIndexedBuffer(index, size, access)
	{
		this->target = GL_SHADER_STORAGE_BUFFER;
		glBindBufferBase(this->target, index, id);
	}

	UBO::UBO(GLuint index, const u_vec& offset, const u_vec& length, const u_vec& stride)
		: IIndexedBuffer(), offset_vec(offset), length_vec(length), stride_vec(stride)
	{
		this->_size = std::accumulate(stride.begin(), stride.end(), 0);
		this->data_ptr = nullptr;
		this->index = index;
		this->target = GL_UNIFORM_BUFFER;

		glCreateBuffers(1, &id);
		glNamedBufferStorage(id, this->_size, NULL, GL_DYNAMIC_STORAGE_BIT);
		glBindBufferBase(this->target, this->index, id);
	}

	UBO::UBO(GLuint shader, GLuint block_id, GLbitfield access) : IIndexedBuffer()
	{
		// base alignment and byte size for each type (3-component vecs/mats are not allowed)
		
		using uvec2 = QVector2D;
		static const std::map<GLenum, uvec2> std_140_430{
			{ GL_INT,               uvec2(4U, 4U) },
			{ GL_UNSIGNED_INT,      uvec2(4U, 4U) },
			{ GL_BOOL,              uvec2(4U, 4U) },
			{ GL_FLOAT,             uvec2(4U, 4U) },
			{ GL_INT_VEC2,          uvec2(8U, 8U) },
			{ GL_INT_VEC4,          uvec2(16, 16) },
			{ GL_UNSIGNED_INT_VEC2, uvec2(8U, 8U) },
			{ GL_UNSIGNED_INT_VEC4, uvec2(16, 16) },
			{ GL_FLOAT_VEC2,        uvec2(8U, 8U) },
			{ GL_FLOAT_VEC4,        uvec2(16, 16) },
			{ GL_FLOAT_MAT2,        uvec2(16, 16) },
			{ GL_FLOAT_MAT4,        uvec2(16, 64) }
			// { GL_FLOAT_VEC3,     uvec2(16, 12) }
			// { GL_FLOAT_MAT3,     uvec2(16, 48) }
		};

		// properties to query from the shader
		static const GLenum props1[] = { GL_NUM_ACTIVE_VARIABLES, GL_NAME_LENGTH, GL_BUFFER_BINDING, GL_BUFFER_DATA_SIZE };
		static const GLenum props2[] = { GL_ACTIVE_VARIABLES };
		static const GLenum unif_props[] = { GL_NAME_LENGTH, GL_TYPE, GL_OFFSET, GL_ARRAY_SIZE, GL_ARRAY_STRIDE };

		//CORE_ASERT(glIsProgram(shader) == GL_TRUE, "Object {0} is not a valid shader program", shader);

		GLint block_info[4];
		glGetProgramResourceiv(shader, GL_UNIFORM_BLOCK, block_id, 4, props1, 4, NULL, block_info);
		this->index = static_cast<GLuint>(block_info[2]);  // uniform block binding point
		this->_size = static_cast<GLsizeiptr>(block_info[3]);  // allocated block buffer size in bytes
		this->target = GL_UNIFORM_BUFFER;

		char* name = new char[block_info[1]];
		glGetProgramResourceName(shader, GL_UNIFORM_BLOCK, block_id, block_info[1], NULL, name);
		std::string block_name(name);  // uniform block name
		delete[] name;

		GLint n_unifs = block_info[0];
		GLint* indices = new GLint[n_unifs];
		glGetProgramResourceiv(shader, GL_UNIFORM_BLOCK, block_id, 1, props2, n_unifs, NULL, indices);
		std::vector<GLint> unif_indices(indices, indices + n_unifs);
		delete[] indices;

		//CORE_INFO("Computing std140 aligned offset for the uniform block \"{0}\"", block_name);

		for (auto& u_index : unif_indices)
		{
			GLint unif_info[5];
			glGetProgramResourceiv(shader, GL_UNIFORM, u_index, 5, unif_props, 5, NULL, unif_info);

			char* name = new char[unif_info[0]];
			glGetProgramResourceName(shader, GL_UNIFORM, u_index, unif_info[0], NULL, name);
			std::string unif_name(name);  // uniform name (useful in debugging)
			delete[] name;

			GLenum u_type = unif_info[1];  // uniform data type
			GLuint u_offset = unif_info[2];  // uniform offset (relative to the uniform block's base)
			GLuint u_arr_size = unif_info[3];  // number of elements in the array (0 or 1 if not an array)
			GLuint u_arr_stride = unif_info[4];  // offset between consecutive elements (0 if not an array)

			//CORE_ASERT(std_140_430.find(u_type) != std_140_430.end(), "Unsupported uniform type \"{0}\"", unif_name);

			// GLuint base_align = u_arr_size <= 1 ? std_140_430.at(u_type).x : 16U;
			GLuint byte_size = u_arr_size <= 1 ? static_cast<GLuint>(std_140_430.at(u_type).y()) : 16U * u_arr_size;

			
			if (u_arr_size > 1)
			{
				//CORE_ASERT(u_arr_stride == 16U, "Array element is not padded to the size of a vec4!");
			}

			this->offset_vec.push_back(u_offset);
			this->length_vec.push_back(byte_size);
		}

		// an implementation of OpenGL may return the list of uniforms in an arbitrary order, there's no
		// guarantee that they will appear in the same order as declared in GLSL (but the order is vital
		// for std140), hence we must sort both vectors based on the queried offset, in increasing order

		std::vector<GLuint> sorted_idx(n_unifs);
		std::iota(sorted_idx.begin(), sorted_idx.end(), 0);
		std::stable_sort(sorted_idx.begin(), sorted_idx.end(), [this](GLuint i, GLuint j) {
			return this->offset_vec[i] < this->offset_vec[j];
		});

		// reorder both vectors using the sorted index
		for (GLuint i = 0; i < n_unifs - 1; ++i)
		{
			if (sorted_idx[i] == i)
			{
				continue;
			}
			GLuint o;
			for (o = i + 1; o < sorted_idx.size(); ++o)
			{
				if (sorted_idx[o] == i)
				{
					break;
				}
			}
			std::swap(offset_vec[i], offset_vec[sorted_idx[i]]);
			std::swap(length_vec[i], length_vec[sorted_idx[i]]);
			std::swap(sorted_idx[i], sorted_idx[o]);
		}

		for (GLint i = 1; i < n_unifs; i++)
		{
			this->stride_vec.push_back(offset_vec[i] - offset_vec[i - 1]);
		}
		this->stride_vec.push_back(length_vec.back());

		// make sure drivers have allocated enough space for the block (no less than the packed size)
		GLuint packed_size = offset_vec.back() + stride_vec.back();
		//CORE_ASERT(packed_size <= static_cast<GLuint>(this->size), "Incorrect block buffer size!");

		glCreateBuffers(1, &id);
		glNamedBufferStorage(id, this->_size, NULL, access);
		glBindBufferBase(this->target, this->index, id);
	}

	void UBO::SetUniform(GLuint uid, const void* data)
	{
		glNamedBufferSubData(id, offset_vec[uid], length_vec[uid], data);  // update a single uniform
	}

	void UBO::SetUniform(GLuint fr, GLuint to, const void* data)
	{
		auto it = stride_vec.begin();
		auto n_bytes = std::accumulate(it + fr, it + to + 1, decltype(stride_vec)::value_type(0));
		glNamedBufferSubData(id, offset_vec[fr], n_bytes, data);  // update a range of uniforms
	}
}
