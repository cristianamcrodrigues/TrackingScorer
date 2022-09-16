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
	fNtuple->RegisterColumnI(&fParticleType, "Particle Type (in PDG Format)");
	//fNtuple->RegisterColumnS(&fOriginProcessName, "Origin Process");
	//fNtuple->RegisterColumnS(&fLastVolumeName, "Last Volume");
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
		
		//secondaries = aStep->GetSecondaryInCurrentStep();
		//G4cout << secondaries << G4endl;
		
	        fEventID        = GetEventID();
		fPType          = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
		//fPosX           = pos.x();
		//fPosY           = pos.y();
		//fPosZ           = pos.z();
		
		fEnergyDeposit  = aStep->GetTotalEnergyDeposit();
		if (fEnergyDeposit <= 0.) return false;
		
		//fKineticEnergy  = aStep->GetPreStepPoint()->GetKineticEnergy();
		fKineticEnergy = aStep->GetTrack()->GetVertexKineticEnergy();
		
		fparentId       = aStep->GetTrack()->GetParentID();
		fTrackLength    = aStep->GetTrack()->GetTrackLength();
		fTrackID        = aStep->GetTrack()->GetTrackID();
		
		//TrackID.push_back(fTrackID);
		//std::cout << fTrackID << std::endl;
	
		const G4VProcess* originProcess = aStep->GetTrack()->GetCreatorProcess();
		if (originProcess){
			fOriginProcessName = originProcess->GetProcessName();}
		else{
			fOriginProcessName = "Primary";
		}  
		
		TsTrackInformation* parentInformation = (TsTrackInformation*)(aStep->GetTrack()->GetUserInformation());
		if (parentInformation) {
			std::vector<G4VPhysicalVolume*> volumes = parentInformation->GetInteractionVolumes();
			if (volumes.size()==0)
				fLastVolumeName = aStep->GetTrack()->GetOriginTouchable()->GetVolume()->GetName();
			else
				fLastVolumeName = volumes.front()->GetName();
		} else {
			fLastVolumeName = aStep->GetTrack()->GetOriginTouchable()->GetVolume()->GetName();
		}
					 
		  //TsTrackerHit* newHit = new TsTrackerHit();
		  TrackerHitClass* newHit = new TrackerHitClass();
		  
		  newHit->SetEventID(fEventID);
		  newHit->SetParticleID(fPType);
		  newHit->SetPos(pos);
		  newHit->SetEdep(fEnergyDeposit);
		  newHit->SetIncidentEnergy(fKineticEnergy);
		  newHit->SetParentID(fparentId);
		  newHit->SetTrackLength(fTrackLength);
		  newHit->SetTrackID(fTrackID);

		  fHitsCollection->insert(newHit);

		  //newHit->Print();
        
       return true;  
}

/*

FUNCIONA

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

	if ( NoHits > 0 ) {
		for ( G4int i = 0; i < NoHits ; i ++ ) {
			if ((*fHitsCollection)[i]->GetIncidentEnergy()>0){
				if ((*fHitsCollection)[i]->GetTrackID() == 1){
				
					fEventNo = (*fHitsCollection)[i]->GetEventID();
					fParticleType = (*fHitsCollection)[i]->GetParticleID();
					
					fPositionX = (*fHitsCollection)[i]->GetPos().x();
					fPositionY = (*fHitsCollection)[i]->GetPos().y();
					fPositionZ = (*fHitsCollection)[i]->GetPos().z();			
					
					fEkin = (*fHitsCollection)[i]->GetIncidentEnergy();
					fEdep += (*fHitsCollection)[i]->GetEdep();
					
					fTrackLen = (*fHitsCollection)[i]->GetTrackLength();
					fTrackNo = (*fHitsCollection)[i]->GetTrackID();
					fParent = (*fHitsCollection)[i]->GetParentID();
					
				}
			}
		}
	}
	
	if (fEdep > 0.){
        	fNtuple->Fill();
        }

	delete fHitsCollection;
	fHitsCollection = new TrackerHitClasssCollection();
}

*/

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
        
       //G4TrackVector* secondaries = TsScoringManager->GimmeSecondaries();
       //size_t nbtrk = (*secondaries).size();
       
       // NÃO ESTA A FUNCIOANR ASSIM RIP, REVER O DOS PROTOES E COLOCAR NUM SCORER Á PARTE E DEIXAR ESTE SO PARA SECUNDARIOS
       
       
       /*
	if ( NoHits > 0 ) {
		for ( G4int k = 0; k < NoHits ; k ++ ) {
			if ((*fHitsCollection)[k]->GetIncidentEnergy()>0){
				if ((*fHitsCollection)[k]->GetTrackID() != (*fHitsCollection)[k+1]->GetTrackID()){
					TrackID.push_back((*fHitsCollection)[k]->GetTrackID());
				}
			}
		}
	}  
	//std::cout << TrackID.size() << std::endl;     
	//std::cout << NoHits << std::endl; 
	*/
	
	// tenho de por uma condição entra para o ultimo valor em que NoHits = max	
	
	
	if ( NoHits > 0 ) {
		for ( G4int i = 0; i < (NoHits-1) ; i ++ ) {
			if ((*fHitsCollection)[i]->GetIncidentEnergy()>0){
				if ((*fHitsCollection)[i]->GetTrackID() == (*fHitsCollection)[i+1]->GetTrackID()){
				//if ((*fHitsCollection)[i]->GetTrackID() == 910 || (*fHitsCollection)[i]->GetTrackID() == 911){
				
					fEventNo = (*fHitsCollection)[i]->GetEventID();
					fParticleType = (*fHitsCollection)[i]->GetParticleID();
					
					fPositionX = (*fHitsCollection)[i]->GetPos().x();
					fPositionY = (*fHitsCollection)[i]->GetPos().y();
					fPositionZ = (*fHitsCollection)[i]->GetPos().z();			
					
					fEkin = (*fHitsCollection)[i]->GetIncidentEnergy();
					fEdep += (*fHitsCollection)[i]->GetEdep();
					
					fTrackLen = (*fHitsCollection)[i]->GetTrackLength();
					fTrackNo = (*fHitsCollection)[i]->GetTrackID();
					fParent = (*fHitsCollection)[i]->GetParentID();
					
				}
				else{
					if (fEdep > 0.){
						fNtuple->Fill();
					}
				}
			}
		}
	}	
	
	/*
	if (fEdep > 0.){
        	fNtuple->Fill();
        	//std::cout << "tracks" << TrackID.size() << std::endl;     
		std::cout << "Hits" << NoHits << std::endl;  
        }
        
        */

	delete fHitsCollection;
	fHitsCollection = new TrackerHitClasssCollection();
}

