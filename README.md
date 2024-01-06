# smol_solver

* Description: A Smoluchowski equation solver coded in C++ (“smol_solver” shared by Prof. Donald Hamelberg), which can be used to calculate kinetic rates across free energy barriers as needed to estimate the apparent diffusion coefficients in Kramers' rate theory.

* Compile:
./make.sh

* Test example:
cd example
./run.sh

# Parameter settings: 
smol_solver pmf.dat $xmin $xmax $bc $dx $dt $nsteps

pmf.dat: 1D potential of mean force (PMF) data file

$xmin $xmax: the range of reaction coordinate that covers an energy barrier crossing event (e.g., ligand binding or unbinding)

$bc: flag for setting boundary conditions of Smoluchowski equation solver: 1: both absorbing; 2: both reflective; 3: left reflective/right absorbing; 4: left absorbing/right reflective

$dx: bin size of the PMF profile

$dt: time step used to output the time-dependent survival function S(t)

$nsteps: the total number of steps used to output the time-dependent survival function S(t)

Please refer to run.sh and the associated Fig. 4D in the LiGaMD paper (https://doi.org/10.1021/acs.jctc.0c00395) about how to set these parameters. They may be changed according to your simulation PMF.

With the output data such as “example/St_smol-q4MD-cD-aspirin-igamd11-reweightedno-binding.dat”, it saves “t(s)	Ln(S(t))”. One can calculate the kinetic rate k as negative of the Ln(S(t)) vs. t, i.e., k= -d Ln(S(t))/dt.

