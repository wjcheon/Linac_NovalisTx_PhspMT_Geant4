#include "PrimaryGeneratorPSF.hh"
//#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"
#include "G4IAEAphspReader.hh"
#include "G4AutoLock.hh"

#include "G4ThreeVector.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

namespace { G4Mutex theIAEAReaderMutex = G4MUTEX_INITIALIZER; }
G4IAEAphspReader* PrimaryGeneratorPSF::theIAEAReader = 0;

PrimaryGeneratorPSF::PrimaryGeneratorPSF() : G4VUserPrimaryGeneratorAction()
{
  G4String filename = "10x10_MDA1";

  G4AutoLock lock(&theIAEAReaderMutex);
  if(!theIAEAReader) theIAEAReader = new G4IAEAphspReader(filename);
// Define the isocentre position
  
  G4ThreeVector globalPos(0.,0.,-143.5*cm);
  theIAEAReader->SetGlobalPhspTranslation(globalPos);

  G4ThreeVector isoPos(0., 0., 14.*cm);
  theIAEAReader->SetIsocenterPosition(isoPos);

// Partitions
  //theIAEAReader->SetTotalParallelRuns(10);
  //theIAEAReader->SetParallelRun(3);
  //theIAEAReader->SetTimesRecycled(24);

}

PrimaryGeneratorPSF::~PrimaryGeneratorPSF()
{
  G4AutoLock lock(&theIAEAReaderMutex);
  if (theIAEAReader) {delete theIAEAReader; theIAEAReader = 0; }
}

void PrimaryGeneratorPSF::GeneratePrimaries(G4Event* anEvent)
{
  //G4ThreeVector particlePos(0,0,0);
  //std::vector<G4ThreeVector>* posVector;
  //posVector = theIAEAReader->GetPositionVector();
  //particlePos = posVector; 
  //particlePos *= cm;
  //G4cout << "The particle's x position is :" << posVector << G4endl;
  G4AutoLock lock(&theIAEAReaderMutex);
  theIAEAReader->GeneratePrimaryVertex(anEvent);
}
