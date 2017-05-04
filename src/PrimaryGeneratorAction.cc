// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// 	Author: W.J. Cheon
//  Major:
//  Radiolodical Science, Yonsei Univ., Korea. (B.S)
//  Communication and Information Eng., Computer Eng.,Yonsei Univ., Korea (B.Eng)
//  Medical Physics, Sungkyunkwan Univ., Korea (Ph.D integrated course)
//  Email: wonjoongcheon@gmail.com
//                                     *

#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "G4Threading.hh"

#include "G4AutoLock.hh"
#include "FileReader.hh"
#include "G4ThreeVector.hh"

namespace{G4Mutex PrimGenMutex = G4MUTEX_INITIALIZER;}
FileReader* PrimaryGeneratorAction::fileReader=0;
static G4int nGeneratedPrimaries;

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction()
{
	nGeneratedPrimaries = 0;
	G4AutoLock	lock(&PrimGenMutex);
	//calculatedPhaseSpaceFileIN = "MomentomPhsp.txt";
	//calculatedPhaseSpaceFileIN = "PhSp_Iplan_1_2_1.txt";
	calculatedPhaseSpaceFileIN = "Phsp_FieldType3_v2.0_num1.txt";
	if(!fileReader) fileReader = new FileReader(calculatedPhaseSpaceFileIN);
	//if(!FileReader) fileReader = new FileReader();
	particleGun = new G4ParticleGun();


}



PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	// Free the G4ParticleGun and PhaseSpaceFile
	G4AutoLock	lock(&PrimGenMutex);
	if(fileReader){delete fileReader; fileReader = 0;}
	delete this->particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{


	FileReader::Sparticle aParticle_return;
	if(fileReader){
		G4AutoLock	lock(&PrimGenMutex);
		aParticle_return = fileReader->GetParticleContainer();
	}

	//Cheaker
	//G4cout<<"aParticle_return.dir is " << aParticle_return.dir << G4endl;
	//G4cout<<"aParticle_return.kinEnergy is " << aParticle_return.kinEnergy << G4endl;
	//G4cout<<"aParticle_return.partPDGE is " << aParticle_return.partPDGE << G4endl;

	switch (aParticle_return.partPDGE)
	{
	case -11:
		this->particleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("e+"));
		break;
	case 11:
		this->particleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("e-"));
		break;
	case 22:
		this->particleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("gamma"));
		break;
	}
	this->particleGun->SetParticlePosition(aParticle_return.pos);
	this->particleGun->SetParticleMomentumDirection((G4ParticleMomentum)aParticle_return.dir);
	this->particleGun->SetParticleEnergy(aParticle_return.kinEnergy*MeV);


	nGeneratedPrimaries= nGeneratedPrimaries + 1;
	G4cout<< " nGeneratedPrimaries is "<< nGeneratedPrimaries <<G4endl;
	particleGun->GeneratePrimaryVertex(anEvent);

	/*
	G4ThreeVector momDirection(0.,0.,0.);
	if(fileReader){
		G4AutoLock	lock(&PrimGenMutex);
		momDirection = fileReader->GetAnEvent();
	}
	G4cout<<"momDirection is "<<momDirection<<G4endl;


	particleGun->SetParticleMomentum(momDirection);
	G4double x0 = (10*cm) * (G4UniformRand() - 0.5);
	G4double y0 = (10*cm) * (G4UniformRand() - 0.5);
	particleGun->SetParticlePosition(G4ThreeVector(x0,y0,0.0));


	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	 */


}


