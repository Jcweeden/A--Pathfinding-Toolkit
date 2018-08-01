#ifndef _Vector2D_
#define _Vector2D_

class Vector2D {
public:

  Vector2D(float p_x, float p_y) {x = p_x; y = p_y;}
  
  float getX() {return x;}
  float getY() {return y;}
  
  void setX(float newX) {x = newX;}
  void setY(float newY) {y = newY;}

  void clear()
  {
    x = 0;
    y = 0;
  }

  //returns length of the vector
  float magnitude() {return sqrt(x * x + y * y); }

  Vector2D normalize(Vector2D v2)
  {
    x = x / v2.magnitude();
    y = y / v2.magnitude();

    return Vector2D(x,y);
  }

   void normalise()
  {
    float l = magnitude();
    if (l > 0) {
      (*this) *= 1 / l;
    }
  }

  float calcDistance(Vector2D v2)
  {
    //calculate distance between this vector and vector v2 using pyth theorem
    return sqrt(pow(v2.x - x, 2) + 
                pow(v2.y - y, 2) * 1.0);
  }
  
  void addScaledVector(/*const*/ Vector2D v1, float scale)
  {
    x += v1.getX() * scale;
    y += v1.getY() * scale;
  }
  
  void operator+=(const Vector2D& v)
  {
    x += v.x;
    y += v.y;
  }

 Vector2D operator-(const Vector2D& v2) const
  {
    return Vector2D(x - v2.x, y - v2.y);
  }

  Vector2D operator*(const float val) const
  {
    return Vector2D(x * val, y * val);
  }
  
  void operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
  }

  
 Vector2D operator/(float scalar) {
   return Vector2D((float)x/scalar, (float)y/scalar);
 }
  
protected:
  float x;
  float y;
};

#endif
