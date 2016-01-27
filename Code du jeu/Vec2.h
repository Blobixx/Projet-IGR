// --------------------------------------------------------------------------
// Copyright(C) 2009-2015
// Tamy Boubekeur
//
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License (http://www.gnu.org/licenses/gpl.txt)
// for more details.
// --------------------------------------------------------------------------

#pragma once

#include <cmath>
#include <iostream>

/// Vector in 2 dimensions, with basics operators overloaded.
template <class T>
class Vec2 {

public:
  inline Vec2 (void)	{ p[0] = p[1] - 0.0; }

  inline Vec2 (T p0, T p1) {
	p[0] = p0;
	p[1] = p1;
  };

  inline Vec2 (const Vec2 & v) {
	init (v[0], v[1]);
  }

  ~Vec2() {}

  inline Vec2 (T* pp) {
	p[0] = pp[0];
	p[1] = pp[1];
  };

  // *********
  // Operators
  // *********

  inline T& operator[] (int Index) {
	return (p[Index]);
  };

  inline const T& operator[] (int Index) const {
	return (p[Index]);
  };

  inline Vec2& operator= (const Vec2 & P) {
	p[0] = P[0];
	p[1] = P[1];
	return (*this);
  };

  inline Vec2& operator+= (const Vec2 & P) {
	p[0] += P[0];
	p[1] += P[1];
	return (*this);
  };

  inline Vec2& operator-= (const Vec2 & P) {
	p[0] -= P[0];
	p[1] -= P[1];
	return (*this);
  };

  inline Vec2& operator*= (const Vec2 & P) {
	p[0] *= P[0];
	p[1] *= P[1];
	return (*this);
  };

  inline Vec2& operator*= (T s) {
	p[0] *= s;
	p[1] *= s;
	return (*this);
  };

  inline Vec2& operator/= (const Vec2 & P) {
	p[0] /= P[0];
	p[1] /= P[1];;
	return (*this);
  };

  inline Vec2& operator/= (T s) {
	p[0] /= s;
	p[1] /= s;
	return (*this);
  };

  inline Vec2 operator+ (const Vec2 & P) const {
	Vec2 res;
	res[0] = p[0] + P[0];
	res[1] = p[1] + P[1];
	return (res);
  };

  inline Vec2 operator- (const Vec2 & P) const {
	Vec2 res;
	res[0] = p[0] - P[0];
	res[1] = p[1] - P[1];
	return (res);
  };

  inline Vec2 operator- () const {
	Vec2 res;
	res[0] = -p[0];
	res[1] = -p[1];
	return (res);
  };

  inline Vec2 operator* (const Vec2 & P) const {
	Vec2 res;
	res[0] = p[0] * P[0];
	res[1] = p[1] * P[1];
	return (res);
  };

  inline Vec2 operator* (T s) const {
	Vec2 res;
	res[0] = p[0] * s;
	res[1] = p[1] * s;
	return (res);
  };

  inline Vec2 operator/ (const Vec2 & P) const {
	Vec2 res;
	res[0] = p[0] / P[0];
	res[1] = p[1] / P[1];
	return (res);
  };

  inline Vec2 operator/ (T s) const {
	Vec2 res;
	res[0] = p[0] / s;
	res[1] = p[1] / s;
	return (res);
  };

  inline bool operator == (const Vec2 & a) const {
	return(p[0] == a[0] && p[1] == a[1]);
  };

  inline bool operator != (const Vec2 & a) const {
	return(p[0] != a[0] || p[1] != a[1]);
  };

  inline bool operator < (const Vec2 & a) const {
	return(p[0] < a[0] && p[1] < a[1] );
  };

  inline bool operator >= (const Vec2 &a) const {
	return(p[0] >= a[0] && p[1] >= a[1]);
  };


  /////////////////////////////////////////////////////////////////

  inline Vec2 & init (T x, T y) {
	p[0] = x;
	p[1] = y;
	return (*this);
  };

  inline T squaredLength() const {
        return (dot (*this, *this));
  };

  inline T length() const {
        return (T)sqrt (squaredLength());
  };

  /// Return length after normalization
  inline T normalize (void) {
        T l = length ();
        if (l == 0.0f)
	  return 0;
        T invL = 1.0f / l;
        p[0] *= invL;
        p[1] *= invL;
        return l;
  };

  protected:
    T p[2];
};


template <class T>
inline T dist (const Vec2<T> & a, const Vec2<T> & b) {
    return (a-b).length ();
}

template <class T>
inline T dot (const Vec2<T> & a, const Vec2<T> & b) {
      return (a[0] * b[0] + a[1] * b[1] );
}


template <class T>
inline Vec2<T> normalize (const Vec2<T> & x) {
    Vec2<T> n (x);
    n.normalize ();
    return n;
}

template <class T>
inline Vec2<T> interpolate (const Vec2<T> & u, const Vec2<T> & v, float alpha) {
      return (u * (1.0 - alpha) + v * alpha);
}



template <class T>
inline Vec2<T> projectOntoVector (const Vec2<T> & v1, const Vec2<T> & v2) {
      return v2 * dotProduct (v1, v2);
}

template <class T>
inline Vec2<T> operator * (const T &s, const Vec2<T> &P) {
   return (P * s);
}

template <class T>
std::ostream & operator<< (std::ostream & output, const Vec2<T> & v) {
  output << v[0] << " " << v[1];
  return output;
}

template <class T>
std::istream & operator>> (std::istream & input, Vec2<T> & v) {
  input >> v[0] >> v[1] >> v[2];
  return input;
}

typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;
typedef Vec2<int> Vec2i;


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
