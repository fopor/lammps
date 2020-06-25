echo "Creating Security Group used for ESF and MPI communication"

sg_return=$(ansible-playbook ~/.clap/groups/roles/create-sg.yml)

AWS_SG_ID=$(echo $sg_return | grep "group=sg-" | cut -d '=' -f 2)

echo "Got the Security ID = $AWS_SG_ID"

echo "Creating EFS"
efs_return=$(ansible-playbook ~/.clap/groups/roles/create-efs.yml --extra "aws_sg_id=$AWS_SG_ID")

echo $efs_return | grep ip_address 