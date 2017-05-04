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

#include "PolygonDataImport.hh"

PolygonDataImport::PolygonDataImport(G4String filename) {
	std::ifstream ifp;
	ifp.open(filename.c_str());
	G4String temp_str;

	if(!ifp.is_open()) {
		G4cerr << "File open error!!" << G4endl;
	}

	G4cout << filename  << " data file loading...." << G4endl;

	while(1)
	{
		ifp >> temp_str;

		if(temp_str.find("element")!=G4String::npos)
		{
			ifp >> temp_str;
			if(temp_str.find("vertex")!=G4String::npos)
				ifp >> numVertex;
			else if(temp_str.find("face")!=G4String::npos)
				ifp >> numFacet;
		}
		else if(temp_str.find("end_header")!=G4String::npos)
			break;
	}

	G4double P1, P2, P3;
	G4int F0, F[4];

	for(int i=0;i<numVertex;i++) {
		ifp >> P1 >> P2 >> P3;
		vertexVector.push_back(G4ThreeVector(P1,P2,P3));
	}

	for(int i=0;i<numFacet;i++)	{
		ifp >> F0;	// number of facet
		for(int j=0;j<F0;j++) {
			ifp >> F[j];
		}

		if(F0==3) {
			G4VFacet* facet = new G4TriangularFacet(vertexVector[F[0]],
					                                vertexVector[F[1]],
					                                vertexVector[F[2]],
					                                ABSOLUTE);
			facetVector.push_back((G4VFacet*)facet);
		}
		else if(F0==4) {
			G4VFacet* facet = new G4QuadrangularFacet(vertexVector[F[0]],
					                                  vertexVector[F[1]],
					                                  vertexVector[F[2]],
					                                  vertexVector[F[3]], ABSOLUTE);
			facetVector.push_back((G4VFacet*)facet);
		}
		else {
			G4cerr << "Can not support this types of polygon facet" << G4endl;
		}
	}
	ifp.close();


}


PolygonDataImport::~PolygonDataImport() {

	for(int i=0;i<numFacet;i++)
	{
		delete facetVector[i];
	}

}

