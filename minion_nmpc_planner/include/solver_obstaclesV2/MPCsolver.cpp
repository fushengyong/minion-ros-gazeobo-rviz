#include "MPCsolver.h"
#include <iostream>

Vars vars;
Params params;
Workspace work;
Settings settings;

MPCsolver::MPCsolver(){}


Eigen::Matrix<double, 9, 36> MPCsolver::OCPsolDesigner(double deltaT, Eigen::Matrix<double, 3, 36> obstacle_force, Eigen::Vector3d ExternalForce, Eigen::Matrix<double, 6, 1> curState, Eigen::Matrix<double, 6, 1> termState, Eigen::Matrix<double, 9, 1> CostFunction, Eigen::Matrix<double, 6, 2> stateLimits, Eigen::Matrix<double, 3, 2> input_constraint)
{

    // initialize 1st predictive state and control input
    Eigen::Matrix<double, 9, 36> StateInput = Eigen::Matrix<double, 9, 36>::Zero();
//    int num_iters_x, num_iters_y, num_iters_z;

    // initialize defaults parameters and variables
    set_defaults();
    setup_indexing();
    // disable output of solver progress
    settings.verbose = 0;
    //settings.max_iters = 100;
    settings.eps = 1e-4;
    settings.resid_tol = 1e-3;

    // generate an optimal control problem for x-axis motion
    load_data(deltaT,obstacle_force, ExternalForce, curState, termState, stateLimits, input_constraint, CostFunction);
    // solve 3d OCP using cvxgen
    solve();
    // output 1st step of nominal input and state estimate (along x-axis) as reference
    StateInput = use_solution(vars);

    return StateInput;
}

void MPCsolver::load_data(double deltaT, Eigen::Matrix<double, 3, 36> obstacle_force, Eigen::Vector3d ExternalForce, Eigen::Matrix<double, 6, 1> InitialState, Eigen::Matrix<double, 6, 1> TerminalState, Eigen::Matrix<double, 6, 2> StateConstraint, Eigen::Matrix<double, 3, 2> InputConstraint, Eigen::Matrix<double, 9, 1> CostFunction)
{  
    for(int i=0; i<2; i++)
      params.f_obs_0[i] = obstacle_force(i,0);

    for(int i=0; i<2; i++)
      params.f_obs_1[i] = obstacle_force(i,1);

    for(int i=0; i<2; i++)
      params.f_obs_2[i] = obstacle_force(i,2);

    for(int i=0; i<2; i++)
      params.f_obs_3[i] = obstacle_force(i,3);

    for(int i=0; i<2; i++)
      params.f_obs_4[i] = obstacle_force(i,4);

    for(int i=0; i<2; i++)
      params.f_obs_5[i] = obstacle_force(i,5);

    for(int i=0; i<2; i++)
      params.f_obs_6[i] = obstacle_force(i,6);

    for(int i=0; i<2; i++)
      params.f_obs_7[i] = obstacle_force(i,7);

    for(int i=0; i<2; i++)
      params.f_obs_8[i] = obstacle_force(i,8);

    for(int i=0; i<2; i++)
      params.f_obs_9[i] = obstacle_force(i,9);

    for(int i=0; i<2; i++)
      params.f_obs_10[i] = obstacle_force(i,10);

    for(int i=0; i<2; i++)
      params.f_obs_11[i] = obstacle_force(i,11);

    for(int i=0; i<2; i++)
      params.f_obs_12[i] = obstacle_force(i,12);

    for(int i=0; i<2; i++)
      params.f_obs_13[i] = obstacle_force(i,13);    
    
    for(int i=0; i<2; i++)
      params.f_obs_14[i] = obstacle_force(i,14);

    for(int i=0; i<2; i++)
      params.f_obs_15[i] = obstacle_force(i,15);

    for(int i=0; i<2; i++)
      params.f_obs_16[i] = obstacle_force(i,16);

    for(int i=0; i<2; i++)
      params.f_obs_17[i] = obstacle_force(i,17);

    for(int i=0; i<2; i++)
      params.f_obs_18[i] = obstacle_force(i,18);

    for(int i=0; i<2; i++)
      params.f_obs_19[i] = obstacle_force(i,19);

    for(int i=0; i<2; i++)
      params.f_obs_20[i] = obstacle_force(i,20);

    for(int i=0; i<2; i++)
      params.f_obs_21[i] = obstacle_force(i,21);

    for(int i=0; i<2; i++)
      params.f_obs_22[i] = obstacle_force(i,22);

    for(int i=0; i<2; i++)
      params.f_obs_23[i] = obstacle_force(i,23);

    for(int i=0; i<2; i++)
      params.f_obs_24[i] = obstacle_force(i,24);

    for(int i=0; i<2; i++)
      params.f_obs_25[i] = obstacle_force(i,25);

    for(int i=0; i<2; i++)
      params.f_obs_26[i] = obstacle_force(i,26);

    for(int i=0; i<2; i++)
      params.f_obs_27[i] = obstacle_force(i,27);

    for(int i=0; i<2; i++)
      params.f_obs_28[i] = obstacle_force(i,28);

    for(int i=0; i<2; i++)
      params.f_obs_29[i] = obstacle_force(i,29);    
    
    for(int i=0; i<2; i++)
      params.f_obs_30[i] = obstacle_force(i,30);

    for(int i=0; i<2; i++)
      params.f_obs_31[i] = obstacle_force(i,31);    

    for(int i=0; i<2; i++)
      params.f_obs_32[i] = obstacle_force(i,32);

    for(int i=0; i<2; i++)
      params.f_obs_33[i] = obstacle_force(i,33);

    for(int i=0; i<2; i++)
      params.f_obs_34[i] = obstacle_force(i,34);    
    
    for(int i=0; i<2; i++)
      params.f_obs_35[i] = obstacle_force(i,35);

    // Gravity 
    for(int i=0;i<3;i++)      
      params.g[i] = ExternalForce(i);
    //Input Cost Matrix
    for(int i=0;i<3;i++)
      params.R[i] = CostFunction(6+i);  //Last four entries of cost matrix are control input constraints
    
    //Terminal Cost
    for(int i=0;i<6;i++)
      params.Q[i] = CostFunction(i);
    
    // X_0 (initial state)
    for(int i=0;i<6;i++)
      params.x_0[i] = InitialState(i);
    // X_N Target State
    for(int i=0;i<6;i++)
      params.xN[i] = TerminalState(i);
    // X_min and X_max
    for(int i=0;i<6;i++)
    {
      params.x_min[i] = StateConstraint(i,0);
      params.x_max[i] = StateConstraint(i,1);
    }
    // U_min and U_max
    for(int i=0;i<3;i++)  
    {
      params.u_min[i] = InputConstraint(i,0);
      params.u_max[i] = InputConstraint(i,1);    
    }


    //Dynamics: A_(16x16): Only non-zero entries. 
    // Check CVXgen documentation for how these entries map to sparse matrix (A's) entries
    params.A[0] = 1;
    params.A[1] = deltaT;
    params.A[2] = 1;
    params.A[3] = 1;
    params.A[4] = deltaT;
    params.A[5] = 1;
    params.A[6] = 1;
    params.A[7] = deltaT;
    params.A[8] = 1;
    

    //Transfer Control Matrix: B_(16x4)
    params.B[0] = 0.5*deltaT*deltaT;
    params.B[1] = deltaT;
    params.B[2] = 0.5*deltaT*deltaT;
    params.B[3] = deltaT;
    params.B[4] = 0.5*deltaT*deltaT;
    params.B[5] = deltaT;    

}


