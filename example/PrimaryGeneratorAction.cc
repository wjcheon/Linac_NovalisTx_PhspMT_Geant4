/*
 * InitSetPrimaryGeneratorAction.cc
 *
 *  Created on: 2013. 3. 3.
 *      Author: root
 */

#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include <iostream>
#include <fstream>


// --

std::ifstream in;
using namespace std;
PrimaryGeneratorAction::PrimaryGeneratorAction(){

	G4cout<<"PrimaryGeneratorAction::PrimaryGeneratorAction"<<G4endl;

//	fGPS = new G4GeneralParticleSource();
	nMaxParticlesInPhaseSpace = 100000;
        particleGun = new G4ParticleGun();
        in.open("phsp45", std::ios::in);
        if(in == 0)
        {
                std::cout<<"ERROR phase space file: "<< "NOT found.Run abort "<<G4endl;
                G4RunManager::GetRunManager()->AbortRun(true);
        }
	
        particleTable = G4ParticleTable::GetParticleTable();
}

PrimaryGeneratorAction::PrimaryGeneratorAction(G4String phase)
{

	G4cout<<"PrimaryGeneratorAction::PrimaryGeneratorAction(phase)"<<G4endl;

        nMaxParticlesInPhaseSpace = 100000;
        particleGun = new G4ParticleGun();
        in.open(phase, std::ios::in);
        if(in == 0)
        {
                std::cout<<"ERROR phase space file: "<< "NOT found.Run abort "<<G4endl;
                G4RunManager::GetRunManager()->AbortRun(true);
        }
        particleTable = G4ParticleTable::GetParticleTable();


        in.seekg(0,ios::end);
        length = in.tellg();
        	in.seekg(0,ios::beg);

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



PrimaryGeneratorAction::~PrimaryGeneratorAction(){

//	G4cout<<"PrimaryGeneratorAction::~PrimaryGeneratorAction"<<G4endl;

//	delete fGPS;
	delete particleGun;
	in.close();

}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){

//	G4cout<<"PrimaryGeneratorAction::GeneratePrimaries"<<G4endl;

//	fGPS->GeneratePrimaryVertex(anEvent);
        G4int evtnbt = anEvent->GetEventID();
        G4int evtnb = evtnbt%100000;
        if(evtnb == 0)
        {GenerateFromCalculatedPhaseSpace();}
        particleGun->SetParticleEnergy(particles[evtnb].kinEnergy*MeV);
        particleGun->SetParticlePosition(particles[evtnb].pos*mm);
        particleGun->SetParticleMomentumDirection((G4ParticleMomentum)particles[evtnb].dir);


	part= particleTable->FindParticle(particles[evtnb].partPDGE);
	particleGun->SetParticleDefinition(part);


/*	switch(particles[evtnb].partPDGE)
	{
		case 11:  */       
/*			G4ParticleDefinition* electron
                            = particleTable->FindParticle("e-");
		        particleGun->SetParticleDefinition(electron);*/
/*			break;
		case 22:         
		        G4ParticleDefinition* gamma
                            = particleTable->FindParticle(22);
		        particleGun->SetParticleDefinition(gamma);
			break;
		case -11:         
		        G4ParticleDefinition* positron
                            = particleTable->FindParticle(-11);
		        particleGun->SetParticleDefinition(positron);
			break;
	}
*/

        particleGun->GeneratePrimaryVertex(anEvent);


	
        if(evtnb%100000 == 0)
        {
                G4cout<<evtnbt<<"th particles done / "<<nMaxParticlesInPhaseSpace<<G4endl;
        }
	
}

void PrimaryGeneratorAction::GenerateFromCalculatedPhaseSpace()
{



//G4cout<<"end:"<<length<<G4endl;

        for(i = 0;i<100000;i++)
        {
        in>>x;in>>y;in>>z;
        particles[i].pos.set(x,y,z);
        in>>x; in>>y; in>>z;
        particles[i].dir.set(x,y,z);
        in>>x;
        particles[i].kinEnergy=x;
        in>>d;
	particles[i].partPDGE = d;
//G4cout<<"middle:"<<in.tellg()<<G4endl;

		if(length-1 == in.tellg())
		{
			in.seekg(0, ios::beg);
//G4cout<<"begin:"<<in.tellg()<<G4endl;

		}

        }
//	if(in.tellg()==
//	G4int length = in.tellg();
//	G4cout<<length<<G4endl;
//	G4int eof = in.eof();
//	G4cout<<eof<<G4endl;
//	fseek(in,0,SET_SEEK);
}

