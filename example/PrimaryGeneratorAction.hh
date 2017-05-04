/*
 * InitSetPrimaryGeneratorAction.hh
 *
 *  Created on: 2013. 3. 3.
 *      Author: root
 */

//#ifndef GVTPCPRIMARYGENERATORACTION_HH_
//#define GVTPCPRIMARYGENERATORACTION_HH_

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4Run.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

class G4ParticleGun;
class G4Event;
class DetectorConstruction;

//class G4GeneralParticleSource;
// --

class PrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction{
public:
	PrimaryGeneratorAction();
	PrimaryGeneratorAction(G4String);


	virtual ~PrimaryGeneratorAction();

	void GeneratePrimaries(G4Event* anEvent);

private:
//	G4GeneralParticleSource* fGPS;
void GenerateFromCalculatedPhaseSpace();

  struct particle
  {
                G4ThreeVector dir, pos;
                G4double kinEnergy;
                G4int partPDGE;
  };

  G4ParticleGun*                        particleGun;
  particle                      particles[100000];
  G4ParticleTable* particleTable;
  G4ParticleDefinition* part;
  G4int tempangle;
  G4double x, y, z;
  G4int d;
  G4int nMaxParticlesInPhaseSpace;
  G4int nParticle;
  G4int i, j;
  G4int PDGE;
  G4int length;

};

#endif /* GVTPCPRIMARYGENERATORACTION_HH_ */
