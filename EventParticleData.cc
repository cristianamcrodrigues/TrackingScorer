// Scorer for EventParticleData
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

#include "EventParticleData.hh"

#include "TsTrackInformation.hh"

#include "G4PSDirectionFlag.hh"
#include "G4VProcess.hh"
//#include "TsTrackerHit.hh"
#include "TrackerHitClass.hh"
#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>

EventParticleData::EventParticleData(TsParameterManager* pM, TsMaterialManager* mM, TsGeometryManager* gM, TsScoringManager* scM, TsExtensionManager* eM,
                          G4String scorerName, G4String quantity, G4String outFileName, G4bool isSubScorer)
                         : TsVNtupleScorer(pM, mM, gM, scM, eM, scorerName, quantity, outFileName, isSubScorer)
{
	pM->SetNeedsSteppingAction();
	
	// Set Scorer
        fNtuple->RegisterColumnI(&fEventNo, "Event ID");
	fNtuple->RegisterColumnF(&fPositionX, "Position X", "mm");
	fNtuple->RegisterColumnF(&fPositionY, "Position Y", "mm");
	fNtuple->RegisterColumnF(&fPositionZ, "Position Z", "mm");
	fNtuple->RegisterColumnF(&fEdep, "Energy Deposit", "keV");
	fNtuple->RegisterColumnF(&fEkin, "Initial Kinetic Energy", "MeV");	
	fNtuple->RegisterColumnI(&fParticleType, "Particle Type (PDG Format)");
	fNtuple->RegisterColumnS(&fProcess, "Origin Process");
	fNtuple->RegisterColumnI(&fParent, "Parent ID");
	fNtuple->RegisterColumnF(&fTrackLen, "Track Length", "mm");
	fNtuple->RegisterColumnI(&fTrackNo, "Track ID");		
	
        // Create hits collection
	fHitsCollection = new TrackerHitClasssCollection(); 
}

EventParticleData::~EventParticleData() {;}


G4bool EventParticleData::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
    if (!fIsActive) {
        fSkippedWhileInactive++;
        return false;
    }

    ResolveSolid(aStep);
    
		G4ThreeVector pos = aStep->GetPostStepPoint()->GetPosition();
		
	        fEventID        = GetEventID();
		fPType          = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
		//fPosX           = pos.x();
		//fPosY           = pos.y();
		//fPosZ           = pos.z();
		
		fEnergyDeposit  = aStep->GetTotalEnergyDeposit();
		if (fEnergyDeposit <= 0.) return false;
		
		fKineticEnergy_proton  = aStep->GetPreStepPoint()->GetKineticEnergy();
		fKineticEnergy = aStep->GetTrack()->GetVertexKineticEnergy();
		
		fparentId       = aStep->GetTrack()->GetParentID();
		fTrackLength    = aStep->GetTrack()->GetTrackLength();
		fTrackID        = aStep->GetTrack()->GetTrackID();
		
		const G4VProcess* originProcess = aStep->GetTrack()->GetCreatorProcess();
		if (originProcess){
			fOriginProcessName = originProcess->GetProcessName();}
		else{
			fOriginProcessName = "Primary";
		}  
					 
		  //TsTrackerHit* newHit = new TsTrackerHit();
		  TrackerHitClass* newHit = new TrackerHitClass();
		  
		  newHit->SetEventID(fEventID);
		  newHit->SetParticleID(fPType);
		  newHit->SetPos(pos);
		  newHit->SetEdep(fEnergyDeposit);
		  newHit->SetProtonIncidentEnergy(fKineticEnergy_proton);
		  newHit->SetIncidentEnergy(fKineticEnergy);
		  newHit->SetParentID(fparentId);
		  newHit->SetTrackLength(fTrackLength);
		  newHit->SetTrackID(fTrackID);
		  newHit->SetProcess(fOriginProcessName);

		  fHitsCollection->insert(newHit);

		  //newHit->Print();
        
       return true;  
}

