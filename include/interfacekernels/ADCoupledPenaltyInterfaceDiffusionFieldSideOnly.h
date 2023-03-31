#include "ADInterfaceKernel.h"

/**
 * DG kernel for interfacing diffusion between two variables on adjacent blocks
 */
class ADCoupledPenaltyInterfaceDiffusionFieldSideOnly : public ADInterfaceKernel
{
public:
  static InputParameters validParams();

  ADCoupledPenaltyInterfaceDiffusionFieldSideOnly(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual(Moose::DGResidualType type) override;

  const unsigned int _dim;
  const Real _penalty;
  const Real _component;
  const ADVariableValue & _primary_coupled_value;
  const ADVectorVariableValue & _secondary_coupled_vector_value;

};