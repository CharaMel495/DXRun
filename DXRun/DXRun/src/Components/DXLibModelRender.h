#pragma once
#include "DXLib.h"
#include "CaramelEngine.h"

using namespace CaramelEngine;

class DXLibModelRender : public Component
{
public:

	DXLibModelRender(std::weak_ptr<Actor> owner) :
		Component(owner, "DXLibModelRender"),
		_filePath(""),
		_modelHandle(-1)
	{}

	void initialize() noexcept override;

	void update() noexcept;
	
	void renderModel() noexcept;

	void finalize() noexcept;

	void setFilePath(CString filePath) noexcept { _filePath = filePath; }

private:

	CString _filePath;
	int _modelHandle;
	int _animHandle;
};