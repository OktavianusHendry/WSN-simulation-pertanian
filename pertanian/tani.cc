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

class Esp32 : public cSimpleModule
{
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;

    private:
        std::string temperature;
        std::string light;
        std::string soil;
        std::string forwardMessage;
};

Define_Module(Esp32);

void Esp32::initialize()
{

}

void Esp32::handleMessage(cMessage *msg)
{
    std::string senderName = msg->getSenderModule()->getFullName();
        if (senderName.find("senTanah") != std::string::npos) {
            soil = msg->getName();
        } else if (senderName.find("senCahaya") != std::string::npos) {
            light = msg->getName();
        } else if (senderName.find("senSuhu") != std::string::npos) {
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
        } else if (senderName.find("esp32_3") != std::string::npos) {
            forwardMessage = msg->getName();
            std::string fMsg = forwardMessage;
            cMessage *fMessage = new cMessage(fMsg.c_str());
            send(fMessage, "out");
        }

        delete msg;
}

class Server : public cSimpleModule
{
  protected:
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Server);

void Server::handleMessage(cMessage *msg)
{
    send(msg, "out1");

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
