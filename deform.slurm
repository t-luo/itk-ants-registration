#!/bin/bash
#SBATCH --mail-user=terry.luo@vanderbilt.edu
#SBATCH --mail-type=ALL
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=00:30:00
#SBATCH --mem=8G
#SBATCH --output=deform_%a.log
#SBATCH --error=error_deform_%a.log
#SBATCH --job-name=deform_registration

module purge
module restore cs8395-modules

cd ~/itk-ants-hw/cs8395-ITK-assignment/build/
./DeformableReg $1 $2 $3
