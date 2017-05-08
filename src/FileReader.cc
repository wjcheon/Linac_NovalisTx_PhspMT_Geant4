//
//
//
//	FileReader.cc
//
//
// 	Author: W.J. Cheon
//  Major:
//  Radiolodical Science, Yonsei Univ., Korea. (B.S)
//  Communication and Information Eng., Computer Eng.,Yonsei Univ., Korea (B.Eng)
//  Medical Physics, Sungkyunkwan Univ., Korea (Ph.D integrated course)
//  Email: wonjoongcheon@gmail.com
//

#include "FileReader.hh"
#include "G4RunManager.hh"
#include "G4ThreeVector.hh"
#include "Shared.hh"


namespace{ G4Mutex PrimGenMutx = G4MUTEX_INITIALIZER;}
FileReader::FileReader() {
	// TODO Auto-generated constructor stub


}

FileReader::FileReader(G4String fileName) {
	//Init
	startDataFilePosition = currentFilePosition = currentFileSize =0;
	ex = ey = ez = 0;
	x = y = z = 0.0;
	d =0;

	inputFile.open(fileName.data());


	static bool bFirstTime=true;
	if (bFirstTime)
	{
		inputFile.seekg(-1,std::ios::end);
		currentFileSize=inputFile.tellg();
		inputFile.seekg(0,std::ios::beg);
		bFirstTime=false;
	}

	// Remove the information of phase space
	char a[1000];
	inputFile.getline(a,1000,'\n');
	inputFile.getline(a,1000,'\n');
	startDataFilePosition=inputFile.tellg();

}


FileReader::~FileReader() {
	// TODO Auto-generated destructor stub
	inputFile.close();

}


G4ThreeVector FileReader::GetAnEvent(){


	if (evList.size() ==0 )
	{
		for(int ii =0; ii<NUMBER_OF_PARTICLE_RAM; ii++)
		{

			inputFile >> ex >> ey >> ez ;
			evList.push_back(G4ThreeVector(ex,ey,ez));

		}
	}

	G4ThreeVector evv = evList.front();
	evList.pop_front();
	return evv;

}


FileReader::Sparticle FileReader::GetParticleContainer(){


	static bool checkFileRewind=false;
	static bool bRewindTheFile=false;
	static int nPhSpFileRewind=0;
	static bool bFirstFillContainer=true;


	if (bFirstFillContainer)
	{
		inputFile.seekg(startDataFilePosition, std::ios::beg);
		bFirstFillContainer=false;
	}


	if (ParticleContainer.size() ==0 )
	{
		for(int ii =0; ii<NUMBER_OF_PARTICLE_RAM; ii++)
		{

			if (bRewindTheFile) // to read the phase space file again to fill the container
			{
				inputFile.clear();
				inputFile.seekg(startDataFilePosition, std::ios::beg);
				checkFileRewind=true;
				bRewindTheFile=false;
			}


			//trash
			inputFile >> d;


			//position
			inputFile >> x; inputFile >>y; inputFile >> z;
			z = z-1200;
			this->aParticle.pos.set(x,y,z);

			//G4cout<<"currentFilePosition is on container" << inputFile.tellg() << G4endl;
			//direction
			inputFile >> x; inputFile >>y; inputFile >> z;
			this->aParticle.dir.set(x,y,z);

			//Kinectic E
			inputFile >> x;
			this->aParticle.kinEnergy=x;


			//partPDGE
			inputFile >> d;;
			this->aParticle.partPDGE=d;

			//trash
			inputFile >> d; inputFile >> d;
			ParticleContainer.push_back(aParticle);

			if (inputFile.eof())	{bRewindTheFile=true;}
			if (checkFileRewind)	{checkFileRewind=false;}
		}

	}

	Sparticle evv = ParticleContainer.front();
	ParticleContainer.pop_front();
	currentFilePosition=inputFile.tellg(); // to remind the actual position in the phase space file

	if (currentFilePosition>=currentFileSize) // to read the phase space file again
	{
		//G4cerr<<"This function is working"<<G4endl;
		inputFile.clear();
		inputFile.seekg(startDataFilePosition, std::ios::beg);
	}

	return evv;


}
