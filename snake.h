#ifndef SNAKE_H
#define SNAKE_H

class Snake {
        public:
		int hp, juggernaut, defense, offense, maxhp, xp, level, x, y, food;
		Snake () {
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
			if (xp >= getXpToLevel()) {
				xp-= getXpToLevel();
				level++;
			}
		}
		int getXpToLevel() {
			return level*10;
		}
};

#endif // SNAKE_H
