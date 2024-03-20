simple SenTanah
{
    @display("i=block/source,,0");
    gates:
        output out;
}

simple SenCahaya
{
    @display("i=misc/sun,,0");
    gates:
        output out;
}

simple SenSuhu
{
    @display("i=misc/cloud,,0");
    gates:
        output out;
}

simple Server
{
    @display("i=device/server2,,0");
    gates:
        input inTanah;
        input inCahaya;
        input inSuhu;
        output out;
}

simple User
{
    @display("i=block/user,,0");
    gates:
        input in;
}


network PertanianNetwork
{
    @display("bgi=background/terrain,s;bgb=300,300");
    submodules:
        senTanah: SenTanah {
            @display("p=81.899994,29.699999");
        }
        senCahaya: SenCahaya {
            @display("p=29,28");
        }
        senSuhu: SenSuhu {
            @display("p=29.699999,85.49999");
        }
        server: Server {
            @display("p=150.29999,161.09999");
        }
        user: User {
            @display("p=274.5,29.699999");
        }
    connections allowunconnected:
        senTanah.out --> { delay = 0.001s; } --> server.inTanah;
        senCahaya.out --> { delay = 0.002s; } --> server.inCahaya;
        senSuhu.out --> { delay = 0.0025s; } --> server.inSuhu;
        server.out --> { delay = 0.001s; } --> user.in;

}
