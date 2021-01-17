#!/bin/sh

# systemS manufacture
R=$(dmidecode -s system-manufacturer)
echo "System manufacturer is" $R

# do not run this scrip from a virtual machine
if test "$R" = "innotek GmbH"
then
	echo "exiting script"
	exit 1
fi


test "innotek GmbH" != "$R" && RETURNED="real" || RETURNED="vitrual"

echo "**********************************************************************************"
echo $RETURNED
echo "**********************************************************************************"
