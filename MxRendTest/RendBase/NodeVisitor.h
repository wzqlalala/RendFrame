#pragma once
#include "Array.h"
#include "Drawable.h"
#include "RenderPass.h"

namespace mxr
{
	struct MXR_EXPORT DrawableData
	{
		int offset;
		int size;
		int data_size;
		GLenum type;
		const GLvoid* data;
	};

	struct MXR_EXPORT DrawableAttribute
	{
		Drawable* drawable;
		bool isCompile;
		bool isClear = false;
		std::map<int, DrawableData> census();
	};


	struct MXR_EXPORT OnceDrawAttribute
	{
		asset_ref<VAO> _vao;
		asset_ref<StateSet> _state;
		asset_ref<DrawBuffer> _drawbuffer;
		int drawCount;
		bool HaveIBO;
	};


	struct MXR_EXPORT VaoDrawArrayAttribute
	{
		asset_ref<VAO> vao;
		std::map<int, asset_ref<VBO>> vbos;
		asset_ref<IBO> _ibo;
		std::map <int, int>    vbos_datasize;
		std::map <int, GLenum> vbos_datatype;

		std::map<asset_ref<StateSet>, asset_ref<DrawBuffer> > _drawbuffers;
		std::map<asset_ref<StateSet>, std::vector<DrawArraysIndirectCommand> > _arraystates;
		std::map<asset_ref<StateSet>, std::vector<DrawElementsIndirectCommand> > _elementstates;

		bool isSetVAOFormat = false;
		bool HaveIBO = false;
		void compile(std::vector<DrawableAttribute> &_drawableattribute);
	};


	class MXR_EXPORT NodeVisitor
	{
	public:

		//具有相同顶点格式的drawable放到一起
		typedef std::vector< std::vector<asset_ref<Drawable> > > DrawableList;
		NodeVisitor() {}
		//组织node数据，将相同数据格式的node放在一起
		void accept(Drawable* node);
		//生成vao,vbo,drawbuffer等
		void compile();
		void clear();//清除原始渲染数据
		void run();
		void RemoveDrawableAttribute(Drawable *node);
		void RemoveAllData();
		
	protected:
		//具有相同顶点数据格式的drawable为一组
		std::vector<std::vector<DrawableAttribute>> drawableattributes;
		//每个vaoAttribute对应上面的一组drawable;
		std::vector<VaoDrawArrayAttribute> _vaoattributes;
		void RemoveVaoAttribute(Drawable *node, int index);
	};

}