void EventParticleData::UserHookForEndOfEvent()
{
        G4int NoHits = fHitsCollection->entries();  // number of hits in event
        
        fEventNo = 0.;
        fParticleType = 0.;
        fPositionX = 0.;
        fPositionY = 0.;
        fPositionZ = 0.;
        fEkin = 0.;
        fEdep = 0.;
        fTrackLen = 0.;
        fTrackNo = 0.;
        fParent = 0.;	
        fProcess = "";
        
        fEdep_phot = 0.;
 	
	if ( NoHits > 0 ) {
		for ( G4int i = 1; i < NoHits ; i ++ ) {
			if ((*fHitsCollection)[i]->GetIncidentEnergy()>0){
				if ((*fHitsCollection)[i]->GetTrackID() == (*fHitsCollection)[i-1]->GetTrackID()){
				
					//fEventNo = (*fHitsCollection)[i]->GetEventID();
					//fParticleType = (*fHitsCollection)[i]->GetParticleID();
					
					//fPositionX = (*fHitsCollection)[i]->GetPos().x();
					//fPositionY = (*fHitsCollection)[i]->GetPos().y();
					//fPositionZ = (*fHitsCollection)[i]->GetPos().z();			
					
					//fEkin = (*fHitsCollection)[i]->GetIncidentEnergy();					
					
					fEdep += (*fHitsCollection)[i]->GetEdep();
					
					if ((*fHitsCollection)[i]->GetParticleID() == 22) {
						fEdep_phot += (*fHitsCollection)[i]->GetEdep();
					}
									
					//fTrackLen = (*fHitsCollection)[i]->GetTrackLength();
					//fTrackNo = (*fHitsCollection)[i]->GetTrackID();
					//fParent = (*fHitsCollection)[i]->GetParentID();
					
					if (i == NoHits-1){
				
						fEventNo = (*fHitsCollection)[i]->GetEventID();
						fParticleType = (*fHitsCollection)[i]->GetParticleID();
						
						fPositionX = (*fHitsCollection)[i]->GetPos().x();
						fPositionY = (*fHitsCollection)[i]->GetPos().y();
						fPositionZ = (*fHitsCollection)[i]->GetPos().z();			
						
						if (fParticleType == 2212){
							fEkin = (*fHitsCollection)[0]->GetProtonIncidentEnergy();
						}
						else{
							fEkin = (*fHitsCollection)[0]->GetIncidentEnergy();
						}
						//fEdep += (*fHitsCollection)[i]->GetEdep();
						
						fTrackLen = (*fHitsCollection)[i]->GetTrackLength();
						fTrackNo = (*fHitsCollection)[i]->GetTrackID();
						fParent = (*fHitsCollection)[i]->GetParentID();
						fProcess = (*fHitsCollection)[i]->GetProcess();
						
						//if (fEdep > 0.){
							fNtuple->Fill();
						//}
					}	
				}
				else{ 
				
					fEventNo = (*fHitsCollection)[i-1]->GetEventID();
					fParticleType = (*fHitsCollection)[i-1]->GetParticleID();
					
					fPositionX = (*fHitsCollection)[i-1]->GetPos().x();
					fPositionY = (*fHitsCollection)[i-1]->GetPos().y();
					fPositionZ = (*fHitsCollection)[i-1]->GetPos().z();	
					
					if (fParticleType == 2212){
						fEkin = (*fHitsCollection)[0]->GetProtonIncidentEnergy();
					}
					else{
						fEkin = (*fHitsCollection)[i-1]->GetIncidentEnergy();
					}							
					
					//fEkin = (*fHitsCollection)[i-1]->GetIncidentEnergy();
					
					if (fParticleType == 2212){ // or primary process?
						fEdep = fEdep +  (*fHitsCollection)[0]->GetEdep();
					}
					else if (fParticleType == 22 && (*fHitsCollection)[i-1]->GetTrackID() == (*fHitsCollection)[i-2]->GetTrackID()){
						fEdep = fEdep_phot;
					}
					else if (fParticleType == 11 || fParticleType == -11) {   
						fEdep = fEkin;
					}
					else if (fParticleType == 22 && (*fHitsCollection)[i-1]->GetTrackID() != (*fHitsCollection)[i-2]->GetTrackID()) {
						fEdep = (*fHitsCollection)[i-1]->GetEdep();
					}
					else{
						fEdep = fEkin;
					}
					
					fTrackLen = (*fHitsCollection)[i-1]->GetTrackLength();
					fTrackNo = (*fHitsCollection)[i-1]->GetTrackID();
					fParent = (*fHitsCollection)[i-1]->GetParentID();
					fProcess = (*fHitsCollection)[i-1]->GetProcess();  
					
					//if (fEdep > 0.){
						fNtuple->Fill();
					//}
				}
			}
		}
	}

	delete fHitsCollection;
	fHitsCollection = new TrackerHitClasssCollection();
}

