#include <iostream>
#include <fstream>
using namespace std;

template <typename t_data>
class binary_tree
{
    private:
    class node
    {
        public:
        t_data data;
        node* next_left;
        node* next_right;
        node* parent;
        node(){}
        node(t_data data, node* parent = nullptr) {this->data = data; next_left = nullptr; next_right = nullptr; this->parent = parent;}
    };
    node* root;
    node* search_(t_data data) // return list: nullptr - nothing found, visit - found
    {
        node* visit = root;
        while (visit!=nullptr)
        {
            if (visit->data == data) {return visit;}
            if (data < visit->data) {visit = visit->next_left;}
            else {visit = visit->next_right;}
        }
        return nullptr;
    }
    void export_(fstream& file, node* visit)
    {
        if (visit!=nullptr)
        {
            file << visit->data << endl;
            export_(file,visit->next_left);
            export_(file,visit->next_right);
        }
    }
    public:
    binary_tree() { root = nullptr;}
    int export_to_file(string file_path) // -1 - file wasn't open, 0 - success
    {
        fstream file;
        file.open(file_path, std::ofstream::out | std::ofstream::trunc);
        if (!file) {return -1;}
        else
        {
            export_(file,root);
        }
        file.close();
        return 0;
    }
    int push(t_data data) // return list: 1 - data already exists 0 - pushed successfully
    {
        if (root == nullptr) { root = new node(data);}
        else
        {
            node* visited = nullptr;
            node* visit = root;
            while (visit!=nullptr)
            {
                if (data<visit->data) {visited = visit; visit = visit->next_left;}
                else
                {
                    if (data>visit->data) {visited = visit; visit = visit->next_right;}
                    else {return 1;} // found same existing data
                }
            }
            if (data<visited->data) {visited->next_left = new node(data,visited);}
            else {visited->next_right = new node(data,visited);}
        }
        return 0;
    }
    int search(t_data data) // return: -1 - not found, 0 - found
    {
        if (search_(data) == nullptr) {return -1;}
        else {return 0;}
    }
    int del(t_data data) // return: -1 - not found, 0 - deleted
    {
        node* to_delete = search_(data);
        if (to_delete == nullptr) {return -1;} // no node with such data
        node* parent = to_delete->parent;
        if (to_delete->next_left == nullptr and to_delete->next_right == nullptr) // leaf
        {
            if (parent == nullptr)
            {
                delete(to_delete);
                root = nullptr;
            }
            else
            {
                if (to_delete->data<parent->data) {parent->next_left = nullptr;}
                else {parent->next_right = nullptr;}
                delete(to_delete);
            }
        }
        else
        {
            if (!(to_delete->next_left != nullptr and to_delete->next_right != nullptr)) // one child
            {
                node* only_child = nullptr;
                if (to_delete->next_left!=nullptr) {only_child = to_delete->next_left;}
                else {only_child = to_delete->next_right;} // saving only child

                if (parent == nullptr) 
                {
                    delete(to_delete);
                    root = only_child;
                }
                else
                {
                    if (only_child->data < parent->data) {parent->next_left = only_child;}
                    else {parent->next_right = only_child;} // comparing only child with parent to choose branch
                    delete(to_delete);
                }
            }
            else // both children
            {
                node* visit = to_delete->next_right;
                node* visited = nullptr;
                while (visit != nullptr) // searching for min data that is more than to_delete->data
                {
                    visited = visit;
                    visit = visit->next_left;
                }
                t_data data_buf = visited->data; 
                del(visited->data); // visited may only be a leaf or with one child
                to_delete->data = data_buf;
            }
        }
        return 0;
    }
};

int main()
{
    binary_tree<int> a;
    cout << "binary tree created" << endl;
    cout << "Enter:" << endl << "'p' to push data" << endl << "'s' to search for data" << endl << "'e' to export tree to file" << endl <<"'d' to delete data" << endl << "'q' to quit program" << endl;
    char command;
    int data;
    int func_res;
    do
    {
        cin >> command;
        switch (command)
        {
        case 'p':
            cout << "Enter data: ";
            cin >> data;
            func_res = a.push(data);
            switch (func_res)
            {
            case -1:
                cout << "error" << endl;
                break;
            case 0:
                cout << "data was pushed successfully" << endl;
                break;
            case 1:
                cout << "same data already exists" << endl;
                break;
            default:
                break;
            }
            break;
        case 's':
            cout << "Enter search request: ";
            cin >> data;
            func_res = a.search(data);
            switch (func_res)
            {
            case -1:
                cout << "no matching data" << endl;
                break;
            case 0:
                cout << "data was found" << endl;
                break;
            default:
                break;
            }
            break;
        case 'e':
            func_res = a.export_to_file("tree_revisited.txt");
            switch (func_res)
            {
            case -1:
                cout << "error. file wasn't open" << endl;
                break;
            case 0:
                cout << "exported to file successfully" << endl;
                break;
            default:
                break;
            }
            break;
        case 'd':
            cout << "enter data to delete: ";
            cin >> data;
            func_res = a.del(data);
            switch (func_res)
            {
            case -1:
                cout << "no such data to clear" << endl;
                break;
            case 0:
                cout << "data was deleted successfully" << endl;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    } while (command!='q');
}