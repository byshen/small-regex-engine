#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED
template<class T>
class stack{
private:
        struct node{
                T data;
                node* next;
                node(T d = 0,node* n = 0):data(d),next(n){}
        };
        node* top;
        int size;
public:
        stack(){ top = 0; size = 0;}
        ~stack(){
                node* p = top;
                while(p!=0){
                        p = p->next;
                        delete top;
                        top = p;
                }
        }
        bool isEmpty() const
        {
                return (!top);
        }
        int Size(){return size;}
        void push(const T &x)
        {
                node* t = new node(x,top);
                top = t;
                size++;
        }
        T pop()
        {
                size--;
                node* tmp = top;
                T x = tmp->data;
                top = top->next;
                delete tmp;
                return x;
        }
        T get_top(){return top->data;}

};


#endif // STACK_H_INCLUDED
