#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "test.h"
#include "prim.h"

void perform_position_testing();
void perform_1d_testing();
void perform_2d_testing();
void perform_3d_testing();
void perform_4d_testing();

void perform_testing() {
  perform_position_testing();
  perform_1d_testing();
  perform_2d_testing();
  perform_3d_testing();
  perform_4d_testing();
  printf("All tests passed successfully!\n");
}

void perform_position_testing() {
  approx(dist1(.2, .8), .6);
  approx(dist2(.2, .3, .9, .7), sqrt(pow(.7, 2) + pow(.4, 2)));
  approx(dist3(.1, .2, .6, .3, .5, .8), sqrt(pow(.2, 2) + pow(.3, 2) + pow(.2, 2)));
  approx(dist4(.1, .2, .6, .2, .3, .5, .8, .1), sqrt(pow(.2, 2) + pow(.3, 2) + pow(.2, 2) + pow(.1, 2)));
}

void perform_1d_testing() {

  position *positions;

  positions = (position *)malloc(sizeof(position) * 2);
  positions[0] = new_position(0.0, 0, 0, 0);
  positions[1] = new_position(1.0, 0, 0, 0);
  approx(perform_trial(2, 1, positions), 1.0);

  positions = (position *)malloc(sizeof(position) * 2);
  positions[0] = new_position(0.25, 0, 0, 0);
  positions[1] = new_position(0.85, 0, 0, 0);
  approx(perform_trial(2, 1, positions), 0.6);

  positions = (position *)malloc(sizeof(position) * 3);
  positions[0] = new_position(0.25, 0, 0, 0);
  positions[1] = new_position(0.85, 0, 0, 0);
  positions[2] = new_position(0.95, 0, 0, 0);
  approx(perform_trial(3, 1, positions), 0.7);

}

void perform_2d_testing() {

  position *positions;

  positions = (position *)malloc(sizeof(position) * 2);
  positions[0] = new_position(0.0, 0.0, 0, 0);
  positions[1] = new_position(1.0, 1.0, 0, 0);
  approx(perform_trial(2, 2, positions), sqrt(2.0));

  positions = (position *)malloc(sizeof(position) * 3);
  positions[0] = new_position(0.0, 0.0, 0, 0);
  positions[1] = new_position(1.0, 1.0, 0, 0);  
  positions[2] = new_position(1.0, 0.0, 0, 0);
  approx(perform_trial(3, 2, positions), 2.0);

  positions = (position *)malloc(sizeof(position) * 4);
  positions[0] = new_position(0.0, 0.0, 0, 0);
  positions[1] = new_position(1.0, 1.0, 0, 0);  
  positions[2] = new_position(1.0, 0.0, 0, 0);
  positions[3] = new_position(0.0, 1.0, 0, 0);  
  approx(perform_trial(4, 2, positions), 3.0);

  positions = (position *)malloc(sizeof(position) * 4);
  positions[0] = new_position(0.0, 0.0, 0, 0);
  positions[1] = new_position(0.5, 1.0, 0, 0);
  positions[2] = new_position(0.5, 0.0, 0, 0);
  positions[3] = new_position(1.0, 0.5, 0, 0);  
  approx(perform_trial(4, 2, positions), .5 + 2 * sqrt(2.0) * .5);

}

void perform_3d_testing() {

  position *positions;

  positions = (position *)malloc(sizeof(position) * 2);
  positions[0] = new_position(0.0, 0.0, 0.0, 0);
  positions[1] = new_position(1.0, 1.0, 1.0, 0);
  approx(perform_trial(2, 3, positions), sqrt(pow(sqrt(2.0),2.0)+1.0));

  positions = (position *)malloc(sizeof(position) * 4);
  positions[0] = new_position(0.0, 0.0, 0.0, 0);
  positions[1] = new_position(0.5, 0.5, 0.0, 0);
  positions[2] = new_position(1.0, 1.0, 0.2, 0);
  positions[3] = new_position(0.5, 1.0, 1.0, 0);
  approx(perform_trial(4, 3, positions), dist2(0.0,0.0,0.5,0.5)+dist3(0.5,0.5,0.0,1.0,1.0,0.2)
                                        +dist3(1.0,1.0,0.2,0.5,1.0,1.0));

}

void perform_4d_testing() {

  position *positions;

  positions = (position *)malloc(sizeof(position) * 4);
  positions[0] = new_position(0.0, 0.0, 0.0, 0);
  positions[1] = new_position(0.5, 0.5, 0.0, 0);
  positions[2] = new_position(1.0, 1.0, 0.2, 0);
  positions[3] = new_position(0.5, 1.0, 1.0, 1.0);
  approx(perform_trial(4, 4, positions), dist2(0.0,0.0,0.5,0.5)+dist3(0.5,0.5,0.0,1.0,1.0,0.2)
                                        +dist4(1.0,1.0,0.2,0.0,0.5,1.0,1.0,1.0));

}

void approx(double a, double b) {
  // printf("%f=%f? (%d)\n", a, b, abs(a - b));
  assert(fabs(a - b) < .001);
}
