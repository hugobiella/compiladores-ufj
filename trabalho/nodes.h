#include <iostream>
#include <vector>
#include <map>
#include <set>

using namespace std;

extern char *build_file_name;
extern int yylineno;
int error_count = 0;

class Node
{
protected:
    int lineno;

public:
    vector<Node *> children;
    Node()
    {
        lineno = yylineno;
    }
    int getLineNo()
    {
        return lineno;
    }
    virtual string getType()
    {
        return " ";
    }
    virtual string toStr()
    {
        return "stmts";
    }
    void append(Node *n)
    {
        children.push_back(n);
    }
    vector<Node *> &getChildren()
    {
        return children;
    }
};

class Program : public Node
{
public:
    virtual string toStr()
    {
        return "Program";
    }
};

class Inteiro : public Node
{
protected:
    int value;

public:
    Inteiro(const int v)
    {
        value = v;
    }
    virtual string getType() override
    {
        return "int";
    }

    virtual string toStr() override
    {
        return to_string(value);
    }
};

class Pf : public Node
{
protected:
    float value;

public:
    Pf(const float v)
    {
        value = v;
    }
    virtual string getType() override
    {
        return "float";
    }
    virtual string toStr() override
    {
        return to_string(value);
    }
};

class Boleano : public Node
{
protected:
    bool value;

public:
    Boleano(const bool v)
    {
        value = v;
    }
    virtual string getType() override
    {
        return "bool";
    }
    virtual bool getValue()
    {
        return value;
    }
    virtual string toStr() override
    {
        if (value)
        {
            return "true";
        }
        else
        {
            return "false";
        }
    }
};

class Palavra : public Node
{
protected:
    string frase;

public:
    Palavra(string v)
    {
        frase = v;
    }
    virtual string getType() override
    {
        return "Palavra";
    }
    virtual string getFrase()
    {
        return frase;
    }
    virtual string toStr() override
    {
        string a = frase;
        a.pop_back();
        string aux = string("\\") + a + string("\\\"");
        return aux;
    }
};

class Loop : public Node
{
protected:
    Node *value;

public:
    Loop(Node *v)
    {
        value = v;
        children.push_back(v);
    }

    virtual string toStr() override
    {
        return "Loop";
    }
};

class Enquanto : public Node
{
protected:
    Node *condition;
    Node *content;

public:
    Enquanto(Node *c, Node *n)
    {
        content = n;
        condition = c;
        children.push_back(c);
        children.push_back(n);
    }
    virtual Node *getCondition()
    {
        return condition;
    }

    virtual string toStr() override
    {
        return "Enquanto";
    }
};

class Se : public Node
{
protected:
    Node *condition;
    Node *content;

public:
    Se(Node *c, Node *n)
    {
        condition = c;
        content = n;
        children.push_back(c);
        children.push_back(n);
    }

    virtual Node *getCondition() { return condition; }

    virtual string toStr() override
    {
        return "Se";
    }
};

class SeSenao : public Node
{
protected:
    Node *contentSe;
    Node *contentSenao;
    Node *condition;

public:
    SeSenao(Node *c, Node *s, Node *sn)
    {
        condition = c;
        contentSe = s;
        contentSenao = sn;
        children.push_back(c);
        children.push_back(s);
        children.push_back(sn);
    }

    virtual Node *getCondition()
    {
        return condition;
    }
    virtual string toStr() override
    {
        return "Se Senão";
    }
};

class Id : public Node
{
protected:
    string name;

public:
    Id(const string n)
    {
        name = n;
    }

    virtual string getName()
    {
        return name;
    }

    virtual string getType()
    {
        return "Identificador";
    }

    virtual string toStr() override
    {
        return name;
    }
};

class Variavel : public Node
{
protected:
    string name;
    Node *value;

public:
    Variavel(const string n, Node *v)
    {
        name = n;
        value = v;
        children.push_back(v);
    }

    virtual Node *getNode()
    {
        return value;
    }

    virtual string getName()
    {
        return name;
    }

    virtual string toStr() override
    {
        return name + " =";
    }
};

