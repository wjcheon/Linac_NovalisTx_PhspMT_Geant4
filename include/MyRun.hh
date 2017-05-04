//
// MyRun.hh
//
// 	Author: W.J. Cheon
//  Major:
//  Radiolodical Science, Yonsei Univ., Korea. (B.S)
//  Communication and Information Eng., Computer Eng.,Yonsei Univ., Korea (B.Eng)
//  Medical Physics, Sungkyunkwan Univ., Korea (Ph.D integrated course)
//  Email: wonjoongcheon@gmail.com
//

#ifndef MyRun_h
#define MyRun_h 1

#include "G4Run.hh"

class MyRun : public G4Run
{
 public:
  MyRun();
  virtual ~MyRun();
  virtual void Merge(const G4Run*);
 private:

};



#endif
