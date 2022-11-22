// Component for MyCrystal
//
// ********************************************************************
// *                                                                  *
// *                                                                  *
// * This file was obtained from Topas MC Inc under the license       *
// * agreement set forth at http://www.topasmc.org/registration       *
// * Any use of this file constitutes full acceptance of              *
// * this TOPAS MC license agreement.                                 *
// *                                                                  *
// ********************************************************************
//
// Rectangular Crystal

#include "MyCrystal.hh"

#include "TsParameterManager.hh"
#include "G4NistManager.hh"

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"

#include "G4CrystalExtension.hh"
#include "G4ExtendedMaterial.hh"
#include "G4LogicalCrystalVolume.hh"

#include "G4ChannelingMaterialData.hh"

MyCrystal::MyCrystal(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
				   TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name)
	: TsVGeometryComponent(pM, eM, mM, gM, parentComponent, parentVolume, name)
{
}

MyCrystal::~MyCrystal()
{
}

G4VPhysicalVolume* MyCrystal::Construct()
{
	BeginConstruction();

	// get information from parameters
	G4double totalHLX = fPm->GetDoubleParameter(GetFullParmName("HLX"), "Length");
	G4double totalHLY = fPm->GetDoubleParameter(GetFullParmName("HLY"), "Length");
	G4double totalHLZ = fPm->GetDoubleParameter(GetFullParmName("HLZ"), "Length");
	
	G4double BRx = fPm->GetDoubleParameter(GetFullParmName("BendingRadiusX"), "Length");
	G4double BRy = fPm->GetDoubleParameter(GetFullParmName("BendingRadiusY"), "Length");
	G4double BRz = fPm->GetDoubleParameter(GetFullParmName("BendingRadiusZ"), "Length");
	G4ThreeVector fBR = G4ThreeVector(BRx,BRy,BRz);
	
	G4double CellSizeX = fPm->GetDoubleParameter(GetFullParmName("CrystalCellSizeX"), "Length");
	G4double CellSizeY = fPm->GetDoubleParameter(GetFullParmName("CrystalCellSizeY"), "Length");
	G4double CellSizeZ = fPm->GetDoubleParameter(GetFullParmName("CrystalCellSizeZ"), "Length");
	
	G4double CellAngleX = fPm->GetDoubleParameter(GetFullParmName("CrystalCellAngleX"), "Angle");
	G4double CellAngleY = fPm->GetDoubleParameter(GetFullParmName("CrystalCellAngleY"), "Angle");
	G4double CellAngleZ = fPm->GetDoubleParameter(GetFullParmName("CrystalCellAngleZ"), "Angle");
	
	G4int LatticeSystem = fPm->GetUnitlessParameter(GetFullParmName("LatticeSystem"));
	G4String Material = fPm->GetStringParameter(GetFullParmName("Material"));
	
	G4String fECfileName = fPm->GetStringParameter(GetFullParmName("ChannelingData"));

	// crystal definition
	G4Box* crystalSolid = new G4Box(fName+".solid", totalHLX, totalHLY, totalHLZ);
	
	G4Material* mat = G4NistManager::Instance()->FindOrBuildMaterial(Material);
	G4ExtendedMaterial* Crystal = new G4ExtendedMaterial(fName+".material",mat);
	
	Crystal->RegisterExtension(std::unique_ptr<G4CrystalExtension>(new G4CrystalExtension(Crystal)));
	G4CrystalExtension* crystalExtension = (G4CrystalExtension*)Crystal->RetrieveExtension("crystal");
	crystalExtension->SetUnitCell(new G4CrystalUnitCell(CellSizeX * CLHEP::angstrom,
							      CellSizeY * CLHEP::angstrom,
							      CellSizeZ * CLHEP::angstrom,
							      CellAngleX,
							      CellAngleY,
							      CellAngleZ,
							      LatticeSystem));
    
	Crystal->RegisterExtension(std::unique_ptr<G4ChannelingMaterialData>(new G4ChannelingMaterialData("channeling")));
	G4ChannelingMaterialData* crystalChannelingData = (G4ChannelingMaterialData*)Crystal->RetrieveExtension("channeling");
	crystalChannelingData->SetFilename(fECfileName);

	if(fBR!=G4ThreeVector()){
		crystalChannelingData->SetBR(fBR.x());
	}
	
	G4LogicalCrystalVolume* fcrystalLogic = new G4LogicalCrystalVolume(crystalSolid, Crystal, fName+".logic");
	G4VPhysicalVolume* fcrystalPhys = CreatePhysicalVolume(fcrystalLogic);
	
	InstantiateChildren(fcrystalPhys);
	
	return fcrystalPhys;
}
   
