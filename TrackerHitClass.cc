// Extra Class for use by EventParticleData
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
// $Id: TrackerHitClass.cc 69706 2013-05-13 09:12:40Z gcosmo $
//
/// \file TrackerHitClass.cc
/// \brief Implementation of the TrackerHitClass class

#include "TrackerHitClass.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<TrackerHitClass>* TrackerHitClassAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackerHitClass::TrackerHitClass()
: 		 G4VHit(),
		 fTrackID(-1),
		 fEdep(0.),
		 fPos(G4ThreeVector()),
		 fParticleID(-1),
		 fIncidentEnergy(0.),
		 fProtonIncidentEnergy(0.),
		 fTrackLength(0.),
		 fEventID(-1),
		 fParentID(-1),
		 fProcess(""),
		 fVolume(""),
		 fStepNumber(-1)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackerHitClass::~TrackerHitClass() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackerHitClass::TrackerHitClass(const TrackerHitClass& right)
  : G4VHit()
{
  fTrackID   = right.fTrackID;
  fEdep      = right.fEdep;
  fPos       = right.fPos;
  fProtonIncidentEnergy = right.fProtonIncidentEnergy;
  fIncidentEnergy = right.fIncidentEnergy;
  fParticleID = right.fParticleID;
  fTrackLength = right.fTrackLength;
  fEventID = right.fEventID;
  fParentID = right.fParentID;
  fProcess = right.fProcess;
  fVolume = right.fVolume;
  fStepNumber = right.fStepNumber;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const TrackerHitClass& TrackerHitClass::operator=(const TrackerHitClass& right)
{
  fTrackID   = right.fTrackID;
  fEdep      = right.fEdep;
  fPos       = right.fPos;
  fProtonIncidentEnergy = right.fProtonIncidentEnergy;
  fIncidentEnergy = right.fIncidentEnergy;
  fParticleID = right.fParticleID;
  fTrackLength = right.fTrackLength;
  fEventID = right.fEventID;
  fParentID = right.fParentID;
  fProcess = right.fProcess;
  fVolume = right.fVolume;
  fStepNumber = right.fStepNumber;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int TrackerHitClass::operator==(const TrackerHitClass& right) const
{
  return ( this == &right ) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


