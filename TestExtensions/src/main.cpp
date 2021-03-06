/*
 * File:   main.cpp
 * Author: DrSobik
 *
 * Created on July 15, 2014, 6:25 PM
 */

#include <QtCore>
#include <QThread>
#include <QApplication>

#include <string>
#include <iostream>
#include <map>
#include <unordered_map>
#include <functional>
#include <vector>
#include <atomic>
#include <mutex>
#include <typeinfo>
#include <typeindex>

#include "Object"
#include "Clonable"
#include "Runnable"
#include "Assignable"
#include "Functor"
#include "Comparable"
#include "SavableRestorable"
#include "WritableReadable"
#include "Exceptions"
#include "Operationable"
#include "SmartPointer"
#include "MathExt"
#include "RandExt"
#include "SenderReceiver"
#include "Signals"
#include "Operationable"

#include "Solver"
#include "Algorithm"
#include "Stopable"

#include "Variables"
#include "Parser"

#include "test.h"

#include <QTextStream>
#include <QVector>

#include <string>

#include<QMutex>

using namespace std;

using namespace Common;
using namespace Common::Interfaces;
using namespace Common::Exceptions;
using namespace Common::SmartPointers;
using namespace Common::Rand;
using namespace Common::Signals;

class OB1 : public QObject {
public:

    OB1() :
    QObject() {
    }

};

class OB2 : public QObject {
public:

    OB2() :
    QObject(0) {
    }

};

class D : public OB1 {
    Q_OBJECT

public:

signals:

    void someSig();

public slots:

    void someSlot() {
    }

public:

    D() {
        QObject::connect(this, SIGNAL(someSig()), this, SLOT(someSlot()));
    }

};

class Cloner : public Object<>, Clonable {
public:
    int x;

    Cloner() : BasicObject(), Clonable() {
        x = -1;
    }

    Cloner(const Cloner& other) : BasicObject(), Object(other), Clonable() {
        x = other.x;
    }

    virtual ~Cloner() {
        QTextStream out(stdout);
        //out << "~Cloner()" << endl;
    }

    virtual Cloner* clone() {
        return new Cloner(*this);
    }

};

class Assigner : public Object<QObject>, AssignableFrom<const Assigner&> {
public:
    int x;

    Assigner() : BasicObject(), Object(), AssignableFrom() {
    }

    Assigner(const Assigner& other) : BasicObject(), Object(other), AssignableFrom() {
    }

    Assigner& operator=(const Assigner& other) {

        x = other.x;

        return *this;

    }

};

class AssignerClonner : public Object<QObject>, AssignableFrom<const AssignerClonner&>, ClonableTo<AssignerClonner> {
public:
    int x;

    AssignerClonner() : BasicObject(), Object(), AssignableFrom(), ClonableTo() {
    }

    AssignerClonner(const AssignerClonner& other) : BasicObject(), Object(other), AssignableFrom(), ClonableTo() {

        x = other.x;

    }

    AssignerClonner& operator=(const AssignerClonner& other) {

        AssignerClonner& o = (AssignerClonner&) other;

        x = o.x;

        return *this;

    }

    virtual AssignerClonner* clone() {
        return new AssignerClonner(*this);
    }

};

class Dfunctor : public Functor<double, double, double> {
public:

    virtual double operator()(const double& x1, const double& x2) {
        return 150.0 + x1 + x2;
    }

};

class Comparator : public ComparableTo<Comparator> {
public:

    int x;

    Comparator(const int& x) {
        this->x = x;
    }

    virtual bool operator==(const Comparator& x) {
        return this->x == x.x;
    }

    virtual bool operator<(const Comparator& x) {
        return this->x < x.x;
    }

};

class ComparatorEQnEQ : public ComparableEqTo<ComparatorEQnEQ> {
public:
    int x;

    virtual bool operator==(const ComparatorEQnEQ& other) {
        return x == other.x;
    }

};

class SR : public SavableRestorable {
public:
    int x;
    int savedX;

    virtual ~SR() {
    }

    virtual void save() {
        savedX = x;
    }

    virtual void restore() {
        x = savedX;
    }

};

class SToRFrom : public SavableToRestorableFrom<int, int> {
private:
    int x;

public:

    SToRFrom() : SavableToRestorableFrom() {
    }

    void setX(int x) {
        this->x = x;
    }

    int getX() const {
        return x;
    }

    virtual void saveTo(int& other) {
        other = x;
    }

    virtual void restoreFrom(const int& other) {
        x = other;
    }

};

class WR : public WritableToReadableFrom<QTextStream, QTextStream> {
private:
    int x;

public:

    void setX(int x) {
        this->x = x;
    }

    int getX() const {
        return x;
    }

    virtual QTextStream& operator>>(QTextStream& out) const {

        out << "Written : " << getX() << endl;

        return out;
    }

    virtual QTextStream& operator<<(QTextStream& in) {

        QTextStream out(stdout);

        out << "Enter a number : " << endl;

        int readX = -1;

        in >> readX;

        setX(readX);

        return in;
    }

};

class NonAssigner : public AssignableFrom<const double&>, NonAssignableFrom<unsigned int>, NonAssignableFrom<QString> {
private:
    double x;

public:

    virtual AssignableFrom& operator=(const double& otherD) {
        x = otherD;

        return *this;
    }

};

class NonAssigner1 : public NonAssignableFrom<Object<>>
{
    private:
    double x;

    public:

    //virtual NonAssignableFrom<Object<>>& operator=(const Object<>& other){
    //x = other.x;

    //	return *this;
    //}

};

class Deleter {
public:

    const char* x;

    Deleter() : x(NULL) {
    }

    Deleter(const char* msg) : x(NULL) {
        x = new char[strlen(msg) + 1];

        strcpy((char*) x, msg);
    }

    virtual ~Deleter() {
        if (x != NULL) {
            delete[] x;
        }
    }

};

class Additor : public Addible<Additor>, public Subtractable<Additor>, Multipliable<Additor>, Divisible<Additor> {
public:
    int x;

    Additor() : Addible(), Subtractable(), Multipliable(), Divisible() {
    }

    Additor(const Additor& other) : Addible(), Subtractable(), Multipliable(), Divisible() {
        x = other.x;
    }

    Additor(const int& other) {
        x = other;
    }

    virtual Additor& operator+=(const Additor& other) {
        x += other.x;

        return *this;
    }

    virtual Additor& operator-=(const Additor& other) {
        x -= other.x;
        return *this;
    }

    virtual Additor& operator*=(const Additor& other) {
        x *= other.x;
        return *this;
    }

    virtual Additor& operator/=(const Additor& other) {
        x /= other.x;
        return *this;
    }

};

class PrePostInc : public PostIncrementable<PrePostInc>, public PreIncrementable<PrePostInc> {
public:
    int x;

    PrePostInc() : PostIncrementable(), PreIncrementable() {
    }

    PrePostInc(const PrePostInc& other) : PrePostInc() {
        x = other.x;
    }

    PrePostInc(const int& other) {
        x = other;
    }

    virtual PrePostInc operator++(int) override {
        x++;
        return *this;
    }

    virtual PrePostInc& operator++() override {
        ++x;
        return *this;
    }

};

class Inv : public Inversible<Inv&> {
public:
    double x;

    virtual Inv& inv() {

        if (x == 0.0) {
            x = 10E300;

            MessageWritableTo<QTextStream, QString> errMsg;

            errMsg.setMsgData("Inv::void inv() : division by 0!");

            throw ErrMsgException<MessageWritableTo<QTextStream, QString> >(errMsg);
        };

        x = 1.0 / x;

        return *this;
    }

};

class SomeClonable1 : public ClonableTo<SomeClonable1>, public Clonable {
public:

    QString name;

