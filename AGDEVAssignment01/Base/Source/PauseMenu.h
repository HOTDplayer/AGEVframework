#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "FPSCamera.h"
#include "SpriteEntity.h"

class SceneManager;
//class TextEntity;
class PauseMenu :public Scene
{
public:
	PauseMenu();
	~PauseMenu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	bool ispaused() { return pause; };
private:
	FPSCamera camera;
	SpriteEntity* IntroStateBackground;
	bool canpause = false;
	bool pause = false;
	//TextEntity* textObj[3];
};