Eigen::Matrix<double, 9, 36> MPCsolver::use_solution(Vars vars)
{
  Eigen::Matrix<double, 9, 36> StateInput_output = Eigen::Matrix<double, 9, 36>::Zero();   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,1) = vars.x_1[i];
   }

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,2) = vars.x_2[i];
   }

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,3) = vars.x_3[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,4) = vars.x_4[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,5) = vars.x_5[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,6) = vars.x_6[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,7) = vars.x_7[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,8) = vars.x_8[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,9) = vars.x_9[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,10) = vars.x_10[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,11) = vars.x_11[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,12) = vars.x_12[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,13) = vars.x_13[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,14) = vars.x_14[i];
   }
   
   for(int i=0; i<6; i++)
   {
       StateInput_output(i,15) = vars.x_15[i];
   }   
   
   for(int i=0; i<6; i++)
   {
       StateInput_output(i,16) = vars.x_16[i];
   }      


   for(int i=0; i<6; i++)
   {
       StateInput_output(i,17) = vars.x_17[i];
   }

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,18) = vars.x_18[i];
   }

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,19) = vars.x_19[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,20) = vars.x_20[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,21) = vars.x_21[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,22) = vars.x_22[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,23) = vars.x_23[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,24) = vars.x_24[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,25) = vars.x_25[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,26) = vars.x_26[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,27) = vars.x_27[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,28) = vars.x_28[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,29) = vars.x_29[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,30) = vars.x_30[i];
   }
   
   for(int i=0; i<6; i++)
   {
       StateInput_output(i,31) = vars.x_31[i];
   }   
   
   for(int i=0; i<6; i++)
   {
       StateInput_output(i,32) = vars.x_32[i];
   }      

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,33) = vars.x_33[i];
   }
   

   for(int i=0; i<6; i++)
   {
       StateInput_output(i,34) = vars.x_34[i];
   }
   
   for(int i=0; i<6; i++)
   {
       StateInput_output(i,35) = vars.x_35[i];
   }   
 
   //StateInput_output(0,16) = vars.u_0[0];
   //StateInput_output(1,16) = vars.u_0[1];   
   
   
   ///////////////////////////////////////////
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,0) = vars.u_0[i];
   }
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,1) = vars.u_1[i];
   }   
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,2) = vars.u_2[i];
   }      
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,3) = vars.u_3[i];
   }
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,4) = vars.u_4[i];
   }   
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,5) = vars.u_5[i];
   }      
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,6) = vars.u_6[i];
   }
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,7) = vars.u_7[i];
   }   
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,8) = vars.u_8[i];
   }      
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,9) = vars.u_9[i];
   }
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,10) = vars.u_10[i];
   }   
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,11) = vars.u_11[i];
   }      
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,12) = vars.u_12[i];
   }
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,13) = vars.u_13[i];
   }   
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,14) = vars.u_14[i];
   }         
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,15) = vars.u_15[i];
   }          
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,16) = vars.u_16[i];
   }
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,17) = vars.u_17[i];
   }   
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,18) = vars.u_18[i];
   }      
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,19) = vars.u_19[i];
   }
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,20) = vars.u_20[i];
   }   
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,21) = vars.u_21[i];
   }      
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,22) = vars.u_22[i];
   }
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,23) = vars.u_23[i];
   }   
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,24) = vars.u_24[i];
   }      
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,25) = vars.u_25[i];
   }
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,26) = vars.u_26[i];
   }   
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,27) = vars.u_27[i];
   }      
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,28) = vars.u_28[i];
   }
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,29) = vars.u_29[i];
   }   
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,30) = vars.u_30[i];
   }         
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,31) = vars.u_31[i];
   }          
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,32) = vars.u_32[i];
   }
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,33) = vars.u_33[i];
   }   
   for(int i=6; i<9; i++)
   {
       StateInput_output(i,34) = vars.u_34[i];
   }         


   return StateInput_output;
}