    const QString type;

    SomeClonable1() : ClonableTo<SomeClonable1>(), Clonable(), type(QString("SomeClonable1")) {
    }

    SomeClonable1(const SomeClonable1&) : ClonableTo<SomeClonable1>(), Clonable(), type(QString("SomeClonable1")) {
    }

    virtual ~SomeClonable1() {
        QTextStream out(stdout);
        out << name + "::~SomeClonable1()" << endl;
    }

    SomeClonable1* clone() {

        QTextStream out(stdout);
        out << name + "::SomeClonable1::clone()" << endl;

        return new SomeClonable1(*this);

    }

};

class SomeClonable2 : public ClonableTo<SomeClonable2> {
public:

    QString name;
    int x;
    QString something1;
    const QString type;

    SomeClonable2() : ClonableTo(), type(QString("SomeClonable2")) {
    }

    SomeClonable2(const SomeClonable2&) : ClonableTo(), type(QString("SomeClonable2")) {
    }

    virtual ~SomeClonable2() {
        QTextStream out(stdout);
        out << name + "::~SomeClonable2()" << endl;
    }

    SomeClonable2* clone() {

        QTextStream out(stdout);
        out << "SomeClonable2::clone()" << endl;

        return new SomeClonable2(*this);

    }

};

class Subscriptor : public Subscriptable<QVector<int>, int> {
public:

    QVector<QVector<int>> data;

    Subscriptor() : Subscriptable<QVector<int>, int>() {
    }

    Subscriptor(const Subscriptor&) : Subscriptable<QVector<int>, int>() {
    }

    virtual ~Subscriptor() {
    }

    virtual QVector<int>& operator[](const int& idx) {
        return data[idx];
    }

    virtual const QVector<int>& operator[](const int& idx) const {
        return data[idx];
    }


};

class Evt1 {
};

class Evt2 {
};

class Evt3 : public Evt1 {
};

class EvtReceiver : public Object<QObject>, ReceiverOf<Evt1>, ReceiverOf<Evt2>, ReceiverOf<Evt3> {

    Q_OBJECT

public:

    EvtReceiver() {
    }

    EvtReceiver(const EvtReceiver&other) : BasicObject(other), Object(other), ReceiverOf<Evt1>(), ReceiverOf<Evt2>(), ReceiverOf<Evt3>() {
    }

    virtual ~EvtReceiver() {
    }

public slots:

    virtual void receive(Evt1*) {

        QTextStream out(stdout);

        out << "EvtReceiver::receive : Evt1" << endl;

    }

    virtual void receive(Evt2*) {

        QTextStream out(stdout);

        out << "EvtReceiver::receive : Evt2" << endl;

    }

    virtual void receive(Evt3*) {

        QTextStream out(stdout);

        out << "EvtReceiver::receive : Evt3" << endl;

    }

};


// The Qt Sender engine which uses signals and slots

#define QT_SENDER_API_FOR(type)\
virtual void addReceiver(const ReceiverOf<type>& receiver) {\
	QObject::connect(this, SIGNAL(sigEvt(type*)), (EvtReceiver*) & receiver, SLOT(receive(type*)));\
}\
virtual void removeReceiver(const ReceiverOf<type>& receiver) {\
	QObject::disconnect(this, SIGNAL(sigEvt(type*)), (EvtReceiver*) & receiver, SLOT(receive(type*)));\
}\
virtual void removeAllReceiversOf(const type&) {\
	QObject::disconnect(SIGNAL(sigEvt(type*)));\
}\
virtual void send(type* evt) {\
	emit sigEvt(evt);\
}\
;

// A sender engine for using signals and slots must know in advance all events which can be sent/received

class QtSenderEngine : public QObject {

    Q_OBJECT
public:

    QtSenderEngine() {
    }

    QtSenderEngine(const QtSenderEngine& other) : QObject((QObject*) & other) {
    }

    QT_SENDER_API_FOR(Evt1);
    QT_SENDER_API_FOR(Evt2);
    //SENDER_API_FOR(Evt3);

signals:
    void sigEvt(Evt1*);
    void sigEvt(Evt2*);
    //void sigEvt(Evt3*);

};

// A template class which hides the Qt engine, since Q_OBJECT does not support templates (anly a beautiful wrapper)

template<class T> class QtSenderOf : public QtSenderEngine, public SenderOf<T> {
public:

    virtual void addReceiver(const ReceiverOf<T>& receiver) {
        QtSenderEngine::addReceiver(receiver);
    }

    virtual void removeReceiver(const ReceiverOf<T>& receiver) {
        QtSenderEngine::removeReceiver(receiver);
    }

    virtual void removeAllReceiversOf(const T& t) {
        QtSenderEngine::removeAllReceiversOf(t);
    }

    virtual void send(T* evt) {
        QtSenderEngine::send(evt);
    }

};

template<class T> class OtherSenderOf : public DefaultSenderOf<T> {
public:

    virtual void send(T* evt) {
        qDebug("Some other sender ...");
        DefaultSenderOf<T>::send(evt);
    }

};

// The Qt Sender engine which uses signals and slots

class EvtSender : public Object<QObject>, public QtSenderOf<Evt1>, public QtSenderOf<Evt2>, public OtherSenderOf<Evt3> {
public:

    EvtSender() {
    }

    EvtSender(const EvtSender & other) : BasicObject(other), Object(other) {
    }

    virtual ~EvtSender() {
    }

    // Unhide the inherited methods

    SENDER_API_FOR(QtSenderOf<Evt1>);
    SENDER_API_FOR(QtSenderOf<Evt2>);
    SENDER_API_FOR(OtherSenderOf<Evt3>);

};

/********************  Alternative send/receive implementation  ***************/
template<class T> class TestReceiverOf {
public:

    string name;

};

template <class evtT, class senderT, class receiverT> class TestSenderOf;

class BasicTestSender {
public:

    template <class senderT, class receiverT, class signalT, class slotT>
    static void connectTest(senderT*, signalT, receiverT* receiver, slotT) {
        //((TestSenderOf<evtT, senderT, receiverT>*)sender)->connect(senderMethod, receiver, receiverMethod);
    }

    template <class evtT, class senderT, class receiverT>
    static void connect(senderT* sender, void (senderT::*senderMethod)(evtT), receiverT* receiver, void (receiverT::*receiverMethod)(evtT)) {

        ((TestSenderOf<evtT, senderT, receiverT>*)sender)->connect(senderMethod, receiver, receiverMethod);

    }

    template <class evtT, class senderT, class receiverT>
    static void disconnect(senderT* sender, void (senderT::*senderMethod)(evtT), receiverT* receiver, void (receiverT::*receiverMethod)(evtT)) {

        ((TestSenderOf<evtT, senderT, receiverT>*)sender)->disconnect(senderMethod, receiver, receiverMethod);

    }

    template <class evtT, class senderT, class receiverT>
    static void disconnect(senderT* sender, void (senderT::*senderMethod)(evtT)) {

        ((TestSenderOf<evtT, senderT, receiverT>*)sender)->disconnect(senderMethod);

    }

};

