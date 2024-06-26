#pragma once
#include "Zombie.h"

class FootballZombie : public Zombie {
private:
	Clock reverseClock;
	float reverseDelay;
	string direction = "left";

public:
	FootballZombie(Texture& tex, int columns, float pos[2], TextureManager* TM, SoundManager* SM) {
		this->SMptr = SM;
		this->type = "football";
		this->TMptr = TM;
		this->sprite.setTexture(tex);
		this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
		this->speed = 0.125;
		this->health = 180;
		this->exists = true;
		this->limit = 40;
		this->position[0] = pos[0], this->position[1] = pos[1];
		this->anim = Animation(166, 144, columns);
		this->moveClock.restart(), this->reverseClock.restart();
		this->anim.setDelay(80);
		this->reverseDelay = 20 + rand() % 10;
		this->xFactor += 30;
		// head
		this->headSprite.setTexture(this->TMptr->getTexture("spritesheet-head"));
		this->headSprite.setTextureRect(IntRect(0, 0, 166, 144));
		this->headAnim = Animation(150, 186, 12);
	}

	void loadBuffer() {
		int d = rand() % 11;
		switch (d) {
		case 1:
		case 2:
			this->growlBuffer.loadFromFile("assets/sounds/zombie/mczombie.mp3");
			break;
		case 3:
		case 4:
		case 5:
		case 6:
			this->growlBuffer.loadFromFile("assets/sounds/zombie/scream.mp3");
			break;
		case 7:
		case 8:
			this->growlBuffer.loadFromFile("assets/sounds/zombie/lowgroan.ogg");
			break;
		case 9:
		case 10:
			this->growlBuffer.loadFromFile("assets/sounds/zombie/lowgroan2.ogg");
			break;
		}
		this->growlSound.setBuffer(this->growlBuffer);

	}

	void saveEverything(ofstream& file) {
		file.write(reinterpret_cast<char*>(&position[0]), sizeof(float));
		file.write(reinterpret_cast<char*>(&position[1]), sizeof(float));
		file.write(reinterpret_cast<char*>(&health), sizeof(int));
		file.write(reinterpret_cast<char*>(&exists), sizeof(bool));



		this->anim.saveEverything(file);

		this->headAnim.saveEverything(file);

		file.write(reinterpret_cast<char*>(&speed), sizeof(float));
		file.write(reinterpret_cast<char*>(&moveDelay), sizeof(float));
		file.write(reinterpret_cast<char*>(&limit), sizeof(float));

		file.write(reinterpret_cast<char*>(&state), sizeof(string));

		file.write(reinterpret_cast<char*>(&eatIndex), sizeof(int));

		file.write(reinterpret_cast<char*>(&flicker), sizeof(bool));
		file.write(reinterpret_cast<char*>(&ashes), sizeof(bool));
		file.write(reinterpret_cast<char*>(&dead), sizeof(bool));
		file.write(reinterpret_cast<char*>(&blocked), sizeof(bool));
		file.write(reinterpret_cast<char*>(&headFall), sizeof(bool));
		file.write(reinterpret_cast<char*>(&headOnceFell), sizeof(bool));

		// own
		file.write(reinterpret_cast<char*>(&reverseDelay), sizeof(float));
		file.write(reinterpret_cast<char*>(&direction), sizeof(string));


	}

