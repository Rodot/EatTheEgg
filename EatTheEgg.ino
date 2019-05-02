#include <Gamebuino-Meta.h>

// Eggs
const int NUM_OF_EGGS = 10;
int eggs_x[NUM_OF_EGGS] = {};
int eggs_y[NUM_OF_EGGS] = {};

const int EGG_WIDTH = 3;
const int EGG_HEIGHT = 3;

// carrots
const int MAX_NUM_OF_CARROTS = 10;
int num_of_carrots = 0;
int carrots_x[MAX_NUM_OF_CARROTS] = {};
int carrots_y[MAX_NUM_OF_CARROTS] = {};
int carrots_speed[MAX_NUM_OF_CARROTS] = {};

const int CARROT_WIDTH = 6;
const int CARROT_HEIGHT = 6;

// Basket
int basket_width = 10;
int basket_height = 3;
int basket_x = 20;  // Not const because it moves side to side
int basket_y = gb.display.height() - basket_height;  // Const because the basket does not move up and down

int score = 0;
const int MAX_FOOD = 1000;
int food = 0;  // Percentage. The more the better; 0 is dead


void setup() {
  gb.begin();
  // Place the eggs ABOVE the sceen at different heights
  for (int i = 0; i < NUM_OF_EGGS; i++) {
    eggs_x[i] = random(0, gb.display.width());
    eggs_y[i] = i * -10;  // Above the screen -> not drawn
  }

  for (int i = 0; i < MAX_NUM_OF_CARROTS; i++) {
    carrots_x[i] = random(0, gb.display.width());
    carrots_y[i] = - CARROT_HEIGHT;
    carrots_speed[i] = random(2, 4);
  }
}

void loop() {
  while (!gb.update());
  // INPUTS //
  if (gb.buttons.repeat(BUTTON_LEFT, 0) && basket_x > (basket_width / 2)) {
    basket_x -= 3;
  }
  else if (gb.buttons.repeat(BUTTON_RIGHT, 0) && basket_x < gb.display.width() - (basket_width / 2)) {
    basket_x += 3;
  }

  // LOGIC //
  // Food goes down
  food--;
  basket_width = max(2, food / 20);
  basket_height = max(1, food / 60);
  basket_y = gb.display.height() - basket_height;
  if (food <= 0) {  // Are we dead?
    num_of_carrots = 0;  // Reset difficulty
    score = 0;  // Game lost
    food = MAX_FOOD / 2;  // Reset food
    // Place the eggs ABOVE the sceen at different heights
    for (int i = 0; i < NUM_OF_EGGS; i++) {
      eggs_x[i] = random(0, gb.display.width());
      eggs_y[i] = i * -10;  // Above the screen -> not drawn
    }
  }

  // Eggs
  for (int i = 0; i < NUM_OF_EGGS; i++) {
    // Move down
    eggs_y[i]++;

    // Check for collition with the basket
    if (eggs_x[i] + EGG_WIDTH >= basket_x - (basket_width / 2)
        && eggs_x[i] <= basket_x + (basket_width / 2)
        && eggs_y[i] + EGG_HEIGHT >= basket_y
        && eggs_y[i] <= basket_y + basket_height) {
      score++;
      food = min(food + 20, MAX_FOOD);  // Yummy eggs
      gb.sound.playTick();
      gb.lights.fill(BLACK);

      // Add more carrots with score. Don't have more than the maximum
      num_of_carrots = min(score / 25, MAX_NUM_OF_CARROTS);

      // Reset the egg
      eggs_x[i] = random(0, gb.display.width());
      eggs_y[i] = 0;
    }

    // Check if the egg didn't go below the screen
    if (eggs_y[i] >= gb.display.height()) {
      // Reset the egg
      eggs_x[i] = random(0, gb.display.width());
      eggs_y[i] = 0;
    }
  }

  // Carrots
  for (int i = 0; i < num_of_carrots; i++) {
    // Move down
    carrots_y[i] += carrots_speed[i];

    // Check for collition with the basket
    if (carrots_x[i] + CARROT_WIDTH >= basket_x - (basket_width / 2)
        && carrots_x[i] <= basket_x + (basket_width / 2)
        && carrots_y[i] + CARROT_HEIGHT >= basket_y
        && carrots_y[i] <= basket_y + basket_height) {

      food -= 40;  // Lose food :(
      gb.sound.playCancel();
      gb.lights.fill(RED);

      // Reset the carrot
      carrots_x[i] = random(0, gb.display.width());
      carrots_y[i] = 0;
      carrots_speed[i] = random(2, 4);
    }

    // Check if the carrot didn't go below the screen
    if (carrots_y[i] >= gb.display.height()) {
      // Reset the carrot
      carrots_x[i] = random(0, gb.display.width());
      carrots_y[i] = 0;
      carrots_speed[i] = random(2, 4);
    }
  }

  // DRAW //
  gb.display.clear();

  // Eggs
  for (int i = 0; i < NUM_OF_EGGS; i++) {
    gb.display.setColor(BROWN);
    gb.display.fillRect(eggs_x[i], eggs_y[i], EGG_WIDTH, EGG_HEIGHT);
    gb.display.setColor(RED);
    gb.display.fillRect(eggs_x[i] + 1, eggs_y[i] - 1, 1, EGG_HEIGHT + 2);
  }

  // Carrots
  for (int i = 0; i < num_of_carrots; i++) {
    gb.display.setColor(GREEN);
    gb.display.fillRoundRect(carrots_x[i], carrots_y[i], CARROT_WIDTH, CARROT_HEIGHT, 3);
    gb.display.setColor(LIGHTGREEN);
    gb.display.fillRect(carrots_x[i] + 1, carrots_y[i] + 1, CARROT_WIDTH - 3, CARROT_HEIGHT - 3);
  }

  // Player
  gb.display.setColor(BEIGE);
  gb.display.fillRoundRect(basket_x - (basket_width / 2), basket_y, basket_width , basket_height, basket_height / 2);
  gb.display.setColor(BROWN);
  gb.display.setCursor(basket_x - 5, basket_y + 2);
  gb.display.print("x_x");

  // Score
  gb.display.setColor(WHITE);
  gb.display.setCursor(0, 0);
  gb.display.print(score);

  if (score == 10) {
    gb.gui.popup("YUMMI HOT DOGS!", 25);
  }
  if (score == 20) {
    gb.gui.popup("I WANT MORE!", 25);
  }
  if (score == 40) {
    gb.gui.popup("NO, NO SALADS!", 25);
  }
  if (score == 60) {
    gb.gui.popup("MOOOORE!", 25);
  }
}
