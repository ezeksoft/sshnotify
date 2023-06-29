compile:
	g++ src/main.cpp -lcurl -I /home/linuxbrew/.linuxbrew/include -o /home/eze/bin/main

dev:
	/home/eze/bin/main env /home/eze/src/.env

compile_and_dev:
	make compile dev

run:
	sshnotify env /etc/sshnotify/.env

append:
	cat /home/eze/src/auth.log.example >> /var/log/auth.log

env:
	cp ./src/.env /etc/sshnotify/.env

centos:
	cd /etc/yum.repos.d/;
	sed -i 's/mirrorlist/#mirrorlist/g' /etc/yum.repos.d/CentOS-*;
	sed -i 's|#baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g' /etc/yum.repos.d/CentOS-*;
	yum update -y;
	yum -y install libcurl-devel

centos_dev:
	cd /etc/yum.repos.d/
	sed -i 's/mirrorlist/#mirrorlist/g' /etc/yum.repos.d/CentOS-*
	sed -i 's|#baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g' /etc/yum.repos.d/CentOS-*
	yum update -y

	yum -y install gcc-c++.x86_64 libcurl-devel git
	/bin/bash brew.sh
	brew install nlohmann-json

debian:
	apt update
	apt upgrade
	apt -y install libcurl4-openssl-dev

debian_dev:
	apt -y install g++ curl libcurl4-openssl-dev git # build-essential
	/bin/bash brew.sh
	brew install nlohmann-json

ubuntu:
	make debian
	
ubuntu_dev:
	make debian_dev

install:
	mkdir -p /etc/sshnotify
	cp ./bin/main /etc/sshnotify
	cp -n ./src/.env.example /etc/sshnotify/.env
	/bin/bash -c 'echo "* * * * * /etc/sshnotify/main env /etc/sshnotify/.env" >> /etc/crontab'
	/bin/bash -c 'echo "* * * * * sleep 10; /etc/sshnotify/main env /etc/sshnotify/.env" >> /etc/crontab'
	/bin/bash -c 'echo "* * * * * sleep 20; /etc/sshnotify/main env /etc/sshnotify/.env" >> /etc/crontab'
	/bin/bash -c 'echo "* * * * * sleep 30; /etc/sshnotify/main env /etc/sshnotify/.env" >> /etc/crontab'
	/bin/bash -c 'echo "* * * * * sleep 40; /etc/sshnotify/main env /etc/sshnotify/.env" >> /etc/crontab'
	/bin/bash -c 'echo "* * * * * sleep 50; /etc/sshnotify/main env /etc/sshnotify/.env" >> /etc/crontab'
	rm -rf /usr/bin/sshnotify
	ln -s /etc/sshnotify/main /usr/bin/sshnotify

docker_centos:
	docker run --name SSHNotify -d -t -i --mount type=bind,source=C:/www/github/sshnotify,target=/home/eze -it centos
	docker start SSHNotify
	docker exec -u 0 -w /home/eze -it SSHNotify /bin/bash
	
docker_debian:
	docker run --name SSHNotifyDeb -d -t -i --mount type=bind,source=C:/www/github/sshnotify,target=/home/eze -it debian
	docker start SSHNotifyDeb
	docker exec -u 0 -w /home/eze -it SSHNotifyDeb /bin/bash
	
test_centos:
	make centos install env append run

test_debian:
	make ubuntu install env append run