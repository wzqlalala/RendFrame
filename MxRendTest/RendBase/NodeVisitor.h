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

		//������ͬ�����ʽ��drawable�ŵ�һ��
		typedef std::vector< std::vector<asset_ref<Drawable> > > DrawableList;
		NodeVisitor() {}
		//��֯node���ݣ�����ͬ���ݸ�ʽ��node����һ��
		void accept(Drawable* node);
		//����vao,vbo,drawbuffer��
		void compile();
		void clear();//���ԭʼ��Ⱦ����
		void run();
		void RemoveDrawableAttribute(Drawable *node);
		void RemoveAllData();
		
	protected:
		//������ͬ�������ݸ�ʽ��drawableΪһ��
		std::vector<std::vector<DrawableAttribute>> drawableattributes;
		//ÿ��vaoAttribute��Ӧ�����һ��drawable;
		std::vector<VaoDrawArrayAttribute> _vaoattributes;
		void RemoveVaoAttribute(Drawable *node, int index);
	};

}