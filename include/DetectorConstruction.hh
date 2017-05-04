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
//

#ifndef DetectorConstruction_hh_
#define DetectorConstruction_hh_

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"


class DetectorConstruction: public G4VUserDetectorConstruction
{
public:
	DetectorConstruction();
	virtual ~DetectorConstruction();

	virtual G4VPhysicalVolume* Construct();
	virtual void ConstructSDandField();
private:
	//function
	void SetDimension();

	G4bool USE_HOMO_PHANTOM;


	//variable
	G4double TargetPositionZ;

	G4double WorldX;
	G4double WorldY;
	G4double WorldZ;

	G4double WaterBoxX;
	G4double WaterBoxY;
	G4double WaterBoxZ;

	G4double WaterBoxPositionX;
	G4double WaterBoxPositionY;
	G4double WaterBoxPositionZ;

	G4double InvivoBoxX;
	G4double InvivoBoxY;
	G4double InvivoBoxZ;

	G4double InvivoBoxPosX;
	G4double InvivoBoxPosY;
	G4double InvivoBoxPosZ;



	G4double PhantomMotherX;
	G4double PhantomMotherY;
	G4double PhantomMotherZ;

	G4double PhantomMotherPosX;
	G4double PhantomMotherPosY;
	G4double PhantomMotherPosZ;

	G4int NumOfReplica;

	G4double ExitDoseBoardPosX;
	G4double ExitDoseBoardPosY;
	G4double ExitDoseBoardPosZ;


};

#endif
