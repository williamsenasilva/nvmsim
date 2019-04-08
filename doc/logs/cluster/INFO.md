# Cluster - Info

#### ssh
```bash
will@odysseuss:~/workspace/particular/nvmsim$ ssh william@172.17.163.240
The authenticity of host '172.17.163.240 (172.17.163.240)' can't be established.
ECDSA key fingerprint is SHA256:Ab41zbYEL9Xbr+2C/5LX+01Py7BeTZC061SXRcwYrMM.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added '172.17.163.240' (ECDSA) to the list of known hosts.
william@172.17.163.240's password: 
Welcome to Ubuntu 18.04.2 LTS (GNU/Linux 4.15.0-46-generic x86_64)

 * Documentation:  https://help.ubuntu.com
 * Management:     https://landscape.canonical.com
 * Support:        https://ubuntu.com/advantage

 * Ubuntu's Kubernetes 1.14 distributions can bypass Docker and use containerd
   directly, see https://bit.ly/ubuntu-containerd or try it now with

     snap install microk8s --classic

 * Canonical Livepatch is available for installation.
   - Reduce system reboots and improve kernel security. Activate at:
     https://ubuntu.com/livepatch

16 packages can be updated.
0 updates are security updates.

*** System restart required ***
Last login: Tue Apr  2 21:00:05 2019 from 172.31.43.5
william@cluster-4:~$ passwd
Changing password for william.
(current) UNIX password: 
Enter new UNIX password: 
Retype new UNIX password: 
passwd: password updated successfully
```

#### Número de processadores
```bash
william@cluster-4:~$ echo $(nproc)
24
```

#### Espaço em disco
```bash
william@cluster-4:~/workspace$ df -h
Filesystem                              Size  Used Avail Use% Mounted on
udev                                     63G     0   63G   0% /dev
tmpfs                                    13G  2,9M   13G   1% /run
/dev/mapper/centos_lcpd--cluster4-root   49G   40G  7,2G  85% /
tmpfs                                    63G  160M   63G   1% /dev/shm
tmpfs                                   5,0M  4,0K  5,0M   1% /run/lock
tmpfs                                    63G     0   63G   0% /sys/fs/cgroup
/dev/sda1                               477M  110M  338M  25% /boot
/dev/mapper/centos_lcpd--cluster4-home   57G   18G   36G  34% /home
/dev/loop1                              2,3M  2,3M     0 100% /snap/gnome-calculator/260
/dev/loop2                               35M   35M     0 100% /snap/gtk-common-themes/818
/dev/loop5                              141M  141M     0 100% /snap/gnome-3-26-1604/78
/dev/loop4                               36M   36M     0 100% /snap/gtk-common-themes/1198
/dev/loop6                               13M   13M     0 100% /snap/gnome-characters/139
/dev/loop7                              141M  141M     0 100% /snap/gnome-3-26-1604/74
/dev/loop8                              141M  141M     0 100% /snap/gnome-3-26-1604/82
/dev/loop10                             3,8M  3,8M     0 100% /snap/gnome-system-monitor/57
/dev/loop9                               91M   91M     0 100% /snap/core/6405
/dev/loop11                             3,8M  3,8M     0 100% /snap/gnome-system-monitor/51
/dev/loop12                              15M   15M     0 100% /snap/gnome-logs/45
/dev/loop13                              13M   13M     0 100% /snap/gnome-characters/103
/dev/loop15                              35M   35M     0 100% /snap/gtk-common-themes/1122
/dev/loop14                             2,4M  2,4M     0 100% /snap/gnome-calculator/180
/dev/loop16                              15M   15M     0 100% /snap/gnome-logs/37
tmpfs                                    13G   16K   13G   1% /run/user/121
tmpfs                                    13G  104K   13G   1% /run/user/1005
/dev/loop17                              92M   92M     0 100% /snap/core/6531
/dev/sdc1                               917G  373M  870G   1% /media/home2
/dev/sdb1                               917G  479G  392G  56% /media/home
/dev/loop0                               54M   54M     0 100% /snap/core18/782
/dev/loop18                             1,0M  1,0M     0 100% /snap/gnome-logs/57
/dev/loop19                             4,2M  4,2M     0 100% /snap/gnome-calculator/352
/dev/loop20                             3,8M  3,8M     0 100% /snap/gnome-system-monitor/70
/dev/loop21                              15M   15M     0 100% /snap/gnome-characters/206
/dev/loop22                             144M  144M     0 100% /snap/gnome-3-28-1804/23
tmpfs                                    13G   56K   13G   1% /run/user/1004
/dev/loop23                              90M   90M     0 100% /snap/core/6673
tmpfs                                    13G     0   13G   0% /run/user/1006
```