	void readEverything(ifstream& file) {
		file.read(reinterpret_cast<char*>(&position[0]), sizeof(float));
		file.read(reinterpret_cast<char*>(&position[1]), sizeof(float));
		this->sprite.setPosition(this->xFactor + this->position[0] * 80, this->yFactor + this->position[1] * 96);
		this->headSprite.setPosition(this->xFactor + 50 + this->position[0] * 80, this->yFactor - 10 + this->position[1] * 96);

		file.read(reinterpret_cast<char*>(&health), sizeof(int));



		file.read(reinterpret_cast<char*>(&exists), sizeof(bool));
		this->anim.readEverything(file);

		this->headAnim.readEverything(file);

		file.read(reinterpret_cast<char*>(&speed), sizeof(float));
		file.read(reinterpret_cast<char*>(&moveDelay), sizeof(float));
		file.read(reinterpret_cast<char*>(&limit), sizeof(float));

		file.read(reinterpret_cast<char*>(&state), sizeof(string));

		file.read(reinterpret_cast<char*>(&eatIndex), sizeof(int));

		file.read(reinterpret_cast<char*>(&flicker), sizeof(bool));
		file.read(reinterpret_cast<char*>(&ashes), sizeof(bool));
		file.read(reinterpret_cast<char*>(&dead), sizeof(bool));
		file.read(reinterpret_cast<char*>(&blocked), sizeof(bool));
		file.read(reinterpret_cast<char*>(&headFall), sizeof(bool));
		file.read(reinterpret_cast<char*>(&headOnceFell), sizeof(bool));

		// own
		file.read(reinterpret_cast<char*>(&reverseDelay), sizeof(float));
		file.read(reinterpret_cast<char*>(&direction), sizeof(string));

		if (this->direction == "left") {
			if (state == "walk") {
				if (this->health > 120) {
					this->sprite = Sprite(this->TMptr->getTexture("football-walk"));
				}
				else if (this->health > this->limit && this->health <= 120) {
					this->sprite = Sprite(this->TMptr->getTexture("football-walk-2"));
				}
				else {
					this->sprite = Sprite(this->TMptr->getTexture("football-walk-3"));
				}
			}
			else {
				if (this->health > 120) {
					this->sprite = Sprite(this->TMptr->getTexture("football-eat"));
				}
				else if (this->health > this->limit && this->health <= 120) {
					this->sprite = Sprite(this->TMptr->getTexture("football-eat-2"));
				}
				else {
					this->sprite = Sprite(this->TMptr->getTexture("football-eat-3"));
				}
			}
			this->sprite.setTextureRect(IntRect(this->anim.getFrame() * 166, 0, 166, 144));
		}
		else {
			if (this->state == "walk") {
				if (this->health > 120) {
					this->sprite = Sprite(this->TMptr->getTexture("football-walk-right"));
				}
				else if (this->health > this->limit && this->health <= 120) {
					this->sprite = Sprite(this->TMptr->getTexture("football-walk-right-2"));
				}
				else {
					this->sprite = Sprite(this->TMptr->getTexture("football-walk-right-3"));
				}
			}
			else {
				if (this->health > 120) {
					this->sprite = Sprite(this->TMptr->getTexture("football-eat-right"));
				}
				else if (this->health > this->limit && this->health <= 120) {
					this->sprite = Sprite(this->TMptr->getTexture("football-eat-right-2"));
				}
				else {
					this->sprite = Sprite(this->TMptr->getTexture("football-eat-right-3"));
				}
			}
			this->sprite.setTextureRect(IntRect(this->anim.getFrame() * 166, 0, 166, 144));
		}


		if (this->dead) {
			this->sprite = Sprite(this->TMptr->getTexture("football-die"));
			this->sprite.setTextureRect(IntRect(this->anim.getFrame() * 166, 0, 166, 144));
		}
		if (this->ashes) {
			if (this->direction == "left") {
				this->sprite = Sprite(this->TMptr->getTexture("spritesheet-zombieAshes"));
			}
			else {
				this->sprite = Sprite(this->TMptr->getTexture("spritesheet-zombieAshesRight"));
			}
			this->sprite.setTextureRect(IntRect(this->anim.getFrame() * 166, 0, 166, 144));
		}

	}


