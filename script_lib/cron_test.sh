#!/bin/sh


LOG=/media/Caldera/Logs/bkup.log

echo '\nstarting crontab test ....\n'
echo '\nstarting crontab test ....\n' >> $LOG

echo '**** cron triggered  - ' $(date)
echo '**** cron triggered  - ' $(date) >> $LOG

