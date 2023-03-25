#pragma once
#include "mxbasicrender_global.h"

//Qt
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>

using namespace std;
namespace MViewBasic
{

	template<class T>
	class Data
	{
	public:
		//int _position = 0;//属性位置
		QVector<T> _vertex;//顶点数据1
		int _size = 1;//QVector3D为3,其余大部分都为1
		GLenum _dataType;//数据类型
	};

	class MXBASICRENDER_EXPORT mBasicMeshRend : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
	{

	public:
		explicit mBasicMeshRend() 
		{ 
			VBO = nullptr; 
		};

		~mBasicMeshRend() Q_DECL_OVERRIDE 
		{
			deleteAllPartBuffer();
			if (isInitialized() && glIsVertexArray(vao[0]))
			{
				glDeleteVertexArrays(1, vao);
			}
			if (VBO != nullptr)
			{
				delete VBO;
				VBO = nullptr;
			}

		};

		//初始化
		void initial()
		{
			VBO = new GLuint[VBONum];
			this->initializeOpenGLFunctions();
			glGenVertexArrays(1, vao);
			glGenBuffers(VBONum, VBO);
		};

		//设置渲染方式
		void setDrawMode(GLenum drawMode) { _drawMode = drawMode; };

		//渲染
		void draw()
		{
			if (!isInitialized())
			{
				this->initial();
			}
			glBindVertexArray(vao[0]);
			glDrawArrays(_drawMode, 0, num);
		};

		//删除所有部件更新buffer
		void deleteAllPartBuffer()
		{
			//if (!isInitialized())
			//{
			//	this->initial();
			//}
			if (VBO != nullptr && glIsBuffer(VBO[0]))
			{
				glDeleteBuffers(VBONum, VBO);
			}
			_partIndex.clear();
			Size_begin = num = 0;
			//glDeleteBuffers(3, VBO);
		};

		//删除一个部件更新buffer
		void deleteOnePartBuffer(QString partName, QList<QString> _partOrder)
		{
			int begin, n, index;
			if (_partIndex.contains(partName))
			{
				_delete = _partIndex.value(partName);

				begin = _delete.first;
				n = _delete.second;

				_partIndex.remove(partName);
				num -= _delete.second;

				index = _partOrder.indexOf(partName);
				for (int i = index + 1; i < _partOrder.size(); ++i)
				{
					_partIndex[_partOrder.at(i)].first = _partIndex[_partOrder.at(i)].first - n;
				}
				UpdateDeleteBuffer(vao[0], VBO, _delete, num, Size_begin);
			}

		};

		//更新单个部件缓存
		template<class C>
		void updateOnePartBuffer(int index, QString partName, C value)
		{
			int IDbegin, IDsize;
			IDbegin = _partIndex.value(partName).first;
			IDsize = _partIndex.value(partName).second;
			QVector<C> a(IDsize, value);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
			int size = sizeof(C);
			switch (index)
			{
			//case 0:
				//glBufferSubData(GL_ARRAY_BUFFER, IDbegin * sizeof(float) * 3, IDsize * sizeof(float) * 3, a.data()); break;
			default:
				glBufferSubData(GL_ARRAY_BUFFER, IDbegin * size, IDsize * size, a.data()); break;
			}
		};

		//更新所有部件缓存
		template<class C>
		void updateAllPartBuffer(int index, C value)
		{
			QVector<C> a(num, value);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
			switch (index)
			{
			//case 0:
				//glBufferSubData(GL_ARRAY_BUFFER, 0, num * sizeof(float) * 3, a.data()); break;
			default:
				glBufferSubData(GL_ARRAY_BUFFER, 0, num * sizeof(C), a.data()); break;
				break;
			}
		};

		//更新所有部件缓存
		template<class C>
		void updateAllPartBuffer(int index, QVector<C> values)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
			switch (index)
			{
			//case 0:
				//glBufferSubData(GL_ARRAY_BUFFER, 0, num * sizeof(float) * 3, values.data()); break;
			default:
				glBufferSubData(GL_ARRAY_BUFFER, 0, num * sizeof(C), values.data()); break;
				break;
			}
		};

		//重命名部件
		void renamePartName(QString newname, QString oldname) 
		{
			QPair<int, int> temp = _partIndex.take(oldname);
			_partIndex.insert(newname, temp);
		};
	protected:
		//更新删除缓存函数
		virtual void UpdateDeleteBuffer(GLuint &VAO, GLuint *VBO, QPair<int, int> deleteBeginSize, int &size, int &begin) = 0;

