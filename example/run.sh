#!/bin/bash

sys=q4MD-cD
ligand=aspirin
igamd=11

T=298
dx=0.5
for trans in binding unbinding ; do
for reweighted in yes no ; do
if [ $trans == "binding" ]; then
xmin=0.5
xmax=7.5
bc=4
nsteps=1000000 # 1000 #
dt=0.00001
elif [ $trans == "unbinding" ]; then
xmin=6
xmax=20 # 120 # 1
bc=3
nsteps=1000000 # 1000 #
dt=0.0001
fi # trans
if [ $igamd == 0 ]; then
grep -v "#" data-pmf-1D-dist-lig-host-$sys-$ligand.dat | grep -v "cMD" | grep -v "\-\-" | head -n 60 | awk '{print $1 "        " $2/(0.001987*298)}' > pmf.dat
elif [ $igamd == 10 ]; then
if [ $reweighted == "yes" ]; then
grep -v "#" data-pmf-1D-dist-lig-host-$sys-$ligand.dat | grep -v "cMD" | grep -v "\-\-" | head -n 60 | awk '{print $3 "        " $4/(0.001987*298) }' > pmf.dat
elif [ $reweighted == "no" ]; then
grep -v "#" data-pmf-1D-dist-lig-host-$sys-$ligand.dat | grep -v "cMD" | grep -v "\-\-" | head -n 60 | awk '{print $9 "        " $10/(0.001987*298) }' > pmf.dat
fi # reweighted
elif [ $igamd == 11 ]; then
if [ $reweighted == "yes" ]; then
grep -v "#" data-pmf-1D-dist-lig-host-$sys-$ligand.dat | grep -v "cMD" | grep -v "\-\-" | head -n 60 | awk '{print $6 "        " $7/(0.001987*298) }' > pmf.dat
elif [ $reweighted == "no" ]; then
grep -v "#" data-pmf-1D-dist-lig-host-$sys-$ligand.dat | grep -v "cMD" | grep -v "\-\-" | head -n 60 | awk '{print $12 "        " $13/(0.001987*298) }' > pmf.dat
fi # reweighted
fi # igamd
echo "../smol_solver pmf.dat $xmin $xmax $bc $dx $dt $nsteps | tee St_smol-$sys-$ligand-igamd$igamd-reweighted$reweighted-${trans}.dat"
../smol_solver pmf.dat $xmin $xmax $bc $dx $dt $nsteps | tee St_smol-$sys-$ligand-igamd$igamd-reweighted$reweighted-${trans}.dat
rm -v pmf.dat
done
done

