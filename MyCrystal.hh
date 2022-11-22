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

#ifndef MyCrystal_hh
#define MyCrystal_hh

#include "TsVGeometryComponent.hh"

class MyCrystal : public TsVGeometryComponent
{    
public:
	MyCrystal(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
				  TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name);
	~MyCrystal();
	
	//void DefineMaterials();
	G4VPhysicalVolume* Construct();

//private:
	//void ConstructSDandField();
};

#endif


