#ifndef PrimaryGeneratorPSF_h
#define PrimaryGeneratorPSF_h 1


#include "G4VUserPrimaryGeneratorAction.hh"

//class G4ParticleGun;
class G4Event;
class G4IAEAphspReader;

class PrimaryGeneratorPSF : public G4VUserPrimaryGeneratorAction
{

public:
  PrimaryGeneratorPSF();
  ~PrimaryGeneratorPSF();
  
  virtual void GeneratePrimaries(G4Event*);
  
private:
  //G4ParticleGun* fParticleGun;
  static G4IAEAphspReader* theIAEAReader;
  
};
  
#endif
