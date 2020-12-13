#include "Scene.h"

class Level2 : public Scene {

public:
	enum gameMode { PLAY, WIN, LOSE };

	void Initialize() override;
	void Update(float deltaTime) override;
	void playJumpSound() override;
	void Render(ShaderProgram* program) override;

	int setLives(int num);
	int getLives();
	int loseLife();

	int setHumansFound(int num);
	int getHumansFound();
	int findHuman();

	void stopMotion();
	void loseGame();
};