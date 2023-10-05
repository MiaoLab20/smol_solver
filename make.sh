#!/bin/bash

g++ 1D-Smol-Solver.cpp -o smol_solver
rsync -avuh smol_solver ~/bin

