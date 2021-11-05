#include "source/headers.h"

char ALIVE = '#';
char DEAD = '.';

int TERMINAL_HEIGHT = 24;
int TERMINAL_WIDTH = 80;
int SPAWN_RATE = 10;
int SPEED = 100;

struct Move {
  int dx, dy;
};

std::vector<Move> near = {{-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, 
                          {0, -1}, {1, -1}, {1, 0}, {1, 1}};

bool alive(int x, int y, std::string screen[]) {
  if (x < 0 || x >= TERMINAL_HEIGHT)
    return false;
  
  if (y < 0 || y >= TERMINAL_WIDTH)
    return false;

  if (screen[x][y] == ALIVE)
    return true;

  return false;
}

void update_screen(std::string screen[]) {
  std::string new_screen[TERMINAL_HEIGHT];
  for (int x = 0; x < TERMINAL_HEIGHT; x++) {
    new_screen[x] = screen[x];
    for (int y = 0; y < TERMINAL_WIDTH; y++) {
      int alive_neighbours = 0;
      for (auto d : near) {
        if (alive(x + d.dx, y + d.dy, screen))
          alive_neighbours++;

      }

      if (alive_neighbours == 3) {
        new_screen[x][y] = ALIVE;
      } else if (screen[x][y] == ALIVE && alive_neighbours != 2) {
        new_screen[x][y] = DEAD;
      }
    }
  }
  for (int x = 0; x < TERMINAL_HEIGHT; x++) {
    screen[x] = new_screen[x];
  }
}

void move_coursor(char d, int& px, int& py, std::string screen[], bool& alive_under_coursor ) {
  screen[px][py] = ((alive_under_coursor) ? ALIVE : DEAD);
  if (d == 'd') {
    py++;
  } else if (d == 'a') {
    py--;
  } else if (d == 'w') {
    px--;
  } else if (d == 's') {
    px++;
  }
  px = std::max(px, 1);
  py = std::max(py, 0);
  px = std::min(px, TERMINAL_HEIGHT);
  py = std::min(py, TERMINAL_WIDTH);

  alive_under_coursor = (screen[px][py] == ALIVE);
  screen[px][py] = '_';
}

void next_frame(std::string screen[]) {
  for (int x = 0; x <= TERMINAL_HEIGHT; x++) {
    std::cout << "\033[F";
  }
  std::cout << "\033[F";
  for (int x = 0; x < TERMINAL_HEIGHT; x++) {
    std::cout << screen[x];
    if (x != TERMINAL_HEIGHT - 1) {
      std::cout << '\n';
    }
  }
}

void init_screen(std::string screen[]) {
  for (int x = 0; x < TERMINAL_HEIGHT; x++) {
    screen[x] = std::string(TERMINAL_WIDTH, '.');
    for (int y = 0; y < TERMINAL_WIDTH; y++) {
      int r = rand() % 100;
      if (r < SPAWN_RATE) {
        screen[x][y] = '#';
      }
    }
    if (x == TERMINAL_HEIGHT - 1) {
      screen[x].pop_back();
      std::cout << screen[x];
      break;
    }
    std::cout << screen[x] << '\n';
  }
}

void editor(std::string screen[]) {
  int px = 1, py = 0;
  
  std::cout << "\033[F";
  bool alive_under_coursor = (screen[px][py] == ALIVE);
  screen[px][py] = '_';
  next_frame(screen);

  while (true) {
    char c = getch();
    if (c == 'p') {
      screen[px][py] = (alive_under_coursor) ? ALIVE : DEAD;
      next_frame(screen);
      break;
    } else if (c == 'i') {
      alive_under_coursor = true;
    } else if (c == 'o') {
      alive_under_coursor = false;
    } else if (c == 'd' || c == 'a' || c == 'w' || c == 's') {
      move_coursor(c, px, py, screen, alive_under_coursor);
    }

    std::cout << "\033[F";
    next_frame(screen);
  }
}

void welcome() {
  std::cout << "    CONWAY's GAME OF \"LIFE\"\n\n";
  std::cout << "editor controls:\n  w, a, s, d - movement controls\n  i - insert\n  o - delete\n  p - start the game";
  std::cout << "\n\n created by @vitaliyeroshin\n";
}

int main() {
  init (TERMINAL_HEIGHT, TERMINAL_WIDTH);

  welcome();

  ask(SPAWN_RATE, "Enter spawn rate (0-100): ");
  ask(SPEED, "Enter speed (default 100): ");

  std::string screen[TERMINAL_HEIGHT];

  init_screen(screen);
  editor(screen);

  while (true) {
    update_screen(screen);
    next_frame(screen);
    usleep(1000 * SPEED);
  }
}