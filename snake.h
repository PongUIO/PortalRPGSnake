#ifndef SNAKE_H
#define SNAKE_H

class Snake {
        public:
		int hp, juggernaut, defense, offense, maxhp, xp, level, x, y, food;
		Snake () {
		}
		void init() {
			x = y = food = 2;
			hp = 10;
			maxhp = 10;
			juggernaut = 0;
			defense = 0;
			offense = 0;
			xp = 0;
			level = 1;
		}
		void update() {
			if (hp < 0) {
				x = -1;
				return;
			}
			if (hp < maxhp) {
				hp++;
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
};

#endif // SNAKE_H