	void move(Plant** plants, int plantsArrayIndex) {
		if (this->exists == false) return;
		if (this->moveClock.getElapsedTime().asMilliseconds() < this->moveDelay) return;

		int plantsInRow = 0;
		for (int i = 0; i < plantsArrayIndex; i++) {
			if (plants[i]->getExist()) {
				if (plants[i]->getPosition()[1] == this->position[1] && this->position[0] <= plants[i]->getPosition()[0]) {
					plantsInRow++;
				}
			}
		}

		handleFlicker();
		reverseDirection(plantsInRow);

		if (this->blocked) {
			if (this->eatIndex != -1) {
				//this->state = "eat";
				eat(plants[this->eatIndex]);
			}
			return;
		}

		this->moveClock.restart();
		this->position[0] -= this->speed;
		this->sprite.setPosition(this->xFactor + this->position[0] * 80, this->yFactor + this->position[1] * 96);



		for (int i = 0; i < plantsArrayIndex; i++) {
			if (plants[i]->getExist()) {
				float* plantPos = plants[i]->getPosition();

				if (plantPos[1] == this->position[1]) {
					float dt = plantPos[0] - this->position[0];
					if (this->direction == "left") {
						if (dt <= 0 && dt >= -0.6875) {
							this->blocked = true;
							this->state = "eat";
							if (this->health > 120) {
								this->changeTexture(this->TMptr->getTexture("football-eat"), 0, 10);
							}
							else if (this->health > this->limit && this->health <= 120) {
								this->changeTexture(this->TMptr->getTexture("football-eat-2"), 0, 11);
							}
							else {
								this->changeTexture(this->TMptr->getTexture("football-eat-3"), 0, 10);
							}
							this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
							this->eatIndex = i;
							return;
						}
					}
					else {
						if (dt >= 0 && dt <= 0.6875) {
							this->blocked = true;
							this->state = "eat";
							if (this->health > 120) {
								this->changeTexture(this->TMptr->getTexture("football-eat-right"), 0, 10);
							}
							else if (this->health > this->limit && this->health <= 120) {
								this->changeTexture(this->TMptr->getTexture("football-eat-right-2"), 0, 11);
							}
							else {
								this->changeTexture(this->TMptr->getTexture("football-eat-right-3"), 0, 10);
							}
							this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
							this->eatIndex = i;
							return;
						}
					}
				}
			}
		}
	}

	void eat(Plant* plant) {
		float* plantPos = plant->getPosition();
		float dt = this->position[0] - plantPos[0];
		bool keepEating = false;
		if (this->direction == "left") {
			if (dt >= 0 && dt <= 0.6875) { // plant in range
				keepEating = true;
			}
		}
		else {
			if (dt >= -0.6875 && dt <= 0) { // plant in range
				keepEating = true;
			}
		}
		if (plant->getExist() && keepEating) {
			if (plant->getEatClock().getElapsedTime().asMilliseconds() > 500) {
				plant->beEaten();
				this->SMptr->getSound("eating")->play();
				plant->getEatClock().restart();
			}
		}
		else {
			this->SMptr->getSound("eating")->stop();
			this->blocked = false;
			this->state = "walk";

			if (this->direction == "left") {
				if (this->health > 120) {
					this->changeTexture(this->TMptr->getTexture("football-walk"), 0, 11);
				}
				else if (this->health > this->limit && this->health <= 120) {
					this->changeTexture(this->TMptr->getTexture("football-walk-2"), 0, 11);
				}
				else {
					this->changeTexture(this->TMptr->getTexture("football-walk-3"), 0, 10);
				}
			}
			else {
				if (this->health > 120) {
					this->changeTexture(this->TMptr->getTexture("football-walk-right"), 0, 11);
				}
				else if (this->health > this->limit && this->health <= 120) {
					this->changeTexture(this->TMptr->getTexture("football-walk-right-2"), 0, 11);
				}
				else {
					this->changeTexture(this->TMptr->getTexture("football-walk-right-3"), 0, 10);
				}
			}

			this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
			this->eatIndex = -1;
		}
	}

	void makeDead() {
		if (this->exists == false && this->ashes != true) {
			this->changeTexture(this->TMptr->getTexture("football-die"), 0, 7);
			this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
			this->dead = true, this->deadClock.restart();
			this->SMptr->playSound("zombie-fall");
		}
	}

