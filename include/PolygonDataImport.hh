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
// 	Author: mchan
//

#ifndef PolygondataImport_h
#define PolygondataImport_h 1

#include <vector>
#include <fstream>
#include "G4ThreeVector.hh"
#include "G4VFacet.hh"
#include "G4TriangularFacet.hh"
#include "G4QuadrangularFacet.hh"

class PolygonDataImport{
public:
	PolygonDataImport(G4String filename);
	virtual ~PolygonDataImport();

	G4int GetNumFacet() {return numFacet;};
	G4VFacet* GetFacet(int i) {return facetVector[i];};

private:
	std::vector<G4ThreeVector> vertexVector;
	std::vector<G4VFacet* > facetVector;

	G4int numVertex;
	G4int numFacet;
};

#endif
