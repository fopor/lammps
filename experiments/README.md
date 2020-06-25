# Reproducint Experiments

### Compiling LAMMPS on an EFS
We will use a c5.large machine to quickly build LAMMPS on an EFS. The procedure is described bellow.

1. Create an EFS

AFTER ENV ACTIVATE
aws_secret_key:~/.clap/private/mo833_acess.pem
aws_access_key:~/.clap/private/mo833_acess.pub

`$ ansible-playbook ~/.clap/groups/roles/create-sg.yml   `

`$ ansible-playbook ~/.clap/groups/roles/create-efs.yml --extra "aws_sg_id=sg-0ffff401d95202efb"`


2. Start compiling instance:

`$ clapp node start i-c5.large `

Add it to EFS group:



`$ clapp group add ec2-efs node-3 `

ARRUMAR PRA PEGAR ESSE IP AUTOMATICAMENTE
clapp group action ec2-efs mount --extra "efs_mount_ip=172.31.88.21" "efs_mount_point=/efs/"


3. Compile LAMMPS
`$ clapp group add lammps node-4`

`$ clapp group action lammps compile`

4. Delete the node used for compilation
`$ clapp node stop node-4`

5. Start our cluster

`$ clapp cluster start cluster-t3.medium-2x`

`$ clapp cluster group cluster-1 lammps`

`$ clapp group action lammps setup-mpi`

`$ clapp cluster group cluster-1 ec2-efs`

`$ clapp group action ec2-efs mount --extra "efs_mount_ip=172.31.88.21" "efs_mount_point=/efs/"`

`$ clapp group action lammps run-tc-1 --extra "cfgalias=CFG-2"`

`$ clapp group action lammps fetch-results --extra "destfolder=~/Documents/lammps" `



Remember to DELETED your EFS!
PLaybook not working