#include <stdbool.h>
#include <stdio.h>
#include "game_core.h"
/* update_square(c, r, width, height) updates the value of the 
square with the number of  squares surrounding it that have mines. */
// requires: c, r, width, height >= 0
// effects: may mutate ([c]olumn, [r]ow)
int update_square(int c, int r, const int width, const int height) {
  map[width * r + c] = mine_at(c, r - 1) + mine_at(c, r + 1) + 
    mine_at(c - 1, r) + mine_at(c + 1, r) + mine_at(c - 1, r - 1) +
    mine_at(c + 1, r - 1) + mine_at(c + 1, r + 1) + 
    mine_at(c - 1, r + 1);
  return map[width * r + c];
}
/* update_square(c, r, width, height) updates the value of the 
square with the number of not marked squares surrounding it that have mines. */
// requires: c, r, width, height >= 0
// effects: may mutate ([c]olumn, [r]ow)
int update_square_not_marked(int c, int r, const int width, const int height) {
  int value = (mine_at(c,r - 1) && map[width * (r - 1) + c] != MARKED) + (mine_at(c,r + 1) && map[width * (r + 1) + c] != MARKED)
    + (mine_at(c - 1,r) && map[width * r  + c - 1] != MARKED) +
    (mine_at(c + 1,r) && map[width * r + c + 1] != MARKED) +
    (mine_at(c - 1,r - 1) && map[width * (r - 1) + c - 1] != MARKED)
    + (mine_at(c + 1,r - 1) && map[width * (r - 1) + c + 1] !=
       MARKED) +  (mine_at(c - 1,r + 1) && map[width * (r + 1) + c -
                                               1] != MARKED) +
    (mine_at(c + 1,r + 1) && map[width *(r + 1) + c + 1] !=
     MARKED);   
  return value;
}
/*
 * step(c, r, width, height) reveals the content of a square with the
 *   coordinates ([c]olumn,[r]ow). The function returns false if the square does
 *   not contain a mine or if ([c],[r]) has been stepped on before.
 * requires : c, r, width, height >= 0
 * effects: sets map at ([c],[r]) accordingly.
 */
bool step(const int c, const int r, const int width, const int height) {
  if (map[width * r + c] != UNKNOWN) {
    return false;
  }
  else {
    if (mine_at(c, r)) {
      map[width * r + c] = MINE;
      return true;
    }
    update_square(c, r, width, height);
  }
  return false;
}
bool in_bounds(int c, int r, const int width, const int height) {
  return  c >= 0 && c < width && r >= 0 && r < height && 
    map[width * r + c] != MARKED &&
    !((map[width * r + c] >= 0 && 
       map[width * r + c] <= 8));
}
/*
 * step_adv(c, r, width, heigth) reveales the content of a square with
 *   the coordinates ([c]olumn,[r]ow). If the revealed square is not next to a
 *   mine, it reveales also all adjacent fields. The function returns false if
 *   the square does not contain a mine or if ([c],[r]) has been stepped on
 *   before.
 * requires: c, r, width, height >= 0 && c < width && r < height
 * effects: sets map at ([c],[r]) accordingly.
 */
bool step_adv(const int c, const int r, const int width, const int height) {
  if (c >= width || r >= height || c < 0 || r < 0) {
    return false;
  }
  if (mine_at(c, r) && map[width * r + c] != MARKED) {
    map[width * r + c] = MINE;
    return true;
  }
  if (map[width * r + c] >= 0 && map[width * r + c] <= 8) {
    return false;
  }
  else {
    if (map[width * r + c] != MARKED) {
      update_square(c, r, width, height);
    }
    if (!(update_square_not_marked(c, r, width, height))) {
      if (in_bounds(c, r + 1, width, height)) {
        step_adv(c, r + 1, width, height);
      }
      if (in_bounds(c - 1, r, width, height)) {
        step_adv(c - 1, r, width, height);
      }
      if (in_bounds(c + 1, r, width, height)) {
        step_adv(c + 1, r, width, height);
      }
      if  (in_bounds(c, r - 1, width, height)) {
        step_adv(c, r - 1, width, height);
      }
      if (in_bounds(c + 1, r + 1, width, height)) {
        step_adv(c + 1, r + 1, width, height);
      }
      if (in_bounds(c + 1, r - 1, width, height)) {
        step_adv(c + 1, r - 1, width, height);
      }
      if (in_bounds(c - 1, r - 1, width, height)) {
        step_adv(c - 1, r - 1, width, height);
      }
      if (in_bounds(c - 1, r + 1, width, height)) {
        step_adv(c - 1, r + 1, width, height);
      }
    }
  }
  return false;
}
/*
 * mark(c, r, width, heigth) marks or unmarks the square with coordinates
 *   ([c]olumn,[r]ow), i.e., it signals weather this square contains a mine or
 *   not. The function returns true if all mines on the board have been marked,
 *   or false otherwise.
 * requires : c, r, width, height >= 0
 * effects: sets map at ([c],[r]) accordingly.
 */
bool mark(const int c, const int r, int width, int height) {
  if (map[width * r + c] >= 0 && map[width * r + c] <= 8) {
    return all_marked();
  }
  if (map[width * r + c] == MARKED) {
    map[width * r + c] = UNKNOWN;
  }
  else {
    map[width * r + c] = MARKED;
  }
  return all_marked();
}

void print(int width, int height) {
  for (int j = 0; j < width * height ; ++j) {
    int square = map[j];
    int row = j / width;
    int column = j % width;
    if (square == UNKNOWN && j % width) {
      printf(" _");
    }
    else if (square == UNKNOWN && !(j % width)) {
      printf("_");
    }
    else if (square == MARKED && j % width) {
      printf(" P");
    }
    else if (square == MARKED && !(j % width)) {
      printf("P");
    }
    else if (square == MINE && j % width) {
      printf(" X");
    }
    else if (square == MINE && !(j % width)) {
      printf("X");   
    }
    else {
      if (j % width) {
        printf(" %d", update_square(column, row, width, height));
      }
      else {
        printf("%d", update_square(column, row, width, height));
      }
    }
    if (!((j + 1) % width)) {
      printf("\n");
    }
  }
}
