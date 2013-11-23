#!/usr/bin/bash

isRoot() { # Make sure only root can run this script 
    if [[ $EUID -ne 0 ]]; then
	echo -e "This script must be run as root" 1>&2
	exit 1
    fi
}

attach_to_device() { # attaches to loopdevice 
    
    loopdev=$1
    container=$2

    losetup $loopdev $container
    echo -e "Container attached to loopdevice : $freeloopdev"
}

gener_keyfile() {
    
    outfile=$1
    dd if="/dev/urandom" of=$outfile bs=1024 count=4
    # + CHMOD ROOT
    echo -e "Key generated as : $outfile"
}

format_as_luks() {

    keyfile="$username-kf.key"
    gener_keyfile $keyfile
    cryptsetup luksFormat $loopdev $keyfile
}

format_volume_fs() {

    volume=$2
    fstype=$1
    
    cryptsetup luksOpen $loopdev $volume -d $keyfile
    mkfs."$fstype" "/dev/mapper/$volume"
}

gener_container() { # Generates user container to be used for home
    
    infile=$1
    username=$3
    outfile=$2"$username-$(date +'%d%m%Y').img"    
    
    dd if=$infile of=$outfile bs=1M count=10 # Size depends on arg
    echo -e "Container for user ' $username ' is : $outfile"
    # Move container ...
}

cleanup() {
    
    cryptsetup luksClose $volume
    losetup -d $loopdev
}

isRoot

if [[ $# -ne 1 ]]; then
    echo -e "Missing required argument : [ username ]"
    exit 1
else
    username=$1
fi

gener_container "/dev/urandom" "./" $username 
attach_to_device $(losetup -f) $outfile
format_as_luks
format_volume_fs "ext4" "$username-tmp-volume"
cleanup

echo -e "Success!"; exit 1
