#!/bin/sh
#
# backup
#        /home/mam1
#        /media/Blackhole
#        /media/Caldera
#        /media/Video
#        /media/sysuts
#
#
#
# run from /etc/crontab
#
# must be run as root
#
##########################################################################################

LOG=/media/Caldera/Logs/bkup.log

echo "*******************************************************************" >> $LOG
echo "******* cron triggered  -  Sun 10 Jan 2021 11:01:57 AM EST ********" >> $LOG
echo "*******************************************************************\n" >> $LOG

echo '******************* starting system backup ************************' >> $LOG
echo '\nlog written to' $LOG $(date) '\n'
echo '\nlog written to' $LOG $(date) '\n' >> $LOG
echo 'backup started\n'

echo '*******************************************************************' >> $LOG
echo '**** BlackHole backup started  - ' $(date) >> $LOG
mount -lv UUID=6bb923c4-6ee3-4623-bdf9-a3a9d06a6b9b  /mnt/BlackHoleBkp >> $LOG
rsync   -arvH  --delete-excluded --exclude  torrent_downloads/complete  --exclude /torrent_downloads/incomplete /media/BlackHole/ /mnt/BlackHoleBkp/  >> $LOG
sleep 5
umount -lv /mnt/BlackHoleBkp 2>&1 | tee -a  $LOG
echo '**** BlackHole backup finished - ' $(date) >> $LOG
echo '*******************************************************************' >> $LOG

echo '**** Caldera backup started  - ' $(date)  >> $LOG
mount -lv UUID=48d3ba3f-65eb-4ef1-a143-246e89f1988c  /mnt/CalderaBkp  >> $LOG
rsync   -arvH  --delete   /media/Caldera/ /mnt/CalderaBkp/  >> $LOG
sleep 5
umount -lv /mnt/CalderaBkp  2>&1 | tee -a  $LOG
echo '**** Caldera backup finished - ' $(date)  >> $LOG
echo '*******************************************************************' >> $LOG

echo '**** Video backup started  - ' $(date)  >> $LOG 
mount -lv UUID=5c987372-bb54-440e-8a37-7dd51c988524  /mnt/VideoBkp   >> $LOG
rsync   -arvH  --delete   /media/Video/ /mnt/VideoBkp/  >> $LOG
sleep 5
umount -lv /mnt/VideoBkp  2>&1 | tee -a  $LOG
echo '**** Video backup finished - ' $(date)  >> $LOG 
echo '*******************************************************************' >> $LOG

echo '**** sysuts backup started  - ' $(date)  >> $LOG
mount -lv UUID=f3decdd6-bd82-46fe-b990-3779ef0068dc  /mnt/sysutsBkp  >> $LOG
rsync   -arvH  --delete   /media/sysuts/ /mnt/sysutsBkp/  >> $LOG
sleep 5
umount -lv /mnt/sysutsBkp   2>&1 | tee -a  $LOG
echo '**** sysuts backup finished - ' $(date)  >> $LOG
echo '*******************************************************************' >> $LOG

echo '**** home backup started  - ' $(date)  >> $LOG
mount -lv UUID=f0f91a83-22e4-469f-8dc5-ba2d05043bb0  /mnt/dataBkp  >> $LOG
rsync   -arvH   --delete-excluded --exclude Git /home/mam1/ /mnt/dataBkp/  >> $LOG
sleep 5
umount -lv /mnt/dataBkp   2>&1 | tee -a  $LOG
echo '**** home backup finished - ' $(date)  >> $LOG
echo '*******************************************************************' >> $LOG

echo '**** all backups complete - ' $(date)  >> $LOG 
echo '*******************************************************************' >> $LOG
echo '*******************************************************************\n' >> $LOG
echo '\nall backups complete\nnormal termination\n' 