template <class evtT, class senderT, class receiverT> class TestSenderOf : public BasicTestSender {
public:

    //typedef void (TestSenderOf<evtT, senderT, receiverT>::*senderMethod)(evtT);
    typedef void (senderT::*senderMethod)(evtT);
    typedef void (receiverT::*receiverMethod)(evtT);

    vector<pair<senderMethod, pair<receiverT*, receiverMethod> > > callPairs;

    unordered_map<void**, vector<pair<receiverT*, receiverMethod> > > callConnection;

    inline void fire(senderMethod& sm, evtT args) {

        /*
        void** smPtr = reinterpret_cast<void**> (&sm);

        cout << "Firing : " << smPtr << endl;

        for (uint i = 0; i < callConnection[smPtr].size(); i++) {
                receiverT* callee = (receiverT*) callConnection[smPtr][i].first;
                receiverMethod rm = callConnection[smPtr][i].second;

                (callee->*rm)(args); // Call the receiver's method	
        }

        return;
         */

        for (uint i = 0; i < callPairs.size(); i++) {
            pair<senderMethod, pair < receiverT*, receiverMethod>>&callPair = (pair<senderMethod, pair < receiverT*, receiverMethod>>&) callPairs[i];

            if (callPair.first == sm) {
                receiverT* callee = (receiverT*) callPair.second.first;
                receiverMethod rm = callPair.second.second;

                (callee->*rm)(args); // Call the receiver's method	
            }
        }

    }

    virtual void connect(senderMethod& sm, receiverT* tr, receiverMethod& rm) {
        //callConnection[reinterpret_cast<void*> (sm)].push_back(pair < receiverT*, void (receiverT::*)(evtT)>(tr, rm));

        pair < receiverT*, void (receiverT::*)(evtT) > receiverPair(tr, rm);
        pair<void (senderT::*)(evtT), pair < receiverT*, void (receiverT::*)(evtT) >> callPair(sm, receiverPair);

        callPairs.push_back(callPair);


        // Test
        void** ptr = reinterpret_cast<void**> (&sm);
        cout << "Connecting : " << ptr << endl;
        callConnection[ptr].push_back(receiverPair);

    }

    virtual void disconnect(senderMethod& sm, receiverT* tr, receiverMethod& rm) {

        set<uint> remIdx;

        for (uint i = 0; i < callPairs.size(); i++) {
            pair<senderMethod, pair < receiverT*, receiverMethod>>&callPair = (pair<senderMethod, pair < receiverT*, receiverMethod>>&) callPairs[i];

            if (callPair.first == sm && callPair.second.first == tr && callPair.second.second == rm) {
                remIdx.insert(i);
            }

        }

        vector<pair<senderMethod, pair<receiverT*, receiverMethod> > > newCallPairs;

        for (uint i = 0; i < callPairs.size(); i++) {
            if (remIdx.count(i) == 0) {
                newCallPairs.push_back(callPairs[i]);
            }
        }

        callPairs = newCallPairs;

    }

    virtual void disconnect(senderMethod& sm) {

        set<uint> remIdx;

        for (uint i = 0; i < callPairs.size(); i++) {

            pair<senderMethod, pair < receiverT*, receiverMethod>>&callPair = (pair<senderMethod, pair < receiverT*, receiverMethod>>&) callPairs[i];

            if (callPair.first == sm) {
                remIdx.insert(i);
            }

        }

        vector<pair<senderMethod, pair<receiverT*, receiverMethod> > > newCallPairs;

        for (uint i = 0; i < callPairs.size(); i++) {
            if (remIdx.count(i) == 0) {
                newCallPairs.push_back(callPairs[i]);
            }
        }

        callPairs = newCallPairs;

    }

};

class DfTestReceiver : public TestReceiverOf<int>, public TestReceiverOf<const string&> {
public:

    void receive1(int) {
        cout << TestReceiverOf<int>::name << " : " << "TestReceiver::receive1..." << endl;
    }

    void receive2(int) {
        cout << TestReceiverOf<int>::name << " : " << "TestReceiver::receive2..." << endl;
    }

    void receiveString(const string& arg) {
        cout << TestReceiverOf<const string&>::name << " : " << "TestReceiver::receiveString... : " << arg << endl;
    }

};

class DfTestSender : public TestSenderOf<int, DfTestSender, DfTestReceiver>, public TestSenderOf<const string&, DfTestSender, DfTestReceiver> {
public:

    void send1(int args) {
        TestSenderOf<int, DfTestSender, DfTestReceiver>::senderMethod sm = &DfTestSender::send1;
        TestSenderOf<int, DfTestSender, DfTestReceiver>::fire(sm, args);
    }

    void send2(int args) {
        TestSenderOf<int, DfTestSender, DfTestReceiver>::senderMethod sm = &DfTestSender::send2;
        TestSenderOf<int, DfTestSender, DfTestReceiver>::fire(sm, args);
    }

    void sendString(const string& arg) {
        TestSenderOf<const string&, DfTestSender, DfTestReceiver>::senderMethod sm = &DfTestSender::sendString;
        TestSenderOf<const string&, DfTestSender, DfTestReceiver>::fire(sm, arg);
    }

};

class QtTestReceiver : public QObject, public TestReceiverOf<int> {

    Q_OBJECT

public slots:

    void receive1(int) {
        cout << name << " : " << "QtTestReceiver::receive1..." << endl;
    }

    void receive2(int) {
        cout << name << " : " << "QtTestReceiver::receive2..." << endl;
    }

};

class QtTestSender : public QObject, public TestSenderOf<int, QtTestSender, QtTestReceiver> {

    Q_OBJECT
public:

    virtual void connect(void (QtTestSender::*&sm)(int), QtTestReceiver* tr, void (QtTestReceiver::*&rm)(int)) override {
        //cout << "Connecting ... " << endl;
        QObject::connect(this, sm, tr, rm);
        //getchar();
    }

    virtual void disconnect(void (QtTestSender::*&sm)(int), QtTestReceiver* tr, void (QtTestReceiver::*&rm)(int)) override {
        //cout << "Disconnecting ... " << endl;
        QObject::disconnect(this, sm, tr, rm);
        //getchar();
    }

    virtual void disconnect(void (QtTestSender::*&sm)(int)) override {
        //cout << "Disconnecting ... " << endl;
        QObject::disconnect(this, sm, 0, 0);
        //getchar();
    }

signals:

    void send1(int);

    void send2(int args);

};

/******************************************************************************/

template<class T> void foo(T r) {
    ++r;
}

/******************************************************************************/

void functionSlot1(const int&) {
    qDebug("functionSlot1");
}

double functionSlot2(const int&) {
    qDebug("functionSlot2");

    return 0.0;
}

void functionSlotOther(const int&) {
    qDebug("functionSlotOther");
}

void functionSlotQt1() {
    qDebug("functionSlotQt1");
}

double functionSlotQt2() {
    qDebug("functionSlotQt2");

    return 0.0;
}

void functionSlotQtOther() {
    qDebug("functionSlotQtOther");
}

class FunctorSlot : public Functor<void, int> {
public:

    int num;

    FunctorSlot() {
    }

    FunctorSlot(const FunctorSlot&) : Functor() {
    }

    void operator()(const int&) override {
        std::cout << QString("FunctorSlot: " + QString::number(num)).toStdString().data() << std::endl;
    }

};

class FunctorSlotD : public Functor<double, int> {
public:

    FunctorSlotD() {
    }

    FunctorSlotD(const FunctorSlotD&) : Functor() {
    }

    double operator()(const int&) override {
        qDebug("FunctorSlotD");

        return 0.0;
    }

};

class FunctorSlotQt1 : public Functor<void> {
public:

    FunctorSlotQt1() {
    }

    FunctorSlotQt1(const FunctorSlotQt1&) : Functor() {
    }

    void operator()() override {
        qDebug("FunctorSlotQt1");
    }

};

class FunctorSlotQt2 : public Functor<int> {
public:

    FunctorSlotQt2() {
    }

    FunctorSlotQt2(const FunctorSlotQt2&) : Functor() {
    }

    int operator()() override {
        qDebug("FunctorSlotQt2");

        return 10;
    }

};

class ClassSlot : public QObject, public BasicObject {

    Q_OBJECT
public:

    ClassSlot() {
    }

    void someSlot(const int&) {
        qDebug("ClassSlot::someSlot");
    }

