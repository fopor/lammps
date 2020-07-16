# Running the experiment
This file describes how to use the CLAP and Ansible scripts present on the folder ./clap to run the experiment.

## Installing CLAP
The step-by-step described below uses Ansible and CLAP to instantiate virutal machines on the AWS. Before proceeding, you should perform this [instalation](https://github.com/lmcad-unicamp/CLAP).


## Configuring credentials
The procedure described below assumes that the following keys are on the corresponding place (i.e. put your AWS Secret key on the text file `~/.clap/private/mo833_acess.pem`, as the following table describes):

|           Key          |            File location           |
|:----------------------:|:----------------------------------:|
|     AWS Secret key     |   ~/.clap/private/mo833_acess.pem  |
|      AWS Acess key     |   ~/.clap/private/mo833_acess.pub  |
| AWS Key Pair (private) | ~/.clap/private/mo833_rfreitas.pem |
|  AWS Key Pair (public) | ~/.clap/private/mo833_rfreitas.pub |

After setting the keys, copy the contents of the folder `clap` present in this directory to `~/.clap/`.

## Compiling LAMMPS on an EFS
To avoid having to compile the application before each test, we will use a c5.large machine to quickly build LAMMPS on an EFS and then use this disk on the following runs. The procedure is described below.

1. Create an EFS

This steps will use Ansible to create an MPI-ready AWS security group:

```
$ ansible-playbook ~/.clap/groups/roles/create-sg.yml
```

This will output the security group ID on the standard output. Use this ID to create an EFS, replacing 'SG_ID' with the string received from the previous command. Replace "EFS_NAME" with any name you would like to use with your EFS:

```
$ ansible-playbook ~/.clap/groups/roles/create-efs.yml --extra "aws_sg_id=SG_ID aws_efs_name=EFS_NAME"
```

This will output a mounting IP, that will be used to mount the disk on the instances. It will be referenced below as MOUNT_IP.

2. Start compiling instance

Now that disk is created, we just need to boot a machine and use it to compile LAMMPS. We will use CLAP to start a c5.large node:
```
$ clapp node start i-c5.large
```

Add it to EFS group:

```
$ clapp group add ec2-efs node-1
```

We can now mount the EFS on this node:

```
$ clapp group action ec2-efs mount --extra "efs_mount_ip=MOUNT_IP" "efs_mount_point=/efs/"
```


3. Compile LAMMPS

This step will install all required dependencies and run the scripts that compile LAMMPS on the EFS.

```
$ clapp group add lammps node-1
```
```
$ clapp group action lammps compile
```


4. Delete the node used for compilation

After this step, we can terminate the instance used for compilation to avoid unnecessary costs.

```
$ clapp node stop node-1
```

## Running the experiment on a cluster

Since at this point we already compiled LAMMPS, can simply start a cluster, mount the EFS and run the application. The procedure is described as bellow.

1. Start the cluster

```
$ clapp cluster start cluster-t3.medium-2x
```

This will tell CLAP to start the t3.medium-2x cluster. You can use any desired cluster configuration, like cluster-c5.large-2x, cluster-t3.medium-4x or or cluster-t3.medium-8x.


The next steps will install the required dependencies to run LAMMPS. 
```
$ clapp cluster group cluster-1 lammps
```

This will exchange SSH public keys across nodes, allowing them to run MPI applications.

```
$ clapp group action lammps setup-mpi
```


2. Mount the EFS disk

Since our nodes are ready to run MPI programs, we can simply mount the EFS containing the compiled aplication.

```
$ clapp cluster group cluster-1 ec2-efs
```

Again, replace MOUNT_IP with your EFS mount IP.

```
$ clapp group action ec2-efs mount --extra "efs_mount_ip=MOUNT_IP" "efs_mount_point=/efs/"
```

3. Run the test case

At this poing, we have everything ready to run the experiments. The argument "cfgalias" allows you to set a name for the current cluster configuration. This name will appear on the automatically generated charts.

There is a CLAP script for each one of the three test cases available on the ./input folder.

```
$ clapp group action lammps run-tc-1 --extra "cfgalias=CFG-1"
```

```
$ clapp group action lammps run-tc-2 --extra "cfgalias=CFG-1"
```

```
$ clapp group action lammps run-tc-3 --extra "cfgalias=CFG-1"
```

4. Fetch the results

After running the experiment, you can download the data to your local machine:

```
$ clapp group action lammps fetch-results --extra "destfolder=`pwd`"
```

5. Stop the cluster

```
$ clapp stop cluster-1
```

6. Delete the EFS

You can run the command below if you do not intend to run more experiments. This will destroy the EFS, avoiding undesirable costs. Replace "EFS_NAME" with your EFS name.

```
$ ansible-playbook ~/.clap/groups/roles/delete-efs.yml --extra "aws_efs_name=EFS_NAME"
```

## Generating the charts

Since the experimental data was fetched to your computer, you can now generate some informative charts using the Jupyter Notebook present in this folder: ExperimentResultParser.ipynb. Simply open it using Python 3 and run its cells. You may have to install [Jupyter](https://jupyter.org/install).
