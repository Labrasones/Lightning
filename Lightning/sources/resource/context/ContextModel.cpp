#include "resource/context/ContextModel.hpp"

using namespace Resource;
using namespace Context;

ContextModel::ContextModel(Manager::ContextResourceManager* container) : BaseContextResource(container)
{}

ContextModel::~ContextModel()
{}

bool ContextModel::load(Manager::ResourceManager* resourceContainer, std::string path)
{
	return true;
}