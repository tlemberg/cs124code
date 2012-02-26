// struct for graph node positions. we only need these for parts 2 and 3
// so we use this seperate struct when we need it
typedef struct {
    double x;
    double y;
    double z;
    double w;
} position;

// basic functions for position
double dist1(double x1, double x2);
double dist2(double x1, double y1, double x2, double y2);
double dist3(double x1, double y1, double z1, double x2, double y2, double z2);
double dist4(double x1, double y1, double z1, double w1, double x2, double y2, double z2, double w2);
position new_position(double x, double y, double z, double w);
double dist(position a, position b, int dimension);
position *random_positions(int numpoints, int dimension);
