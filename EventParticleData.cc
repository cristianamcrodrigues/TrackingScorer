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
	fNtuple->RegisterColumnS(&fVolume, "Volume");
	fNtuple->RegisterColumnI(&fParent, "Parent ID");
	fNtuple->RegisterColumnF(&fTrackLen, "Track Length", "mm");
	fNtuple->RegisterColumnI(&fTrackNo, "Track ID");		
	
        // Create hits collection
	fHitsCollection = new TrackerHitClasssCollection(); 
	
	if (fPm->ParameterExists(GetFullParmName("Component"))){
		fScoringVolume = fPm->GetStringParameter(GetFullParmName("Component"));
	}
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
		//if (fEnergyDeposit <= 0.) return false;  // if not commented then particle that are created in scoring volume but that not deposit energy are not accounted.
		
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
		
		TsTrackInformation* parentInformation = (TsTrackInformation*)(aStep->GetTrack()->GetUserInformation());
		if (parentInformation) {
			std::vector<G4VPhysicalVolume*> volumes = parentInformation->GetInteractionVolumes();
			if (volumes.size()==0)
				fLastVolume = aStep->GetTrack()->GetOriginTouchable()->GetVolume()->GetName();
			else
				fLastVolume = volumes.front()->GetName();
		} else {
			fLastVolume = aStep->GetTrack()->GetOriginTouchable()->GetVolume()->GetName();
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
		  newHit->SetVolume(fLastVolume);

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
        fEdep_e = 0.;
 	
	if ( NoHits > 0 ) {
		for ( G4int i = 1; i < NoHits ; i ++ ) {
			if ((*fHitsCollection)[i]->GetIncidentEnergy()>0){
				if ((*fHitsCollection)[i]->GetTrackID() == (*fHitsCollection)[i-1]->GetTrackID()){					
					
					//fEdep += (*fHitsCollection)[i]->GetEdep();
					
					if ((*fHitsCollection)[i]->GetParticleID() == 22) {
						fEdep_phot += (*fHitsCollection)[i-1]->GetEdep();
					}
					else if ((*fHitsCollection)[i]->GetParticleID() == 11 || (*fHitsCollection)[i]->GetParticleID() == -11) {
						fEdep_e += (*fHitsCollection)[i-1]->GetEdep();
					}
					else if ((*fHitsCollection)[i]->GetParticleID() == 2212) {
						fEdep_p += (*fHitsCollection)[i-1]->GetEdep();
					}
					else{
						fEdep_ion += (*fHitsCollection)[i-1]->GetEdep();
					}
					
					if ((*fHitsCollection)[i]->GetVolume() != fScoringVolume && (*fHitsCollection)[i]->GetParticleID() == 11){
						fEkin_e += (*fHitsCollection)[i-1]->GetProtonIncidentEnergy();
						fEkin_res += (*fHitsCollection)[i]->GetProtonIncidentEnergy();
					}
					
					if ((*fHitsCollection)[i]->GetParticleID() == 2212){
						fEkin_p += (*fHitsCollection)[i-1]->GetProtonIncidentEnergy();
						fEkin_res += (*fHitsCollection)[i]->GetProtonIncidentEnergy();	
					}
										
					if (i == NoHits-1){
				
						fEventNo = (*fHitsCollection)[i]->GetEventID();
						fParticleType = (*fHitsCollection)[i]->GetParticleID();
						
						fPositionX = (*fHitsCollection)[i]->GetPos().x();
						fPositionY = (*fHitsCollection)[i]->GetPos().y();
						fPositionZ = (*fHitsCollection)[i]->GetPos().z();			
						
						if (fParticleType == 2212){
							fEdep = fEdep_p + (*fHitsCollection)[i]->GetEdep();
							fEdep_p = 0.;
							fEkin = fEkin_p - fEkin_res + (*fHitsCollection)[i]->GetProtonIncidentEnergy();
							fEkin_p = 0;
							fEkin_res = 0;
						}
						else if (fParticleType == 11 || fParticleType == -11){
							fEdep = fEdep_e + (*fHitsCollection)[i]->GetEdep();
							fEdep_e = 0.;
							fEkin = (*fHitsCollection)[i]->GetIncidentEnergy();
							if ((*fHitsCollection)[i]->GetVolume() != fScoringVolume){
								fEkin = fEkin_e - fEkin_res + (*fHitsCollection)[i]->GetProtonIncidentEnergy();
								fEkin_e = 0;
								fEkin_res = 0;
							}
						}
						else if (fParticleType == 22){
							fEdep = fEdep_phot + (*fHitsCollection)[i]->GetEdep();
							fEdep_phot = 0.;
							fEkin = (*fHitsCollection)[i]->GetIncidentEnergy();
						}
						else{ // heavy ions with low range or neutrons
							fEkin = (*fHitsCollection)[i]->GetIncidentEnergy();
							if ((*fHitsCollection)[i]->GetTrackID() == (*fHitsCollection)[i-1]->GetTrackID()){
								fEdep = fEdep_ion + (*fHitsCollection)[i]->GetEdep();
								fEdep_ion = 0.;
							}
							else{ // heavy ions with low range or neutrons
								fEdep = (*fHitsCollection)[i]->GetEdep();
							}
						}
						
						fTrackLen = (*fHitsCollection)[i]->GetTrackLength();
						fTrackNo = (*fHitsCollection)[i]->GetTrackID();
						fParent = (*fHitsCollection)[i]->GetParentID();
						fProcess = (*fHitsCollection)[i]->GetProcess();
						fVolume = (*fHitsCollection)[i]->GetVolume();
						
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
					
					if (fParticleType == 2212){ // or primary process?
						fEdep = fEdep_p + (*fHitsCollection)[i-1]->GetEdep();
						fEdep_p = 0.;
						fEkin = fEkin_p - fEkin_res + (*fHitsCollection)[i-1]->GetProtonIncidentEnergy();
						fEkin_p = 0;
						fEkin_res = 0;
					}
					else if (fParticleType == 11 || fParticleType == -11 && (*fHitsCollection)[i-1]->GetTrackID() == (*fHitsCollection)[i-2]->GetTrackID()){
						fEdep = fEdep_e + (*fHitsCollection)[i-1]->GetEdep();
						fEdep_e = 0.;
						fEkin = (*fHitsCollection)[i-1]->GetIncidentEnergy();
							if ((*fHitsCollection)[i-1]->GetVolume() != fScoringVolume){
								fEkin = fEkin_e - fEkin_res + (*fHitsCollection)[i-1]->GetProtonIncidentEnergy();
								fEkin_e = 0;
								fEkin_res = 0;
							}
					}
					else if (fParticleType == 11 || fParticleType == -11 && (*fHitsCollection)[i-1]->GetTrackID() != (*fHitsCollection)[i-2]->GetTrackID()) {
						fEdep = (*fHitsCollection)[i-1]->GetEdep();
						fEkin = (*fHitsCollection)[i-1]->GetIncidentEnergy();
					}
					else if (fParticleType == 22 && (*fHitsCollection)[i-1]->GetTrackID() == (*fHitsCollection)[i-2]->GetTrackID()){
						fEdep = fEdep_phot + (*fHitsCollection)[i-1]->GetEdep();
						fEdep_phot = 0.;
						fEkin = (*fHitsCollection)[i-1]->GetIncidentEnergy();
					}
					else if (fParticleType == 22 && (*fHitsCollection)[i-1]->GetTrackID() != (*fHitsCollection)[i-2]->GetTrackID()) {
						fEdep = (*fHitsCollection)[i-1]->GetEdep();
						fEkin = (*fHitsCollection)[i-1]->GetIncidentEnergy();
					}
					else{   // heavy ions with low range or neutrons
						fEkin = (*fHitsCollection)[i-1]->GetIncidentEnergy();
						if ((*fHitsCollection)[i-1]->GetTrackID() == (*fHitsCollection)[i-2]->GetTrackID()){
							fEdep = fEdep_ion + (*fHitsCollection)[i-1]->GetEdep();
							fEdep_ion = 0.;
						}
						else{
							fEdep = (*fHitsCollection)[i-1]->GetEdep();
						}
					}
					
					fTrackLen = (*fHitsCollection)[i-1]->GetTrackLength();
					fTrackNo = (*fHitsCollection)[i-1]->GetTrackID();
					fParent = (*fHitsCollection)[i-1]->GetParentID();
					fProcess = (*fHitsCollection)[i-1]->GetProcess();  
					fVolume = (*fHitsCollection)[i-1]->GetVolume();
					
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

