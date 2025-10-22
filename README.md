# FLASH M33
Connect to satelite exp by RNDIS.
Copy file from armgcc/debug/satelite_exp_v100.elf and paste it to the RNDIS directory.
Connect to core A using uart console or ssh.
If you use ssh, type sudo -i to go into root.
If you use uart to connect to core A, then go to next step.
Every single time when you want to flash M33, type:
m33_stop
m33_run /home/bee/satelite_exp_v100.elf