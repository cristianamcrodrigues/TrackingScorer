//
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
// This example is provided by the Geant4-DNA collaboration
// Any report or published results obtained using the Geant4-DNA software
// shall cite the following Geant4-DNA collaboration publication:
// Med. Phys. 37 (2010) 4692-4708
// The Geant4-DNA web site is available at http://geant4-dna.org
//
// $Id: TrackerHitClass.hh 69706 2013-05-13 09:12:40Z gcosmo $
//
/// \file TrackerHitClass.hh
/// \brief Definition of the TrackerHitClass class

#ifndef TrackerHitClass_h
#define TrackerHitClass_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"

/// Tracker hit class
///
/// It defines data members to store the trackID, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fEdep, fPos

class TrackerHitClass : public G4VHit
{
  public:
    TrackerHitClass();
    TrackerHitClass(const TrackerHitClass&);
    virtual ~TrackerHitClass();

    // operators
    const TrackerHitClass& operator=(const TrackerHitClass&);
    G4int operator==(const TrackerHitClass&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // Set methods
    void SetTrackID  (G4int track)      { fTrackID = track; };
    void SetEdep     (G4double de)      { fEdep = de; };
    void SetPos      (G4ThreeVector xyz){ fPos = xyz; };
    void SetIncidentEnergy (G4double e) {fIncidentEnergy = e;};
    void SetParticleID(G4int id) {fParticleID = id;}
    void SetTrackLength (G4double tracklength) {fTrackLength = tracklength;};
    void SetEventID (G4int event) {fEventID = event;};
    void SetParentID (G4int parent) {fParentID = parent;};
    void SetProcess (G4String process) {fProcess = process;};
    
      // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4double GetEdep() const     { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };
    G4double GetIncidentEnergy() const {return fIncidentEnergy;};
    G4int GetParticleID() const {return fParticleID;}
    G4double GetTrackLength() const {return fTrackLength;}
    G4int GetEventID() const {return fEventID;}
    G4int GetParentID() const {return fParentID;}
    G4String GetProcess() const {return fProcess;}


  private:

      G4int         fTrackID;
      G4double      fEdep;
      G4ThreeVector fPos;
      G4int         fParticleID;
      G4double      fIncidentEnergy;
      G4double      fTrackLength;
      G4int         fEventID;
      G4int         fParentID;
      G4String      fProcess;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<TrackerHitClass> TrackerHitClasssCollection;

extern G4ThreadLocal G4Allocator<TrackerHitClass>* TrackerHitClassAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* TrackerHitClass::operator new(size_t)
{
  if(!TrackerHitClassAllocator)
      TrackerHitClassAllocator = new G4Allocator<TrackerHitClass>;
  return (void *) TrackerHitClassAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void TrackerHitClass::operator delete(void *hit)
{
  TrackerHitClassAllocator->FreeSingle((TrackerHitClass*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