    void someSlot1(const int&) {
        qDebug("ClassSlot::someSlot1");
    }

    void someSlotStr(const string&) {
        qDebug("ClassSlot::someSlotStr");
    }

public slots:

    void slotQt() {
        qDebug("ClassSlot::slotQt ");
    }

};

template<class objT, class...argT> class QtSignal : public Functor<void, argT...> {
private:

    //typedef void(*FunctionPtr)(const argT&...);
    //typedef Functor<void, argT...>* FunctorPtr;
    typedef void(objT::*QtMethodPtr)(const argT&...);

    objT* sender;
    QtMethodPtr signal;

    QtSignal() : Functor<void, argT...>() {
    }

public:

    QtSignal(objT* sender, QtMethodPtr signal) {
        this->sender = sender;
        this->signal = signal;
    }

    virtual ~QtSignal() {
    }

    /** Connect Qt member slots. */
    template<class receiverT, typename methodT> void connect(receiverT* o, methodT qtM, bool establish = true) {
        if (establish) {
            QObject::connect(this->sender, this->signal, o, qtM);
        } else {
            // Do nothing so far: QObject::disconnect acts weird here
        }
    }

    template<class receiverT, typename methodT> void disconnect(receiverT* o, methodT qtM) {
        QObject::disconnect(this->sender, this->signal, o, qtM);
    }

    /** Connect function slot. */
    template<class objectT> void connect(objectT& obj) {

        QObject::connect(this->sender, this->signal, obj);

    }

    /** Disconnect function slot. */
    template<class objectT> void disconnect(objectT& obj) {

        QObject::disconnect(this->sender, this->signal, obj);

    }

    /** Connect other signal. */
    void connect(QtSignal& other) {

        QObject::connect(this->sender, this->signal, other.sender, other.signal);

    }

    void disconnect(QtSignal& other) {

        QObject::disconnect(this->sender, this->signal, other.sender, other.signal);

    }

    virtual void operator()(const argT&... args) {
        (sender->*signal)(args...);
    }

};

class ClassWithSig : public QObject, public BasicObject {
    Q_OBJECT

public:

    Signal<void, int> sig;
    Signal<void, int> sigOther;
    Signal<double, int> sigD;
    QtSignal<ClassWithSig> qtSig;
    QtSignal<ClassWithSig> qtSigOther;

    ClassWithSig() : qtSig(this, &ClassWithSig::sigQt), qtSigOther(this, &ClassWithSig::sigQtOther) {
    }

signals:
    void sigQt();
    void sigQtOther();

};

class ThreadRunnable : public Object<QObject>, RunnableIn<QThread> {
    Q_OBJECT
public:

    QtSignal<ThreadRunnable> finishedQt; // Qt finish signal

    ThreadRunnable() : finishedQt(this, &ThreadRunnable::sigFinished) {

        connect(&env, SIGNAL(started()), this, SLOT(runActions()));

        this->moveToThread(&env);

    }

    virtual ~ThreadRunnable() {

        env.quit();
        env.wait();

    }

signals:

    void sigFinished();

public slots:

    void runActions() {

        QTextStream out(stdout);

        out << "Running object : " << objectName() << endl;

        double x = 0.00000000000000000000000000000000000000000000001;
        for (unsigned long int i = 0; i < 1000000; i++) {
            x += sqrt(x) + pow(x, 0.000001);
        }

        out << x << endl;

        out << "Finished running : " << objectName() << endl;


        //emit ThreadRunnable::sigFinished();
        finishedQt();

    }

    void run() {

        env.start();

    }

};


/******************************************************************************/

thread_local QVector<unsigned int> locGenVals;

class ThreadRunnableLocStor : public Object<QObject>, RunnableIn<QThread> {
    Q_OBJECT
public:

    unsigned int ID;

    ThreadRunnableLocStor() {

        connect(&env, SIGNAL(started()), this, SLOT(runActions()));

        this->moveToThread(&env);

    }

    virtual ~ThreadRunnableLocStor() {

        env.quit();
        env.wait();

    }

signals:

    void sigFinished();

public slots:

    void runActions() {

        QTextStream out(stdout);

        Math::uint64 numExp = 10; //0000000;

        int curVal;

        Rand::rndSeed(ID);

        out << objectName() << "(" << QThread::currentThread() << ")" << ": rndSeed = " << Rand::rndSeed() << endl;
        //getchar();

        for (Math::uint64 i = 0; i < numExp; i++) {

            //out << objectName() << ": " << Rand::rndInt() << endl;
            out << objectName() << ": " << Rand::rnd<Math::uint32>() << endl;
            //curVal = Rand::rndInt();
            curVal = Rand::rnd<Math::uint32>();

            locGenVals << curVal;
        }

        QMutex mutex;
        mutex.lock();
        out << "GenVals: ";
        for (int i = 0; i < locGenVals.size(); i++) {
            out << locGenVals[i] << " ";
        }
        out << endl;
        out << "Generated rand nums: " << locGenVals.size() << endl;
        mutex.unlock();

        emit ThreadRunnableLocStor::sigFinished();

    }

    void run() {

        env.start();

    }

};

/******************************************************************************/

/******************************************************************************/

class SomeIterativeSolver : public IterativeSolver<int, Math::intUNI, const Math::intUNI&>, public Stopable<SomeIterativeSolver> {
public:

    Math::intUNI x;

    SomeIterativeSolver() {
        x = 0;
    }

    SomeIterativeSolver(const SomeIterativeSolver&) : IterativeSolver(), Stopable() {
        x = 0;
    }

    virtual ~SomeIterativeSolver() {
    }

    int solve(const Math::intUNI& otherX) override {

        x = otherX;

        for (int i = 0; i < 10; i++) {

            stop(i > 5);

            ++(*this);
        }

        return (int) x;
    }

    Math::intUNI& operator++() override {
        x++;
        x++;

        return (Math::intUNI&) x;
    }

    void stopActions() override {
    }

    Math::intUNI getLastResult() {
        return x;
    }

private:

protected:

};

/******************************************************************************/


#include "main.moc"

/******************************************************************************/

class SomeParser : public Parser<> {
public:

    void parse() override {
        QTextStream out(stdout);

        out << "Parser::parse" << endl;
    }

};

void testParser() {

    SomeParser parser;

    parser.parse();

}

/******************************************************************************/

/******************************************************************************/

class SomeSrtSetting : public Variable<QString> {
private:

    bool isChanged;

    QString val;

protected:

public:

    SomeSrtSetting() : isChanged(false) {
    }

    SomeSrtSetting(const SomeSrtSetting& other) : Variable() {
        *this = other;
    }

    virtual ~SomeSrtSetting() {
    }

    virtual SomeSrtSetting& set(const QString& val) override {

        if (this->val == val) { // Leave as is

        } else {
            this->val = val;
            setChanged(true);
        }

        return *this;
    }

    virtual QString& get() override {
        return val;
    }

    virtual const QString& get() const override {
        return val;
    }

    using Variable::operator=;

    void setChanged(const bool& ch = true) override {
        isChanged = ch;
    }

    bool changed() const override {
        return isChanged;
    }

};

class SomeIntSetting : public Variable<int> {
private:

    bool isChanged;

    int val;

protected:

public:

    SomeIntSetting() : isChanged(false) {
    }

    SomeIntSetting(const SomeIntSetting& other) : Variable() {
        *this = other;
    }

    virtual ~SomeIntSetting() {
    }

    virtual SomeIntSetting& set(const int& val) override {

        if (this->val == val) { // Leave as is

        } else {
            this->val = val;
            setChanged(true);
        }

        return *this;
    }

    virtual int& get() override {
        return val;
    }

    virtual const int& get() const override {
        return val;
    }

    using Variable::operator=;

