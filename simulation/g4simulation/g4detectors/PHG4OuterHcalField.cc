// $Id: $

/*!
 * \file PHG4OuterHcalField.cc
 * \brief
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision:   $
 * \date $Date: $
 */

#include "PHG4OuterHcalField.h"

#include <Geant4/G4Vector3D.hh>
#include <Geant4/G4Transform3D.hh>
#include <Geant4/G4FieldManager.hh>
#include <Geant4/G4TransportationManager.hh>
#include <Geant4/G4EquationOfMotion.hh>
#include <Geant4/G4PhysicalConstants.hh>
#include <Geant4/G4SystemOfUnits.hh>
#include <iostream>
using namespace std;

PHG4OuterHcalField::PHG4OuterHcalField(bool isInIron, G4int steelPlates,
    G4double scintiGap, G4double tiltAngle) :
    /*double*/relative_permeability_absorber(1514), // relative permeability for Steel 1006 @ B = 1.06T
    /*double*/relative_permeability_gap(1),
    /*bool*/is_in_iron(isInIron),
    /*G4int*/n_steel_plates(steelPlates),
    /*G4double*/scinti_gap(scintiGap),
    /*G4double*/tilt_angle(tiltAngle)
{
}

PHG4OuterHcalField::~PHG4OuterHcalField()
{

}

void
PHG4OuterHcalField::GetFieldValue(const double Point[4], double *Bfield) const
{
  G4FieldManager* field_manager =
      G4TransportationManager::GetTransportationManager()->GetFieldManager();

  if (!field_manager)
    {
      static bool once = true;

      if (once)
        {
          once = false;
          cout << "PHG4OuterHcalField::GetFieldValue"
              << " - Error! can not find field manager in G4TransportationManager"
              << endl;
        }
    }

  const G4Field* default_field = field_manager->GetDetectorField();

  if (default_field)
    {
      default_field->GetFieldValue(Point, Bfield);

      // scale_factor for field component along the plate surface
      double x = Point[0];
      double y = Point[1];
//      double z = Point[2];

      assert(cos(tilt_angle)>0);
      assert(n_steel_plates>0);

      const double R = sqrt(x * x + y * y);
      const double layer_width = R * twopi / n_steel_plates * cos(tilt_angle);
      const double gap_width = scinti_gap;

      assert(gap_width<layer_width);

      double scale_factor = layer_width
          / (relative_permeability_absorber * (layer_width - gap_width)
              + relative_permeability_gap * gap_width);
      scale_factor *=
          is_in_iron ?
              relative_permeability_absorber : relative_permeability_gap;

      // input 2D magnetic field vector
      const G4Vector3D B0(Bfield[0], Bfield[1], Bfield[2]);

      // decompose to B_perp and B_T (along the plate surface)
      const G4Vector3D absorber_perb(
          cos(atan2(y, x) + tilt_angle + halfpi),
          sin(atan2(y, x) + tilt_angle + halfpi), 0);
      const G4Vector3D B_perp = B0 .cross( absorber_perb);
      const G4Vector3D B_T = B0 - B_perp;

      // scale B_T component
      G4Vector3D BScaled = B_perp + B_T * scale_factor;

      static bool once = true;
      if (once)
        {
          once = false;
          cout << "PHG4OuterHcalField::GetFieldValue"
              << " - After burner to produce magnetic field in inner HCal. "
              << (is_in_iron ? "Inside iron, " : "Inside gap, ") << "and R = "
              << R / cm
              << " cm, fiend change from " //
              << "(" << B0.x() / tesla << "," << B0.y() / tesla << ","
              << B0.z() / tesla << ") T" << " T to " << "("
              << BScaled.x() / tesla << "," << BScaled.y() / tesla << ","
              << BScaled.z() / tesla << ") T" << " or x" << scale_factor
              << endl;
        }
    }
  else
    {
      static bool once = true;

      if (once)
        {
          once = false;
          cout << "PHG4OuterHcalField::GetFieldValue"
              << " - Error! can not find detecor field in field manager!"
              << endl;
        }
    }
}
