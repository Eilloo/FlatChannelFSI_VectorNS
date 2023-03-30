#include "ADCoupledPenaltyInterfaceDiffusionVectorSideOnly.h"

registerMooseObject("FsiApp", ADCoupledPenaltyInterfaceDiffusionVectorSideOnly);

InputParameters
ADCoupledPenaltyInterfaceDiffusionVectorSideOnly::validParams()
{
    InputParameters params = ADVectorInterfaceKernel::validParams();
    params.addClassDescription("Interface kernel to apply Penalty*(primary - secondary) to the residual for a vector variable "
                                "On one side of the interface only. primary_coupled_vector_var should be a vector, with the "
                                "x, y, and z components of the secondary variable provided appropriately. The kernel will "
                                "drive the x component of primary_vector_var to equal secondary_var_x, and so on. Note this kernel "
                                "should be used in conjunction with matching fieldSideOnly interface kernels on the other side of "
                                "the boundary.");
    params.addRequiredParam<Real>(
      "penalty",
      "The penalty that penalizes jump between primary and neighbor secondary variables.");
    params.addRequiredCoupledVar("primary_coupled_vector_var", "The coupled vector variable on the master side");
    params.addRequiredCoupledVar("secondary_coupled_var_x", "The coupled variable on the slave side");
    params.addCoupledVar("secondary_coupled_var_y", "The coupled variable on the slave side");
    params.addCoupledVar("secondary_coupled_var_z", "The coupled variable on the slave side");
    return params;
}

ADCoupledPenaltyInterfaceDiffusionVectorSideOnly::ADCoupledPenaltyInterfaceDiffusionVectorSideOnly(
    const InputParameters & parameters)
    : ADVectorInterfaceKernel(parameters),
    _dim(_subproblem.mesh().dimension()),
    _penalty(getParam<Real>("penalty")),
    _primary_coupled_vector_value(adCoupledVectorValue("primary_coupled_vector_var")),
    _secondary_coupled_value_x(adCoupledNeighborValue("secondary_coupled_var_x")),
    _secondary_coupled_value_y(isCoupled("secondary_coupled_var_y") ? adCoupledNeighborValue("secondary_coupled_var_y") : _ad_zero),
    _secondary_coupled_value_z(isCoupled("secondary_coupled_var_z") ? adCoupledNeighborValue("secondary_coupled_var_z") : _ad_zero)
{
    //Constructor
}

ADReal
ADCoupledPenaltyInterfaceDiffusionVectorSideOnly::computeQpResidual(Moose::DGResidualType type)
{
    Real r = 0;

    Real r_x = 0;
    Real r_y = 0;
    Real r_z = 0;

  switch (type)
  {
    case Moose::Element:
        r_x = (_primary_coupled_vector_value[_qp](0).value() - _secondary_coupled_value_x[_qp].value());
        if(_dim > 1) {
            r_y = (_primary_coupled_vector_value[_qp](1).value() - _secondary_coupled_value_y[_qp].value());
        } if(_dim > 2) {
            r_z = (_primary_coupled_vector_value[_qp](2).value() - _secondary_coupled_value_z[_qp].value());
        }
        r = _test[_i][_qp] * _penalty * RealVectorValue(r_x, r_y, r_z);

        break;

    case Moose::Neighbor:
        r = 0;
        break;
  }

  return r;
}