efs_mount_ip="172.31.6.202"

# Running on CFG-1: 
cfg_alias="CFG-1"
clapp cluster start cluster-c5.large-2x
cluster_id="cluster-?"


# Running on CFG-2: 
cfg_alias="CFG-2"
clapp cluster start cluster-t3.medium-2x
cluster_id="cluster-?"

# Running on CFG-3:
cfg_alias="CFG-3"
clapp cluster start cluster-t3.medium-4x
cluster_id="cluster-?"

# Running on CFG-4
cfg_alias="CFG-4"
clapp cluster start cluster-t3.medium-8x
cluster_id="cluster-?"





clapp cluster group $cluster_id lammps
clapp group action lammps setup-mpi
clapp cluster group $cluster_id ec2-efs
clapp group action ec2-efs mount --extra "efs_mount_ip=$efs_mount_ip" "efs_mount_point=/efs/"

clapp group action lammps run-tc-1 --extra "cfgalias=$cfg_alias"
clapp group action lammps run-tc-2 --extra "cfgalias=$cfg_alias"
clapp group action lammps run-tc-3 --extra "cfgalias=$cfg_alias"

clapp group action lammps fetch-results --extra "destfolder=`pwd`"

clapp cluster stop $cluster_id