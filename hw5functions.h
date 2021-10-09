#ifndef HW5FUNCTIONS_H
#define HW5FUNCTIONS_H

// Cosine and Sine in degrees
// copying this from ex8 bc it's pretty useful
#define Cos(x) (cos((x)*3.14159265/180))
#define Sin(x) (sin((x)*3.14159265/180))

// All the convenience functions from ex8
void Fatal(const char* format , ...);
void ErrCheck(const char* where);
void Print(const char* format, ...);

// I like using rgb from 0-255
// so this is just a function that does that
void Color(double r, double g, double b);

// nice function for changing projection
// basically copied from..idk which but an example
//void Project();

// I want a nice picket fence, no reason why really
void fence();
void fence_line();
void fence_post();
void picket();

#endif