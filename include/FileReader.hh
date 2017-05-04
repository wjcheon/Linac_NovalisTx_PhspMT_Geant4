//
//
//  FileReader.hh
//
//
// 	Author: W.J. Cheon
//  Major:
//  Radiolodical Science, Yonsei Univ., Korea. (B.S)
//  Communication and Information Eng., Computer Eng.,Yonsei Univ., Korea (B.Eng)
//  Medical Physics, Sungkyunkwan Univ., Korea (Ph.D integrated course)
//  Email: wonjoongcheon@gmail.com
//


#ifndef SRC_FILEREADER_HH_
#define SRC_FILEREADER_HH_


#include <list>
#include <fstream>
#include <iostream>

#include "G4ThreeVector.hh"



class FileReader {
public:
	FileReader();
	FileReader(G4String fileName);
	virtual ~FileReader();

	G4ThreeVector GetAnEvent(); // this function is test function.


	struct Sparticle
	{
		G4ThreeVector pos, dir;
		G4double kinEnergy;
		G4int nPrimaryPart, primaryParticlePDGE, partPDGE, volumeId;
		G4String volumeName;
	};

	Sparticle GetParticleContainer();



private:
	std::ifstream inputFile;
	std::list<G4ThreeVector> evList;
	std::list<Sparticle> ParticleContainer;
	Sparticle aParticle;
	G4double ex,ey,ez;
	G4double x,y,z;
	G4int d;


	G4int startDataFilePosition, currentFilePosition, currentFileSize;





};

#endif /* SRC_FILEREADER_HH_ */