class Unario : public Node
{
protected:
    Node *value;
    string operation;

public:
    Unario(string op, Node *v)
    {
        value = v;
        operation = op;
        children.push_back(v);
    }
    virtual Node *getNode()
    {
        return value;
    }
    virtual string getType()
    {
        return value->getType();
    }
    virtual string toStr() override
    {
        return operation;
    }
};

class OpBinaria : public Node
{
protected:
    Node *value1;
    Node *value2;
    string operation;

public:
    OpBinaria(Node *v1, char op, Node *v2)
    {
        value1 = v1;
        value2 = v2;
        operation = op;
        children.push_back(v1);
        children.push_back(v2);
    }

    virtual Node *getNode1() { return value1; }

    virtual Node *getNode2() { return value2; }
    virtual string getOperation() { return operation; }

    virtual string toStr() override
    {
        return operation;
    }
};

class print : public Node
{
protected:
    Node *value;

public:
    print(Node *v)
    {
        value = v;
        children.push_back(v);
    }

    virtual string toStr()
    {
        return "print";
    }
};

void printf_tree_recursive(Node *noh)
{
    for (Node *c : noh->getChildren())
    {
        printf_tree_recursive(c);
    }
    cout << "N" << (long int)noh << "[label=\"" << noh->toStr() << "\"];" << endl;

    for (Node *c : noh->getChildren())
    {
        cout << "N" << (long int)noh << "--" << "N" << (long int)c << ";" << endl;
    }
}

void printf_tree(Node *root)
{
    cout << "graph {" << endl;
    printf_tree_recursive(root);
    cout << "}" << endl;
}

class CheckVarDecl
{
private:
    set<string> symbolsInt, symbolsPf, symbolsBol, symbolsPal;

public:
    CheckVarDecl() {}

