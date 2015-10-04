#ifndef RGX_H_INCLUDED
#define RGX_H_INCLUDED

#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include "stack.h"
#include "linkTable.h"
using namespace std;

class RGX
{
private:
        char*   regular_exp;
        char*   regular_exp_post;
        char*   char_table;
        int     char_number;
        int     DFA_state_num;
        int     NFA_state_num;
        table*  NFA_table;
        table*  DFA_table;
        inline  int  priority(char s)
        {
                if (s == '|')
                {
                        return 1;
                }
                else if (s == '.')
                {
                        return 2;
                }
                else if (s == '*')
                {
                        return 3;
                }
                return 0;
        }
        int judge(int state, int nfa[][101])
        {
                int i,j,sum;
                i = 2;
                while(nfa[i][0]!=0)
                {
                        sum  = 0;
                        j = 0;
                        while(nfa[i][j] != 0)
                        {
                                if(nfa[state][j] == nfa[i][j])
                                        sum++;
                                j++;
                        }
                        if(sum == j)
                                return i;
                        i++;
                }
                return state;
        }
public:
        RGX()
        {
                regular_exp             = new char[128];
                regular_exp_post        = new char[128];
                char_table              = new char[128];
                char_number             = 0;
                DFA_state_num           = 0;
                NFA_state_num           = 0;
                NFA_table               = new table();
                DFA_table               = new table();
        }
        ~RGX()
        {
                delete []regular_exp;
                delete []regular_exp_post;
                delete []char_table;
                delete NFA_table;
                delete DFA_table;
        }
        void input_regex()
        {
                cout << "step #0: input the regular expression\n";
                cin >> regular_exp;
                //   cout << regular_exp;
        }
        // add . between each segment
        void preprocess()
        {
                int len = strlen(regular_exp);
                int lnn = len;
                //cout << len << endl;
                for (int j = len - 1, i = 0; j != 0; j--, i++)
                {
                        char c1 = regular_exp[i], c2 = regular_exp[i + 1];
                        if ( ((c1 != '|' && c1 != '(' && c1 != '.') || c1 == ')' || c1 == '*')
                                        &&
                                        (c2 != '|' && c2 != '.' && c2 != ')' && c2 != '*'))
                        {
                                for (int k = lnn; k > i; k--)
                                {
                                        regular_exp[k + 1] = regular_exp[k];
                                }
                                regular_exp[i + 1] = '.';
                                lnn++;
                                regular_exp[lnn] = '\0';
                                i++;
                        }

                }
                cout <<"preprocess :" << regular_exp << endl;
        }
        // mid-fix -> post-fix
        // data structure P75
        void get_post()
        {
                int i(0), j(0);
                strcpy(regular_exp_post, "\0");
                stack<char>* pool = new stack<char>();
                pool->push('@');
                char ch = regular_exp[i];
                while (ch != '\0')
                {
                        if (ch == '(')
                        {
                                pool->push(ch);
                        }
                        else if (ch == ')')
                        {
                                while (pool->get_top() != '(')
                                {
                                        regular_exp_post[j++] = pool->pop();
                                }
                                pool->pop();
                        }
                        else if (ch == '|' || ch == '.' || ch == '*')
                        {
                                char ch2 = pool->get_top();
                                while (priority(ch2) >= priority(ch))
                                {
                                        regular_exp_post[j++] = ch2;
                                        pool->pop();
                                        ch2 = pool->get_top();
                                }
                                pool->push(ch);
                        }
                        else
                        {
                                regular_exp_post[j++] = ch;
                        }
                        ch = regular_exp[++i];

                }
                ch = pool->pop();
                while (ch != '@')
                {
                        regular_exp_post[j++] = ch;
                        ch = pool->pop();
                }
                regular_exp_post[j] = '\0';
               // pool->~stack();
                cout << "post fix :" << regular_exp_post << endl;
        }
        void get_char_table()
        {
                int i=0,j;
                char ch;
                while((ch = regular_exp_post[i])!='\0')
                {
                        if(ch == '.'|| ch == '|'|| ch =='*')
                        {
                                i++;
                                continue;
                        }
                        for (j = 0;j<char_number;++j)
                        {
                                if(ch == char_table[j])
                                break;
                        }
                        if(j == char_number)
                        {
                                char_table[j] = ch;
                                char_number++;
                        }
                        i++;
                }
                char_table[char_number] = '\0';
                cout <<"\n";
                for(int i = 0;i<char_number;++i)
                {
                        cout << char_table[i]<< " ";
                }
                cout << "number of chars : " << char_number << endl;
        }
        void rgx2nfa()
        {
                stack<int >* states = new stack<int >;
                int i =1, j=0;
                int s1, s2;
                int t1, t2;
                if(strlen(regular_exp_post) < 1) return;
                char ch = regular_exp_post[j];

                while(ch != '\0')
                {
                        if(ch == '|')
                        {
                                s2 = states->pop();
                                s1 = states->pop();
                                t2 = states->pop();
                                t1 = states->pop();
                                NFA_table->insertVer(i);
                                NFA_table->insertVer(i+1);
                                NFA_table->insertEdge(i,t1);
                                NFA_table->insertEdge(i,s1);
                                NFA_table->insertEdge(s2,i+1);
                                NFA_table->insertEdge(t2,i+1);
                                states->push(i);
                                states->push(i+1);
                                i+=2;
                        }
                        else if(ch == '*')
                        {
                                s2 = states->pop();
                                s1 = states->pop();
                                NFA_table->insertVer(i);
                                NFA_table->insertVer(i+1);
                                NFA_table->insertEdge(s2,s1);
                                NFA_table->insertEdge(i,i+1);
                                NFA_table->insertEdge(i,s1);
                                NFA_table->insertEdge(s2,i+1);
                                states->push(i);
                                states->push(i+1);
                                i+=2;
                        }
                        else if(ch == '.') // t1-t2 ~ s1-s2
                        {
                                s2 = states->pop();
                                s1 = states->pop();
                                t2 = states->pop();
                                t1 = states->pop();
                                NFA_table->insertEdge(t2,s1);
                                states->push(t1);
                                states->push(s2);
                        }
                        else{
                                NFA_table->insertVer(i);
                                NFA_table->insertVer(i+1);
                                NFA_table->insertEdge(i,i+1,ch);
                                states->push(i);
                                states->push(i+1);
                                i+=2;
                        }
                        ch = regular_exp_post[++j];
                }
                s2 = states->pop();
                NFA_state_num = s2+1;
                cout << "-----------------------------\n";
                NFA_table->Print();
                cout << "-----------------------------\n";

        }
        void nfa2dfa()
        {
                int state = 0;
                int ptr = 0;
                bool ver_table[1001] = {0};
                stack<int > dfastack;
                stack<int > nfastack;
                int nfa_hash_table[101][101];
                for(int i =0;i<101;++i)
                        for(int j =0;j<101;++j)
                                nfa_hash_table[i][j] = 0;
                for(int i =0;i<NFA_table->ver_num;++i)
                {
                        ver_table[i] = 0;
                }
                state = 1;
                DFA_table->insertVer(1);// initial state...


                for(int i = 0 ; i < char_number;++i)// 几种可能的转换
                {
                        char weight = char_table[i];
                        verNode* p = NFA_table->head;

                        for (int j =0;j<state;++j)
                                p = p->vnext;
                        // p is the first state to be considered..

                        edgeNode* q = p->next;
                        while (q)
                        {
                                if(q->weight==weight && ver_table[q->value] == 0)
                                {
                                        nfastack.push(q->value);
                                }
                                ver_table[q->value] = 1;
                                q = q->next;
                               // cout << "999999999999999\n";
                        }
                        if(nfastack.isEmpty())
                                continue;

                        state++;
                        dfastack.push(state);
                        while (!nfastack.isEmpty())
                        {
                                int v = nfastack.pop();
                                // for every possible node, push it out and mark it in the hash table
                                nfa_hash_table[state][ptr++] = v;
                                //mark it in the hash table, the ptr of state is v
                                verNode* p = NFA_table->head;
                                for (int j = 0; j < v; ++j) p = p->vnext;
                                edgeNode* q = p->next;
                                while (q)
                                {
                                        if (q->weight == '~' && ver_table[q->value] == 0)
                                                nfastack.push(q->value);
                                        ver_table[q->value] = 1;
                                      //  cout << "66666666666666\n";
                                        q = q->next;
                                }

                        }
                        cout << state << " finished ... \n";
                        DFA_table->insertVer(state);
                        DFA_table->insertEdge(1,state,char_table[i]);
                }
                // 3-32
                cout << "1 finished ... \n";
                while (!dfastack.isEmpty())
                {
                        NFA_state_num = dfastack.pop();
                        for (int j = 0; j < char_number; ++j)
                        {
                                int i = 0;
                                char weight = char_table[j];

                                while (nfa_hash_table[NFA_state_num][i] != 0)
                                {
                                        int ver_table[1001] = {0};
                                        verNode* p = NFA_table->head;
                                        for(int k =0; k < nfa_hash_table[NFA_state_num][i]; ++k)
                                                p = p->vnext;
                                        ++i;
                                        if(p->next == 0) continue;
                                        edgeNode* q = p->next;
                                        while(q)
                                        {
                                                if (q->weight == weight && ver_table[q->value]==0)
                                                {
                                                        nfastack.push(q->value);
                                                }
                                                ver_table[q->value] =1;
                                                q = q->next;
                                         //       cout <<"#####"<<endl;
                                        }

                                        if(nfastack.isEmpty()) continue;
                                        state++;
                                        ptr = 0;
                                        while(!nfastack.isEmpty())
                                        {
                                                int v = nfastack.pop();
                                                // for every possible node, push it out and mark it in the hash table
                                                nfa_hash_table[state][ptr++] = v;
                                                //mark it in the hash table, the ptr of state is v
                                                verNode* p = NFA_table->head;
                                                for (int j = 0; j < v; ++j) p = p->vnext;
                                                edgeNode* q = p->next;
                                                while (q)
                                                {
                                                        if (q->weight == '~' && ver_table[q->value] == 0)
                                                                nfastack.push(q->value);
                                                        ver_table[q->value] = 1;
                                                        q = q ->next;
                                                       // cout <<"!!!!!"<<endl;
                                                }

                                        }
                                        int tmp = state;
                                        state = judge(state,nfa_hash_table);
                                        //cout << tmp << state;
                                        if (state == tmp)
                                        {
                                               // cout << "yes"<<endl;
                                                DFA_table->insertVer(state);
                                            //    cout <<"state"<<endl;
                                                dfastack.push(state);
                                        }
                                        DFA_table->insertEdge(NFA_state_num,state,char_table[j]);
                                        state = tmp;
                                }
                        }
                }
                DFA_state_num = DFA_table->ver_num - 1;
                NFA_state_num = state;

                DFA_table->Print();
        }
        void run()
        {
                input_regex();
                preprocess();
                get_post();
                get_char_table();
                rgx2nfa();
                nfa2dfa();
        }
};


#endif
