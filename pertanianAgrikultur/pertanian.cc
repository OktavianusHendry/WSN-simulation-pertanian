#include <omnetpp.h>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace omnetpp;

class SenTanah : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

  private:
    simsignal_t collisionSignal;
};

Define_Module(SenTanah);

void SenTanah::initialize()
{
    collisionSignal = registerSignal("collision");

    scheduleAt(simTime(), new cMessage("sendValue"));
}

void SenTanah::handleMessage(cMessage *msg)
{
    srand(time(NULL));
    double angkaAcak = (static_cast<double>(rand()) / RAND_MAX);
    double angkaAcak2 = (static_cast<double>(rand()) / RAND_MAX);
            EV << angkaAcak;
            EV << angkaAcak2;
    if (angkaAcak2 < angkaAcak) { // dibalik 2 ato ga 1 dluan
       EV << "Collision detected!\n";
       emit(collisionSignal, 1);
       delete msg;
       return;
    }

    double value = uniform(0, 100);

    cMessage *valueMsg = new cMessage(std::to_string(value).c_str());

    send(valueMsg, "out");

    scheduleAt(simTime() + 1.0, new cMessage("sendValue"));
}

class SenCahaya : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

  private:
    simsignal_t collisionSignal;
};

Define_Module(SenCahaya);

void SenCahaya::initialize()
{
    collisionSignal = registerSignal("collision");

    scheduleAt(simTime(), new cMessage("sendValue"));
}

void SenCahaya::handleMessage(cMessage *msg)
{
    srand(time(NULL));
    double angkaAcak = (static_cast<double>(rand()) / RAND_MAX);
    double angkaAcak2 = (static_cast<double>(rand()) / RAND_MAX);
                EV << angkaAcak;
                EV << angkaAcak2;
    if (angkaAcak2 < angkaAcak) {
       EV << "Collision detected!\n";
       emit(collisionSignal, 1);
       delete msg;
       return;
    }

    double value = uniform(73, 440);

    cMessage *valueMsg = new cMessage(std::to_string(value).c_str());

    send(valueMsg, "out");

    scheduleAt(simTime() + 1.0, new cMessage("sendValue"));
}

class SenSuhu : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

  private:
    simsignal_t collisionSignal;
};

Define_Module(SenSuhu);

void SenSuhu::initialize()
{
    collisionSignal = registerSignal("collision");

    scheduleAt(simTime(), new cMessage("sendValue"));
}

void SenSuhu::handleMessage(cMessage *msg)
{
    srand(time(NULL));
        double angkaAcak = (static_cast<double>(rand()) / RAND_MAX);
        double angkaAcak2 = (static_cast<double>(rand()) / RAND_MAX);
        EV << angkaAcak;
        EV << angkaAcak2;
    if (angkaAcak2 < angkaAcak) {
       EV << "Collision detected!\n";
       emit(collisionSignal, 1);
       delete msg;
       return;
    }

    double value = uniform(23, 36);

    cMessage *valueMsg = new cMessage(std::to_string(value).c_str());

    send(valueMsg, "out");

    scheduleAt(simTime() + 1.0, new cMessage("sendValue"));
}

class Server : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

  private:
    std::string temperature;
    std::string light;
    std::string soil;
    simsignal_t collisionSignal;
};

Define_Module(Server);

void Server::initialize()
{
    collisionSignal = registerSignal("collision");
}

void Server::handleMessage(cMessage *msg)
{
    if (msg->arrivedOn("inTanah") && soil.empty()) {
        soil = msg->getName();
    } else if (msg->arrivedOn("inCahaya") && light.empty()) {
        light = msg->getName();
    } else if (msg->arrivedOn("inSuhu") && temperature.empty()) {
        temperature = msg->getName();
    } else {
        EV << "Collision detected at Server!\n";
        emit(collisionSignal, 1);
        delete msg;
        return;
    }

    if (!temperature.empty() && !light.empty() && !soil.empty()) {
        std::string mergedMsgName = "Temp: " + temperature + "°C" + " Light: " + light + "lx" +" and soil moisture: " + soil + "%";
        cMessage *mergedMsg = new cMessage(mergedMsgName.c_str());
        send(mergedMsg, "out");

        temperature.clear();
        light.clear();
        soil.clear();
    }

    delete msg;
}

class User : public cSimpleModule
{
  protected:
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(User);

void User::handleMessage(cMessage *msg)
{
    EV << "Received message: " << msg->getName() << endl;
    delete msg;
}
