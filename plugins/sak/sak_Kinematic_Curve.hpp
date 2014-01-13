/*******************************************************************
Code: sak_KinematicCurve.hpp

Description: This class gives the kinematic curves as a TGraph
  object. The masses for the reaction and the energy of excitation
  must be set before getting the curve.

Author: Daniel Santiago-Gonzalez
2013-March
*******************************************************************/
namespace sak{
  TGraph GetCurve(int Points,const double& hi_ex_set);
  TGraph GetSecondaryDecayCurve(int Points,const double & hi_ex_set,const double & decay_ex_set);
}
