/*+P
 * This file is part of OffiX,
 * a C++ API for the X Window System and Unix
 * Copyright (C) 1995-98  Stefan Seefeld
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge,
 * MA 02139, USA.
 -P*/
/*$Id$*/
#ifndef _Time_hh
#define _Time_hh

#include <sys/time.h>
#include <ctime>
#include <iostream>

/* @Class{Time : public timeval}
 *
 * @Description{}
 */
class Time : public timeval
{
public:
  Time() { tv_sec = tv_usec = 0;}
  Time(const timeval &t) : timeval(t) { normalize();}
  Time(long l) { tv_sec = l/1000, tv_usec = (l - tv_sec*1000)*1000;}
  Time(int, int, int, int, int, int, int);
  Time(int, int, int, int);
  ~Time() {}
  bool  operator == (const Time &T) const { return tv_sec == T.tv_sec && tv_usec == T.tv_usec;}
  bool  operator != (const Time &T) const { return tv_sec != T.tv_sec || tv_usec != T.tv_usec;}
  bool  operator >  (const Time &T) const { return tv_sec > T.tv_sec || (tv_sec == T.tv_sec && tv_usec > T.tv_usec);}
  bool  operator <  (const Time &T) const { return tv_sec < T.tv_sec || (tv_sec == T.tv_sec && tv_usec < T.tv_usec);}
  bool  operator >= (const Time &T) const { return operator > (T) || operator == (T);}
  bool  operator <= (const Time &T) const { return operator < (T) || operator == (T);}
  Time &operator += (const Time &T) { tv_sec += T.tv_sec, tv_usec += T.tv_usec; normalize(); return *this;}
  Time &operator -= (const Time &T) { tv_sec -= T.tv_sec, tv_usec -= T.tv_usec; normalize(); return *this;}
  Time  operator +  (const Time &T) const { Time t = *this; t += T; return t;}
  Time  operator -  (const Time &T) const { Time t = *this; t -= T; return t;}
  operator bool () const { return tv_sec != 0 || tv_usec != 0;}
  operator const char *() const { return ctime(&tv_sec);}
  static Time currentTime();
  friend ostream &operator << (ostream &os, const Time &T) { return os << T.tv_sec << " s, " << T.tv_usec << " us";}
protected:
  inline void normalize();
private:
};

/* @Method{void Time::normalize()}
 *
 * @Description{}
 */
inline void Time::normalize()
{
  while (tv_usec >=  1000000) tv_usec -= 1000000, tv_sec++;
  while (tv_usec < 0)
    { 
      if (tv_sec > 0) tv_usec += 1000000, tv_sec--;
      else { tv_usec = 0; break;}
    }
}

#endif /* _Time_hh */