		//复制buffer
		void CopyBuffer(GLuint read, GLuint write, int read_offset, int write_offset, int size)
		{
			glBindBuffer(GL_COPY_READ_BUFFER, read);
			glBindBuffer(GL_COPY_WRITE_BUFFER, write);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, read_offset, write_offset, size);
		};

		template<class C> 
		void BindSubBuffer(int position, GLint vbo, int sizeofT, Data<C> data)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferSubData(GL_ARRAY_BUFFER, size_begin * sizeofT, sizeofT * data._vertex.size(), data._vertex.data());
			glVertexAttribPointer(position, data._size1, data._dataType1, GL_FALSE, sizeofT, (void*)0);
			glEnableVertexAttribArray(position);
		};

		template<class C>
		void BindBuffer(int position, GLint vbo, Data<C> data)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glVertexAttribPointer(position, data._size1, data._dataType1, GL_FALSE, sizeof(C), (void*)0);
			glEnableVertexAttribArray(position);
		};

		//创建buffer
		GLuint CreateArrayBufferData(int size, GLenum usage = GL_STATIC_DRAW) 
		{
			GLuint tempVBO;
			glGenBuffers(1, &tempVBO);
			if (size > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, tempVBO);//point		
				glBufferData(GL_ARRAY_BUFFER, size, NULL, usage);
			}
			return tempVBO;
		};

		template<class T>
		void getGLenumType(GLenum &dataType, int &size)
		{
			if (is_same<T, QVector2D>::value) { dataType = GL_FLOAT; size = 2; }
			else if (is_same<T, QVector3D>::value) { dataType = GL_FLOAT; size = 3; }
			else if (is_same<T, float>::value) { dataType = GL_FLOAT; size = 1; }
			else if (is_same<T, GLubyte>::value) { dataType = GL_UNSIGNED_BYTE; size = 1; }
			else if (is_same<T, GLushort>::value) { dataType = GL_UNSIGNED_SHORT; size = 1; }
			else if (is_same<T, GLshort>::value) { dataType = GL_SHORT; size = 1; }
			else if (is_same<T, int>::value) { dataType = GL_INT; size = 1; }
		};

	protected:
		uint vao[1];//vao
		GLenum _polygonMode;//填充模式
		GLenum _drawMode = GL_LINES;//绘制方式

	public:

		int num = 0;//渲染图形个数
		GLuint *VBO; //存放VBO的数组
		int VBONum = 0;//VBO的个数
		QPair<int, int> _delete;//删除的个数
		int Size_begin = 0;//删除的起始位置
		QHash<QString, QPair<int, int>> _partIndex;//每个部件的内部渲染起始索引和个数

	};

	template<class T0>
	class mBasicMeshRend0 : public mBasicMeshRend
	{

	public:
		mBasicMeshRend0()
		{
			getGLenumType<T0>(_data0._dataType, _data0._size);

			VBONum = 1;
		};

		~mBasicMeshRend0() { };

		//添加部件更新buffer
		void AppendPartBuffer()
		{
			if (!isInitialized())
			{
				this->initial();
			}

			int sizeofT0 = sizeof(T0);
			//更新数据
			GLuint temp0 = CreateArrayBufferData(sizeofT0*num);

			if (Size_begin != 0)
			{
				CopyBuffer(VBO[0], temp0, 0, 0, Size_begin * sizeofT0);
				glDeleteBuffers(VBONum, VBO);
			}

			glBindVertexArray(vao[0]);

			BindSubBuffer(0, temp0, sizeofT0, _data0);

			glBindVertexArray(0);
			Size_begin = num;
			VBO[0] = temp0;
			QVector<T0>().swap(_data0._vertex);
		};

		void UpdateBuffer(GLuint *VBO1, int index1, int n)
		{
			if (!isInitialized())
			{
				this->initial();
			}

			num = n;

			VBO = VBO1;

			glBindVertexArray(vao[0]);

			BindBuffer<T0>(0, VBO[index1], _data0);

			glBindVertexArray(0);
		};

	private:
		//更新删除的buffer
		virtual void UpdateDeleteBuffer(GLuint &VAO, GLuint *VBO, QPair<int, int> deleteBeginSize, int &size, int &begin) override
		{
			int sizeofT0 = sizeof(T0);

			GLuint temp0 = CreateArrayBufferData(sizeofT0*size);
			GLuint temp1 = CreateArrayBufferData(sizeofT1*size);

			int begin2 = deleteBeginSize.first;
			int end2 = deleteBeginSize.first + deleteBeginSize.second;

			if (size > 0)
			{
				CopyBuffer(VBO[0], temp0, 0, 0, begin2 * sizeofT0);
				CopyBuffer(VBO[0], temp0, end2 * sizeofT0, begin2 * sizeofT0, (begin - end2) * sizeofT0);
			}
			glDeleteBuffers(VBONum, VBO);
			VBO[0] = temp0;

			glBindVertexArray(VAO);
			BindBuffer<T0>(0, VBO[0], _data0);

			glBindVertexArray(0);

			begin = size;
		};

	public:
		Data<T0> _data0;
	};

	template<class T0, class T1>
	class mBasicMeshRend1 : public mBasicMeshRend
	{

	public:
		mBasicMeshRend1()
		{
			getGLenumType<T0>(_data0._dataType, _data0._size);
			getGLenumType<T1>(_data1._dataType, _data1._size);

			VBONum = 2;
		};

		~mBasicMeshRend1() { };

		//添加部件更新buffer
		void AppendPartBuffer()
		{
			if (!isInitialized())
			{
				this->initial();
			}

			int sizeofT0 = sizeof(T0);
			int sizeofT1 = sizeof(T1);
			//更新数据
			GLuint temp0 = CreateArrayBufferData(sizeofT0*num);
			GLuint temp1 = CreateArrayBufferData(sizeofT1*num);

			if (Size_begin != 0)
			{
				CopyBuffer(VBO[0], temp0, 0, 0, Size_begin * sizeofT0);
				CopyBuffer(VBO[1], temp1, 0, 0, Size_begin * sizeofT1);
				glDeleteBuffers(VBONum, VBO);
			}

			glBindVertexArray(vao[0]);

			BindSubBuffer(0, temp0, sizeofT0, _data0);
			BindSubBuffer(1, temp1, sizeofT1, _data1);

			glBindVertexArray(0);
			Size_begin = num;
			VBO[0] = temp0; VBO[1] = temp1;
			QVector<T0>().swap(_data0._vertex);
			QVector<T1>().swap(_data1._vertex);
		};

		void UpdateBuffer(GLuint *VBO1, int index1, int index2, int n)
		{
			if (!isInitialized())
			{
				this->initial();
			}

			num = n;

			VBO = VBO1;

			glBindVertexArray(vao[0]);

			BindBuffer<T0>(0, VBO[index1], _data0);
			BindBuffer<T1>(1, VBO[index2], _data1);

			glBindVertexArray(0);
		};

	private:
		//更新删除的buffer
		virtual void UpdateDeleteBuffer(GLuint &VAO, GLuint *VBO, QPair<int, int> deleteBeginSize, int &size, int &begin) override
		{
			int sizeofT0 = sizeof(T0);
			int sizeofT1 = sizeof(T1);

			GLuint temp0 = CreateArrayBufferData(sizeofT0*size);
			GLuint temp1 = CreateArrayBufferData(sizeofT1*size);

			int begin2 = deleteBeginSize.first;
			int end2 = deleteBeginSize.first + deleteBeginSize.second;

			if (size > 0)
			{
				CopyBuffer(VBO[0], temp0, 0, 0, begin2 * sizeofT0);
				CopyBuffer(VBO[0], temp0, end2 * sizeofT0, begin2 * sizeofT0, (begin - end2) * sizeofT0);

				CopyBuffer(VBO[1], temp1, 0, 0, begin2 * sizeofT1);
				CopyBuffer(VBO[1], temp1, end2 * sizeofT1, begin2 * sizeofT1, (begin - end2) * sizeofT1);
			}
			glDeleteBuffers(VBONum, VBO);
			VBO[0] = temp0; VBO[1] = temp1;

			glBindVertexArray(VAO);
			BindBuffer<T0>(0, VBO[0], _data0);
			BindBuffer<T0>(1, VBO[1], _data1);

			glBindVertexArray(0);

			begin = size;
		};

	public:
		Data<T0> _data0;
		Data<T1> _data1;
	};

	template<class T0, class T1, class T2>
	class mBasicMeshRend2 : public mBasicMeshRend
	{

	public:
		mBasicMeshRend2()
		{
			getGLenumType<T0>(_data0._dataType, _data0._size);
			getGLenumType<T1>(_data1._dataType, _data1._size);
			getGLenumType<T2>(_data2._dataType, _data2._size);

			VBONum = 3;
		};

		~mBasicMeshRend2() { };

		//添加部件更新buffer
		void AppendPartBuffer()
		{
			if (!isInitialized())
			{
				this->initial();
			}

			int sizeofT0 = sizeof(T0);
			int sizeofT1 = sizeof(T1);
			int sizeofT2 = sizeof(T2);
			//更新数据
			GLuint temp0 = CreateArrayBufferData(sizeofT0*num);
			GLuint temp1 = CreateArrayBufferData(sizeofT1*num);
			GLuint temp2 = CreateArrayBufferData(sizeofT2*num);

			if (Size_begin != 0)
			{
				CopyBuffer(VBO[0], temp0, 0, 0, Size_begin * sizeofT0);
				CopyBuffer(VBO[1], temp1, 0, 0, Size_begin * sizeofT1);
				CopyBuffer(VBO[2], temp2, 0, 0, Size_begin * sizeofT2);
				glDeleteBuffers(VBONum, VBO);
			}

			glBindVertexArray(vao[0]);

			BindSubBuffer(0, temp0, sizeofT0, _data0);
			BindSubBuffer(1, temp1, sizeofT1, _data1);
			BindSubBuffer(2, temp2, sizeofT2, _data2);

			glBindVertexArray(0);
			Size_begin = num;
			VBO[0] = temp0; VBO[1] = temp1; VBO[2] = temp2;
			QVector<T0>().swap(_data0._vertex);
			QVector<T1>().swap(_data1._vertex);
			QVector<T2>().swap(_data2._vertex);
		};

		void UpdateBuffer(GLuint *VBO1, int index1, int index2, int index3, int n)
		{
			if (!isInitialized())
			{
				this->initial();
			}

			num = n;

			VBO = VBO1;

			glBindVertexArray(vao[0]);

			BindBuffer<T0>(0, VBO[index1], _data0);
			BindBuffer<T1>(1, VBO[index2], _data1);
			BindBuffer<T2>(2, VBO[index3], _data2);

			glBindVertexArray(0);
		};

	private:
		//更新删除的buffer
		virtual void UpdateDeleteBuffer(GLuint &VAO, GLuint *VBO, QPair<int, int> deleteBeginSize, int &size, int &begin) override
		{
			int sizeofT0 = sizeof(T0);
			int sizeofT1 = sizeof(T1);
			int sizeofT2 = sizeof(T2);

			GLuint temp0 = CreateArrayBufferData(sizeofT0*size);
			GLuint temp1 = CreateArrayBufferData(sizeofT1*size);
			GLuint temp2 = CreateArrayBufferData(sizeofT2*size);

			int begin2 = deleteBeginSize.first;
			int end2 = deleteBeginSize.first + deleteBeginSize.second;

			if (size > 0)
			{
				CopyBuffer(VBO[0], temp0, 0, 0, begin2 * sizeofT0);
				CopyBuffer(VBO[0], temp0, end2 * sizeofT0, begin2 * sizeofT0, (begin - end2) * sizeofT0);

				CopyBuffer(VBO[1], temp1, 0, 0, begin2 * sizeofT1);
				CopyBuffer(VBO[1], temp1, end2 * sizeofT1, begin2 * sizeofT1, (begin - end2) * sizeofT1);

				CopyBuffer(VBO[2], temp2, 0, 0, begin2 * sizeofT2);
				CopyBuffer(VBO[2], temp2, end2 * sizeofT2, begin2 * sizeofT2, (begin - end2) * sizeofT2);
			}
			glDeleteBuffers(VBONum, VBO);
			VBO[0] = temp0; VBO[1] = temp1; VBO[2] = temp2;

			glBindVertexArray(VAO);
			BindBuffer<T0>(0, VBO[0], _data0);
			BindBuffer<T0>(1, VBO[1], _data1);
			BindBuffer<T0>(2, VBO[2], _data2);

			glBindVertexArray(0);

			begin = size;
		};

	public:
		Data<T0> _data0;
		Data<T1> _data1;
		Data<T2> _data2;
	};

	template<class T0, class T1, class T2, class T3>
	class mBasicMeshRend3 : public mBasicMeshRend
	{

	public:
		mBasicMeshRend3()
		{
			getGLenumType<T0>(_data0._dataType, _data0._size);
			getGLenumType<T1>(_data1._dataType, _data1._size);
			getGLenumType<T2>(_data2._dataType, _data2._size);
			getGLenumType<T3>(_data3._dataType, _data3._size);

			VBONum = 4;
		};

		~mBasicMeshRend3() { };

		//添加部件更新buffer
		void AppendPartBuffer()
		{
			if (!isInitialized())
			{
				this->initial();
			}

			int sizeofT0 = sizeof(T0);
			int sizeofT1 = sizeof(T1);
			int sizeofT2 = sizeof(T2);
			int sizeofT3 = sizeof(T3);
			//更新数据
			GLuint temp0 = CreateArrayBufferData(sizeofT0*num);
			GLuint temp1 = CreateArrayBufferData(sizeofT1*num);
			GLuint temp2 = CreateArrayBufferData(sizeofT2*num);
			GLuint temp3 = CreateArrayBufferData(sizeofT3*num);
			if (Size_begin != 0)
			{
				CopyBuffer(VBO[0], temp0, 0, 0, Size_begin * sizeofT0);
				CopyBuffer(VBO[1], temp1, 0, 0, Size_begin * sizeofT1);
				CopyBuffer(VBO[2], temp2, 0, 0, Size_begin * sizeofT2);
				CopyBuffer(VBO[3], temp3, 0, 0, Size_begin * sizeofT3);
				glDeleteBuffers(VBONum, VBO);
			}

			glBindVertexArray(vao[0]);

			BindSubBuffer(0, temp0, sizeofT0, _data0);
			BindSubBuffer(1, temp1, sizeofT1, _data1);
			BindSubBuffer(2, temp2, sizeofT2, _data2);
			BindSubBuffer(3, temp3, sizeofT3, _data3);

			glBindVertexArray(0);
			Size_begin = num;
			VBO[0] = temp0; VBO[1] = temp1; VBO[2] = temp2; VBO[3] = temp3;
			QVector<T0>().swap(_data0._vertex);
			QVector<T1>().swap(_data1._vertex);
			QVector<T2>().swap(_data2._vertex);
			QVector<T3>().swap(_data3._vertex);
		};

		void UpdateBuffer(GLuint *VBO1, int index1, int index2, int index3, int index4, int n)
		{
			if (!isInitialized())
			{
				this->initial();
			}

			num = n;

			VBO = VBO1;

			glBindVertexArray(vao[0]);

			BindBuffer<T0>(0, VBO[index1], _data0);
			BindBuffer<T1>(1, VBO[index2], _data1);
			BindBuffer<T2>(2, VBO[index3], _data2);
			BindBuffer<T3>(3, VBO[index4], _data3);

			glBindVertexArray(0);
		};

	private:
		//更新删除的buffer
		virtual void UpdateDeleteBuffer(GLuint &VAO, GLuint *VBO, QPair<int, int> deleteBeginSize, int &size, int &begin) override
		{
			int sizeofT0 = sizeof(T0);
			int sizeofT1 = sizeof(T1);
			int sizeofT2 = sizeof(T2);
			int sizeofT3 = sizeof(T3);

			GLuint temp0 = CreateArrayBufferData(sizeofT0*size);
			GLuint temp1 = CreateArrayBufferData(sizeofT1*size);
			GLuint temp2 = CreateArrayBufferData(sizeofT2*size);
			GLuint temp3 = CreateArrayBufferData(sizeofT3*size);

			int begin2 = deleteBeginSize.first;
			int end2 = deleteBeginSize.first + deleteBeginSize.second;

			if (size > 0)
			{
				CopyBuffer(VBO[0], temp0, 0, 0, begin2 * sizeofT0);
				CopyBuffer(VBO[0], temp0, end2 * sizeofT0, begin2 * sizeofT0, (begin - end2) * sizeofT0);

				CopyBuffer(VBO[1], temp1, 0, 0, begin2 * sizeofT1);
				CopyBuffer(VBO[1], temp1, end2 * sizeofT1, begin2 * sizeofT1, (begin - end2) * sizeofT1);

				CopyBuffer(VBO[2], temp2, 0, 0, begin2 * sizeofT2);
				CopyBuffer(VBO[2], temp2, end2 * sizeofT2, begin2 * sizeofT2, (begin - end2) * sizeofT2);

				CopyBuffer(VBO[3], temp3, 0, 0, begin2 * sizeofT3);
				CopyBuffer(VBO[3], temp3, end2 * sizeofT3, begin2 * sizeofT3, (begin - end2) * sizeofT3);
			}
			glDeleteBuffers(VBONum, VBO);
			VBO[0] = temp0; VBO[1] = temp1; VBO[2] = temp2; VBO[3] = temp3;

			glBindVertexArray(VAO);
			BindBuffer<T0>(0, VBO[0], _data0);
			BindBuffer<T0>(1, VBO[1], _data1);
			BindBuffer<T0>(2, VBO[2], _data2);
			BindBuffer<T0>(3, VBO[3], _data3);

			glBindVertexArray(0);

			begin = size;
		};

	public:
		Data<T0> _data0;
		Data<T1> _data1;
		Data<T2> _data2;
		Data<T3> _data3;
	};

	template<class T0, class T1, class T2, class T3, class T4>
	class mBasicMeshRend4 : public mBasicMeshRend
	{

	public:
		mBasicMeshRend4()
		{
			getGLenumType<T0>(_data0._dataType, _data0._size);
			getGLenumType<T1>(_data1._dataType, _data1._size);
			getGLenumType<T2>(_data2._dataType, _data2._size);
			getGLenumType<T3>(_data3._dataType, _data3._size);
			getGLenumType<T4>(_data4._dataType, _data4._size);

			VBONum = 5;
		};

		~mBasicMeshRend4() { };

		//添加部件更新buffer
		void AppendPartBuffer()
		{
			if (!isInitialized())
			{
				this->initial();
			}

			int sizeofT0 = sizeof(T0);
			int sizeofT1 = sizeof(T1);
			int sizeofT2 = sizeof(T2);
			int sizeofT3 = sizeof(T3);
			int sizeofT4 = sizeof(T4);
			//更新数据
			GLuint temp0 = CreateArrayBufferData(sizeofT0*num);
			GLuint temp1 = CreateArrayBufferData(sizeofT1*num);
			GLuint temp2 = CreateArrayBufferData(sizeofT2*num);
			GLuint temp3 = CreateArrayBufferData(sizeofT3*num);
			GLuint temp4 = CreateArrayBufferData(sizeofT4*num);
			if (Size_begin != 0)
			{
				CopyBuffer(VBO[0], temp0, 0, 0, Size_begin * sizeofT0);
				CopyBuffer(VBO[1], temp1, 0, 0, Size_begin * sizeofT1);
				CopyBuffer(VBO[2], temp2, 0, 0, Size_begin * sizeofT2);
				CopyBuffer(VBO[3], temp3, 0, 0, Size_begin * sizeofT3);
				CopyBuffer(VBO[4], temp4, 0, 0, Size_begin * sizeofT4);
				glDeleteBuffers(VBONum, VBO);
			}

			glBindVertexArray(vao[0]);

			BindSubBuffer(0, temp0, sizeofT0, _data0);
			BindSubBuffer(1, temp1, sizeofT1, _data1);
			BindSubBuffer(2, temp2, sizeofT2, _data2);
			BindSubBuffer(3, temp3, sizeofT3, _data3);
			BindSubBuffer(4, temp4, sizeofT4, _data4);

			glBindVertexArray(0);
			Size_begin = num;
			VBO[0] = temp0; VBO[1] = temp1; VBO[2] = temp2; VBO[3] = temp3; VBO[4] = temp4;
			QVector<T0>().swap(_data0._vertex);
			QVector<T1>().swap(_data1._vertex);
			QVector<T2>().swap(_data2._vertex);
			QVector<T3>().swap(_data3._vertex);
			QVector<T4>().swap(_data4._vertex);
		};

	private:
		//更新删除的buffer
		virtual void UpdateDeleteBuffer(GLuint &VAO, GLuint *VBO, QPair<int, int> deleteBeginSize, int &size, int &begin) override
		{
			int sizeofT0 = sizeof(T0);
			int sizeofT1 = sizeof(T1);
			int sizeofT2 = sizeof(T2);
			int sizeofT3 = sizeof(T3);
			int sizeofT4 = sizeof(T4);

			GLuint temp0 = CreateArrayBufferData(sizeofT0*size);
			GLuint temp1 = CreateArrayBufferData(sizeofT1*size);
			GLuint temp2 = CreateArrayBufferData(sizeofT2*size);
			GLuint temp3 = CreateArrayBufferData(sizeofT3*size);
			GLuint temp4 = CreateArrayBufferData(sizeofT4*size);

			int begin2 = deleteBeginSize.first;
			int end2 = deleteBeginSize.first + deleteBeginSize.second;

			if (size > 0)
			{
				CopyBuffer(VBO[0], temp0, 0, 0, begin2 * sizeofT0);
				CopyBuffer(VBO[0], temp0, end2 * sizeofT0, begin2 * sizeofT0, (begin - end2) * sizeofT0);

				CopyBuffer(VBO[1], temp1, 0, 0, begin2 * sizeofT1);
				CopyBuffer(VBO[1], temp1, end2 * sizeofT1, begin2 * sizeofT1, (begin - end2) * sizeofT1);

				CopyBuffer(VBO[2], temp2, 0, 0, begin2 * sizeofT2);
				CopyBuffer(VBO[2], temp2, end2 * sizeofT2, begin2 * sizeofT2, (begin - end2) * sizeofT2);

				CopyBuffer(VBO[3], temp3, 0, 0, begin2 * sizeofT3);
				CopyBuffer(VBO[3], temp3, end2 * sizeofT3, begin2 * sizeofT3, (begin - end2) * sizeofT3);

				CopyBuffer(VBO[4], temp4, 0, 0, begin2 * sizeofT4);
				CopyBuffer(VBO[4], temp4, end2 * sizeofT4, begin2 * sizeofT4, (begin - end2) * sizeofT4);
			}
			glDeleteBuffers(VBONum, VBO);
			VBO[0] = temp0; VBO[1] = temp1; VBO[2] = temp2; VBO[3] = temp3; VBO[4] = temp4;

			glBindVertexArray(VAO);
			BindBuffer<T0>(0, VBO[0], _data0);
			BindBuffer<T0>(1, VBO[1], _data1);
			BindBuffer<T0>(2, VBO[2], _data2);
			BindBuffer<T0>(3, VBO[3], _data3);
			BindBuffer<T0>(4, VBO[4], _data4);

			glBindVertexArray(0);

			begin = size;
		};

	public:
		Data<T0> _data0;
		Data<T1> _data1;
		Data<T2> _data2;
		Data<T3> _data3;
		Data<T4> _data4;
	};

	template<class T0, class T1, class T2, class T3, class T4, class T5>
	class mBasicMeshRend5 : public mBasicMeshRend
	{

	public:
		mBasicMeshRend5()
		{
			getGLenumType<T0>(_data0._dataType, _data0._size);
			getGLenumType<T1>(_data1._dataType, _data1._size);
			getGLenumType<T2>(_data2._dataType, _data2._size);
			getGLenumType<T3>(_data3._dataType, _data3._size);
			getGLenumType<T4>(_data4._dataType, _data4._size);
			getGLenumType<T5>(_data5._dataType, _data5._size);

			VBONum = 6;
		};

		~mBasicMeshRend5() { };

		//添加部件更新buffer
		void AppendPartBuffer()
		{
			if (!isInitialized())
			{
				this->initial();
			}

			int sizeofT0 = sizeof(T0);
			int sizeofT1 = sizeof(T1);
			int sizeofT2 = sizeof(T2);
			int sizeofT3 = sizeof(T3);
			int sizeofT4 = sizeof(T4);
			int sizeofT5 = sizeof(T5);
			//更新数据
			GLuint temp0 = CreateArrayBufferData(sizeofT0*num);
			GLuint temp1 = CreateArrayBufferData(sizeofT1*num);
			GLuint temp2 = CreateArrayBufferData(sizeofT2*num);
			GLuint temp3 = CreateArrayBufferData(sizeofT3*num);
			GLuint temp4 = CreateArrayBufferData(sizeofT4*num);
			GLuint temp5 = CreateArrayBufferData(sizeofT5*num);
			if (Size_begin != 0)
			{
				CopyBuffer(VBO[0], temp0, 0, 0, Size_begin * sizeofT0);
				CopyBuffer(VBO[1], temp1, 0, 0, Size_begin * sizeofT1);
				CopyBuffer(VBO[2], temp2, 0, 0, Size_begin * sizeofT2);
				CopyBuffer(VBO[3], temp3, 0, 0, Size_begin * sizeofT3);
				CopyBuffer(VBO[4], temp4, 0, 0, Size_begin * sizeofT4);
				CopyBuffer(VBO[5], temp5, 0, 0, Size_begin * sizeofT5);
				glDeleteBuffers(VBONum, VBO);
			}

			glBindVertexArray(vao[0]);

			BindSubBuffer(0, temp0, sizeofT0, _data0);
			BindSubBuffer(1, temp1, sizeofT1, _data1);
			BindSubBuffer(2, temp2, sizeofT2, _data2);
			BindSubBuffer(3, temp3, sizeofT3, _data3);
			BindSubBuffer(4, temp4, sizeofT4, _data4);
			BindSubBuffer(5, temp5, sizeofT5, _data5);

			glBindVertexArray(0);
			Size_begin = num;
			VBO[0] = temp0; VBO[1] = temp1; VBO[2] = temp2; VBO[3] = temp3; VBO[4] = temp4; VBO[5] = temp5;
			QVector<T0>().swap(_data0._vertex);
			QVector<T1>().swap(_data1._vertex);
			QVector<T2>().swap(_data2._vertex);
			QVector<T3>().swap(_data3._vertex);
			QVector<T4>().swap(_data4._vertex);
			QVector<T5>().swap(_data5._vertex);
		};

	private:
		//更新删除的buffer
		virtual void UpdateDeleteBuffer(GLuint &VAO, GLuint *VBO, QPair<int, int> deleteBeginSize, int &size, int &begin) override
		{
			int sizeofT0 = sizeof(T0);
			int sizeofT1 = sizeof(T1);
			int sizeofT2 = sizeof(T2);
			int sizeofT3 = sizeof(T3);
			int sizeofT4 = sizeof(T4);
			int sizeofT5 = sizeof(T5);

			GLuint temp0 = CreateArrayBufferData(sizeofT0*size);
			GLuint temp1 = CreateArrayBufferData(sizeofT1*size);
			GLuint temp2 = CreateArrayBufferData(sizeofT2*size);
			GLuint temp3 = CreateArrayBufferData(sizeofT3*size);
			GLuint temp4 = CreateArrayBufferData(sizeofT4*size);
			GLuint temp5 = CreateArrayBufferData(sizeofT5*size);

			int begin2 = deleteBeginSize.first;
			int end2 = deleteBeginSize.first + deleteBeginSize.second;

			if (size > 0)
			{
				CopyBuffer(VBO[0], temp0, 0, 0, begin2 * sizeofT0);
				CopyBuffer(VBO[0], temp0, end2 * sizeofT0, begin2 * sizeofT0, (begin - end2) * sizeofT0);

				CopyBuffer(VBO[1], temp1, 0, 0, begin2 * sizeofT1);
				CopyBuffer(VBO[1], temp1, end2 * sizeofT1, begin2 * sizeofT1, (begin - end2) * sizeofT1);

				CopyBuffer(VBO[2], temp2, 0, 0, begin2 * sizeofT2);
				CopyBuffer(VBO[2], temp2, end2 * sizeofT2, begin2 * sizeofT2, (begin - end2) * sizeofT2);

				CopyBuffer(VBO[3], temp3, 0, 0, begin2 * sizeofT3);
				CopyBuffer(VBO[3], temp3, end2 * sizeofT3, begin2 * sizeofT3, (begin - end2) * sizeofT3);

				CopyBuffer(VBO[4], temp4, 0, 0, begin2 * sizeofT4);
				CopyBuffer(VBO[4], temp4, end2 * sizeofT4, begin2 * sizeofT4, (begin - end2) * sizeofT4);

				CopyBuffer(VBO[5], temp5, 0, 0, begin2 * sizeofT5);
				CopyBuffer(VBO[5], temp5, end2 * sizeofT5, begin2 * sizeofT5, (begin - end2) * sizeofT5);
			}
			glDeleteBuffers(VBONum, VBO);
			VBO[0] = temp0; VBO[1] = temp1; VBO[2] = temp2; VBO[3] = temp3; VBO[4] = temp4; VBO[5] = temp5;

			glBindVertexArray(VAO);
			BindBuffer<T0>(0, VBO[0], _data0);
			BindBuffer<T0>(1, VBO[1], _data1);
			BindBuffer<T0>(2, VBO[2], _data2);
			BindBuffer<T0>(3, VBO[3], _data3);
			BindBuffer<T0>(4, VBO[4], _data4);
			BindBuffer<T0>(5, VBO[5], _data5);

			glBindVertexArray(0);

			begin = size;
		};

	public:
		Data<T0> _data0;
		Data<T1> _data1;
		Data<T2> _data2;
		Data<T3> _data3;
		Data<T4> _data4;
		Data<T5> _data5;
	};

	template<class T0, class T1, class T2, class T3, class T4, class T5, class T6>
	class mBasicMeshRend6 : public mBasicMeshRend
	{

	public:
		mBasicMeshRend6()
		{
			getGLenumType<T0>(_data0._dataType, _data0._size);
			getGLenumType<T1>(_data1._dataType, _data1._size);
			getGLenumType<T2>(_data2._dataType, _data2._size);
			getGLenumType<T3>(_data3._dataType, _data3._size);
			getGLenumType<T4>(_data4._dataType, _data4._size);
			getGLenumType<T5>(_data5._dataType, _data5._size);
			getGLenumType<T6>(_data6._dataType, _data6._size);

			VBONum = 7;
		};

		~mBasicMeshRend6() { };

		//添加部件更新buffer
		void AppendPartBuffer()
		{
			if (!isInitialized())
			{
				this->initial();
			}

			int sizeofT0 = sizeof(T0);
			int sizeofT1 = sizeof(T1);
			int sizeofT2 = sizeof(T2);
			int sizeofT3 = sizeof(T3);
			int sizeofT4 = sizeof(T4);
			int sizeofT5 = sizeof(T5);
			int sizeofT6 = sizeof(T6);
			//更新数据
			GLuint temp0 = CreateArrayBufferData(sizeofT0*num);
			GLuint temp1 = CreateArrayBufferData(sizeofT1*num);
			GLuint temp2 = CreateArrayBufferData(sizeofT2*num);
			GLuint temp3 = CreateArrayBufferData(sizeofT3*num);
			GLuint temp4 = CreateArrayBufferData(sizeofT4*num);
			GLuint temp5 = CreateArrayBufferData(sizeofT5*num);
			GLuint temp6 = CreateArrayBufferData(sizeofT6*num);
			if (Size_begin != 0)
			{
				CopyBuffer(VBO[0], temp0, 0, 0, Size_begin * sizeofT0);
				CopyBuffer(VBO[1], temp1, 0, 0, Size_begin * sizeofT1);
				CopyBuffer(VBO[2], temp2, 0, 0, Size_begin * sizeofT2);
				CopyBuffer(VBO[3], temp3, 0, 0, Size_begin * sizeofT3);
				CopyBuffer(VBO[4], temp4, 0, 0, Size_begin * sizeofT4);
				CopyBuffer(VBO[5], temp5, 0, 0, Size_begin * sizeofT5);
				CopyBuffer(VBO[6], temp6, 0, 0, Size_begin * sizeofT6);
				glDeleteBuffers(VBONum, VBO);
			}

			glBindVertexArray(vao[0]);

			BindSubBuffer(0, temp0, sizeofT0, _data0);
			BindSubBuffer(1, temp1, sizeofT1, _data1);
			BindSubBuffer(2, temp2, sizeofT2, _data2);
			BindSubBuffer(3, temp3, sizeofT3, _data3);
			BindSubBuffer(4, temp4, sizeofT4, _data4);
			BindSubBuffer(5, temp5, sizeofT5, _data5);
			BindSubBuffer(6, temp6, sizeofT6, _data6);

			glBindVertexArray(0);
			Size_begin = num;
			VBO[0] = temp0; VBO[1] = temp1; VBO[2] = temp2; VBO[3] = temp3; VBO[4] = temp4; VBO[5] = temp5; VBO[6] = temp6;
			QVector<T0>().swap(_data0._vertex);
			QVector<T1>().swap(_data1._vertex);
			QVector<T2>().swap(_data2._vertex);
			QVector<T3>().swap(_data3._vertex);
			QVector<T4>().swap(_data4._vertex);
			QVector<T5>().swap(_data5._vertex);
			QVector<T6>().swap(_data6._vertex);
		};

	private:
		//更新删除的buffer
		virtual void UpdateDeleteBuffer(GLuint &VAO, GLuint *VBO, QPair<int, int> deleteBeginSize, int &size, int &begin) override
		{
			int sizeofT0 = sizeof(T0);
			int sizeofT1 = sizeof(T1);
			int sizeofT2 = sizeof(T2);
			int sizeofT3 = sizeof(T3);
			int sizeofT4 = sizeof(T4);
			int sizeofT5 = sizeof(T5);
			int sizeofT6 = sizeof(T6);

			GLuint temp0 = CreateArrayBufferData(sizeofT0*size);
			GLuint temp1 = CreateArrayBufferData(sizeofT1*size);
			GLuint temp2 = CreateArrayBufferData(sizeofT2*size);
			GLuint temp3 = CreateArrayBufferData(sizeofT3*size);
			GLuint temp4 = CreateArrayBufferData(sizeofT4*size);
			GLuint temp5 = CreateArrayBufferData(sizeofT5*size);
			GLuint temp6 = CreateArrayBufferData(sizeofT6*size);

			int begin2 = deleteBeginSize.first;
			int end2 = deleteBeginSize.first + deleteBeginSize.second;

			if (size > 0)
			{
				CopyBuffer(VBO[0], temp0, 0, 0, begin2 * sizeofT0);
				CopyBuffer(VBO[0], temp0, end2 * sizeofT0, begin2 * sizeofT0, (begin - end2) * sizeofT0);

				CopyBuffer(VBO[1], temp1, 0, 0, begin2 * sizeofT1);
				CopyBuffer(VBO[1], temp1, end2 * sizeofT1, begin2 * sizeofT1, (begin - end2) * sizeofT1);

				CopyBuffer(VBO[2], temp2, 0, 0, begin2 * sizeofT2);
				CopyBuffer(VBO[2], temp2, end2 * sizeofT2, begin2 * sizeofT2, (begin - end2) * sizeofT2);

				CopyBuffer(VBO[3], temp3, 0, 0, begin2 * sizeofT3);
				CopyBuffer(VBO[3], temp3, end2 * sizeofT3, begin2 * sizeofT3, (begin - end2) * sizeofT3);

				CopyBuffer(VBO[4], temp4, 0, 0, begin2 * sizeofT4);
				CopyBuffer(VBO[4], temp4, end2 * sizeofT4, begin2 * sizeofT4, (begin - end2) * sizeofT4);

				CopyBuffer(VBO[5], temp5, 0, 0, begin2 * sizeofT5);
				CopyBuffer(VBO[5], temp5, end2 * sizeofT5, begin2 * sizeofT5, (begin - end2) * sizeofT5);

				CopyBuffer(VBO[6], temp6, 0, 0, begin2 * sizeofT6);
				CopyBuffer(VBO[6], temp6, end2 * sizeofT6, begin2 * sizeofT6, (begin - end2) * sizeofT6);
			}
			glDeleteBuffers(VBONum, VBO);
			VBO[0] = temp0; VBO[1] = temp1; VBO[2] = temp2; VBO[3] = temp3; VBO[4] = temp4; VBO[5] = temp5; VBO[6] = temp6;

			glBindVertexArray(VAO);
			BindBuffer<T0>(0, VBO[0], _data0);
			BindBuffer<T0>(1, VBO[1], _data1);
			BindBuffer<T0>(2, VBO[2], _data2);
			BindBuffer<T0>(3, VBO[3], _data3);
			BindBuffer<T0>(4, VBO[4], _data4);
			BindBuffer<T0>(5, VBO[5], _data5);
			BindBuffer<T0>(6, VBO[6], _data6);

			glBindVertexArray(0);

			begin = size;
		};

	public:
		Data<T0> _data0;
		Data<T1> _data1;
		Data<T2> _data2;
		Data<T3> _data3;
		Data<T4> _data4;
		Data<T5> _data5;
		Data<T6> _data6;
	};

	template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	class mBasicMeshRend7 : public mBasicMeshRend
	{

	public:
		mBasicMeshRend7()
		{
			getGLenumType<T0>(_data0._dataType, _data0._size);
			getGLenumType<T1>(_data1._dataType, _data1._size);
			getGLenumType<T2>(_data2._dataType, _data2._size);
			getGLenumType<T3>(_data3._dataType, _data3._size);
			getGLenumType<T4>(_data4._dataType, _data4._size);
			getGLenumType<T5>(_data5._dataType, _data5._size);
			getGLenumType<T6>(_data6._dataType, _data6._size);
			getGLenumType<T7>(_data7._dataType, _data7._size);

			VBONum = 8;
		};

		~mBasicMeshRend7() { };

		//添加部件更新buffer
		void AppendPartBuffer()
		{
			if (!isInitialized())
			{
				this->initial();
			}

			int sizeofT0 = sizeof(T0);
			int sizeofT1 = sizeof(T1);
			int sizeofT2 = sizeof(T2);
			int sizeofT3 = sizeof(T3);
			int sizeofT4 = sizeof(T4);
			int sizeofT5 = sizeof(T5);
			int sizeofT6 = sizeof(T6);
			int sizeofT7 = sizeof(T7);
			//更新数据
			GLuint temp0 = CreateArrayBufferData(sizeofT0*num);
			GLuint temp1 = CreateArrayBufferData(sizeofT1*num);
			GLuint temp2 = CreateArrayBufferData(sizeofT2*num);
			GLuint temp3 = CreateArrayBufferData(sizeofT3*num);
			GLuint temp4 = CreateArrayBufferData(sizeofT4*num);
			GLuint temp5 = CreateArrayBufferData(sizeofT5*num);
			GLuint temp6 = CreateArrayBufferData(sizeofT6*num);
			GLuint temp7 = CreateArrayBufferData(sizeofT7*num);
			if (Size_begin != 0)
			{
				CopyBuffer(VBO[0], temp0, 0, 0, Size_begin * sizeofT0);
				CopyBuffer(VBO[1], temp1, 0, 0, Size_begin * sizeofT1);
				CopyBuffer(VBO[2], temp2, 0, 0, Size_begin * sizeofT2);
				CopyBuffer(VBO[3], temp3, 0, 0, Size_begin * sizeofT3);
				CopyBuffer(VBO[4], temp4, 0, 0, Size_begin * sizeofT4);
				CopyBuffer(VBO[5], temp5, 0, 0, Size_begin * sizeofT5);
				CopyBuffer(VBO[6], temp6, 0, 0, Size_begin * sizeofT6);
				CopyBuffer(VBO[7], temp7, 0, 0, Size_begin * sizeofT7);
				glDeleteBuffers(VBONum, VBO);
			}

			glBindVertexArray(vao[0]);

			BindSubBuffer(0, temp0, sizeofT0, _data0);
			BindSubBuffer(1, temp1, sizeofT1, _data1);
			BindSubBuffer(2, temp2, sizeofT2, _data2);
			BindSubBuffer(3, temp3, sizeofT3, _data3);
			BindSubBuffer(4, temp4, sizeofT4, _data4);
			BindSubBuffer(5, temp5, sizeofT5, _data5);
			BindSubBuffer(6, temp6, sizeofT6, _data6);
			BindSubBuffer(7, temp7, sizeofT7, _data7);

			glBindVertexArray(0);
			Size_begin = num;
			VBO[0] = temp0; VBO[1] = temp1; VBO[2] = temp2; VBO[3] = temp3; VBO[4] = temp4; VBO[5] = temp5; VBO[6] = temp6; VBO[7] = temp7;
			QVector<T0>().swap(_data0._vertex);
			QVector<T1>().swap(_data1._vertex);
			QVector<T2>().swap(_data2._vertex);
			QVector<T3>().swap(_data3._vertex);
			QVector<T4>().swap(_data4._vertex);
			QVector<T5>().swap(_data5._vertex);
			QVector<T6>().swap(_data6._vertex);
			QVector<T7>().swap(_data7._vertex);
		};

	private:
		//更新删除的buffer
		virtual void UpdateDeleteBuffer(GLuint &VAO, GLuint *VBO, QPair<int, int> deleteBeginSize, int &size, int &begin) override
		{
			int sizeofT0 = sizeof(T0);
			int sizeofT1 = sizeof(T1);
			int sizeofT2 = sizeof(T2);
			int sizeofT3 = sizeof(T3);
			int sizeofT4 = sizeof(T4);
			int sizeofT5 = sizeof(T5);
			int sizeofT6 = sizeof(T6);
			int sizeofT7 = sizeof(T7);

			GLuint temp0 = CreateArrayBufferData(sizeofT0*size);
			GLuint temp1 = CreateArrayBufferData(sizeofT1*size);
			GLuint temp2 = CreateArrayBufferData(sizeofT2*size);
			GLuint temp3 = CreateArrayBufferData(sizeofT3*size);
			GLuint temp4 = CreateArrayBufferData(sizeofT4*size);
			GLuint temp5 = CreateArrayBufferData(sizeofT5*size);
			GLuint temp6 = CreateArrayBufferData(sizeofT6*size);
			GLuint temp7 = CreateArrayBufferData(sizeofT7*size);

			int begin2 = deleteBeginSize.first;
			int end2 = deleteBeginSize.first + deleteBeginSize.second;

			if (size > 0)
			{
				CopyBuffer(VBO[0], temp0, 0, 0, begin2 * sizeofT0);
				CopyBuffer(VBO[0], temp0, end2 * sizeofT0, begin2 * sizeofT0, (begin - end2) * sizeofT0);

				CopyBuffer(VBO[1], temp1, 0, 0, begin2 * sizeofT1);
				CopyBuffer(VBO[1], temp1, end2 * sizeofT1, begin2 * sizeofT1, (begin - end2) * sizeofT1);

				CopyBuffer(VBO[2], temp2, 0, 0, begin2 * sizeofT2);
				CopyBuffer(VBO[2], temp2, end2 * sizeofT2, begin2 * sizeofT2, (begin - end2) * sizeofT2);

				CopyBuffer(VBO[3], temp3, 0, 0, begin2 * sizeofT3);
				CopyBuffer(VBO[3], temp3, end2 * sizeofT3, begin2 * sizeofT3, (begin - end2) * sizeofT3);

				CopyBuffer(VBO[4], temp4, 0, 0, begin2 * sizeofT4);
				CopyBuffer(VBO[4], temp4, end2 * sizeofT4, begin2 * sizeofT4, (begin - end2) * sizeofT4);

				CopyBuffer(VBO[5], temp5, 0, 0, begin2 * sizeofT5);
				CopyBuffer(VBO[5], temp5, end2 * sizeofT5, begin2 * sizeofT5, (begin - end2) * sizeofT5);

				CopyBuffer(VBO[6], temp6, 0, 0, begin2 * sizeofT6);
				CopyBuffer(VBO[6], temp6, end2 * sizeofT6, begin2 * sizeofT6, (begin - end2) * sizeofT6);

				CopyBuffer(VBO[7], temp7, 0, 0, begin2 * sizeofT7);
				CopyBuffer(VBO[7], temp7, end2 * sizeofT7, begin2 * sizeofT7, (begin - end2) * sizeofT7);
			}
			glDeleteBuffers(VBONum, VBO);
			VBO[0] = temp0; VBO[1] = temp1; VBO[2] = temp2; VBO[3] = temp3; VBO[4] = temp4; VBO[5] = temp5; VBO[6] = temp6; VBO[7] = temp7;

			glBindVertexArray(VAO);
			BindBuffer<T0>(0, VBO[0], _data0);
			BindBuffer<T0>(1, VBO[1], _data1);
			BindBuffer<T0>(2, VBO[2], _data2);
			BindBuffer<T0>(3, VBO[3], _data3);
			BindBuffer<T0>(4, VBO[4], _data4);
			BindBuffer<T0>(5, VBO[5], _data5);
			BindBuffer<T0>(6, VBO[6], _data6);
			BindBuffer<T0>(7, VBO[7], _data7);

			glBindVertexArray(0);

			begin = size;
		};

	public:
		Data<T0> _data0;
		Data<T1> _data1;
		Data<T2> _data2;
		Data<T3> _data3;
		Data<T4> _data4;
		Data<T5> _data5;
		Data<T6> _data6;
		Data<T7> _data7;
	};

	template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	class mBasicMeshRend8 : public mBasicMeshRend
	{

	public:
		mBasicMeshRend8()
		{
			getGLenumType<T0>(_data0._dataType, _data0._size);
			getGLenumType<T1>(_data1._dataType, _data1._size);
			getGLenumType<T2>(_data2._dataType, _data2._size);
			getGLenumType<T3>(_data3._dataType, _data3._size);
			getGLenumType<T4>(_data4._dataType, _data4._size);
			getGLenumType<T5>(_data5._dataType, _data5._size);
			getGLenumType<T6>(_data6._dataType, _data6._size);
			getGLenumType<T7>(_data7._dataType, _data7._size);
			getGLenumType<T8>(_data8._dataType, _data8._size);

			VBONum = 9;
		};

		~mBasicMeshRend8() { };

		//添加部件更新buffer
		void AppendPartBuffer()
		{
			if (!isInitialized())
			{
				this->initial();
			}

			int sizeofT0 = sizeof(T0);
			int sizeofT1 = sizeof(T1);
			int sizeofT2 = sizeof(T2);
			int sizeofT3 = sizeof(T3);
			int sizeofT4 = sizeof(T4);
			int sizeofT5 = sizeof(T5);
			int sizeofT6 = sizeof(T6);
			int sizeofT7 = sizeof(T7);
			int sizeofT8 = sizeof(T8);
			//更新数据
			GLuint temp0 = CreateArrayBufferData(sizeofT0*num);
			GLuint temp1 = CreateArrayBufferData(sizeofT1*num);
			GLuint temp2 = CreateArrayBufferData(sizeofT2*num);
			GLuint temp3 = CreateArrayBufferData(sizeofT3*num);
			GLuint temp4 = CreateArrayBufferData(sizeofT4*num);
			GLuint temp5 = CreateArrayBufferData(sizeofT5*num);
			GLuint temp6 = CreateArrayBufferData(sizeofT6*num);
			GLuint temp7 = CreateArrayBufferData(sizeofT7*num);
			GLuint temp8 = CreateArrayBufferData(sizeofT8*num);
			if (Size_begin != 0)
			{
				CopyBuffer(VBO[0], temp0, 0, 0, Size_begin * sizeofT0);
				CopyBuffer(VBO[1], temp1, 0, 0, Size_begin * sizeofT1);
				CopyBuffer(VBO[2], temp2, 0, 0, Size_begin * sizeofT2);
				CopyBuffer(VBO[3], temp3, 0, 0, Size_begin * sizeofT3);
				CopyBuffer(VBO[4], temp4, 0, 0, Size_begin * sizeofT4);
				CopyBuffer(VBO[5], temp5, 0, 0, Size_begin * sizeofT5);
				CopyBuffer(VBO[6], temp6, 0, 0, Size_begin * sizeofT6);
				CopyBuffer(VBO[7], temp7, 0, 0, Size_begin * sizeofT7);
				CopyBuffer(VBO[8], temp8, 0, 0, Size_begin * sizeofT8);
				glDeleteBuffers(VBONum, VBO);
			}

			glBindVertexArray(vao[0]);

			BindSubBuffer(0, temp0, sizeofT0, _data0);
			BindSubBuffer(1, temp1, sizeofT1, _data1);
			BindSubBuffer(2, temp2, sizeofT2, _data2);
			BindSubBuffer(3, temp3, sizeofT3, _data3);
			BindSubBuffer(4, temp4, sizeofT4, _data4);
			BindSubBuffer(5, temp5, sizeofT5, _data5);
			BindSubBuffer(6, temp6, sizeofT6, _data6);
			BindSubBuffer(7, temp7, sizeofT7, _data7);
			BindSubBuffer(8, temp8, sizeofT8, _data8);

			glBindVertexArray(0);
			Size_begin = num;
			VBO[0] = temp0; VBO[1] = temp1; VBO[2] = temp2; VBO[3] = temp3; VBO[4] = temp4; VBO[5] = temp5; VBO[6] = temp6; VBO[7] = temp7;
			VBO[8] = temp8;
			QVector<T0>().swap(_data0._vertex);
			QVector<T1>().swap(_data1._vertex);
			QVector<T2>().swap(_data2._vertex);
			QVector<T3>().swap(_data3._vertex);
			QVector<T4>().swap(_data4._vertex);
			QVector<T5>().swap(_data5._vertex);
			QVector<T6>().swap(_data6._vertex);
			QVector<T7>().swap(_data7._vertex);
			QVector<T8>().swap(_data8._vertex);
		};

	private:
		//更新删除的buffer
		virtual void UpdateDeleteBuffer(GLuint &VAO, GLuint *VBO, QPair<int, int> deleteBeginSize, int &size, int &begin) override
		{
			int sizeofT0 = sizeof(T0);
			int sizeofT1 = sizeof(T1);
			int sizeofT2 = sizeof(T2);
			int sizeofT3 = sizeof(T3);
			int sizeofT4 = sizeof(T4);
			int sizeofT5 = sizeof(T5);
			int sizeofT6 = sizeof(T6);
			int sizeofT7 = sizeof(T7);
			int sizeofT8 = sizeof(T8);

			GLuint temp0 = CreateArrayBufferData(sizeofT0*size);
			GLuint temp1 = CreateArrayBufferData(sizeofT1*size);
			GLuint temp2 = CreateArrayBufferData(sizeofT2*size);
			GLuint temp3 = CreateArrayBufferData(sizeofT3*size);
			GLuint temp4 = CreateArrayBufferData(sizeofT4*size);
			GLuint temp5 = CreateArrayBufferData(sizeofT5*size);
			GLuint temp6 = CreateArrayBufferData(sizeofT6*size);
			GLuint temp7 = CreateArrayBufferData(sizeofT7*size);
			GLuint temp8 = CreateArrayBufferData(sizeofT8*size);

			int begin2 = deleteBeginSize.first;
			int end2 = deleteBeginSize.first + deleteBeginSize.second;

			if (size > 0)
			{
				CopyBuffer(VBO[0], temp0, 0, 0, begin2 * sizeofT0);
				CopyBuffer(VBO[0], temp0, end2 * sizeofT0, begin2 * sizeofT0, (begin - end2) * sizeofT0);

				CopyBuffer(VBO[1], temp1, 0, 0, begin2 * sizeofT1);
				CopyBuffer(VBO[1], temp1, end2 * sizeofT1, begin2 * sizeofT1, (begin - end2) * sizeofT1);

				CopyBuffer(VBO[2], temp2, 0, 0, begin2 * sizeofT2);
				CopyBuffer(VBO[2], temp2, end2 * sizeofT2, begin2 * sizeofT2, (begin - end2) * sizeofT2);

				CopyBuffer(VBO[3], temp3, 0, 0, begin2 * sizeofT3);
				CopyBuffer(VBO[3], temp3, end2 * sizeofT3, begin2 * sizeofT3, (begin - end2) * sizeofT3);

				CopyBuffer(VBO[4], temp4, 0, 0, begin2 * sizeofT4);
				CopyBuffer(VBO[4], temp4, end2 * sizeofT4, begin2 * sizeofT4, (begin - end2) * sizeofT4);

				CopyBuffer(VBO[5], temp5, 0, 0, begin2 * sizeofT5);
				CopyBuffer(VBO[5], temp5, end2 * sizeofT5, begin2 * sizeofT5, (begin - end2) * sizeofT5);

				CopyBuffer(VBO[6], temp6, 0, 0, begin2 * sizeofT6);
				CopyBuffer(VBO[6], temp6, end2 * sizeofT6, begin2 * sizeofT6, (begin - end2) * sizeofT6);

				CopyBuffer(VBO[7], temp7, 0, 0, begin2 * sizeofT7);
				CopyBuffer(VBO[7], temp7, end2 * sizeofT7, begin2 * sizeofT7, (begin - end2) * sizeofT7);

				CopyBuffer(VBO[8], temp8, 0, 0, begin2 * sizeofT8);
				CopyBuffer(VBO[8], temp8, end2 * sizeofT8, begin2 * sizeofT8, (begin - end2) * sizeofT8);
			}
			glDeleteBuffers(VBONum, VBO);
			VBO[0] = temp0; VBO[1] = temp1; VBO[2] = temp2; VBO[3] = temp3; VBO[4] = temp4; VBO[5] = temp5; VBO[6] = temp6; VBO[7] = temp7;
			VBO[8] = temp8;

			glBindVertexArray(VAO);
			BindBuffer<T0>(0, VBO[0], _data0);
			BindBuffer<T0>(1, VBO[1], _data1);
			BindBuffer<T0>(2, VBO[2], _data2);
			BindBuffer<T0>(3, VBO[3], _data3);
			BindBuffer<T0>(4, VBO[4], _data4);
			BindBuffer<T0>(5, VBO[5], _data5);
			BindBuffer<T0>(6, VBO[6], _data6);
			BindBuffer<T0>(7, VBO[7], _data7);
			BindBuffer<T0>(8, VBO[8], _data8);

			glBindVertexArray(0);

			begin = size;
		};

	public:
		Data<T0> _data0;
		Data<T1> _data1;
		Data<T2> _data2;
		Data<T3> _data3;
		Data<T4> _data4;
		Data<T5> _data5;
		Data<T6> _data6;
		Data<T7> _data7;
		Data<T8> _data8;
	};







}
