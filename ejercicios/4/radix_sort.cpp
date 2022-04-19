// implementation of radix sort using bin/bucket sort
#include <bits/stdc++.h>
#include "../../common/actividad.h"
using namespace std;

// structure for a single linked list to help further in the
// sorting
struct node {
    pair<actividad, int> data;
    node* next;
};

// function for creating a new node in the linked list
struct node* create(pair<actividad, int> x)
{
    node* temp = new node();
    temp->data = x;
    temp->next = NULL;

    return temp;
}

// utility function to append node in the linked list
// here head is passed by reference, to know more about this
// search pass by reference
void insert(node*& head, pair<actividad, int> n)
{
    if (head == NULL) {
        head = create(n);
        return;
    }

    node* t = head;
    while (t->next != NULL)
        t = t->next;
    t->next = create(n);
}

// utility function to pop an element from front in the list
// for the sake of stability in sorting
pair<actividad, int> del(node*& head)
{
    if (head == NULL)
        return make_pair(actividad(), 0);
    node* temp = head;
    // storing the value of head before updating
    pair<actividad, int> val = head->data;

    // updation of head to next node
    head = head->next;

    delete temp;
    return val;
}

// utility function to get the number of digits in the
// max_element
int digits(int n)
{
    int i = 1;
    if (n < 10)
        return 1;

    while (n > (int)pow(10, i))
        i++;
    return i;
}

vector<pair<actividad, int>> radix_sort_modificado(vector<actividad>& arr)
{
    // size of the array to be sorted
    int sz = arr.size();
    vector<pair<actividad, int>> res;

    // getting the maximum element in the array
    int max_element = 0;
    for (actividad element : arr) {
        if(max_element < element.t){
            max_element = element.t;
        }
    }


    // getting digits in the maximum element
    int d = digits(max_element);

    // creating buckets to store the pointers
    node** bins;

    // array of pointers to linked list of size 10 as
    // integers are decimal numbers so they can hold numbers
    // from 0-9 only, that's why size of 10

    bins = new node*[10];

    // initializing the hash array with null to all
    for (int i = 0; i < 10; i++)
        bins[i] = nullptr;

    // first loop working for a constant time only and inner
    // loop is iterating through the array to store elements
    // of array in the linked list by their digits value
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < sz; j++) // bins updation
            insert(bins[(arr[j].t / (int)pow(10, i)) % 10],
                   make_pair(arr[j], j));

        int x = 0, y = 0;
        // write back to the array after each pass

        while (x < 10) {
            while (bins[x] != NULL)
                res[y++] = del(bins[x]);
                //arr[y++] = del(bins[x]);
            x++;
        }
    }
}

// a utility function to print the sorted array
void print(vector<int> arr)
{
    for (int i = 0; i < arr.size(); i++)
        cout << arr[i] << " ";
    cout << endl;
}