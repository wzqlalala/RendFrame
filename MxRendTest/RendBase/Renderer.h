#pragma once
#include "qopenglfunctions_4_5_core.h"
#include "NodeVisitor.h"
#include "vao.h"
#include "buffer.h"

namespace mxr
{
	class MXR_EXPORT Renderer
	{
	public:
		typedef std::map<GLuint, VBO*> VboBind;
		typedef std::map<GLuint, VAO*> VaoBind;
		Renderer();
		void accept(NodeVisitor *visitor);



	protected:
		std::vector<std::vector<DrawArraysIndirectCommand>> indirect_command;
		std::vector<VboBind> vbos;
		std::vector<VAO*> vaos;

	};
}