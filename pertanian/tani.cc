#include <omnetpp.h>
#include <string>

using namespace omnetpp;

class SenTanah : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(SenTanah);

void SenTanah::initialize()
{
    scheduleAt(simTime(), new cMessage("sendValue"));
}

void SenTanah::handleMessage(cMessage *msg)
{
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
};

Define_Module(SenCahaya);

void SenCahaya::initialize()
{
    scheduleAt(simTime(), new cMessage("sendValue"));
}

void SenCahaya::handleMessage(cMessage *msg)
{
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
};

Define_Module(SenSuhu);

void SenSuhu::initialize()
{
    scheduleAt(simTime(), new cMessage("sendValue"));
}

void SenSuhu::handleMessage(cMessage *msg)
{
    double value = uniform(23, 36);

    cMessage *valueMsg = new cMessage(std::to_string(value).c_str());

    send(valueMsg, "out");

    scheduleAt(simTime() + 1.0, new cMessage("sendValue"));
}

class Server : public cSimpleModule
{
  protected:
    virtual void handleMessage(cMessage *msg) override;

  private:
    std::string temperature;
    std::string light;
    std::string soil;
};

Define_Module(Server);

void Server::handleMessage(cMessage *msg)
{
    if (strcmp(msg->getSenderModule()->getFullName(), "senTanah") == 0) {
        soil = msg->getName();
    } else if (strcmp(msg->getSenderModule()->getFullName(), "senCahaya") == 0) {
        light = msg->getName();
    } else if (strcmp(msg->getSenderModule()->getFullName(), "senSuhu") == 0) {
        temperature = msg->getName();
    }

    if (!temperature.empty() && !light.empty() && !soil.empty()) {
        std::string mergedMsgName = "Temp: " + temperature + "°C" +
                " Light: " + light + "lx" +" and soil moisture: " + soil + "%";
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
