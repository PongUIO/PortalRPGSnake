#ifndef SNAKE_H
#define SNAKE_H

typedef enum {
    POWER_NONE = 0,

    POWER_HIGHSPEED,

    POWER_LAST,
} Powerup;

class Snake {
        public:
                int hp, juggernaut, defense, offense, maxhp, xp, level, x, y, food, direction;
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
		}
		int getXpToLevel() {
			return level*10;
                }
                void damage(int dmg) {
                     hp -= std::max(0, dmg-defense);
                }
};

#endif // SNAKE_H