    void setChanged(const bool& ch = true) override {
        isChanged = ch;
    }

    bool changed() const override {
        return isChanged;
    }

};

class SomeSettings : public Variables<QMap, QString, SomeSrtSetting> {
public:

    SomeSettings() {
    }

    SomeSettings(const SomeSettings&) : Variables() {
    }

    using Variables::operator[];

    const SomeSrtSetting& operator[](const QString& key) const override {
        return container().constFind(key).value();
    }

    bool changed() const override {
        for (const QString& curKey : container().keys()) {
            if ((*this)[curKey].changed()) {
                return true;
            }
        }

        return false;
    }

    void accept() override {
        for (const QString& curKey : container().keys()) {
            (*this)[curKey].accept();
        }
    }

    void clear() override {
        vars.clear();
    }


};

template <class T> class SomeSetting : public Variable<T> {
private:

    bool isChanged;

    T val;

protected:

public:

    SomeSetting() : isChanged(false) {
    }

    SomeSetting(const T& otherVal) : Variable<T>() {
        val = otherVal;
    }

    SomeSetting(const SomeSetting& other) : Variable<T>() {
        *this = other;
    }

    virtual ~SomeSetting() {
    }

    virtual SomeSetting& set(const T& val) override {

        if (this->val == val) { // Leave as is

        } else {
            this->val = val;
            setChanged(true);
        }

        return *this;
    }

    virtual T& get() override {
        return val;
    }

    virtual const T& get() const override {
        return val;
    }

    using Variable<T>::operator=;

    void setChanged(const bool& ch = true) override {
        isChanged = ch;
    }

    bool changed() const override {
        return isChanged;
    }

};

class SomeMultiVars : public MultiVariables<QMap, QString, SomeSetting, int, QString> {
public:

    SomeMultiVars() {
    }

    SomeMultiVars(const SomeMultiVars&) : MultiVariables<QMap, QString, SomeSetting, int, QString>() {
    }

    virtual ~SomeMultiVars() {
    }

    using MultiVariables<QMap, QString, SomeSetting, int, QString>::set;
    using MultiVariables<QMap, QString, SomeSetting, int, QString>::get;

};

void testSettings() {
    QTextStream out(stdout);

    SomeSrtSetting set;
    SomeSrtSetting set1;
    SomeSrtSetting set2;
    SomeSettings sets;
    SomeSettings sets1;

    out << (set.changed() ? "Setting changed" : "Setting did not change") << " : " << set.get() << endl;

    set = QString("Hello");

    out << (set.changed() ? "Setting changed" : "Setting did not change") << " : " << set.get() << endl;

    set.accept();

    out << (set.changed() ? "Setting changed" : "Setting did not change") << " : " << set.get() << endl;

    set = QString("Hello");

    out << (set.changed() ? "Setting changed" : "Setting did not change") << " : " << set.get() << endl;

    set = QString("Hello123");

    out << (set.changed() ? "Setting changed" : "Setting did not change") << " : " << set.get() << endl;

    set1 = QString("Setting 2");

    sets["set"] = set;
    sets["set1"] = set1;

    for (const QString& key : sets.container().keys()) {
        out << key << " : " << sets[key].get() << " ->  " << (sets[key].changed() ? "Setting changed" : "Setting did not change") << endl;
    }

    sets["set"].accept();
    sets["set1"].accept();

    for (const QString& key : sets.container().keys()) {
        out << key << " : " << sets[key].get() << " ->  " << (sets[key].changed() ? "Setting changed" : "Setting did not change") << endl;
    }

    sets["set"] = sets["set"];
    sets["set1"] = QString("456");

    for (const QString& key : sets.container().keys()) {
        out << key << " : " << sets[key].get() << " ->  " << (sets[key].changed() ? "Setting changed" : "Setting did not change") << endl;
    }

    out << (sets1.changed() ? "Setting[S] changed" : "Setting[S] did not change") << endl;
    sets1 = sets;
    out << (sets1.changed() ? "Setting[S] changed" : "Setting[S] did not change") << endl;
    sets1.accept();
    out << (sets1.changed() ? "Setting[S] changed" : "Setting[S] did not change") << endl;

    set2 = QString("Setting 3");
    sets1["set2"] = set2;

    sets1["set3"] = "Setting 4";

    out << (sets1.changed() ? "Setting[S] changed" : "Setting[S] did not change") << endl;
    for (const QString& key : sets1.container().keys()) {
        out << key << " : " << sets1[key].get() << " ->  " << (sets1[key].changed() ? "Setting changed" : "Setting did not change") << endl;
    }

    sets1.accept();
    out << (sets1.changed() ? "Setting[S] changed" : "Setting[S] did not change") << endl;
    for (const QString& key : sets1.container().keys()) {
        out << key << " : " << sets1[key].get() << " ->  " << (sets1[key].changed() ? "Setting changed" : "Setting did not change") << endl;
    }

    sets1["set3"] = "Setting 4";

    out << (sets1.changed() ? "Setting[S] changed" : "Setting[S] did not change") << endl;
    for (const QString& key : sets1.container().keys()) {
        out << key << " : " << sets1[key].get() << " ->  " << (sets1[key].changed() ? "Setting changed" : "Setting did not change") << endl;
    }


    out << "Testing tuples..." << endl;

    SomeMultiVars mvars;

    mvars.set("test", int(10));
    mvars.set("test", QString("str 10"));
    mvars.set("test2", QString("str 20"));

    out << mvars.get<int>("test").get() << " changed: " << (mvars.get<int>("test").changed() ? "yes" : "no") << endl;
    out << mvars.get<QString>("test").get() << " changed: " << (mvars.get<QString>("test").changed() ? "yes" : "no") << endl;
    out << mvars.get<QString>("test2").get() << " changed: " << (mvars.get<QString>("test2").changed() ? "yes" : "no") << endl;

    mvars.get<int>("test").setChanged(false);
    mvars.get<QString>("test").setChanged(false);
    mvars.get<QString>("test2").setChanged(false);
    
    out << mvars.get<int>("test").get() << " changed: " << (mvars.get<int>("test").changed() ? "yes" : "no") << endl;
    out << mvars.get<QString>("test").get() << " changed: " << (mvars.get<QString>("test").changed() ? "yes" : "no") << endl;
    out << mvars.get<QString>("test2").get() << " changed: " << (mvars.get<QString>("test2").changed() ? "yes" : "no") << endl;
    
    mvars.set("test", int(10));
    mvars.set("test", QString("str 10"));
    mvars.set("test2", QString("str 30"));
    
    out << mvars.get<int>("test").get() << " changed: " << (mvars.get<int>("test").changed() ? "yes" : "no") << endl;
    out << mvars.get<QString>("test").get() << " changed: " << (mvars.get<QString>("test").changed() ? "yes" : "no") << endl;
    out << mvars.get<QString>("test2").get() << " changed: " << (mvars.get<QString>("test2").changed() ? "yes" : "no") << endl;
    
    
    mvars.clear();

    out << mvars.get<int>("test").get() << endl;
    out << mvars.get<QString>("test").get() << endl;
    out << mvars.get<QString>("test2").get() << endl;
}

/******************************************************************************/

/******************************************************************************/

class SomeRG : public Common::Interfaces::RandGen<Math::uintUNI> {
public:

    SomeRG() {
    }

    SomeRG(const SomeRG&) : RandGen() {
    }

    virtual ~SomeRG() {
    }

    Math::uintUNI rnd() override {
        return 0;
    }

    // Expose other used members
    using Common::Interfaces::RandGen<Math::uintUNI>::rnd;

    // Clone

    virtual SomeRG* clone() override {
        return new SomeRG(*this);
    }

};

class SomeRGD : public Common::Interfaces::RandGen<double> {
public:

