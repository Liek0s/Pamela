#!/usr/bin/bash

xml_output() {

    local output="
	<user username=\"$username\">\n
	<container>$container</container>\n
	<mountpoint>/home/</mountpoint>\n
	<keyfile>$keyfile</keyfile>\n
	<fstype>$fstype</fstype>\n
	</user>\n"
    echo -e "Please copy the following output into pamela-config.xml ...\n\n"
    echo -e $output
}

cleanup() {
    
    echo -e "A bit of housekeeping ..." >&2
    cryptsetup luksClose $2
    losetup -d $1
}

generate_keyfile() {
    
    local inputfile="/dev/urandom"
    # local outfilepath="/root/user-containers/.keyfiles/"
    local outfilepath="./"
    local outfile=$outfilepath"$username.key"

    echo -e "Generating keyfile ..." >&2
    dd if=$inputfile of=$outfile bs=1024 count=4
    echo -e "Done! Keyfile is $outfile. Removing write permissions." >&2
    chmod 0400 $outfile
    echo $outfile
}

format_device() {

    local volume="$username-temp"

    echo -e "Encrypting container as LUKS" >&2
    cryptsetup luksFormat $1 $2    
    cryptsetup luksOpen $1 $volume -d $2
    echo -e "Formatting /dev/mapper/$volume as $fstype" >&2
    mkfs."$fstype" "/dev/mapper/$volume" >&/dev/null
    cleanup $1 $volume
}

attach_loop_device() {
    
    losetup $1 $2
    echo -e "Container attached to $1" >&2
}

create_container() {
    
    local inputfile="/dev/urandom"
    # outfilepath="/root/user-containers/"
    local outfilepath="./"
    local outfile=$outfilepath"$username-$(date +'%d%m%Y').img"    

    echo -e "Generating container...\nPlease wait, this may take a while depending on the size ..." >&2
    dd if=$inputfile of=$outfile count=$size >&/dev/null
    echo -e "Container of $size was created at $outfile!" >&2
    echo $outfile
}

usage() { echo "Usage: $0 [ -u username ] [ -f fstype ] [ -c size ( like dd count= ) ]"; exit 1; }

options() {

    while getopts "u:f:c:" o; do
	case "${o}" in
	    u)
		username=${OPTARG}
		;;
	    f)
		fstype=${OPTARG}
		;;
	    c)
		size=${OPTARG}
		;;
	    *)
		usage
		;;
	esac
    done
    if [ -z $username ] || [ -z $size ]; then
	usage
    elif [ -z $fstype ]; then
	fstype="ext4"
    fi
}

main() {

    if [[ $EUID -ne 0 ]]; then
	echo -e "This script must be run as root" 1>&2
	exit 1
    fi    
    options $@
    container=$(create_container) 
    loopdevice=$(losetup -f)
    attach_loop_device $loopdevice $container
    keyfile=$(generate_keyfile)
    format_device $loopdevice $keyfile
    xml_output
    echo -e "Done!"; exit 0;
}

main $@
