/** @file sparseSolvers_example.cpp

    @brief Testing the use of sparse linear solvers.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#include <gismo.h>

using namespace gismo;

double solver_time(0), compute_time(0);
gsStopwatch timer;

void report( const gsVector<>& computedSolution, const gsVector<>& exactSolution, bool& succeeded )
{
    if(computedSolution.size() < 10) {gsInfo << "  Computed solution: " << computedSolution.transpose() << "\n";}
    if ( (computedSolution-exactSolution).norm() <= 1.e-10 )
    {
        gsInfo << "  Test passed.\n";
    }
    else
    {
        gsInfo << "  Test faild.\n";
        succeeded = false;
    }
    gsInfo << "  Total time:  " << solver_time + compute_time << "\n";
    gsInfo << "     Compute time: " << compute_time << "\n";
    gsInfo << "     Solver time:  " << solver_time << "\n";
    gsInfo << "\n";
}

int main(int argc, char** argv)
{
#ifdef EIGEN_USE_MKL
        gsInfo << "EIGEN_USE_MKL=true.\n";
#endif

    std::string fn("");
    index_t mat_size = 10;

    timer.restart();

    std::string fmat("");

    gsCmdLine cmd("Testing the use of sparse linear solvers.");
    cmd.addPlainString("try", "Name of the solver to try", fn);
    cmd.addInt("n", "size", "Size of the matrices", mat_size);
    cmd.addString( "f", "file", "Input XML matrix", fmat );

    try { cmd.getValues(argc,argv); } catch (int rv) { return rv; }

    gsSparseMatrix<>  Q(mat_size,mat_size);
    gsVector<>        b(mat_size), x(mat_size), x0(mat_size);
    x0.setOnes();

    bool succeeded = true;

    if (!fn.empty())
    {
        gsSparseSolver<>::uPtr slv = gsSparseSolver<>::get(fn);
        slv->compute(Q);
        x = slv->solve(b);
        gsInfo << "Solve Ax = b with "<< *slv <<" sparse linear solver.\n";
        report( x, x0, succeeded );
        return succeeded ? 0 : 1;
    }

    if (!fmat.empty())
      {
        gsFileData<> fd(fmat);
        fd.getFirst(Q);
        gsMatrix<> tmp;
        fd.getFirst(tmp);
        b = give(tmp);
        mat_size = b.size();

      }
    else
      {
        Q.reserve( gsVector<int>::Constant(mat_size,1) ); // Reserve memory for 1 non-zero entry per column
        for (index_t i = 0; i!=mat_size; ++i)
          Q(i,i) = b[i] = i+1;

        Q.makeCompressed(); // always call makeCompressed after sparse matrix has been filled
      }

    gsSparseSolver<>::CGIdentity solverCGI;
    timer.restart();
    solverCGI.compute(Q);
    compute_time = timer.stop();
    timer.restart();
    x = solverCGI.solve(b);
    solver_time = timer.stop();
    gsInfo << "Solve Ax = b with Eigen's CG identity preconditioner.\n";
    if (!fmat.empty()){x0 = x;}
    report( x, x0, succeeded );

    gsSparseSolver<>::CGDiagonal solverCGD;
    timer.restart();
    solverCGD.compute(Q);
    compute_time = timer.stop();
    timer.restart();
    x = solverCGD.solve(b);
    solver_time = timer.stop();
    gsInfo << "Solve Ax = b with Eigen's CG diagonal preconditioner.\n";
    if (!fmat.empty()){x0 = x;}
    report( x, x0, succeeded );

    gsSparseSolver<>::BiCGSTABILUT solverBCGILU;
    timer.restart();
    solverBCGILU.compute(Q);
    compute_time = timer.stop();
    timer.restart();
    x = solverBCGILU.solve(b);
    solver_time = timer.stop();
    gsInfo << "Solve Ax = b with Eigen's BiCG with ILU preconditioner.\n";
    if (!fmat.empty()){x0 = x;}
    report( x, x0, succeeded );

    gsSparseSolver<>::BiCGSTABDiagonal solverBCGD;
    timer.restart();
    solverBCGD.compute(Q);
    compute_time = timer.stop();
    timer.restart();
    x = solverBCGD.solve(b);
    solver_time = timer.stop();
    gsInfo << "Solve Ax = b with Eigen's BiCG with diagonal preconditioner.\n";
    if (!fmat.empty()){x0 = x;}
    report( x, x0, succeeded );

    gsSparseSolver<>::BiCGSTABIdentity solverBCDI;
    timer.restart();
    solverBCDI.compute(Q);
    compute_time = timer.stop();
    timer.restart();
    x = solverBCDI.solve(b);
    solver_time = timer.stop();
    gsInfo << "Solve Ax = b with Eigen's BiCG without preconditioner.\n";
    if (!fmat.empty()){x0 = x;}
    report( x, x0, succeeded );

    gsSparseSolver<>::SimplicialLDLT solverSLDLT;
    timer.restart();
    solverSLDLT.compute(Q);
    compute_time = timer.stop();
    timer.restart();
    x = solverSLDLT.solve(b);
    compute_time = timer.stop();
    gsInfo << "Solve Ax = b with Eigen's Simplicial LDLT.\n";
    if (!fmat.empty()){x0 = x;}
    report( x, x0, succeeded );

    gsSparseSolver<>::QR solverQR;
    timer.restart();
    solverQR.compute(Q);
    compute_time = timer.stop();
    timer.restart();
    x = solverQR.solve(b);
    solver_time = timer.stop();
    gsInfo << "Solve Ax = b with Eigen's QR factorization.\n";
    if (!fmat.empty()){x0 = x;}
    report( x, x0, succeeded );

    gsSparseSolver<>::LU solverLU;
    timer.restart();
    solverLU.compute(Q);
    compute_time = timer.stop();
    timer.restart();
    x = solverLU.solve(b);
    solver_time = timer.stop();
    gsInfo << "Solve Ax = b with Eigen's LU factorization.\n";
    if (!fmat.empty()){x0 = x;}
    report( x, x0, succeeded );

#ifdef GISMO_WITH_PARDISO
    gsSparseSolver<>::PardisoLU solverpLU;
    solverpLU.compute(Q);
    x = solverpLU.solve(b);
    gsInfo << "Error code of pardiso "<< solverpLU.info() <<"\n";
    gsInfo << "Solve Ax = b with PardisoLU.\n";
    if (!fmat.empty()){x0 = x;}
    report( x, x0, succeeded );

    gsSparseSolver<>::PardisoLDLT solverLDLT;
    solverLDLT.compute(Q);
    x = solverLDLT.solve(b);
    gsInfo << "Error code of pardiso "<< solverLDLT.info() <<"\n";
    gsInfo << "Solve Ax = b with PardisoLDLT.\n";
    if (!fmat.empty()){x0 = x;}
    report( x, x0, succeeded );

    gsSparseSolver<>::PardisoLLT solverLLT;
    solverLLT.compute(Q);
    x = solverLLT.solve(b);
    gsInfo << "Error code of pardiso "<< solverLLT.info() <<"\n";
    gsInfo << "Solve Ax = b with PardisoLLT.\n";
    if (!fmat.empty()){x0 = x;}
    report( x, x0, succeeded );
#   else
    gsInfo << "PARDISO is not available.\n";
#   endif

#ifdef GISMO_WITH_SUPERLU
    gsSparseSolver<>::SuperLU solverSLU;
    solverSLU.compute(Q);
    x = solverSLU.solve(b);
    gsInfo << "Solve Ax = b with Super.\n";
    if (!fmat.empty()){x0 = x;}
    report( x, x0, succeeded );

#   else
    gsInfo << "SuperLU is not available.\n";
#   endif

    #ifdef GISMO_WITH_PASTIX
    gsInfo << "PastiX is not available.\n";
#   else
    gsInfo << "PastiX is not available.\n";
#   endif

    return succeeded ? 0 : 1;
}