	virtual void setAshes(bool value) {
		if (this->direction == "left") {
			this->changeTexture(this->TMptr->getTexture("spritesheet-zombieAshes"), 0, 20);
		}
		else {
			this->changeTexture(this->TMptr->getTexture("spritesheet-zombieAshesRight"), 0, 20);
		}
		this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
		this->ashes = value, this->ashesClock.restart();
	}

	void handleDeath(RenderWindow& window) {
		if (!this->exists && this->dead) {
			if (this->deadClock.getElapsedTime().asSeconds() > 0.705 || this->anim.getFrame() == this->anim.getColumns() - 1) {
				this->dead = false;
			}
			this->sprite.setPosition(this->xFactor + this->position[0] * 80, this->yFactor + this->position[1] * 96);
			window.draw(this->sprite);
		}
	}

	void checkHealth() {
		if (this->exists) {
			if (this->direction == "left") {
				if (this->health == 120) {
					if (this->state == "walk") {
						changeTexture(this->TMptr->getTexture("football-walk-2"), 0, 11);
					}
					else {
						changeTexture(this->TMptr->getTexture("football-eat-2"), 0, 11);
					}
					this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
				}
				else if (this->health == this->limit) {
					if (this->state == "walk") {
						changeTexture(this->TMptr->getTexture("football-walk-3"), 0, 10);
					}
					else {
						changeTexture(this->TMptr->getTexture("football-eat-3"), 0, 10);
					}
					setHeadFall(true);
					this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
				}
				else if (this->health == 0) {
					this->exists = false;
					this->makeDead();
				}
			}
			else {
				if (this->health == 120) {
					if (this->state == "walk") {
						changeTexture(this->TMptr->getTexture("football-walk-right-2"), 0, 11);
					}
					else {
						changeTexture(this->TMptr->getTexture("football-eat-right-2"), 0, 11);
					}
					this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
				}
				else if (this->health == this->limit) {
					if (this->state == "walk") {
						changeTexture(this->TMptr->getTexture("football-walk-right-3"), 0, 10);
					}
					else {
						changeTexture(this->TMptr->getTexture("football-eat-right-3"), 0, 10);
					}
					setHeadFall(true);
					this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
				}
				else if (this->health == 0) {
					this->SMptr->getSound("eating")->stop();
					this->exists = false;
					this->makeDead();
				}
			}
		}
	}

	void reverseDirection(int plantsInRow) {
		if (!this->exists) return;
		if (this->blocked) {
			this->reverseClock.restart();
			return;
		}
		if (plantsInRow == 0 && this->direction == "left") {
			return;
		}

		if (this->reverseClock.getElapsedTime().asSeconds() > this->reverseDelay) {
			this->reverseClock.restart();
			this->speed *= -1;

			if (this->direction == "left") {
				this->direction = "right";
				this->reverseDelay = 5 + rand() % 2;

				if (this->health > 120) {
					changeTexture(this->TMptr->getTexture("football-walk-right"), 0, 11);
				}
				else if (this->health > this->limit && this->health <= 120) {
					changeTexture(this->TMptr->getTexture("football-walk-right-2"), 0, 11);
				}
				else {
					changeTexture(this->TMptr->getTexture("football-walk-right-3"), 0, 10);
				}

			}
			else {
				this->direction = "left";
				this->reverseDelay = 12 + rand() % 5;

				if (this->health > 120) {
					changeTexture(this->TMptr->getTexture("football-walk"), 0, 11);
				}
				else if (this->health > this->limit && this->health <= 120) {
					changeTexture(this->TMptr->getTexture("football-walk-2"), 0, 11);
				}
				else {
					changeTexture(this->TMptr->getTexture("football-walk-3"), 0, 10);
				}

			}
			this->sprite.setTextureRect(IntRect(0, 0, 166, 144));
		}
	}
};