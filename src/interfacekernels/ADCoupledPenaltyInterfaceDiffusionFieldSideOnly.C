#include "ADCoupledPenaltyInterfaceDiffusionFieldSideOnly.h"

registerMooseObject("FsiApp", ADCoupledPenaltyInterfaceDiffusionFieldSideOnly);

InputParameters
ADCoupledPenaltyInterfaceDiffusionFieldSideOnly::validParams()
{
  InputParameters params = ADInterfaceKernel::validParams();
  params.addClassDescription(
      "Interface kernel to apply Penalty*(primary - secondary) to the residual for a vector "
      "variable "
      "On one side of the interface only. primary_coupled_vector_var should be a vector, with the "
      "x, y, and z components of the secondary variable provided appropriately. The kernel will "
      "drive the x component of primary_vector_var to equal secondary_var_x, and so on. Note this "
      "kernel "
      "should be used in conjunction with matching fieldSideOnly interface kernels on the other "
      "side of "
      "the boundary.");
  params.addRequiredParam<Real>(
      "penalty",
      "The penalty that penalizes jump between primary and neighbor secondary variables.");
  params.addRequiredParam<Real>(
      "component", "The x (0), y (1), or z (2) component represented by the field variable");
  params.addRequiredCoupledVar("primary_coupled_var",
                               "The coupled field variable on the master side");
  params.addRequiredCoupledVar("secondary_coupled_vector_var",
                               "The coupled vector variable on the slave side");
  return params;
}

ADCoupledPenaltyInterfaceDiffusionFieldSideOnly::ADCoupledPenaltyInterfaceDiffusionFieldSideOnly(
    const InputParameters & parameters)
  : ADInterfaceKernel(parameters),
    _dim(_subproblem.mesh().dimension()),
    _penalty(getParam<Real>("penalty")),
    _component(getParam<Real>("component")),
    _primary_coupled_value(adCoupledValue("primary_coupled_var")),
    _secondary_coupled_vector_value(
        adCoupledVectorNeighborValue("secondary_coupled_vector_var")) // no non-ad version exists

{
  // Constructor
}
ADReal
ADCoupledPenaltyInterfaceDiffusionFieldSideOnly::computeQpResidual(Moose::DGResidualType type)
{
  ADReal r = 0;

  switch (type)
  {
    case Moose::Element:
      r = _test[_i][_qp] * _penalty *
          (_secondary_coupled_vector_value[_qp](_component) - _primary_coupled_value[_qp]);
      break;

    case Moose::Neighbor:
      r = 0;
      break;
  }

  return r;
}
