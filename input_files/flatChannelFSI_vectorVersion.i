[GlobalParams]
    integrate_p_by_parts = true
    displacements = 'disp_x disp_y'
[]

[Mesh]
    [gmg]
        type = GeneratedMeshGenerator
        dim = 2
        xmin = 0
        xmax = 3.0
        ymin = 0
        ymax = 1.0
        nx = 10
        ny = 15
        elem_type = QUAD4
    []

    [subdomain1]
        type = SubdomainBoundingBoxGenerator
        bottom_left = '0.0 0.5 0'
        block_id = 1
        top_right = '3.0 1.0 0'
        input = gmg
    []

    [interface_primary0]
        type = SideSetsBetweenSubdomainsGenerator
        primary_block = '0'
        paired_block = '1'
        new_boundary = 'master0_interface'
        input = subdomain1
    []

    [interface_primary1]
        type = SideSetsBetweenSubdomainsGenerator
        primary_block = '1'
        paired_block = '0'
        new_boundary = 'master1_interface'
        input = interface_primary0
    []

    [break_boundary]
        type = BreakBoundaryOnSubdomainGenerator
        input = interface_primary1
    []
[]
  
[Variables]
    [./vel_fluid]
        family = LAGRANGE_VEC
        block = 0
    [../]
    [./p]
        block = 0
    [../]
    [./disp_x]
    [../]
    [./disp_y]
    [../]
    [./vel_x_solid]
        block = 1
    [../]
    [./vel_y_solid]
        block = 1
    [../]
    [./dummy_vec_solid]
        family = LAGRANGE_VEC
        block = 1
    [../]
    [./dummy_scalar_fluid]
        block = 0
    [../]
[]
  
[Kernels]
    [./mass_conservation]
        type = INSADMass
        variable = p
        block = '0'
        use_displaced_mesh = true
    [../]
    [./momentum_time]
        type = INSADMomentumTimeDerivative
        variable = vel_fluid
        block = '0'
        use_displaced_mesh = true
    [../]
    [momentum_advection]
        type = INSADMomentumAdvection
        variable = vel_fluid
        block = '0'
        use_displaced_mesh = true
    []
    [momentum_pressure]
        type = INSADMomentumPressure
        variable = vel_fluid
        pressure = p
        block = '0'
        use_displaced_mesh = true
    []
    [momentum_viscous]
        type = INSADMomentumViscous
        variable = vel_fluid
        block = '0'
        use_displaced_mesh = true
    []

    [./convected_mesh]                      #We can comment out this kernel to isolate the interface kernels - the simulation should still solve
        type = ADConvectedMeshFluidVector
        disp_x = disp_x
        disp_y = disp_y
        variable = vel_fluid
        block = 0
        use_displaced_mesh = true
    [../]

    [./disp_x_fluid]
        type = ADDiffusion
        variable = disp_x
        block = 0
    [../]
    [./disp_y_fluid]
        type = ADDiffusion
        variable = disp_y
        block = 0
    [../]

    [./accel_tensor_x]
        type = ADCoupledTimeDerivative
        variable = disp_x
        v = vel_x_solid
        block = 1
    [../]
    [./accel_tensor_y]
        type = ADCoupledTimeDerivative
        variable = disp_y
        v = vel_y_solid
        block = 1
    [../]
    [./vxs_time_derivative_term]
        type = ADCoupledTimeDerivative
        variable = vel_x_solid
        v = disp_x
        block = 1
    [../]
    [./vys_time_derivative_term]
        type = ADCoupledTimeDerivative
        variable = vel_y_solid
        v = disp_y
        block = 1
    [../]
    [./source_vxs]
        type = ADMatReaction
        variable = vel_x_solid
        block = 1
        reaction_rate = 1
    [../]
    [./source_vys]
        type = ADMatReaction
        variable = vel_y_solid
        block = 1
        reaction_rate = 1
    [../]

    # [./momentum_supg]
    #     type = INSADMomentumSUPG
    #     variable = vel_fluid
    #     velocity = vel_fluid
    # [../]
    [./mass_pspg]
        type = INSADMassPSPG
        variable = p
    [../]

    [./dummy_scalar_fluid_nullKernel]
        type = ADTimeDerivative
        variable = dummy_scalar_fluid
    [../]
    [./dummy_vec_solid_nullKernel]
        type = ADVectorTimeDerivative
        variable = dummy_vec_solid
    [../]
[]
  
