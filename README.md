               _   ___              _
      _ _  ___| |_| _ )_ _ _  _ _ _| |_
     | ' \/ -_)  _| _ \ '_| || (_-<|   |
     |_||_\___|\__|___/_|  \_,_/__/|_|_|
    ====================================

netBrush is a network based drawing programming, meaning it allows
multiple people to draw on the same canvas at the same time. The
feature set of netBrush includes a variable sized canvas canvas and a
simple brush and airbrush tools, changing brush size, hardness, color,
opacity, etc. is possible. You can also record and play back events
send to the server as well as manipulate them, for example to scale up
an image.

netBrush is based on SDL and doesn't use any external widget
libraries, so it should be reasonably portable. In addition to the
base SDL library netBrush also uses scons, SDL_net and SDL_image.

Latest version of netBrush can be found at:

* https://github.com/Grumbel/netbrush

Question and comments can go to the author, who is reachable at
<grumbel@gmail.com>.


Compile & Install
-----------------

    mkdir build
    cd build
    cmake ..
    make
    make install

Install location can be changed by giving an option to the `cmake`
call:

    cmake .. -DCMAKE_INSTALL_PREFIX:PATH=/tmp/bla/run


Usage
-----

netBrush is client/server based, so to make full use of it, you have
to start the server first, a simple:

    $ ./netbrush-server 4711

will do, 4711 being the port number.

To start the client you have to give it a hostname and a port, like
this:

    $ ./netbrush-client localhost 4711

You can save the canvas by pressing F5 and you can cause the server to
save the event data by sending a "save NAME" to the server port:

    $ nc localhost 4711
    save mypicture

You can also replay an event log back to the server, to do so use:

    $ cat images/nolok-1.nbr | ./scale.rb 1 | nc localhost 4711

Changing the value you give to scale.rb scales the image up or down.