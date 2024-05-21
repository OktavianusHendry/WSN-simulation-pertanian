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

simple Esp32
{
    @display("i=old/card2");
    gates:
        input inTanah;
        input inCahaya;
        input inSuhu;
        input inFree;
        output out;
}

simple Server
{
    @display("i=device/server2,,0");
    gates:
        input in1Esp32;
        input in2Esp32;
        input in3Esp32;
        output out1;
        output out2;
        output out3;
}

simple User
{
    @display("i=block/user,,0");
    gates:
        input in1Server;
        input in2Server;
        input in3Server;
}


network PertanianNetwork
{
    @display("bgi=background/terrain,s;bgb=430,363");
    submodules:
        senTanah: SenTanah {
            @display("p=170,29");
        }
        senCahaya: SenCahaya {
            @display("p=118,28");
        }
        senSuhu: SenSuhu {
            @display("p=118,85");
        }
        esp32: Esp32 {

            @display("p=198,128");
        }

        senTanah2: SenTanah {

            @display("p=387,241");
        }
        senCahaya2: SenCahaya {

            @display("p=387,324");
        }
        senSuhu2: SenSuhu {

            @display("p=323,325");
        }
        esp32_2: Esp32 {
            @display("p=323,241");
        }

        senTanah3: SenTanah {

            @display("p=109,325");
        }
        senCahaya3: SenCahaya {

            @display("p=29,324");
        }
        senSuhu3: SenSuhu {

            @display("p=29,241");
        }
        esp32_3: Esp32 {
            @display("p=109,241");
        }
        server: Server {
            @display("p=323,128");
        }
        user: User {
            @display("p=387,39");
        }
    connections allowunconnected:
        senTanah.out --> {  delay = 0.001s; } --> esp32.inTanah;
        senCahaya.out --> {  delay = 0.002s; } --> esp32.inCahaya;
        senSuhu.out --> {  delay = 0.0025s; } --> esp32.inSuhu;
        esp32.out --> {  delay = 0.01s; } --> server.in1Esp32;
        server.out1 --> {  delay = 0.001s; } --> user.in1Server;

        senTanah2.out --> {  delay = 0.001s; } --> esp32_2.inTanah;
        senCahaya2.out --> {  delay = 0.002s; } --> esp32_2.inCahaya;
        senSuhu2.out --> {  delay = 0.0025s; } --> esp32_2.inSuhu;
        esp32_2.out --> {  delay = 0.02s; } --> server.in2Esp32;
        server.out2 --> {  delay = 0.001s; } --> user.in2Server;

        senTanah3.out --> {  delay = 0.001s; } --> esp32_3.inTanah;
        senCahaya3.out --> {  delay = 0.002s; } --> esp32_3.inCahaya;
        senSuhu3.out --> {  delay = 0.0025s; }--> esp32_3.inSuhu;
        esp32_3.out --> {  delay = 0.04s; } --> esp32.inFree;

        server.out3 --> {  delay = 0.001s; } --> user.in3Server;
}
