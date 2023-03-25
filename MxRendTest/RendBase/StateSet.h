#pragma once
#include "StateAttribute.h"
#include "asset.h"
#include "shader.h"
#include "texture.h"
#include "RenderPass.h"
namespace mxr
{
	class Node;
	class MXR_EXPORT StateSet :public StateAttribute
	{
	public:
		typedef std::vector<Node*> ParentList;
		typedef std::map<StateAttribute::Type, int> ModeList;
		typedef std::map<StateAttribute::Type, asset_ref<StateAttribute>> AttributeList;


		StateSet();
		virtual ~StateSet();

		void setAttributeAndModes(asset_ref<StateAttribute> attribute, int state);
		void removeAttribute(asset_ref<StateAttribute> attribute);
		int getModes(StateAttribute::Type _mode);
		void removeMode(StateAttribute::Type _mode);


		void setUniform(asset_ref<Uniform> uniform, bool enable = true);
		Uniform* getUniform(std::string name);

		void removeUniform() {};
		void setTexture(std::string _name, asset_ref<Texture> texture);
		void setShader(asset_ref<Shader> shader) { _shader = shader; };
		void setRenderPass(asset_ref<RenderPass> renderpass) { _renderpass = renderpass; }
		RenderPass* getRenderPass() { return _renderpass.get(); }

		void setDrawMode(GLenum mode) { _drawmode = mode; }
		GLenum getDrawMode() { return _drawmode; }

		void Bind();
		void UnBind();

	protected:
		StateSet& operator = (const StateSet&) { return *this; }
		void addParent(Node* object);
		void removeParent(Node* object);
		GLenum _drawmode;
		ParentList _parents;
		ModeList	_modelist;
		AttributeList  _attributeList;
		asset_ref<Shader> _shader;
		std::map<std::string, asset_ref<Uniform>> _uniformvalues;

		std::map<std::string, asset_ref<Texture>> _textures;
		asset_ref<RenderPass> _renderpass;
	};
}







