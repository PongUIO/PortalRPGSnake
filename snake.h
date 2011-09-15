#ifndef SNAKE_H
#define SNAKE_H

enum {
    POWER_NONE = 0,

    POWER_SLOWSPEED,

    POWER_LAST,
};

class Snake {
        public:
                int hp, juggernaut, defense, offense, maxhp, xp, level, x, y, food, direction, stepcount;
                bool powerups[POWER_LAST];

                Snake () {}
		void init() {
			x = y = food = 2;
                        hp = 10;
			maxhp = 10;
			juggernaut = 0;
			defense = 0;
			offense = 0;
			xp = 0;
                        level = 1;
                        direction = SNAKERIGHT;

                        resetPowers();
                        stepcount = 0;
		}
		void update() {
                        if (hp <= 0) {
                                x = -1;
                                y = -1;
				return;
			}
			if (xp >= getXpToLevel()) {
				xp-= getXpToLevel();
				level++;
				food+=2;
			}

                        stepcount++;
		}
		int getXpToLevel() {
			return level*10;
                }
                void damage(int dmg) {
                     hp -= std::max(0, dmg-defense);
                }
                bool getPowerup(int powerup) {
                    if (powerup <= POWER_NONE || powerup >= POWER_LAST) {
                        return false;
                    }

                    return powerups[powerup];
                }
                void setPowerup(int powerup, bool state) {
                    if (powerup <= POWER_NONE || powerup >= POWER_LAST) {
                        return;
                    }

                    powerups[powerup] = state;
                }
                void togglePowerup(int powerup) {
                    setPowerup(powerup, !getPowerup(powerup) );
                }

                bool shouldSkipMove() {
                    return (stepcount % 2 == 0);
                }
                void forceStep() {
                    stepcount = 0;
                }
                void resetPowers() {
                    for (int i = 0; i < POWER_LAST; i++) {
                        powerups[i] = false;
                    }
                }
};

#endif // SNAKE_H
