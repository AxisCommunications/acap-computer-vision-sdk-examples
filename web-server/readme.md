# web server on camera
https://www.tecmint.com/install-apache-web-server-in-a-docker-container/
/opt/website/docker.html
```sh
docker -H tcp://$AXIS_TARGET_IP run -it -p 8080:80 -v /opt/website/:/usr/local/apache2/htdocs/ httpd:2.4
docker -H tcp://$AXIS_TARGET_IP run -it -p 8080:80 cblomart/rpi-monkey

docker-compose -H tcp://$AXIS_TARGET_IP:2375 -f ping.yml up

https://github.com/monkey/monkey
cd web-server
export APP=docker-sandbox.se.axis.com/carlcn/monkey
docker build . --build-arg http_proxy --build-arg https_proxy -t $APP
docker push $APP
docker -H tcp://$AXIS_TARGET_IP system prune -af
docker -H tcp://$AXIS_TARGET_IP run --rm -p 8080:80 --privileged -it $APP

docker run --rm -p 8082:80 -it $APP

git clone https://github.com/monkey/monkey && cd monkey && cd build && cmake .. && make depend && make monkey-bin && mv mk_bin/monkey /tmp/monkey && chmod u+x /tmp/monkey




	if [ ! -d src/monkey-$(MONKEY_VERSION)/build ]; then cd src/monkey-$(MONKEY_VERSION) &&
   CFLAGS="-Os -march=native" ./configure --local --static-lib-mode --static-plugins=auth,fastcgi,tls,dirlisting,cheetah,liana,logger --disable-plugins=cgi,cheetah,mandril; fi
	if [ ! -x src/monkey-$(MONKEY_VERSION)/build/monkey ]; then make -C src/monkey-$(MONKEY_VERSION); fi
	strip --strip-all src/monkey-$(MONKEY_VERSION)/build/monkey

git clone https://github.com/monkey/monkey
cd monkey
git reset && git checkout . && git clean -xfd
mkdir build
CFLAGS="-Os -march=native" ./configure --local --static-lib-mode --static-plugins=auth,fastcgi,tls,dirlisting,cheetah,liana,logger --disable-plugins=cgi,cheetah,mandril
cd build
make
bin/monkey -c conf/ -p 8080
```


git clone https://github.com/monkey/monkey
cd monkey
git reset && git checkout . && git clean -xfd
mkdir build
cd build
cmake ..\
  -DMK_CONF_LISTEN=80\
  -DMK_PATH_WWW=/home/carlcn/source/monkey/htdocs\
  -DMK_PATH_PIDPATH=/home/carlcn/source/monkey\
  -DMK_PIDFILE=pid.txt
make
bin/monkey -c conf/ -p 80

git clone https://github.com/monkey/monkey
cd monkey
git reset && git checkout . && git clean -xfd
mkdir build
./configure
cd build
make
bin/monkey -c conf/ -p 8080



RUN git clone https://github.com/monkey/monkey &&\
    cd monkey &&\
    mkdir -p build &&\
    cd build &&\
    cmake ..\
    -DBUILD_LOCAL=1\
    -DMK_CONF_LISTEN=80\
    -DMK_STATIC_LIB_MODE=Yes\
    -DMK_TRACE=Yes\
    -DMK_PLUGIN_AUTH=Yes\
    -DMK_PLUGIN_FASTCGI=Yes\
    -DMK_PLUGIN_TLS=Yes\
    -DMK_PLUGIN_DIRLISTING=Yes\
    -DMK_PLUGIN_CHEETAH=Yes\
    -DMK_PLUGIN_LIANA=Yes\
    -DMK_PLUGIN_LOGGER=Yes\
    -DMK_PLUGIN_CGI=Yes\
    -DMK_PLUGIN_MANDRIL=No\
    -DMK_PATH_WWW=/var/www\
    -DMK_PATH_PIDPATH=/etc/monkey\
    -DMK_PIDFILE=monkey.pid &&\
    make depend &&\
    make monkey-bin
