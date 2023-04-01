#include "StateSet.h"
#include <algorithm>
#include <iostream>
namespace mxr
{
	StateSet::StateSet()
	{
		_drawmode = GL_TRIANGLES;
	}


	void StateSet::setAttributeAndModes(asset_ref<StateAttribute> attribute, int state)
	{
		_modelist[attribute->getType()] = state;
		_attributeList[attribute->getType()] = attribute;
	}

	void StateSet::removeAttribute(asset_ref<StateAttribute> attribute)
	{
		_modelist.erase(attribute->getType());
		_attributeList.erase(attribute->getType());
	}

	int StateSet::getModes(StateAttribute::Type _mode)
	{
		if (_modelist.find(_mode) == _modelist.end())
		{
			return 0;
		}
		return _modelist[_mode];
	}

	void StateSet::removeMode(StateAttribute::Type _mode)
	{
		_modelist.erase(_mode);
		_attributeList.erase(_mode);

	}

	void StateSet::setUniform(asset_ref<Uniform> uniform, bool enable)
	{
		_uniformvalues[uniform->getName()] = uniform;
		uniform->SetEnable(enable);
	}

	Uniform*  StateSet::getUniform(std::string name)
	{
		return _uniformvalues[name].get();
	}

	void StateSet::setTexture(std::string _name, asset_ref<Texture> texture)
	{
		_textures[_name] = texture;
	}


	void StateSet::Bind()
	{
		for (auto &item : _modelist)
		{
			if (item.first == StateAttribute::DEPTH)
			{
				if (item.second)
				{
					glEnable(GL_DEPTH_TEST);
					_attributeList[item.first]->apply();
				}
				else
				{
					glDisable(GL_DEPTH_TEST);
				}
			}
			else if (item.first == StateAttribute::BLENDFUNC)
			{
				if (item.second)
				{
					glEnable(GL_BLEND);
					_attributeList[item.first]->apply();
				}
				else
				{
					glDisable(GL_BLEND);
				}
			}
			else
			{
				_attributeList[item.first]->setEnable(item.second);
				_attributeList[item.first]->apply();
			}
		}

		if (_shader)
		{
			_shader->Bind();
			for (auto &item : _uniformvalues)
			{
				UniformCallback* uc = item.second->getUpdateCallback();
				if (uc)
				{
					uc->operator()(nullptr, nullptr);
				}
				if (item.second->getEnable())
				{
					item.second->SetUniform(_shader);
				}
			}
			int bind_id = 0;
			for (auto &item : _textures)
			{
				_shader->SetUniform(item.first.c_str(), bind_id);
				item.second->Bind(bind_id);
				bind_id++;
			}

		}
	}

	void StateSet::UnBind()
	{

		int bind_id = 0;
		for (auto &item : _textures)
		{
			_shader->SetUniform(item.first.c_str(), bind_id);
			item.second->Unbind(bind_id);
			bind_id++;
		}
		_shader->Unbind();
	}

	StateSet::~StateSet()
	{
	}

	void StateSet::addParent(Node * object)
	{
		_parents.push_back(object);
	}

	void StateSet::removeParent(Node * object)
	{
		_parents.erase(std::find(_parents.begin(), _parents.end(), object));
	}
}