    void check(Node *noh)
    {
        for (Node *c : noh->getChildren())
        {
            checkUnaryPalavra(noh);
            checkUnaryBolean(noh);
            if (dynamic_cast<OpBinaria *>(c))
            {
                CheckBinaryOp(c);
            }
            check(c);
        }
        Se *se = dynamic_cast<Se *>(noh);
        if (se)
        {
            checkStringCondition(se->getCondition());
            return;
        }
        Enquanto *enquanto = dynamic_cast<Enquanto *>(noh);
        if (enquanto)
        {
            checkStringCondition(enquanto->getCondition());
            return;
        }

        SeSenao *senao = dynamic_cast<SeSenao *>(noh);
        if (senao)
        {
            checkStringCondition(senao->getCondition());
            return;
        }
        Id *id = dynamic_cast<Id *>(noh);
        if (id)
        {
            if (symbolsInt.count(id->getName()) <= 0 &&
                symbolsPf.count(id->getName()) <= 0 &&
                symbolsBol.count(id->getName()) <= 0 &&
                symbolsPal.count(id->getName()) <= 0)
            {
                error_count++;
                cout << build_file_name
                     << " "
                     << id->getLineNo()
                     << ":0: erro: "
                     << id->getName()
                     << " undefinied!."
                     << endl;
            }
        }
        Variavel *var = dynamic_cast<Variavel *>(noh);
        if (var)
        {
            Node *n = var->getNode();
            if (dynamic_cast<Inteiro *>(n))
            {
                symbolsInt.insert(var->getName());
                return;
            }
            if (dynamic_cast<Pf *>(n))
            {
                symbolsPf.insert(var->getName());
                return;
            }
            if (dynamic_cast<Palavra *>(n))
            {
                symbolsPal.insert(var->getName());
                return;
            }
            if (dynamic_cast<Boleano *>(n))
            {
                symbolsBol.insert(var->getName());
                return;
            }
            Id *id = dynamic_cast<Id *>(n);
            if (id)
            {
                if (symbolsInt.count(id->getName()) != 0)
                {
                    symbolsInt.insert(var->getName());
                    return;
                }
                if (symbolsPf.count(id->getName()) != 0)
                {
                    symbolsPf.insert(var->getName());
                    return;
                }
                if (symbolsPal.count(id->getName()) != 0)
                {
                    symbolsPal.insert(var->getName());
                    return;
                }
                if (symbolsBol.count(id->getName()) != 0)
                {
                    symbolsBol.insert(var->getName());
                    return;
                }
            }
            OpBinaria *aux, *opb = dynamic_cast<OpBinaria *>(n);
            while (opb != NULL)
            {
                aux = opb;
                opb = dynamic_cast<OpBinaria *>(opb->getNode1());
            }

            Node *n1 = aux->getNode1();
            if (dynamic_cast<Inteiro *>(n1))
            {
                symbolsInt.insert(var->getName());
                return;
            }
            if (dynamic_cast<Pf *>(n1))
            {
                symbolsPf.insert(var->getName());
                return;
            }
            if (dynamic_cast<Palavra *>(n1))
            {
                symbolsPal.insert(var->getName());
                return;
            }
            if (dynamic_cast<Boleano *>(n1))
            {
                symbolsBol.insert(var->getName());
                return;
            }

            id = dynamic_cast<Id *>(n1);
            if (id)
            {
                if (symbolsInt.count(id->getName()) != 0)
                {
                    symbolsInt.insert(var->getName());
                    return;
                }
                if (symbolsPf.count(id->getName()) != 0)
                {
                    symbolsPf.insert(var->getName());
                    return;
                }
                if (symbolsPal.count(id->getName()) != 0)
                {
                    symbolsPal.insert(var->getName());
                    return;
                }
                if (symbolsBol.count(id->getName()) != 0)
                {
                    symbolsBol.insert(var->getName());
                    return;
                }
            }
            Unario *aun, *un = dynamic_cast<Unario *>(n);

            if (un)
            {
                while (un != NULL)
                {
                    aun = un;
                    un = dynamic_cast<Unario *>(un->getNode());
                }

                Node *aunn = aun->getNode();
                if (dynamic_cast<Inteiro *>(aunn))
                {
                    symbolsInt.insert(var->getName());
                    return;
                }
                if (dynamic_cast<Pf *>(aunn))
                {
                    symbolsPf.insert(var->getName());
                    return;
                }
                if (dynamic_cast<Palavra *>(aunn))
                {
                    symbolsPal.insert(var->getName());
                    return;
                }
                if (dynamic_cast<Boleano *>(aunn))
                {
                    symbolsBol.insert(var->getName());
                    return;
                }

                Id *uid = dynamic_cast<Id *>(aunn);
                if (uid)
                {
                    if (symbolsInt.count(uid->getName()) != 0)
                    {
                        symbolsInt.insert(var->getName());
                        return;
                    }
                    if (symbolsPf.count(uid->getName()) != 0)
                    {
                        symbolsPf.insert(var->getName());
                        return;
                    }
                    if (symbolsPal.count(uid->getName()) != 0)
                    {
                        symbolsPal.insert(var->getName());
                        return;
                    }
                    if (symbolsBol.count(uid->getName()) != 0)
                    {
                        symbolsBol.insert(var->getName());
                        return;
                    }
                }
            }
        }
    }

    void checkStringCondition(Node *noh)
    {
        Id *id = dynamic_cast<Id *>(noh);
        if (id)
        {
            if (symbolsPal.count(id->getName()) == 1)
            {
                error_count++;
                cout << build_file_name
                     << " "
                     << id->getLineNo()
                     << ":0: erro: "
                     << "onde word in condition!"
                     << endl;

                return;
            }
        }

        Palavra *p = dynamic_cast<Palavra *>(noh);
        if (p)
        {
            error_count++;
            cout << build_file_name
                 << " "
                 << p->getLineNo()
                 << ":0: erro: "
                 << "one var in condition!"
                 << endl;
            return;
        }
    }