    SomeRGD() {
    }

    SomeRGD(const SomeRGD&) : RandGen() {
    }

    virtual ~SomeRGD() {
    }

    double rnd() override {
        return 0.0;
    }

    // Expose other used members
    using Common::Interfaces::RandGen<double>::rnd;

    // Clone

    virtual SomeRGD* clone() override {
        return new SomeRGD(*this);
    }

};

/*
class SomeGGRG : public Common::AltRand::GeneralRandGen<Common::Interfaces::RandGen<Math::uintUNI>,Common::Interfaces::RandGen<double>>
{
        public:

        SomeGGRG() { }

        SomeGGRG(const SomeRGD&) : GeneralRandGen() { }

        virtual ~SomeGGRG() { }

        Math::uintUNI rnd(const Math::uintUNI& = 0) override {
                return 0;
        }
	
        double rnd(const double& = 0) override {
                return 0.0;
        }
	
        // Expose other used members
        //using Common::Interfaces::RandGen<Math::uintUNI>::rnd;

};
 */

void testAltRand() {

    QTextStream out(stdout);

    SomeRG sRG;
    SomeRGD sRGD;
    Common::Rand::MT19937<Math::uint32> rgmt(10);
    Common::Rand::MT19937<double> rgmtD(10);

    typedef MT19937<Math::uint32> rgInt;
    typedef MT19937<double> rgFloat;
    CombinedRandGen<rgInt, rgFloat> grg;

    sRG.rnd();
    sRG.rnd(1, 1);

    sRGD.rnd();
    sRGD.rnd(1, 1);

    rgmt.rnd();

    rgmtD.rnd();

    for (Math::uint32 i = 0; i < 100; ++i) {
        //Math::uint32 x = grg.rnd<MT19937<Math::uint32>>();
        out << "Integer: " << grg.rnd<rgInt>() << " - " << "Float: " << grg.rnd<rgFloat>() << endl;
        //out << "Integer: " << grg()<rgInt> << " - " << "Float: " << grg()<rgFloat> << endl;
    }

    getchar();

}

/******************************************************************************/
void testSolvers() {
    QTextStream out(stdout);

    Math::intUNI x = 1;

    SomeIterativeSolver sis;

    try {

        out << "SomeIterativeSolver solution: " << sis.solve(x) << endl;

    } catch (StopException<SomeIterativeSolver>& e) {

        out << "Algorithm was stopped with result: " << sis.getLastResult() << endl;

    }

    getchar();
}

void testSigSlot() {

    QTextStream out(stdout);

    FunctorSlot fs;
    FunctorSlotD fsD;

    ClassSlot cs;

    fs.num = 1;

    {
        Signal<void, int> sig;
        Signal<double, int> sigD;


        sig.connect(functionSlot1);
        sig.connect(functionSlot2);
        sigD.connect(functionSlot2);
        sig.connect(fs);
        sig.connect(fsD);
        sig.connect(&cs, &ClassSlot::someSlot);
        sig.connect(&cs, &ClassSlot::someSlot1);

        sig.disconnect(&cs, &ClassSlot::someSlot);

        sig.connect(&cs, &ClassSlot::someSlot);
        sig.connect(&cs, &ClassSlot::someSlot);

        fs.num = 2;

        sig(10);
        //sigD(10);
    }

    //functionSlot1(10);
    //fs(10);
    //fsD(10);

    getchar();

    FunctorSlotQt1 fsQt1;
    FunctorSlotQt2 fsQt2;

    ClassWithSig cls;

    cls.sig.connect(&cs, &ClassSlot::someSlot1);
    cls.sigOther.connect(functionSlotOther);

    cls.sig.connect(cls.sigOther);
    cls.sig(10);
    cls.sigOther(20);

    getchar();

    cls.qtSig.connect(&cs, &ClassSlot::slotQt);
    cls.qtSig.disconnect(&cs, &ClassSlot::slotQt);

    cls.qtSig.connect(functionSlotQt1);
    cls.qtSig.connect(functionSlotQt2);
    cls.qtSig.connect(fsQt1);
    cls.qtSig.connect(fsQt2);

    //	cls.qtSigOther.connect(&functionSlotQtOther);
    cls.qtSig.connect(cls.qtSigOther);

    cls.qtSig();
    cls.qtSigOther();

    getchar();

}

void testTestSenderReceiver() {

    QTextStream out(stdout);

    // Standard
    DfTestReceiver tr1;
    DfTestReceiver tr2;
    DfTestSender ts;

    tr1.TestReceiverOf<int>::name = "tr1";
    tr2.TestReceiverOf<int>::name = "tr2";

    //	BasicTestSender::connectTest(&ts, &DfTestSender::send1, &tr1, &DfTestReceiver::receive1);
    BasicTestSender::connect(&ts, &DfTestSender::send1, &tr1, &DfTestReceiver::receive1);
    BasicTestSender::connect(&ts, &DfTestSender::send1, &tr1, &DfTestReceiver::receive2);
    BasicTestSender::connect(&ts, &DfTestSender::send1, &tr2, &DfTestReceiver::receive2);
    BasicTestSender::connect(&ts, &DfTestSender::send2, &tr2, &DfTestReceiver::receive1);
    BasicTestSender::connect(&ts, &DfTestSender::sendString, &tr1, &DfTestReceiver::receiveString);

    BasicTestSender::disconnect(&ts, &DfTestSender::send1, &tr1, &DfTestReceiver::receive1);
    //BasicTestSender::disconnect<int, DfTestSender, DfTestReceiver>(&ts, &DfTestSender::send1);

    ts.send1(1);
    ts.send2(2);
    ts.sendString("Hello");

    // Qt

    QtTestReceiver qtr1;
    QtTestReceiver qtr2;
    QtTestSender qts;

    qtr1.TestReceiverOf<int>::name = "qtr1";
    qtr2.TestReceiverOf<int>::name = "qtr2";

    BasicTestSender::connect(&qts, &QtTestSender::send1, &qtr1, &QtTestReceiver::receive1);
    BasicTestSender::connect(&qts, &QtTestSender::send1, &qtr1, &QtTestReceiver::receive2);
    BasicTestSender::connect(&qts, &QtTestSender::send1, &qtr2, &QtTestReceiver::receive2);
    BasicTestSender::connect(&qts, &QtTestSender::send2, &qtr2, &QtTestReceiver::receive1);
    //BasicTestSender::connect(&qts, &QtTestSender::sendString, &qtr1, &QtTestReceiver::receiveString);

    //BasicTestSender::disconnect(&qts, &QtTestSender::send1, &qtr1, &QtTestReceiver::receive1);
    //BasicTestSender::disconnect<int, DfTestSender, DfTestReceiver>(&qts, &QtTestSender::send1);

    qts.send1(1);
    qts.send2(2);

    cout << "Done." << endl;
}

void clonnerAssignerTest() {
    QTextStream out(stdout);

    Cloner x1;
    x1.x = 11;

    out << "Created Cloner x1.x= " << x1.x << endl;

    out << "Clonning ... " << endl;
    Cloner* x2 = x1.clone();

    out << "Created Cloner x2.x= " << x2->x << endl;

    delete x2;

    Assigner as1;
    as1.x = 50;

    out << "Created Assigner as1.x= " << as1.x << endl;

    out << "Copying..." << endl;

    Assigner as2;
    as2 = as1;

    out << "Created Assigner as2.x= " << as2.x << endl;

    AssignerClonner asc1;
    asc1.x = 100;

    out << "Created AssignerClonner asc1.x= " << asc1.x << endl;

    out << "Copying..." << endl;

    AssignerClonner asc2;
    asc2 = *asc1.clone();

    out << "Created AssignerClonner asc2.x= " << asc2.x << endl;

}

