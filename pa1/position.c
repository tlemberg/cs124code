#include <stdlib.h>
#include <math.h>

#include "position.h"

position new_position(double x, double y, double z, double w) {
  position r;
  r.x = x;
  r.y = y;
  r.z = z;
  r.w = w;
  return r;
}

double dist1(double x1, double x2) {
  return dist(new_position(x1, 0.0, 0.0, 0.0), new_position(x2, 0.0, 0.0, 0.0), 1);
}

double dist2(double x1, double y1, double x2, double y2) {
  return dist(new_position(x1, y1, 0.0, 0.0), new_position(x2, y2, 0.0, 0.0), 2);
}

double dist3(double x1, double y1, double z1, double x2, double y2, double z2) {
  return dist(new_position(x1, y1, z1, 0.0), new_position(x2, y2, z2, 0.0), 3);
}

double dist4(double x1, double y1, double z1, double w1, double x2, double y2, double z2, double w2) {
  return dist(new_position(x1, y1, z1, w1), new_position(x2, y2, z2, w2), 4);
}

double dist(position a, position b, int dimension) {
  double parameter;
  switch(dimension) {
    case 1:
      parameter = pow(a.x - b.x, 2.0);
      break;
    case 2:
      parameter = pow(a.x - b.x, 2.0) + pow((a.y - b.y), 2.0);
      break;
    case 3:
      parameter = pow(a.x - b.x, 2.0) + pow((a.y - b.y), 2.0)
                + pow(a.z - b.z, 2.0);
      break;
    case 4:
      parameter = pow(a.x - b.x, 2.0) + pow((a.y - b.y), 2.0)
                + pow(a.z - b.z, 2.0) + pow((a.w - b.w), 2.0);
      break;
  }
  return sqrt(parameter);
}

position *random_positions(int numpoints, int dimension) {
  position *positions = (position *)malloc(sizeof(position) * numpoints);
  for(int i = 0; i < numpoints; i++) {
    positions[i].x = (double)rand() / (double)RAND_MAX;
    positions[i].y = (double)rand() / (double)RAND_MAX;
    positions[i].z = (double)rand() / (double)RAND_MAX;
    positions[i].w = (double)rand() / (double)RAND_MAX;
  }
  return positions;
}