    void checkUnaryPalavra(Node *noh)
    { 
        int n_toDelete = 0;

        for (Node *c : noh->getChildren())
        {
            Unario *un = dynamic_cast<Unario *>(c);

            if (un)
            {
                Unario *aux;
                for (Unario *un1 = dynamic_cast<Unario *>(un);
                     un1;
                     aux = un1, un1 = dynamic_cast<Unario *>(un1->getNode()))
                    ;

                Palavra *p = dynamic_cast<Palavra *>(aux->getNode());
                if (p)
                {
                    noh->children.push_back(new Palavra(p->getFrase()));
                    noh->children.erase(noh->children.begin() + n_toDelete);
                }
            }
            n_toDelete++;
        }
    }

    void checkUnaryBolean(Node *noh)
    {
          int n_toDelete = 0;

        for (Node *c : noh->getChildren())
        {
            Unario *un = dynamic_cast<Unario *>(c);

            if (un)
            {
                Unario *aux;
                for (Unario *un1 = dynamic_cast<Unario *>(un);
                     un1;
                     aux = un1, un1 = dynamic_cast<Unario *>(un1->getNode()))
                    ;

                Boleano *b = dynamic_cast<Boleano *>(aux->getNode());
                if (b)
                {
                    noh->children.push_back(new Boleano(b->getValue()));
                    noh->children.erase(noh->children.begin() + n_toDelete);
                }
            }
            n_toDelete++;
        }
    }

    string getNameNode(Node *noh)
    {
        Unario *un = dynamic_cast<Unario *>(noh);
        if (un)
        {
            return getNameNode(un->getNode());
        }
        Id *id = dynamic_cast<Id *>(noh);
        if (id)
        {
            if (symbolsInt.count(id->getName()) != 0)
            {
                return "Inteiro";
            }
            if (symbolsPf.count(id->getName()) != 0)
            {
                return "Ponto Flutuante";
            }
            if (symbolsBol.count(id->getName()) != 0)
            {
                return "Boleano";
            }
            if (symbolsPal.count(id->getName()) != 0)
            {
                return "Palavra";
            }
        }

        if (dynamic_cast<Inteiro *>(noh))
        {
            return "Inteiro";
        }
        if (dynamic_cast<Pf *>(noh))
        {
            return "Ponto Flutuante";
        }
        if (dynamic_cast<Boleano *>(noh))
        {
            return "Boleano";
        }
        if (dynamic_cast<Palavra *>(noh))
        {
            return "Palavra";
        }

        OpBinaria *opb = dynamic_cast<OpBinaria *>(noh);
        if (opb)
        {
            return string(getNameNode(opb->getNode1()) + " " + opb->getOperation() + " " + getNameNode(opb->getNode2()));
        }

        return " ";
    }

    int CheckBinaryOp(Node *noh)
    { 
        Node *aux = noh;
        Unario *un = dynamic_cast<Unario *>(noh);
        if (un)
        {
            return CheckBinaryOp(un->getNode());
        }

        if (dynamic_cast<Inteiro *>(aux))
        {
            return 0;
        }
        if (dynamic_cast<Pf *>(aux))
        {
            return 1;
        }
        if (dynamic_cast<Boleano *>(aux))
        {
            return 2;
        }
        if (dynamic_cast<Palavra *>(aux))
        {
            return 3;
        }
        Id *id = dynamic_cast<Id *>(aux);

        if (id)
        {
            if (symbolsInt.count(id->getName()) != 0)
            {
                return 0;
            }
            if (symbolsPf.count(id->getName()) != 0)
            {
                return 1;
            }
            if (symbolsBol.count(id->getName()) != 0)
            {
                return 3;
            }
            if (symbolsPal.count(id->getName()) != 0)
            {
                return 4;
            }
        }

        OpBinaria *ob = dynamic_cast<OpBinaria *>(noh);
        if (ob)
        {
            if (CheckBinaryOp(ob->getNode1()) != CheckBinaryOp(ob->getNode2()))
            {
                error_count++;
                cout << build_file_name
                     << " "
                     << ob->getLineNo()
                     << ":0: erro:  ["
                     << getNameNode(ob->getNode1())
                     << "] e ["
                     << getNameNode(ob->getNode2())
                     << "] Invalid!"
                     << endl;
                return __INT_MAX__;
            }
            else
            {
                return CheckBinaryOp(ob->getNode1());
            }
        }
        return __INT_MAX__;
    }
};
