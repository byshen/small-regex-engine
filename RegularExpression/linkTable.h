#ifndef LINKTABLE_H_INCLUDED
#define LINKTABLE_H_INCLUDED
#include <iostream>
using namespace std;
struct edgeNode{
                int value;
                char weight;
                edgeNode* next;
                edgeNode(int n=-1, char ch='~',edgeNode* ne = 0):value(n),weight(ch){next = ne;}
        };
        struct verNode{
                int value;
                verNode* vnext;
                edgeNode* next;
                verNode(int v = 0):value(v){vnext = 0; next = 0;}
        };
class table{
public:
        verNode* head;
        int ver_num;
        int edge_num;


        table(){
                ver_num = 1;
                edge_num = 0;
                head = new verNode();// empty node
        }
        //--- NO SAFETY CONSIDERATION ---
        void insertVer(int value)
        {
                verNode* p = head;
                for(int i=0;i<ver_num-1;++i){
                        p = p->vnext;
                }
                p->vnext = new verNode(value);
                ver_num++;
        }
        void insertEdge(int value1, int value2, char weight = '~')
        {
                verNode* p = head;
                for(int i =0;i<ver_num;++i)
                {
                        if(p->value == value1)
                                break;
                        p = p->vnext;
                }
                p->next = new edgeNode(value2,weight,p->next);
                edge_num++;
        }
        // TEST
        void Print()
        {
                verNode* p = head;
                while(p){
                        cout<<p->value << " : ";
                        edgeNode* q = p->next;
                        while(q){
                                cout << q->value <<" ";
                                q= q->next;
                        }
                        p = p->vnext;
                        cout << endl;
                }
        }
};

#endif // LINKTABLE_H_INCLUDED
