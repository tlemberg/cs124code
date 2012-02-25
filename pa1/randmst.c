#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "prim.h"

void perform_submission(int env, int numpoints, int numtrials, int dimension);
void perform_developer(int env, int numpoints, int numtrials, int dimension);
int argtoi(char *arg);
void print_misuse();
void print_usage();

enum {submission, developer} environment;
int n_environment = 2;

int main(int argc, char *argv[]) {
  if(argc == 5) {
    int env = argtoi(argv[1]);
    if(env >= 0 && env < n_environment) {
      int numpoints = argtoi(argv[2]);
      int numtrials = argtoi(argv[3]);
      int dimension = argtoi(argv[4]);
      if(numpoints >=0 && numtrials >= 0 && (dimension == 0 || (dimension >= 2 && dimension <= 4))) {
        srand((unsigned)time(NULL));
        if(dimension == 0) {
          dimension++;
        }
        switch(env) {
          case submission:
            perform_submission(env, numpoints, numtrials, dimension);
            break;
          case developer:
            perform_developer(env, numpoints, numtrials, dimension);
            break;
        }
        return 0;
      } else {
        print_misuse();
        return -1;
      }
    } else {
      print_misuse();
      return -1;
    }
  } else {
    print_misuse();
    return -1;
  }
}

void perform_submission(int env, int numpoints, int numtrials, int dimension) {
  double sum = 0;
  for(int i = 0; i < numtrials; i++) {
    double val = perform_trial(numpoints, dimension);
    sum += val;
  }
  double avg = sum / (double)numtrials;
  printf("%f %d %d %d\n", avg, numpoints, numtrials, dimension);
}

void perform_developer(int env, int numpoints, int numtrials, int dimension) {
  printf("Running %d trials of Prim's algorithm on %d points in %d-dimensional space...\n", numtrials, numpoints, dimension);
  double sum = 0;
  for(int i = 0; i < numtrials; i++) {
    double val = perform_trial(numpoints, dimension);
    printf("Trial %d, size of MST: %f\n", i + 1, val);
    sum += val;
  }
  double avg = sum / (double)numtrials;
  printf("Average size of MST over %d trials: %f\n", numtrials, avg);
}

void print_misuse() {
  printf("Unrecognizable input.\n");
  print_usage();
}

void print_usage() {
  printf("Usage:\n  randmst ENV NUMPOINTS NUMTRIALS DIMENSION\n");
  printf("  ENV          # An integer on the interval [0, %d]\n", n_environment - 1);
  printf("  NUMPOINTS    # The number of points in the graph\n");
  printf("  NUMTRIALS    # The number of trials to perform\n");
  printf("  DIMENSION    # The number of dimensions assigned to each node\n");
  printf("                 # 0     -> Random real on [0, 1]\n");
  printf("                 # 2,3,4 -> n dimensions\n");
}

int argtoi(char *arg) {
  int i = 0;
  int r = 0;
  while(1) {
    char c = arg[i];
    if(c == 0) {
      break;
    } else if(c >= 48 && c <= 58) {
      r = (r * 10) + c - 48;
      i++;
    } else {
      return -1;
    }
  }
  return r;
}