void functorTest() {
    QTextStream out(stdout);

    Dfunctor functor;

    out << "Returning value of the functor : " << functor(1, 2) << endl;

}

void comparatorTest() {
    QTextStream out(stdout);

    Comparator c(200);

    c.x = 200;

    if (c < 250) {
        out << "Comparator : x < 250" << endl;
    }

    if (c == 200) {
        out << "Comparator : x == 200" << endl;
    }

    if (c <= 200) {
        out << "Comparator : x <= 200" << endl;
    }

    if (c >= 200) {
        out << "Comparator : x >= 200" << endl;
    }

    if (c >= 199) {
        out << "Comparator : x >= 199" << endl;
    }

    if (c > 199) {
        out << "Comparator : x > 199" << endl;
    }

    if (c != 150) {
        out << "Comparator : x != 150" << endl;
    }

    Comparator c2(500);

    c2.x = 500;
    if (c < c2) {
        out << "Comparator : c < c2" << endl;
    }


    ComparatorEQnEQ cen1;
    cen1.x = 10;

    ComparatorEQnEQ cen2;
    cen2.x = 20;

    if (cen1 != cen2) {
        out << "cen1 != cen2" << endl;
    }

}

void savableRestorableTest() {
    QTextStream out(stdout);

    SR sr;
    sr.x = 111;
    sr.save();

    sr.x = 0;

    sr.restore();

    out << "Restored x : " << sr.x << endl;

    SToRFrom sTorFrom;

    sTorFrom.setX(112);

    int safe = -1;

    sTorFrom.saveTo(safe);

    sTorFrom.restoreFrom(safe);

    out << "Restored from safe : " << sTorFrom.getX() << endl;
}

void writableReadableTest() {
    QTextStream out(stdout);

    WR wr1;
    WR wr2;

    QTextStream in(stdin);

    //in >> wr1 >> wr2;

    //out << wr1 << wr2 << endl;

}

void nonAssignerTest() {
    QTextStream out(stdout);

    NonAssigner nas;

    unsigned int tmp = 10;

    nas = tmp;
}

void exceptionTest() {
    QTextStream out(stdout);

    try {

        Message<string> msg("Hurrey!!! Custom exception!!!");

        throw ErrMsgException<Message < string >> (msg);
        //Message<> ("MessageTest");

        //{
        //	Deleter deleter("Hello");
        //}


    } catch (MsgException<Message<string>>&e) {

        out << QString::fromStdString(e.getMsg().getMsgData()) << endl;
        out << "Caught exception!" << endl;

    }
}

void operationableTest() {
    QTextStream out(stdout);

    Additor add1;
    Additor add2;

    add1.x = 10;
    add2.x = 20;

    Additor add3 = add1 + add2;
    add3 = add3 + 1;

    out << "Result of addition : " << add3.x << endl;

    add3 = add3 - 2;

    out << "Result of subtraction : " << add3.x << endl;

    add3 = add3 * 10;

    out << "Result of multiplication : " << add3.x << endl;

    add3 = add3 / 5.0;

    out << "Result of division : " << add3.x << endl;

    Inv inv;
    inv.x = 0.0;
    try {
        inv.inv();
    } catch (ErrMsgException<MessageWritableTo<QTextStream, QString>>&e) {
        //} catch (MsgException<MessageWritableTo<QTextStream>>&e) {
        //} catch (Exception<>&e) {

        out << e.getMsg() << endl;
        out << "Exception caught!" << endl;

    }

    out << "Result of inversion : " << inv.x << endl;

    PrePostInc pi;
    pi.x = 0;
    pi++;
    ++pi;
    out << "Result of incrementing : " << pi.x << endl;
    getchar();
}

void someClonableTest() {
    QTextStream out(stdout);

    SomeClonable1 sc11;
    sc11.name = "sc11";
    out << sc11.type << endl;

    SomeClonable1* sc12 = sc11.clone();
    sc12->name = "sc12";
    out << sc12->type << endl;

    delete sc12;

    SomeClonable2 sc21;
    out << sc21.type << endl;

    Clonable* cl11 = &sc11;

    Clonable* cl12 = cl11->clone();

    out << ((SomeClonable1*) cl12)->type << endl;

}

void smartPointerTest() {
    QTextStream out(stdout);

    {



        SomeClonable1* sc1 = new SomeClonable1;
        SomeClonable2* sc2 = new SomeClonable2;

        sc1->name = "cs1";
        sc2->name = "cs2";

        SmartPointer<SomeClonable1> sptr1(sc1);
        SmartPointer<SomeClonable2> sptr2(sc2);

        SmartPointer<SomeClonable1> sptr11;

        try {

            //sptr11 = sptr1;

            //sptr1.setPointer(sc1, true);
            //sptr1.setPointer(sc2, true);

            if (sptr1 == sptr2) {
                out << "sptr1 == sptr2" << endl;
            } else {
                out << "sptr1 != sptr2" << endl;
            }

            out << "SmartPointer : " << sptr1->name << " " << sptr1->type << endl;
            out << "SmartPointer : " << sptr2->name << " " << sptr2->type << endl;

            //out << "SmartPointer sptr11 : " << sptr11->name << " " << sptr11->type << endl;

        } catch (MsgException<Message<string>> &e) {

            out << "Source of exception : " << dynamic_cast<SmartPointer<SomeClonable1>*> (e.getSender()) << endl;

            out << QString::fromStdString(e.getMsg().getMsgData()) << endl;

            exit(-1);

        }

    }
}

