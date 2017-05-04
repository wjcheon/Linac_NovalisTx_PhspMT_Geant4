//
//
//
//	MyRun.cc
//
//
// 	Author: W.J. Cheon
//  Major:
//  Radiolodical Science, Yonsei Univ., Korea. (B.S)
//  Communication and Information Eng., Computer Eng.,Yonsei Univ., Korea (B.Eng)
//  Medical Physics, Sungkyunkwan Univ., Korea (Ph.D integrated course)
//  Email: wonjoongcheon@gmail.com
//


#include "G4SDManager.hh"
#include "MyRun.hh"


MyRun::MyRun()
{

}

MyRun::~MyRun(){

}

void MyRun::Merge(const G4Run* aRun)
{

  G4Run::Merge(aRun);
}
