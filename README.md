## Tutorial

##### Install the dependencies:

CentOS:
```bash
yum -y install make
```

Debian:
```bash
apt -y install make
```

##### Setup

CentOS:
```bash
make centos install
```

Debian:
```bash
make debian install
```

##### Authorize SMTP
Fill in the .env file:
```bash
nano /etc/sshnotify/.env
```

##### Test

Fills with test content and run

```bash
make append run
```

Whenever you run it, you will have a comparison between the files:

```
/var/log/auth.log

/etc/sshnotify/auth.log
```

That will keep them the same.

When there is a difference, an email will be triggered.