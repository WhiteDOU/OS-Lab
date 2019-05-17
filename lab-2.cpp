#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;


struct Process
{
    string name;

    virtual void step(bool recover) = 0;

    explicit Process(string n) : name(move(n))
    {}
};


class ProcessQ : public vector<Process *>
{
public:
    friend ostream &operator<<(ostream &out, const vector<Process *> &Q)
    {
        if (Q.empty())
            out << "No Process";
        else
        {
            for (auto it = Q.begin(); it != Q.end(); it++)
            {
                out << (*it)->name;
                if (it != Q.end() - 1)
                    out << "|";
            }
        }
        return out;
    }
};

class Interaction
{
    int buf[10]{};
    int i, j;//Pointer
    int freeP, goods, mutex;
    ProcessQ freeQ{}, goodsQ{}, mutexQ{};
public:
    Interaction()
    {
        for (auto k : buf)
            buf[k] = 0;
        i = 0;
        j = 0;
        mutex = 1;
        freeP = 10;
        goods = 0;
    }

    bool Pfree(Process *p)
    {
        if (--freeP < 0)
        {
            freeQ.push_back(p);
            return false;
        }
        return true;
    }

    void Vfree()
    {
        if (++freeP <= 0)
        {
            auto Pro = freeQ.front();
            freeQ.erase(freeQ.begin());
            Pro->step(true);
        }
    }

    bool Pgoods(Process *p)
    {
        if (--goods < 0)
        {
            goodsQ.push_back(p);
            return false;
        }
        return true;
    }

    bool Vgoods()
    {
        if (++goods <= 0)
        {
            auto pro = goodsQ.front();
            goodsQ.erase(goodsQ.begin());
            pro->step(true);
        }
    };

    bool Pmutex(Process *p)
    {
        if (--mutex < 0)
        {
            mutexQ.push_back(p);
            return false;
        }
        return true;
    }

    bool Vmutex()
    {
        if (++mutex <= 0)
        {
            auto pro = mutexQ.front();
            mutexQ.erase(mutexQ.begin());
            pro->step(true);
        }
    }

    void put(int value)
    {
        buf[i] = value;
        i = (i + 1) % 10;  //cicurlar Q
    }

    int get()
    {
        auto value = buf[j];
        buf[j] = 0; // erase the element
        j = (j + 1) % 10;
        return value;
    }

    bool mutexCheck(Process *t)
    {
        for (auto ta:mutexQ)
        {
            if (ta == t)
                return false;
        }
        return true;
    }

    void output()
    {
        cout << "------This is a line-------" << endl;
        cout << "WaitQ 1:" << freeQ << endl;
        cout << "WaitQ 2:" << goodsQ << endl;
        cout << "TEMP Q:";
        for (auto b : buf)
        {
            cout << b << "|";
        }
        cout << endl;
        cout << "------This is a line-------";
    }
};


struct Producer : public Process
{
    int i = 0;
    Interaction *interaction = nullptr;

    Producer(string name, Interaction *inter) : Process(move(name)), interaction(inter)
    {}

    void step(bool recover = false) override
    {
        if (recover || interaction->Pfree(this))
        {
            interaction->put(++i);
            cout << "Producer put" << i << endl;
            interaction->Vgoods();
        } else
            return;
    };
};

struct Consumer : public Process
{
    Interaction *interaction = nullptr;

    Consumer(string name, Interaction *e) : Process(move(name)), interaction(e)
    {}

    void step(bool recover = false) override
    {
        if (recover || interaction->Pgoods(this))
        {
            cout << "Consumer get" << interaction->get() << endl;
            interaction->Vfree();
        } else
            return;;
    }
};

struct Competitor : public Process
{
    Interaction *env = nullptr;
    bool got;

    Competitor(string name, Interaction *e) : Process(std::move(name)), env(e), got(false)
    {}

    void step(bool recovery = false) override
    {
        if (got)
        {
            env->Vmutex();
            cout << name << " release mutex" << endl;
            got = false;
            return;
        }
        if (recovery || env->Pmutex(this))
        {
            cout << name << " get mutex" << endl;
            got = true;
        }
    }
};


int main()
{
    Interaction interaction;
    Producer producer("p", &interaction);
    Consumer consumer("c", &interaction);
    Competitor cs[10] = {{"c0", &interaction},
                         {"c1", &interaction},
                         {"c2", &interaction},
                         {"c3", &interaction},
                         {"c4", &interaction},
                         {"c5", &interaction},
                         {"c6", &interaction},
                         {"c7", &interaction},
                         {"c8", &interaction},
                         {"c9", &interaction},

    };

    string terminals;
    while (true)
    {
        cout << "C:Consumer\tP:Producer\tQ:Quit"<<endl<<">>>";
        cin >> terminals;
        transform(terminals.begin(),terminals.end(),terminals.begin(),::tolower);

        if (terminals == "c")
            consumer.step();
        else if(terminals == "p")
            producer.step();
        else if(terminals == "q")
            return 0;
        else
            cout << "Input Error"<<endl;

        interaction.output();

    }

}