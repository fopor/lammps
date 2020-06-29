# Running the experiment

## Configuring credentials
The procedure described bellow assumes that the following keys are on the right place:

AWS Secret key: ~/.clap/private/mo833_acess.pem 

AWS Acess key:  ~/.clap/private/mo833_acess.pub

AWS Key Pair (private key): ~/.clap/private/mo833_rfreitas.pem

AWS Key Pair (public key): ~/.clap/private/mo833_rfreitas.pub

## Compiling LAMMPS on an EFS
We will use a c5.large machine to quickly build LAMMPS on an EFS. The procedure is described bellow.

1. Create an EFS

This steps will use Ansible to create an MPI-ready security group and an EFS. Simply run:

`$ ansible-playbook ~/.clap/groups/roles/create-sg.yml`

This will output the security group ID on the standard output. Use this ID to create an EFS, replacing 'SG_ID' with the string recived from the previus command:

`$ ansible-playbook ~/.clap/groups/roles/create-efs.yml --extra "aws_sg_id=SG_ID"`

This will output an mouting IP, that will be used on to mount the disk on the instances. It will be referenced bellow as MOUNT_IP.

2. Start compiling instance:


`$ clapp node start i-c5.large `

Add it to EFS group:

`$ clapp group add ec2-efs node-1 `

We can now mount it:

`$ clapp group action ec2-efs mount --extra "efs_mount_ip=MOUNT_IP" "efs_mount_point=/efs/"`


3. Compile LAMMPS

This step will install all required dependencies and run the scripts that compile LAMMPS.

`$ clapp group add lammps node-1`

`$ clapp group action lammps compile`



4. Delete the node used for compilation

After the compilation, we can terminate our instances.

`$ clapp node stop node-1`

## Running the experiment on a cluster

1. Start a cluster

This will tell CLAP to start the t3.medium-2x cluster. You can use any desired cluster configuration, like cluster-c5.large-2x, cluster-t3.medium-4x or cluster-t3.medium-8x.

`$ clapp cluster start cluster-t3.medium-2x`

The next three steps will install the required dependencies to mount the EFS and run MPI applications. 

`$ clapp cluster group cluster-1 lammps`

`$ clapp group action lammps setup-mpi`

`$ clapp cluster group cluster-1 ec2-efs`

2. Mount the EFS disk

`$ clapp group action ec2-efs mount --extra "efs_mount_ip=MOUNT_IP" "efs_mount_point=/efs/"`

3. Run the test case

`$ clapp group action lammps run-tc-1 --extra "cfgalias=CFG-1"`

`$ clapp group action lammps run-tc-2 --extra "cfgalias=CFG-1"`

`$ clapp group action lammps run-tc-3 --extra "cfgalias=CFG-1"`


4. Fetch the results

`$ clapp group action lammps fetch-results --extra "destfolder=`pwd`"`

5. Delete the EFS

`$ ansible-playbook ~/.clap/groups/roles/delete-efs.yml`
