#!/usr/bin/env bash
if [ ! "$(ls -A /path/to/local/dir)" ]; then
sudo mount -t cifs //ipaddess/path/to/dir /path/to/local/dir -o user="user",password="password",uid=$(id -u),gid=$(id -g)
digikam
else
digikam
fi
