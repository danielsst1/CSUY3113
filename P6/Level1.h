#include "Scene.h"


class Level1 : public Scene {
public:
	enum gameMode {PLAY, WIN, LOSE};

	void Initialize() override;
	void Update(float deltaTime) override;
	void playJumpSound() override;
	void Render(ShaderProgram* program) override;
	void showText(ShaderProgram* program, float xVal);

	int setLives(int num);
	int getLives();
	int loseLife();

	int setHumansFound(int num);
	int getHumansFound();
	int findHuman();

	void stopMotion();
	int getLevelWidth();
	void loseGame();

	int getRemainingTime();
};