void mathTest() {
    QTextStream out(stdout);

    out << "Max test : " << Math::max(1, 2) << endl;
    out << "Max test : " << Math::max(1.5, 2.6) << endl;
    out << "Min test : " << Math::min(1, 2) << endl;
    out << "Min test : " << Math::min(1.5, 2.6) << endl;

    QVector<double> someDVector;
    someDVector << 1.1 << 2.2 << 3.3 << 4.4 << 5.5;
    QVector<double> someProbVector;
    someProbVector << 0.1 << 0.2 << 0.25 << 0.35 << 0.1;

    vector<double> someStdVector;
    someStdVector = someDVector.toStdVector();

    out << "Min test vector : " << Math::min(someDVector) << endl;
    out << "Max test vector : " << Math::max(someDVector) << endl;

    Rand::randPermut(someDVector);

    Rand::randPermut(someStdVector);

    Math::nextLexPermutation(someDVector);

    for (int i = 0; i < someDVector.size(); i++) {
        out << someDVector[i] << ",";
    }

    out << endl;

    for (int i = 0; i < 10; i++) {
        out << "Randomly selected value: " << Rand::probSelect(someDVector, someProbVector) << endl;
        //getchar();
    }

    Math::sort(someDVector);

    for (int i = 0; i < someDVector.size(); i++) {
        out << someDVector[i] << ",";
    }

    out << endl;

    Math::reverseVec(someDVector);

    for (int i = 0; i < someDVector.size(); i++) {
        out << someDVector[i] << ",";
    }

    out << endl;

    QVector<double> probs;
    probs.append(0.1);
    probs.append(0.2);
    probs.append(0.1);
    probs.append(0.5);
    probs.append(0.1);

    QMap<int, int> freqs;

    Math::uint64 numExp = 1000000;
    for (Math::uint64 i = 0; i < numExp; i++) {
        int curVal = Rand::probSelect(someDVector, probs);
        //out << "Selected Value: " << curVal << endl;
        freqs[curVal] += 1;
    }

    out << "Frequencies: " << endl;
    for (int i = 0; i < someDVector.size(); i++) {
        out << someDVector[i] << " -> " << double(freqs[someDVector[i]]) / double(numExp) << endl;
    }

    out << "Size of unsigned int : " << sizeof (long int) << endl;

    //	out << "MAX_INT : " << Math::MAX_INT << endl;
    //	out << "MIN_INT : " << Math::MIN_INT << endl;
    //	out << "MAX_UINT : " << Math::MAX_UINT << endl;
    //	out << "MIN_UINT : " << Math::MIN_UINT << endl;
    out << "MAX_INT64 : " << Math::MAX_INT64 << " or " << Math::int64(Math::uint64(~Math::uint64(0)) >> 1) << endl;
    out << "MIN_INT64 : " << Math::MIN_INT64 << " or " << Math::int64(Math::uint64(~(Math::uint64(~Math::uint64(0)) >> 1))) << endl;
    out << "MAX_UINT64 : " << Math::MAX_UINT64 << " or " << Math::uint64(~Math::uint64(0)) << endl;
    out << "MIN_UINT64 : " << Math::MIN_UINT64 << endl;
    out << "MAX_INT32 : " << Math::MAX_INT32 << " or " << Math::int32(Math::uint32(~Math::uint32(0)) >> 1) << endl;
    out << "MIN_INT32 : " << Math::MIN_INT32 << " or " << Math::int32(Math::uint32(~(Math::uint32(~Math::uint32(0)) >> 1))) << endl;
    out << "MAX_UINT32 : " << Math::MAX_UINT32 << " or " << Math::uint32(~Math::uint32(0)) << endl;
    out << "MIN_UINT32 : " << Math::MIN_UINT32 << endl;
    out << "MAX_INT16 : " << Math::MAX_INT16 << " or " << Math::int16(Math::uint16(~Math::uint16(0)) >> 1) << endl;
    out << "MIN_INT16 : " << Math::MIN_INT16 << " or " << Math::int16(Math::uint16(~(Math::uint16(~Math::uint16(0)) >> 1))) << endl;
    out << "MAX_UINT16 : " << Math::MAX_UINT16 << " or " << Math::uint16(~Math::uint16(0)) << endl;
    out << "MIN_UINT16 : " << Math::MIN_UINT16 << endl;
    out << "MAX_INT8 : " << Math::MAX_INT8 << " or " << Math::int8(Math::uint8(~Math::uint8(0)) >> 1) << endl;
    out << "MIN_INT8 : " << Math::MIN_INT8 << " or " << Math::int8(Math::uint8(~(Math::uint8(~Math::uint8(0)) >> 1))) << endl;
    out << "MAX_UINT8 : " << Math::MAX_UINT8 << " or " << Math::uint8(~Math::uint8(0)) << endl;
    out << "MIN_UINT8 : " << Math::MIN_UINT8 << endl;
    out << "MAX_INTUNI : " << Math::MAX_INTUNI << " or " << Math::intUNI(Math::uintUNI(~Math::uintUNI(0)) >> 1) << endl;
    out << "MIN_INTUNI : " << Math::MIN_INTUNI << " or " << Math::intUNI(Math::uintUNI(~(Math::uintUNI(~Math::uintUNI(0)) >> 1))) << endl;
    out << "MAX_UINTUNI : " << Math::MAX_UINTUNI << " or " << Math::uintUNI(~Math::uintUNI(0)) << endl;
    out << "MIN_UINTUNI : " << Math::MIN_UINTUNI << endl;

    QVector<int> vecDup;

    vecDup << 1 << 1 << 2 << 5 << 6 << 7 << 7 << 8 << 8;

    Math::removeDuplicates(vecDup);

    for (int i = 0; i < vecDup.size(); i++) {
        out << vecDup[i] << endl;
    }

    getchar();

}

void randGenTest() {
    QTextStream out(stdout);

    out << "####  randGenTest ...  ####" << endl;

    //RandGenMT rgmt(1872638163);
    //	RandGenMT rgmt(654321);
    //	RandGenMT rgmtclone(rgmt);

    //MT19937<Math::uint32> rgmtAlt(1872638163);
    MT19937<double> rgmtAlt(654321);
    MT19937<double> rgmtAltClone(rgmtAlt);

    for (int i = 0; i < 100; i++) {
        //		out << rgmt.rndFloat() << " - " << rgmtclone.rndFloat() << " Alt: " << rgmtAlt.rnd() << " - " << rgmtAltClone.rnd() << endl;
    }

    unsigned int numRndClasses = 10;
    unsigned int numRndTests = 5000000;
    vector<double> testFreqs(numRndClasses);

    for (unsigned int i = 0; i < numRndClasses; i++) {
        testFreqs[i] = 0;
    }
    for (unsigned int i = 0; i < numRndTests; i++) {
        //		unsigned int curIdx = /*rgmtAlt.driver().rnd(1, numRndClasses); //*/rgmt.rndInt(1, numRndClasses);

        //		testFreqs[curIdx - 1]++;
    }
    double sumFreq = 0.0;
    for (unsigned int i = 0; i < numRndClasses; i++) {
        testFreqs[i] /= double(numRndTests);

        sumFreq += testFreqs[i];

        out << testFreqs[i] << " ";
    }
    out << endl;
    out << "sumFreq : " << sumFreq << endl;

    //out << "maxGenInt : " << rgmt.getMaxGenInt() << endl;

    out << "Global rndSeed: " << Rand::rndSeed() << endl;

    out << "####  randGenTest done  ####" << endl;

    getchar();
}

void randGenTestLocStor() {
    QTextStream out(stdout);
    out << "####  randGenTestLocStor ...  ####" << endl;

    out << "randGenTestLocStor setting Rand::rndSeed ..." << endl;
    Rand::rndSeed(123456789);
    out << "The returned seed: " << Rand::rndSeed() << endl;

    ThreadRunnableLocStor trls1;
    ThreadRunnableLocStor trls2;

    trls1.setObjectName("trLS 1");
    trls2.setObjectName("trLS 2");

    trls1.ID = 1357;
    trls2.ID = 1357; //2468;

    trls1.run();
    trls2.run();


    out << "####  randGenTestLocStor done  ####" << endl;
}

void senderReceiverTest() {
    EvtSender sender;
    EvtReceiver receiver;

    sender.addReceiver((ReceiverOf<Evt1>&)receiver);
    sender.addReceiver((ReceiverOf<Evt2>&)receiver);
    sender.addReceiver((ReceiverOf<Evt3>&)receiver);

    //sender.removeReceiver((ReceiverOf<Evt1>&)receiver);
    sender.removeAllReceiversOf(Evt3());

    sender.send((Evt1*) NULL);
    sender.send((Evt2*) NULL);
    sender.send((Evt3*) NULL);

}

void threadRunnableTest(QApplication& app) {
    QTextStream out(stdout);

    ThreadRunnable tr1;
    ThreadRunnable tr2;

    QObject::connect(&tr1, SIGNAL(sigFinished()), &app, SLOT(quit()));
    QObject::connect(&tr2, SIGNAL(sigFinished()), &app, SLOT(quit()));

    //	tr1.finishedQt.connect(&app, &QApplication::quit);
    //	tr1.finishedQt.disconnect(&app, &QApplication::quit);

    tr1.setObjectName("tr 1");
    tr2.setObjectName("tr 2");

    tr1.run();
    tr2.run();

}

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);

    QTextStream out(stdout);

    clonnerAssignerTest();

    functorTest();

    comparatorTest();

    savableRestorableTest();

    writableReadableTest();

    nonAssignerTest();

    exceptionTest();

    operationableTest();

    someClonableTest();

    smartPointerTest();

    mathTest();

    randGenTest();

    senderReceiverTest();

    //throw ErrException();

    threadRunnableTest(app);

    testTestSenderReceiver();

    testSigSlot();

    randGenTestLocStor();

    testSolvers();

    testAltRand();

    testSettings();

    testParser();

    return app.exec();
}
