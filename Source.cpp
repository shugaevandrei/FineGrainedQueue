#include <iostream>
#include <thread>
#include <algorithm>
#include <mutex>

struct Node
{
    int value;
    Node* next;
    std::mutex* node_mutex;
};

class FineGrainedQueue
{
    Node* head;
    int sizeList{ 0 };
    std::mutex* queue_mutex;
public:
    void  insertIntoMiddle(int value, int pos)
    {
        Node* prev, * cur;
        int cnt{ 0 };
        queue_mutex->lock();

        prev = this->head;
        cur = this->head->next;

        prev->node_mutex->lock();
        queue_mutex->unlock();

        cur->node_mutex->lock();

        if (sizeList < pos) {
            prev->node_mutex->unlock();
            cur->node_mutex->unlock();
            cur->value = value;
            ++sizeList;
        }
        else {
            while (cur)
            {
                if (cnt == pos)
                {
                    prev->node_mutex->unlock();
                    cur->node_mutex->unlock();
                    cur->value = value;
                    ++sizeList;
                    return;
                }
                ++cnt;
                Node* old_prev = prev;
                prev = cur;
                cur = cur->next;
                old_prev->node_mutex->unlock();
                cur->node_mutex->lock();
            }
        }
        prev->node_mutex->unlock();
    }

};

int main()
{

    return 0;
}