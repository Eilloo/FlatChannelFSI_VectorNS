#include "ADInterfaceKernel.h"

/**
 * DG kernel for interfacing diffusion between two variables on adjacent blocks
 */
class ADCoupledPenaltyInterfaceDiffusionVectorSideOnly : public ADVectorInterfaceKernel
{
public:
  static InputParameters validParams();

  ADCoupledPenaltyInterfaceDiffusionVectorSideOnly(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual(Moose::DGResidualType type) override;

  const unsigned int _dim;
  const Real _penalty;
  const ADVectorVariableValue & _primary_coupled_vector_value;
  const ADVariableValue & _secondary_coupled_value_x;
  const ADVariableValue & _secondary_coupled_value_y;
  const ADVariableValue & _secondary_coupled_value_z;
};
