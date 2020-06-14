mkdir -p ./clap/configs
mkdir -p ./clap/groups


rsync -au ~/.clap/configs/* ./clap/configs && rsync -au ./clap/configs/* ~/.clap/configs
rsync -au ~/.clap/groups/* ./clap/groups && rsync -au ./clap/groups/* ~/.clap/groups

