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


#include "SensitiveDetectorInPhantom.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"

// For Filename
#include <string.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <time.h>
#include <stdio.h>
#include <string>

#include "Shared.hh"
using namespace std;

SensitiveDetectorInPhantom::SensitiveDetectorInPhantom(G4String name)
:G4VSensitiveDetector(name)
{
	//// Time
	time_t now;
	struct tm t;

	time(&now);
	t = *localtime(&now);
	G4int year = t.tm_year + 1900;
	G4int month = t.tm_mon +1;
	G4int day = t.tm_mday;
	G4int hour = t.tm_hour;
	G4int min = t.tm_min;
	G4int sec = t.tm_sec;

	// File name
	stringstream ss_filename;
	ss_filename<<"InvivoDose_"<< year << month << day << "_" << hour << "_" << min <<"_" <<sec <<"_ThreadNum_" << G4Threading::G4GetThreadId();
	filenameForSave = ss_filename.str();
	//
	ofs.open(filenameForSave.c_str());
	// Initialization
	for(G4int ii=0 ; ii<NUMBER_OF_REPLICA ; ii++){
		for (G4int jj = 0 ; jj<NUMBER_OF_REPLICA ; jj++){
			DEMAtrix[ii][jj] = 0;
		}
	}
}

SensitiveDetectorInPhantom::~SensitiveDetectorInPhantom()
{
	G4int nPrimaries = G4RunManager::GetRunManager()->GetNumberOfEventsToBeProcessed();
	//
	for(G4int ii=0 ; ii<NUMBER_OF_REPLICA ; ii++){
		for (G4int jj = 0 ; jj<NUMBER_OF_REPLICA ; jj++){
			ofs << ii << "\t" << jj << "\t" << DEMAtrix[ii][jj]/nPrimaries/MeV <<G4endl;
		}
		ofs<<G4endl;
	}
	//
	ofs.close();
}

void SensitiveDetectorInPhantom::Initialize(G4HCofThisEvent*)
{

}

G4bool SensitiveDetectorInPhantom::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{

	G4int RepZNo = aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(0);
	G4int RepXNo = aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(1);

	G4double dE = aStep->GetTotalEnergyDeposit();

	DEMAtrix[RepZNo][RepXNo] += dE;

    return true;
}

void SensitiveDetectorInPhantom::EndOfEvent(G4HCofThisEvent*)
{

}
