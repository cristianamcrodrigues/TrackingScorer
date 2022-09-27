//
//
// Header file for EventParticleData

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

#ifndef EventParticleData_hh
#define EventParticleData_hh

#include "TsVNtupleScorer.hh"
#include "G4SDManager.hh"
//#include "TsTrackerHit.hh"
#include "TrackerHitClass.hh"

class EventParticleData : public TsVNtupleScorer
{
public:
    EventParticleData(TsParameterManager* pM, TsMaterialManager* mM, TsGeometryManager* gM, TsScoringManager* scM, TsExtensionManager* eM,
                G4String scorerName, G4String quantity, G4String outFileName, G4bool isSubScorer);
    
    virtual ~EventParticleData();

    G4bool ProcessHits(G4Step*,G4TouchableHistory*);
    virtual void UserHookForEndOfEvent();

private:

        //TsTrackerHitsCollection* fHitsCollection;
        TrackerHitClasssCollection* fHitsCollection;
        
        G4String fScoringVolume;
        
	// Output variables
	G4int fEventID;
	//G4float fPosX;
	//G4float fPosY;
	//G4float fPosZ;
	G4float fEnergyDeposit;
	G4float fKineticEnergy_proton;
	G4float fKineticEnergy;	
	G4int fPType;
	G4String fOriginProcessName;
	G4String fLastVolume;
	G4int fparentId;
	G4float fTrackLength;
	G4int fTrackID;
	
	G4int fEventNo;
	G4int fParticleType;
	G4float fPositionX;
	G4float fPositionY;
	G4float fPositionZ;			
	G4float fEkin;
	G4float fEdep;
	G4float fTrackLen;
	G4int fTrackNo;
	G4int fParent;
	G4String fProcess;	
	G4String fVolume;
	
	G4int fStepNumber;
	
	G4double fEdep_phot;
	G4double fEdep_e;
	
	G4double fEkin_e;
	G4double fEkin_res;
};
#endif
