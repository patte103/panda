/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef FINITESTRAINCRYSTALPLASTICITYDAMAGEAMORMIEGRUNEISEN_H
#define FINITESTRAINCRYSTALPLASTICITYDAMAGEAMORMIEGRUNEISEN_H

#include "FiniteStrainCrystalPlasticityDamageAmorMieGruneisen.h"

/**
 * FiniteStrainCrystalPlasticityDamageAmorMieGruneisen uses the multiplicative decomposition of deformation gradient
 * and solves the PK2 stress residual equation at the intermediate configuration to evolve the material state.
 * The internal variables are updated using an interative predictor-corrector algorithm.
 * Backward Euler integration rule is used for the rate equations.
 * Principal values of the Lagrangian strain used to calculate damage with Amor 2009 model
 * Calculation of the broken plastic energy for temperature calculation
 * Mie Gruneisen equation of state
 * finite strain formalism as in Luscher2017
 * Computes the stress and free energy derivatives for the phase field
 * Allen-Cahn formalism
 */
class FiniteStrainCrystalPlasticityDamageAmorMieGruneisen;

template<>
InputParameters validParams<FiniteStrainCrystalPlasticityDamageAmorMieGruneisen>();

class FiniteStrainCrystalPlasticityDamageAmorMieGruneisen : public FiniteStrainCrystalPlasticity
{
public:
  FiniteStrainCrystalPlasticityDamageAmorMieGruneisen(const InputParameters & parameters);

protected:
  /**
   * This function set variables for internal variable solve.
   */
  virtual void preSolveStatevar();

  /**
   * This function solves internal variables.
   */
  virtual void solveStatevar();

  /**
   * This function update internal variable after solve.
   */
  virtual void postSolveStatevar();


  // update slip system resistances and output slip increment
  virtual void updateGss();

  /**
   * Update elastic and plastic work
   */
  virtual void update_energies();

  /**
   * This function calculate stress residual.
   */
  virtual void calcResidual( RankTwoTensor & );

  virtual void getSlipIncrements();

  // damage phase field
  const VariableValue & _c;

  // temperature
  const VariableValue & _temp;

  /// Small number to avoid non-positive definiteness at or near complete damage
  const Real _kdamage;

  // Gruneisen G (or Gamma) parameter
  const Real _G_Gruneisen;

  // Us-Up slope in Mie-Gruneisen EOS
  const Real _s_UsUp;

  // reference bulk modulus
  const Real _Bulk_Modulus_Ref;

  // KT0 prime correction to the bulk modulus (Menikoff 2001)
  //const Real _Bulk_Modulus_Cor;

  // Von Neumann coefficient
  const Real _C0;

  // Landshoff coefficient
  const Real _C1;

  // volumetric thermal exapnsion coeffcient, as in Austin Barton 2015
  //const Real _thermal_expansion;

  // reference temperature, as in Luscher2017
  const Real _reference_temperature;

  // prefactor of the plastic contribution to damage
  //const Real _plastic_factor;

  const MaterialProperty<Real> & _specific_heat;
  const MaterialProperty<Real> & _density;

  /// Material property defining crack width, declared elsewhere
  //const MaterialProperty<Real> & _l;

  /// Material property defining gc parameter, declared elsewhere
  //const MaterialProperty<Real> & _gc;

  MaterialProperty<Real> & _W0e;
  MaterialProperty<Real> & _W0p;
  const MaterialProperty<Real> & _W0p_old;
  MaterialProperty<Real> & _W0p_broken;
  const MaterialProperty<Real> & _W0p_broken_old;
  MaterialProperty<RankTwoTensor> & _dstress_dc;
  MaterialProperty<RankTwoTensor> & _dW0e_dstrain;
  MaterialProperty<RankTwoTensor> & _dW0p_dstrain;
  MaterialProperty<RankTwoTensor> & _dW0p_broken_dstrain;
  MaterialProperty<RankTwoTensor> & _pk2_undamaged;
  MaterialProperty<RankTwoTensor> & _fe_out; // Elastic deformation gradient for output
  MaterialProperty<std::vector<Real>> & _slip_incr_out; // slip increment output

  Real _W0p_tmp;
  Real _W0p_tmp_old;
  Real _W0p_broken_tmp;
  Real _W0p_broken_tmp_old;  

};

#endif //FINITESTRAINCRYSTALPLASTICITYDAMAGEAMORMIEGRUNEISEN_H
