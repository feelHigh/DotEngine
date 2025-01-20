#pragma once
#include "DEditor.h"

class FlipbookEditor
	: public DEditor
{
public:
	FlipbookEditor();
	~FlipbookEditor();

public:
	virtual void Init() override;
	virtual void Update() override;

private:

};
