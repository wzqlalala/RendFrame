#pragma once
#include "Node.h"
#include "Array.h"
#include "vao.h"

namespace mxr
{
	typedef MXR_EXPORT struct
	{
		GLuint vertexCount;
		GLuint instanceCount;
		GLuint firstVertex;
		GLuint baseInstance;
	} DrawArraysIndirectCommand;


	typedef MXR_EXPORT struct 
	{
		GLuint indexCount;	
		GLuint instanceCount;
		GLuint firstIndex;
		GLuint baseVertex;
		GLuint baseInstance;
		
	}DrawElementsIndirectCommand;

	class NodeVisitor;
	class MXR_EXPORT Drawable : public Node
	{
	public:
		typedef std::vector<asset_ref<Array>>  ArrayList;
		Drawable();
		virtual ~Drawable();
		virtual Drawable* asDrawable() { return this; }
		virtual const Drawable* asDrawable() const { return this; }
		bool empty() ;


		void setVertexAttribArray(unsigned int index, asset_ref<Array> array);
		Array *getVertexAttribArray(unsigned int index);

		void setIndexAttribute(asset_ref<UIntArray> _index) { _indexAttribute = _index; }
		UIntArray *getIndexAttribute() {return _indexAttribute.get(); }

		unsigned int getNumVertexAttribArrays() const { return static_cast<unsigned int>(_vertexAttribList.size()); }

		void setVertexAttribArrayList(const ArrayList& arrayList);
		ArrayList& getVertexAttribArrayList() { return _vertexAttribList; }
		const ArrayList& getVertexAttribArrayList() const { return _vertexAttribList; }

		virtual void accept(asset_ref<NodeVisitor> nv);
		bool CompareForm(Drawable *other);
		void SetVAO(asset_ref<VAO> vao) { _vao = vao; }
		void SetDrawArraysIndirect(DrawArraysIndirectCommand command) { _command = command; }
		DrawArraysIndirectCommand& getDrawArraysIndirect() { return _command; }
		DrawElementsIndirectCommand& getDrawElemnetIndirect() { return _elemnetcommand; }

		void setStateSet(asset_ref<StateSet> stateset) override;

		GLuint getVertexCount() 
		{ 
			if (_vertexAttribList.size() == 0)
			{
				return 0;
			}
			return _vertexAttribList[0]->size()/ _vertexAttribList[0]->getDataSize(); 
		}
		
		void Draw();
	protected:
		ArrayList  _vertexAttribList;
		asset_ref<UIntArray> _indexAttribute;
		int _arrayListSize;
		asset_ref<VAO> _vao;
		asset_ref<NodeVisitor> _nodevisitor;
		DrawArraysIndirectCommand _command;
		DrawElementsIndirectCommand _elemnetcommand;
		bool isShow;
	};

}