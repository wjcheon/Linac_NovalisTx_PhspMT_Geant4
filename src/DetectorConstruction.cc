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
#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4VisAttributes.hh"

#include "SensitiveDetector.hh"
#include "SensitiveDetectorInPhantom.hh"

//Polygon
#include "PolygonDataImport.hh"
#include "G4TessellatedSolid.hh"
//Rotation Matrix
#include "G4RotationMatrix.hh"
//
//Share
#include "Shared.hh"


DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction()
{
	USE_HOMO_PHANTOM = true;   // false: Use Hetero-geneous Phantom
								// true: Use Homo-geneous Phantom

}

DetectorConstruction::~DetectorConstruction()
{

}

void DetectorConstruction::SetDimension(){

	TargetPositionZ = -1200*mm;     //Get information from NovalisTx;

	WorldX = 1.0*m;
	WorldY = 1.0*m;
	WorldZ = 3.0*m;

	// Exit dose Detector
	WaterBoxX = 20*cm;
	WaterBoxY = 20*cm;
	WaterBoxZ = 0.127*mm;          // Thickness of EBT3 film

	WaterBoxPositionX = 0.0*cm;
	WaterBoxPositionY = 0.0*cm;
	WaterBoxPositionZ = TargetPositionZ + 1500*mm + WaterBoxZ/2;      //SSD: 1500.0 mm

	// Invivo dose Detector
	InvivoBoxX = 20.0*cm;
	InvivoBoxY = 20.0*cm;
	InvivoBoxZ = 0.127*mm;     // Thickness of EBT3 film : 0.127

	InvivoBoxPosX = 0.0*cm;
	InvivoBoxPosY = 0.0*cm;
	InvivoBoxPosZ = ((30+0.127)/2)*mm;

	//Hetero and Homogeneous Phantom of Mother
	PhantomMotherX = 20.0*cm;
	PhantomMotherY = 20.0*cm;
	PhantomMotherZ = 11.0*cm;    // Phantom size is 10.0 cm + empty space 1.0 cm

	PhantomMotherPosX = 0.0*cm;
	PhantomMotherPosY = 0.0*cm;      //  SSD: 1000.0 mm
	PhantomMotherPosZ = TargetPositionZ + 1000*mm;


	NumOfReplica = NUMBER_OF_REPLICA;   // in shared.hh
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{

	// Element for material
	G4Element* elH = new G4Element("Hydrogen", "H2", 1., 1.01*g/mole);
	G4Element* elC = new G4Element("Carbon", "C", 6., 12.01*g/mole);
	G4Element* elF = new G4Element("Fluorine", "F", 9., 18.9984*g/mole);

	//Polystyrene
	G4Material *Polystyrene = new G4Material("Polystyrene", 1.06*g/cm3, 2);
	Polystyrene->AddElement(elC, 19);
	Polystyrene->AddElement(elH, 21);

	//Teflon
	G4Material* Teflon = new G4Material("Teflon", 2.2*g/cm3,2);
	Teflon->AddElement(elC,2);
	Teflon->AddElement(elF,4);

	// Geometry
	SetDimension();


	// World
	G4VSolid* sol_World = new G4Box("World", WorldX*0.5,WorldY*0.5, WorldZ*0.5);
	G4Material* AIR = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
	G4LogicalVolume* lv_World = new G4LogicalVolume(sol_World, AIR, "World");
	G4VPhysicalVolume* pv_World =
			new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), lv_World, "World", 0, false, 0);
	//
	//
	//Phantom Mother
	G4double thetaX = 0.0*deg;
	G4double thetaY = 0*deg;
	G4double thetaZ = 0*deg;
	G4RotationMatrix *rotM = new G4RotationMatrix();
	rotM->rotateX(thetaX);
	//
	G4VSolid* sol_PhantomMother = new G4Box("PhantomMother", PhantomMotherX*0.5, PhantomMotherY*0.5, PhantomMotherZ*0.5);
	G4LogicalVolume* lv_PhantomMother = new G4LogicalVolume(sol_PhantomMother, AIR, "PhantomMother");
	G4VPhysicalVolume* pv_PhantomMother =
			new G4PVPlacement(rotM, G4ThreeVector(PhantomMotherPosX, PhantomMotherPosY, PhantomMotherPosZ), lv_PhantomMother, "PhantomMother", lv_World, false, 10);

	// Phantom
	// Define Polygon geometery
	// Upper
	PolygonDataImport* polyData_Upper = new PolygonDataImport("./polygon/200x200x35.ply");
	G4int numFacet_Upper = polyData_Upper->GetNumFacet();
	G4TessellatedSolid* polySolid_Upper= new G4TessellatedSolid("Phantom_Upper");
	for(G4int i=0; i < numFacet_Upper ; i++){
		polySolid_Upper->AddFacet(polyData_Upper->GetFacet(i));
	}
	polySolid_Upper->SetSolidClosed(true);
	G4LogicalVolume *polyLogic_Upper= new G4LogicalVolume(polySolid_Upper,Polystyrene,"Phantom_Upper");
	G4VPhysicalVolume *polyPhys_Upper= new G4PVPlacement(0,G4ThreeVector(-10*cm,-10*cm,((-3.5/2)+((3.0+3.5)/2.0))*cm+InvivoBoxZ),polyLogic_Upper,"Phantom_Upper",lv_PhantomMother,true,11);
	//
	//Lower
	G4VPhysicalVolume *polyPhys_Lower= new G4PVPlacement(0,G4ThreeVector(-10*cm,-10*cm,((-3.5/2)-((3.0+3.5)/2.0))*cm),polyLogic_Upper,"Phantom_Lower",lv_PhantomMother,true,15);
	//
	//
	// Middle
	//200x200x30 with hole
	// Define Polygon Geometery
	G4LogicalVolume *polyLogic_Middle = NULL;
	G4VPhysicalVolume *polyPhys_Middle =NULL;
	G4LogicalVolume *polyLogic_teflon = NULL;
	G4VPhysicalVolume *polyPhys_teflon = NULL;
	if (USE_HOMO_PHANTOM)
	{
		PolygonDataImport* polyData_Middle = new PolygonDataImport("./polygon/200x200x30 with hole.ply");
		G4int numFacet_Middle = polyData_Middle->GetNumFacet();
		G4TessellatedSolid* polySolid_Middle= new G4TessellatedSolid("Phantom_Middle");
		for(G4int i=0; i < numFacet_Middle ; i++){
			polySolid_Middle->AddFacet(polyData_Middle->GetFacet(i));
		}
		polySolid_Middle->SetSolidClosed(true);
		polyLogic_Middle= new G4LogicalVolume(polySolid_Middle,Polystyrene,"Phantom_Middle");
		polyPhys_Middle= new G4PVPlacement(0,G4ThreeVector(-10*cm,-10*cm,(-3.0/2)*cm),polyLogic_Middle,"Phantom_Middle",lv_PhantomMother,false,12);
		//
		//30x30x30 (teflon)
		// Define Polygon Geometery
		PolygonDataImport* polyData_teflon = new PolygonDataImport("./polygon/30x30x30.ply");
		G4int numFacet_teflon = polyData_teflon->GetNumFacet();
		G4TessellatedSolid* polySolid_teflon= new G4TessellatedSolid("Phantom_middle_teflon");
		for(G4int i=0; i < numFacet_teflon ; i++){
			polySolid_teflon->AddFacet(polyData_teflon->GetFacet(i));
		}
		polySolid_teflon->SetSolidClosed(true);
		polyLogic_teflon= new G4LogicalVolume(polySolid_teflon,Teflon,"Phantom_middle_teflon");
		polyPhys_teflon= new G4PVPlacement(0,G4ThreeVector(-10*cm,-10*cm,(-3.0/2)*cm),polyLogic_teflon,"Phantom_middle_teflon",lv_PhantomMother,false,13);
	}
	else
	{
		PolygonDataImport* polyData_Middle = new PolygonDataImport("./polygon/200x200x30.ply");
		G4int numFacet_Middle = polyData_Middle->GetNumFacet();
		G4TessellatedSolid* polySolid_Middle= new G4TessellatedSolid("Phantom_Middle");
		for(G4int i=0; i < numFacet_Middle ; i++){
			polySolid_Middle->AddFacet(polyData_Middle->GetFacet(i));
		}
		polySolid_Middle->SetSolidClosed(true);
		polyLogic_Middle= new G4LogicalVolume(polySolid_Middle,Polystyrene,"Phantom_Middle");
		polyPhys_Middle= new G4PVPlacement(0,G4ThreeVector(-10*cm,-10*cm,(-3.0/2)*cm),polyLogic_Middle,"Phantom_Middle",lv_PhantomMother,false,12);
	}

	//
	// Scorer
	// Exit dose dosimeter
	G4VSolid* sol_Geom = new G4Box("Geom", WaterBoxX*0.5, WaterBoxY*0.5, WaterBoxZ*0.5);
	G4Material* WATER = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
	G4LogicalVolume* lv_Geom = new G4LogicalVolume(sol_Geom, AIR, "Geom");
	G4VPhysicalVolume* pv_Geom =
			new G4PVPlacement(0, G4ThreeVector(WaterBoxPositionX, WaterBoxPositionY, WaterBoxPositionZ), lv_Geom, "Geom", lv_World, false, 100);

	G4VSolid* sol_RepX = new G4Box("RepX",(WaterBoxX/NumOfReplica)*0.5,WaterBoxY*0.5,WaterBoxZ*0.5);
	G4LogicalVolume* lv_RepX = new G4LogicalVolume(sol_RepX,AIR,"RepX");
	G4VPhysicalVolume* pv_RepX = new G4PVReplica("RepX",lv_RepX,lv_Geom,kXAxis,NumOfReplica,WaterBoxX/NumOfReplica);

	G4VSolid* sol_RepY = new G4Box("RepY",(WaterBoxX/NumOfReplica)*0.5,(WaterBoxY/NumOfReplica)*0.5,WaterBoxZ*0.5);
	G4LogicalVolume* lv_RepY = new G4LogicalVolume(sol_RepY,AIR,"RepY");
	G4VPhysicalVolume* pv_RepY = new G4PVReplica("RepY",lv_RepY,lv_RepX,kYAxis,NumOfReplica,WaterBoxY/NumOfReplica);

	//
	// Invivo dose dosimeter
	G4VSolid* sol_Geom_Invivo= new G4Box("Geom_Invivo", InvivoBoxX*0.5, InvivoBoxY*0.5, InvivoBoxZ*0.5);
	G4LogicalVolume* lv_Geom_Invivo = new G4LogicalVolume(sol_Geom_Invivo, AIR, "Geom_Invivo");
	G4VPhysicalVolume* pv_Geom_Invivo =
			new G4PVPlacement(0, G4ThreeVector(InvivoBoxPosX, InvivoBoxPosY, InvivoBoxPosZ), lv_Geom_Invivo, "Geom_Invivo", lv_PhantomMother, false, 200);

	G4VSolid* sol_RepX_Invivo = new G4Box("RepX_Invivo",(InvivoBoxX/NumOfReplica)*0.5,InvivoBoxY*0.5,InvivoBoxZ*0.5);
	G4LogicalVolume* lv_RepX_Invivo = new G4LogicalVolume(sol_RepX_Invivo,AIR,"RepX_Invivo");
	G4VPhysicalVolume* pv_RepX_Invivo = new G4PVReplica("RepX_Invivo",lv_RepX_Invivo,lv_Geom_Invivo,kXAxis,NumOfReplica,InvivoBoxX/NumOfReplica);

	G4VSolid* sol_RepY_Invivo = new G4Box("RepY_Invivo",(InvivoBoxX/NumOfReplica)*0.5,(InvivoBoxY/NumOfReplica)*0.5,InvivoBoxZ*0.5);
	G4LogicalVolume* lv_RepY_Invivo = new G4LogicalVolume(sol_RepY_Invivo,AIR,"RepY_Invivo");
	G4VPhysicalVolume* pv_RepY_Invivo = new G4PVReplica("RepY_Invivo",lv_RepY_Invivo,lv_RepX_Invivo,kYAxis,NumOfReplica,InvivoBoxY/NumOfReplica);



	//
	// Visualization
	G4VisAttributes* va_World = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
	va_World->SetForceWireframe(true);
	lv_World->SetVisAttributes(va_World);

	G4VisAttributes* va_Geom = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
	va_Geom->SetForceWireframe(true);
	lv_Geom->SetVisAttributes(va_Geom);

	G4VisAttributes* va_Rep = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0,0.3));
	va_Rep->SetForceWireframe(true);
	lv_RepX->SetVisAttributes(va_Rep);
	lv_RepY->SetVisAttributes(va_Rep);

	G4VisAttributes* va_Phantom = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
	va_Phantom->SetForceSolid(true);
	polyLogic_Middle->SetVisAttributes(va_Phantom);
	polyLogic_Upper->SetVisAttributes(va_Phantom);


	G4VisAttributes* va_Phantom_teflon = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
	va_Phantom_teflon->SetForceSolid(true);
	if (USE_HOMO_PHANTOM){
		polyLogic_teflon->SetVisAttributes(va_Phantom_teflon);
	}


	return pv_World;
}

void DetectorConstruction::ConstructSDandField()
{
	// Set sensitive detector on "Geom"
	SetSensitiveDetector("RepY", new SensitiveDetector("ExitDoseDosimeter"));
	SetSensitiveDetector("RepY_Invivo", new SensitiveDetectorInPhantom("InvivoDoseDosimeter"));
}