[InterfaceKernels]
    [./fluidSideInterface]
        type = ADCoupledPenaltyInterfaceDiffusionVectorSideOnly
        variable = vel_fluid
        neighbor_var = dummy_vec_solid
        primary_coupled_vector_var = vel_fluid
        secondary_coupled_var_x = vel_x_solid
        secondary_coupled_var_y = vel_y_solid
        penalty = 1e6
        boundary = master0_interface
    [../]
    [./solid_side_x]
        type = ADCoupledPenaltyInterfaceDiffusionFieldSideOnly
        variable = disp_x
        neighbor_var = dummy_scalar_fluid
        primary_coupled_var = vel_x_solid
        secondary_coupled_vector_var = vel_fluid
        component = 0
        penalty = -1e6
        boundary = master1_interface
    [../]
    [./solid_side_y]
        type = ADCoupledPenaltyInterfaceDiffusionFieldSideOnly
        variable = disp_y
        neighbor_var = dummy_scalar_fluid
        primary_coupled_var = vel_y_solid
        secondary_coupled_vector_var = vel_fluid
        component = 1
        penalty = -1e6
        boundary = master1_interface
    [../]
[]
  
[Modules/TensorMechanics/Master]
    [./solid_domain]
        strain = SMALL
        incremental = false
        # generate_output = 'strain_xx strain_yy strain_zz' ## Not at all necessary, but nice
        block = '1'
        use_automatic_differentiation = true
    [../]
[]
  
[Materials]
    [./elasticity_tensor]
        type = ADComputeIsotropicElasticityTensor
        youngs_modulus = 1e2
        poissons_ratio = 0.3
        block = '1'
    [../]
    [./small_stress]
        type = ADComputeLinearElasticStress
        block = 1
    [../]
    [./fluid]
        type = ADGenericConstantMaterial
        prop_names = 'rho mu T cp k'
        prop_values = '1 1 1 1 1'
        block = '0'
    [../]
    [./ins_mat]
        type = INSADStabilized3Eqn
        velocity = vel_fluid
        pressure = p
        temperature = '1'
        block = '0'
        alpha = 1.0
    [../]
[]
  
[BCs]
    [./fluid_x_no_slip]
        type = ADVectorFunctionDirichletBC
        variable = vel_fluid
        boundary = 'bottom'
        function_x = 0
    [../]
    [./fluid_y_no_slip]
        type = ADVectorFunctionDirichletBC
        variable = vel_fluid
        boundary = 'bottom left_to_0'
        function_y = 0
    [../]
    [./x_inlet]
        type = ADVectorFunctionDirichletBC
        variable = vel_fluid
        boundary = 'left_to_0'
        function_x = 'inlet_func'
        function_y = 0
    [../]
    [./no_disp_x]
        type = ADDirichletBC
        variable = disp_x
        boundary = 'bottom top left_to_1 right_to_1 left_to_0 right_to_0'
        value = 0
    [../]
    [./no_disp_y]
        type = ADDirichletBC
        variable = disp_y
        boundary = 'bottom top left_to_1 right_to_1 left_to_0 right_to_0'
        value = 0
    [../]
    [./solid_x_no_slip]
        type = ADDirichletBC
        variable = vel_x_solid
        boundary = 'top left_to_1 right_to_1'
        value = 0.0
    [../]
    [./solid_y_no_slip]
        type = ADDirichletBC
        variable = vel_y_solid
        boundary = 'top left_to_1 right_to_1'
        value = 0.0
    [../]
[]
  
[Preconditioning]
    [./SMP]
        type = SMP
        full = true
    [../]
[]
  
[Executioner]
    type = Transient
    num_steps = 5
    # num_steps = 60
    dt = 0.1
    dtmin = 0.1

    # solve_type = 'PJFNK'
    # petsc_options_iname = '-pc_type'
    # petsc_options_value = 'lu'
    # line_search = none

    solve_type = NEWTON
    petsc_options_iname = '-pc_type -pc_factor_shift_type -off_diagonals_in_auto_scaling'
    petsc_options_value = 'lu           NONZERO                     true'
    automatic_scaling = true
    
    # petsc_options = '-pc_svd_monitor'
    # petsc_options_iname = '-pc_type'
    # petsc_options_value = 'svd'
    # automatic_scaling = true
    
[]
  
[Outputs]
    [./out]
        type = Exodus
    [../]
[]
  
[Functions]
    [./inlet_func]
        type = ParsedFunction
        value = '(-16 * (y - 0.25)^2 + 1) * (1 + cos(t))'
    [../]
[]
  